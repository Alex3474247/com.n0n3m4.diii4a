
idCVar botAi::harm_si_botLevel( "harm_si_botLevel", "0", CVAR_INTEGER | CVAR_GAME | CVAR_NOCHEAT | CVAR_ARCHIVE, "Bot level(0 = auto; 1 - 8 = default difficult level)." );
idCVar botAi::harm_si_botWeapons( "harm_si_botWeapons", "", CVAR_GAME | CVAR_NOCHEAT | CVAR_ARCHIVE, "Bot initial weapons when spawn, separate by comma(,); 0=none, *=all. Allow weapon index(e.g. 2,3), weapon short name(e.g. shotgun,machinegun), weapon full name(e.g. weapon_shotgun,weapon_machinegun), and allow mix(e.g. shotgun,3,weapon_rocketlauncher). All weapon: 1=pistol, 2=shotgun, 3=machinegun, 4=chaingun, 5=handgrenade, 6=plasmagun, 7=rocketlauncher, 8=BFG, 10=chainsaw." );
idCVar botAi::harm_si_botAmmo( "harm_si_botAmmo", "0", CVAR_INTEGER | CVAR_GAME | CVAR_NOCHEAT | CVAR_ARCHIVE, "Bot weapons initial ammo when spawn, depend on `harm_si_botWeapons`. -1=max ammo, 0=none, >0=ammo" );

#ifdef _MOD_BOTS_ASSETS
idCVar botAi::harm_g_botEnableBuiltinAssets( "harm_g_botEnableBuiltinAssets", "0", CVAR_BOOL | CVAR_GAME | CVAR_INIT | CVAR_ARCHIVE, "Enable built-in bot assets if external assets missing." );

bool botAi::botEnableBuiltinAssets = false;
#endif

bool botAi::botAvailable = false;
bool botAi::botInitialized = false;

bool botAi::InitBotSystem(void)
{
    if(botInitialized)
    {
        gameLocal.Warning("BotAI has initialized!");
        return botAvailable;
    }

    int num;
    bool valid = false;

    // check def
    num = declManager->GetNumDecls(DECL_ENTITYDEF);
    for (int i = 0; i < num; i++) {
        const idDeclEntityDef *decl = (const idDeclEntityDef *)declManager->DeclByIndex(DECL_ENTITYDEF, i , false);
        if(!decl)
            continue;
        if(!idStr(decl->GetName()).IcmpPrefix("bot_sabot"))
        {
            valid = true;
            break;
        }
    }
    // check script
    if(valid)
    {
        valid = fileSystem->ReadFile(BOT_SCRIPT_FILE, NULL, NULL) > 0;
    }

#ifdef _MOD_BOTS_ASSETS
    if(valid)
        gameLocal.Printf("BotAI: using external assets\n");
    else if(harm_g_botEnableBuiltinAssets.GetBool()) // using built-in assets if external assets missing
    {
        valid = LoadResource();
        if(valid)
        {
            gameLocal.Printf("BotAI: using built-in assets\n");
            botEnableBuiltinAssets = true;
        }
    }
#endif

    botAvailable = valid;
    botInitialized = true;
    gameLocal.Printf("BotAI initialized: available=%d\n", botAvailable);

    return botAvailable;
}

void botAi::PrepareResource(void)
{
#ifdef _MOD_BOTS_ASSETS
    if(UsingBuiltinAssets())
        ReplaceResource();
#endif
}

#ifdef _MOD_BOTS_ASSETS
bool botAi::ReplaceEntityDefDict(const char *name, const idDict &args)
{
    const idDict *declDict = gameLocal.FindEntityDefDict(name, false);
    if(!declDict)
        return false;

    idDict dict = *declDict;
    dict.Delete("classname");
    dict.Copy(args);

    idStr newName(name);
    newName.Append("_origin");
    // rename old original
    declManager->RenameDecl(DECL_ENTITYDEF, name, newName.c_str());
    // add new
    declManager->AddDeclDef(name, DECL_ENTITYDEF, dict, true);
    return true;
}

bool botAi::SetupEntityDefDict(const char *name, const idDict &args)
{
    const idDict *declDict = gameLocal.FindEntityDefDict(name, false);
    if(!declDict)
        return false;

    int num = args.GetNumKeyVals();
    const idKeyValue *kv;
    for(int i = 0; i < num; i++)
    {
        kv = args.GetKeyVal(i);
        idStr curValue;
        if(declDict->GetString(kv->GetKey(), "", curValue))
        {
            if(!curValue.Cmp(kv->GetValue()))
                continue; // equals
        }
        declManager->EntityDefSet(name, kv->GetKey(), kv->GetValue());
    }
    return true;
}

void botAi::ReplaceResource(void)
{
    // append botaas48 to aas_types, and reload aas_types
    const idDict *declDict = gameLocal.FindEntityDefDict("aas_types", false);
    if (!declDict) {
        gameLocal.Error("Unable to find entityDef for 'aas_types'");
        return;
    }
    const idKeyValue *kv = declDict->MatchPrefix("type");
    int index = 0;
    bool found = false;
    while (kv != NULL) {
        if(!idStr::Icmp(kv->GetValue(), BOT_AAS))
        {
            found = true;
            break;
        }
        idStr indexStr = kv->GetKey().Right(kv->GetKey().Length() - strlen("type"));
        index = atoi(indexStr.c_str());
        kv = declDict->MatchPrefix("type", kv);
    }
    if(found)
    {
        gameLocal.Printf("BotAI: %s found in 'aas_types'\n", BOT_AAS);
    }
    else
    {
        gameLocal.Printf("BotAI: append %s into 'aas_types'\n", BOT_AAS);
        idStr key = va("type%d", index + 1);
        idDict dict;
        dict.Set(key.c_str(), BOT_AAS);
#if 1
        SetupEntityDefDict("aas_types", dict);
#else
        ReplaceEntityDefDict("aas_types", dict);
#endif
    }

    // append botaas48 into info_player_deathmatch, and reload info_player_deathmatch
    gameLocal.Printf("BotAI: append %s into 'info_player_deathmatch'\n", BOT_AAS);
    {
        idDict dict;
        dict.Set("use_aas", BOT_AAS); // TinMan: So runaas compiles the aas for us
        dict.Set("size", "48 48 82"); // TinMan: Must have size set for runaas
        dict.Set("noclipmodel", "1"); // TinMan: ^ size set a clipmodel to bounds size, we don't want that.
#if 1
        SetupEntityDefDict("info_player_deathmatch", dict);
#else
        ReplaceEntityDefDict("info_player_deathmatch", dict);
#endif
    }

    // append botaas48 into info_player_start, and reload info_player_start
    gameLocal.Printf("BotAI: append %s into 'info_player_start'\n", BOT_AAS);
    {
        idDict dict;
#if 1
        dict.Set("use_aas", BOT_AAS); // TinMan: So runaas compiles the aas for us
        dict.Set("size", "48 48 82"); // TinMan: Must have size set for runaas
        dict.Set("noclipmodel", "1"); // TinMan: ^ size set a clipmodel to bounds size, we don't want that.
        SetupEntityDefDict("info_player_start", dict);
#else
        dict.Set("inherit", "info_player_deathmatch"); // TinMan: Much better with all the whatnot I'm adding
        ReplaceEntityDefDict("info_player_start", dict);
#endif
    }

    // append botaas48 into info_player_teleport, and reload info_player_teleport
    gameLocal.Printf("BotAI: append %s into 'info_player_teleport'\n", BOT_AAS);
    {
        idDict dict;
#if 1
        dict.Set("use_aas", BOT_AAS); // TinMan: So runaas compiles the aas for us
        dict.Set("size", "48 48 82"); // TinMan: Must have size set for runaas
        dict.Set("noclipmodel", "1"); // TinMan: ^ size set a clipmodel to bounds size, we don't want that.
        SetupEntityDefDict("info_player_teleport", dict);
#else
        dict.Set("inherit", "info_player_deathmatch"); // TinMan: Much better with all the whatnot I'm adding
        ReplaceEntityDefDict("info_player_teleport", dict);
#endif
    }
}

bool botAi::LoadResource(void)
{
#define BOT_CHECK_DEF(name, dict) \
    decl = declManager->FindType(DECL_ENTITYDEF, name, false); \
    if(decl) { \
        gameLocal.Printf("BotAI: using external entityDef: %s\n", name); \
    } else { \
        gameLocal.Printf("BotAI: using built-in entityDef: %s\n", name); \
        decl = declManager->AddDeclDef(name, DECL_ENTITYDEF, dict); \
    } \
    if(!decl) { \
        gameLocal.Printf("BotAI: entityDef not found: %s\n", name); \
        return false; \
    }

    if(!botAi::harm_g_botEnableBuiltinAssets.GetBool())
        return false;

    ReplaceResource();

    const idDecl *decl;

    // create entity defs
    BOT_CHECK_DEF(BOT_AAS, GetBotAASDef()); // botaas48
    BOT_CHECK_DEF("bot_base", GetBotBaseDef());
    BOT_CHECK_DEF("bot_sabot", GetBotSabotDef());
    BOT_CHECK_DEF("bot_sabot_a8", GetBotSabotA8xDef());
    BOT_CHECK_DEF("bot_sabot_tinman", GetBotSabotTinmanDef());
    BOT_CHECK_DEF("bot_sabot_fluffy", GetBotSabotFluffyDef());
    BOT_CHECK_DEF("bot_sabot_blackstar", GetBotSabotBlackstarDef());
    BOT_CHECK_DEF("bot_names", GetBotSabotNamesDef());
    idList<idDict> levels = GetBotSabotLevelDef();
    for(int i = 0; i < levels.Num(); i++)
    {
        idStr name = va("bot_level%d", i + 1);
        BOT_CHECK_DEF(name.c_str(), levels[i]);
    }

    bool valid = fileSystem->ReadFile(BOT_SCRIPT_FILE, NULL, NULL) > 0;
    if(valid)
    {
        gameLocal.Printf("BotAI: using external script\n");
    }
    else
    {
        gameLocal.Printf("BotAI: using built-in script\n");
        if(!CompileBotScript())
            return false;
    }

    return true;
#undef BOT_CHECK_DEF
}

// From D3XP
bool botAi::CompileBotScript(bool check)
{
    if(check)
    {
        if(!IsAvailable() || !UsingBuiltinAssets())
            return false;
    }

    const char botMainScript[] = "script/bot_main.script";
    idStr source = GetBotMainScript();
    gameLocal.Printf("Compile built-in bot script: %s, %d bytes\n", botMainScript, source.Length());
    bool result = gameLocal.program.CompileText(botMainScript, source.c_str(), false);
    if (g_disasm.GetBool()) {
        gameLocal.program.Disassemble();
    }
    if (!result) {
        gameLocal.Warning("Compile failed in file %s.", botMainScript);
        return false;
    }
    gameLocal.program.FinishCompilation();
    gameLocal.Printf("Compile built-in bot script finish\n");
    return true;
}
#endif

int botAi::GetBotDefs( idStrList &list )
{
    int num;
    int i;
    int res = 0;
    const idDeclEntityDef *decl;

    num = declManager->GetNumDecls(DECL_ENTITYDEF);

    for (i = 0; i < num; i++) {
        decl = (const idDeclEntityDef *)declManager->DeclByIndex(DECL_ENTITYDEF, i , false);
        if(!decl)
            continue;
        if(!idStr(decl->GetName()).IcmpPrefix("bot_sabot"))
        {
            list.Append(decl->GetName());
            res++;
        }
    }

    return res;
}

int botAi::GetBotLevels( idDict &list )
{
    int num;
    int i;
    int res = 0;
    const idDeclEntityDef *decl;

    num = declManager->GetNumDecls(DECL_ENTITYDEF);

    for (i = 0; i < num; i++) {
        decl = (const idDeclEntityDef *)declManager->DeclByIndex(DECL_ENTITYDEF, i , false);
        if(!decl)
            continue;
        if(!idStr(decl->GetName()).IcmpPrefix("bot_level"))
        {
            list.Set(decl->GetName() + strlen("bot_level"), decl->GetName());
            res++;
        }
    }

    return res;
}

int botAi::GetBotLevelData( int level, idDict &ret )
{
    idDict dict;
    int num;
    int botLevel;
    const char *defName;
    const idDeclEntityDef *decl;

    num = GetBotLevels(dict);
    if(num == 0)
        return -1;

    botLevel = level;
    if(botLevel <= 0)
    {
        int n = gameLocal.random.RandomInt(num);
        const idKeyValue *kv = dict.GetKeyVal(n);
        if(kv)
            botLevel = atoi(kv->GetKey());
    }
    defName = dict.GetString(va("%d", botLevel), "");
    if(!defName || !defName[0])
        return -2;

    decl = (const idDeclEntityDef *)declManager->FindType(DECL_ENTITYDEF, defName , false);
    if(!decl)
        return -3;
    ret = decl->dict;
    return botLevel;
}

idStr botAi::GetBotName( int index )
{
    const idDeclEntityDef *decl = (const idDeclEntityDef *)declManager->FindType(DECL_ENTITYDEF, "bot_names" , false);
    if(!decl)
        return idStr();
    if(index > 0)
    {
        const char *name = decl->dict.GetString(va("ui_name%d", index), "");
        if(name && name[0])
            return name;
    }
    else if(index == 0)
    {
        const char *name = decl->dict.GetString("ui_name", "");
        if(name && name[0])
            return name;
        name = decl->dict.GetString("ui_name0", "");
        if(name && name[0])
            return name;
    }
    return decl->dict.RandomPrefix("ui_name", gameLocal.random);
}

void botAi::SetBotLevel(int level)
{
    if(level >= 0)
    {
        idDict botLevelDict;
        int find = GetBotLevelData(level, botLevelDict);
        if(find > 0)
        {
            float fovDegrees;
            botLevelDict.GetFloat( "fov", "90", fovDegrees );
            SetFOV( fovDegrees );

            aimRate			= botLevelDict.GetFloat( "aim_rate", "0.1" );
            aimRate = idMath::ClampFloat(0.1f, 1.0f, aimRate);

			findRadius		= botLevelDict.GetFloat("find_radius", "-1.0");

            botLevel = find;

            return;
        }
    }

    float fovDegrees;
    spawnArgs.GetFloat( "fov", "90", fovDegrees );
    SetFOV( fovDegrees );

    aimRate			= spawnArgs.GetFloat( "aim_rate", "0.1" );
    aimRate = idMath::ClampFloat(0.1f, 1.0f, aimRate);
	findRadius		= spawnArgs.GetFloat("find_radius", "-1.0");

    botLevel = spawnArgs.GetInt( "botLevel", "0" );
}

bool botAi::IsGametypeTeamBased(void)   /* CTF */
{
    switch (gameLocal.gameType)
    {
        case GAME_SP:
        case GAME_DM:
        case GAME_TOURNEY:
        case GAME_LASTMAN:
            return false;
#ifdef CTF
            case GAME_CTF:
#endif
        case GAME_TDM:
            return true;

        default:
            assert(!"Add support for your new gametype here.");
    }

    return false;
}

int botAi::MakeWeaponMask(const char *wp)
{
    if(!idStr::Cmp("*", wp))
        return BOT_ALL_MP_WEAPON;
    idStrList list = idStr::SplitUnique(wp, ',');
    if(list.Num() == 0)
        return 0;
    for(int i = 0; i < list.Num(); i++)
        idStr::StripWhitespace(list[i]);
    return MakeWeaponMask(list);
}

int botAi::MakeWeaponMask(const idStrList &list)
{
#define BOT_MW(I, N, A, U, M) \
	if(list.FindIndex(#I) != -1 || list.FindIndex(N) != -1 || list.FindIndex("weapon_" N) != -1) { \
		i |= (1 << I); \
	}
    int i = 0;

    //BOT_MW(0, "fists", "", 0, 0);
    BOT_MW(1, "pistol", "bullets", 12, 320);
    BOT_MW(2, "shotgun", "shells", 8, 320);
    BOT_MW(3, "machinegun", "clip", 60, 600);
    BOT_MW(4, "chaingun", "belt", 60, 600);
    BOT_MW(5, "handgrenade", "grenades", 5, 50);
    BOT_MW(6, "plasmagun", "cells", 30, 500);
    BOT_MW(7, "rocketlauncher", "rockets", 5, 96);
    BOT_MW(8, "bfg", "bfg", 4, 32);
    BOT_MW(10, "chainsaw", "", 0, 0);
    //BOT_MW(11, "flashlight", "", 0, 0);

    return i;
#undef BOT_MW
}

int botAi::InsertBasicWeaponMask(int i)
{
    // weapon_fists,weapon_pistol,weapon_flashlight,weapon_handgrenade
    i |= 1;
    i |= (1 << 1);
    i |= (1 << 5);
    i |= (1 << 11);
    return i;
}

idStr botAi::MakeWeaponString(int w)
{
#define BOT_MW(I, N, A, U, M) \
	if(w & (1 << I)) { \
		str.Append(",weapon_" N); \
	}
    idStr str;
    if(w == 0)
        return str;

    BOT_MW(0, "fists", "", 0, 0);
    BOT_MW(1, "pistol", "bullets", 12, 320);
    BOT_MW(2, "shotgun", "shells", 8, 320);
    BOT_MW(3, "machinegun", "clip", 60, 600);
    BOT_MW(4, "chaingun", "belt", 60, 600);
    BOT_MW(5, "handgrenade", "grenades", 5, 50);
    BOT_MW(6, "plasmagun", "cells", 30, 500);
    BOT_MW(7, "rocketlauncher", "rockets", 5, 96);
    BOT_MW(8, "bfg", "bfg", 4, 32);
    BOT_MW(10, "chainsaw", "", 0, 0);
    BOT_MW(11, "flashlight", "", 0, 0);

    str.StripLeading(',');
    return str;
#undef BOT_MW
}

idDict botAi::MakeAmmoDict(int w, int num)
{
#define BOT_MW(I, N, A, U, M) \
	if(w & (1 << I)) {\
		dict.SetInt("ammo_" A, num >= 0 ? U * num : M); \
	}
    idDict dict;
    if(w == 0)
        return dict;

    //BOT_MW(0, "fists", "", 0, 0);
    BOT_MW(1, "pistol", "bullets", 12, 320);
    BOT_MW(2, "shotgun", "shells", 8, 320);
    BOT_MW(3, "machinegun", "clip", 60, 600);
    BOT_MW(4, "chaingun", "belt", 60, 600);
    BOT_MW(5, "handgrenade", "grenades", 5, 50);
    BOT_MW(6, "plasmagun", "cells", 30, 500);
    BOT_MW(7, "rocketlauncher", "rockets", 5, 96);
    BOT_MW(8, "bfg", "bfg", 4, 32);
    //BOT_MW(10, "chainsaw", "", 0, 0);
    //BOT_MW(11, "flashlight", "", 0, 0);

    return dict;
#undef BOT_MW
}

void botAi::InsertEmptyAmmo(idDict &dict)
{
#define BOT_MW(I, N, A, U, M) \
	if(!dict.FindKey("ammo_" A)) {\
		dict.SetInt("ammo_" A, 0); \
	}

    //BOT_MW(0, "fists", "", 0, 0);
    BOT_MW(1, "pistol", "bullets", 12, 320);
    BOT_MW(2, "shotgun", "shells", 8, 320);
    BOT_MW(3, "machinegun", "clip", 60, 600);
    BOT_MW(4, "chaingun", "belt", 60, 600);
    BOT_MW(5, "handgrenade", "grenades", 5, 50);
    BOT_MW(6, "plasmagun", "cells", 30, 500);
    BOT_MW(7, "rocketlauncher", "rockets", 5, 96);
    BOT_MW(8, "bfg", "bfg", 4, 32);
    //BOT_MW(10, "chainsaw", "", 0, 0);
    //BOT_MW(11, "flashlight", "", 0, 0);

#undef BOT_MW
}

