#ifndef __ET_VOICEMACROS_H__
#define __ET_VOICEMACROS_H__

class Client;

typedef enum
{
	VCHAT_NONE,

	// Team messages.
	VCHAT_TEAM_PATHCLEARED,
	VCHAT_TEAM_ENEMYWEAK,
	VCHAT_TEAM_ALLCLEAR,
	VCHAT_TEAM_INCOMING,	
	VCHAT_TEAM_FIREINTHEHOLE,
	VCHAT_TEAM_ONDEFENSE,
	VCHAT_TEAM_ONOFFENSE,
	VCHAT_TEAM_TAKINGFIRE,
	VCHAT_TEAM_MINESCLEARED,
	VCHAT_TEAM_ENEMYDISGUISED,

	VCHAT_TEAM_MEDIC,
	VCHAT_TEAM_NEEDAMMO,
	VCHAT_TEAM_NEEDBACKUP,
	VCHAT_TEAM_NEEDENGINEER,
	VCHAT_TEAM_COVERME,
	VCHAT_TEAM_HOLDFIRE,
	VCHAT_TEAM_WHERETO,
	VCHAT_TEAM_NEEDOPS,

	VCHAT_TEAM_FOLLOWME,
	VCHAT_TEAM_LETGO,
	VCHAT_TEAM_MOVE,
	VCHAT_TEAM_CLEARPATH,
	VCHAT_TEAM_DEFENDOBJECTIVE,
	VCHAT_TEAM_DISARMDYNAMITE,
	VCHAT_TEAM_CLEARMINES,
	VCHAT_TEAM_REINFORCE_OFF,
	VCHAT_TEAM_REINFORCE_DEF,

	VCHAT_TEAM_AFFIRMATIVE,
	VCHAT_TEAM_NEGATIVE,
	VCHAT_TEAM_THANKS,
	VCHAT_TEAM_WELCOME,
	VCHAT_TEAM_SORRY,
	VCHAT_TEAM_OOPS,

	// Command related
	VCHAT_TEAM_COMMANDACKNOWLEDGED,
	VCHAT_TEAM_COMMANDDECLINED,
	VCHAT_TEAM_COMMANDCOMPLETED,
	VCHAT_TEAM_DESTROYPRIMARY,
	VCHAT_TEAM_DESTROYSECONDARY,
	VCHAT_TEAM_DESTROYCONSTRUCTION,
	VCHAT_TEAM_CONSTRUCTIONCOMMENCING,
	VCHAT_TEAM_REPAIRVEHICLE,
	VCHAT_TEAM_DESTROYVEHICLE,
	VCHAT_TEAM_ESCORTVEHICLE,

	VCHAT_IMA_SOLDIER,
	VCHAT_IMA_MEDIC,
	VCHAT_IMA_ENGINEER,
	VCHAT_IMA_FIELDOPS,
	VCHAT_IMA_COVERTOPS,

	VCHAT_TEAM_NUMMESSAGES, // LEAVE THIS AFTER THE TEAM MESSAGES

	// Global messages
	VCHAT_GLOBAL_AFFIRMATIVE,
	VCHAT_GLOBAL_NEGATIVE,
	VCHAT_GLOBAL_ENEMYWEAK,
	VCHAT_GLOBAL_HI,
	VCHAT_GLOBAL_BYE,
	VCHAT_GLOBAL_GREATSHOT,
	VCHAT_GLOBAL_CHEER,

	VCHAT_GLOBAL_THANKS,
	VCHAT_GLOBAL_WELCOME,
	VCHAT_GLOBAL_OOPS,
	VCHAT_GLOBAL_SORRY,
	VCHAT_GLOBAL_HOLDFIRE,
	VCHAT_GLOBAL_GOODGAME,

	VCHAT_GLOBAL_NUMMESSAGES, // LEAVE THIS AFTER THE GLOBAL MESSAGES

	// NoQuarter mod private voice chats
	VCHAT_PRIVATE_DEPLOYMORTAR,
	VCHAT_PRIVATE_HEALSQUAD,
	VCHAT_PRIVATE_HEALME,
	VCHAT_PRIVATE_REVIVETEAMMATE,
	VCHAT_PRIVATE_REVIVEME,
	VCHAT_PRIVATE_DESTROYOBJECTIVE,
	VCHAT_PRIVATE_REPAIROBJECTIVE,
	VCHAT_PRIVATE_CONSTRUCTOBJECTIVE,
	VCHAT_PRIVATE_DEPLOYLANDMINES,
	VCHAT_PRIVATE_DISARMLANDMINES,
	VCHAT_PRIVATE_CALLAIRSTRIKE,
	VCHAT_PRIVATE_CALLARTILLERY,
	VCHAT_PRIVATE_RESUPPLYSQUAD,
	VCHAT_PRIVATE_RESUPPLYME,
	VCHAT_PRIVATE_EXPLOREAREA,
	VCHAT_PRIVATE_CHECKFORLANDMINES,
	VCHAT_PRIVATE_SATCHELOBJECTIVE,
	VCHAT_PRIVATE_INFILTRATE,
	VCHAT_PRIVATE_GOUNDERCOVER,
	VCHAT_PRIVATE_PROVIDESNIPERCOVER,
	VCHAT_PRIVATE_ATTACK,
	VCHAT_PRIVATE_FALLBACK,

	VCHAT_PRIVATE_NUMMESSAGES, // LEAVE THIS AFTER THE PRIVATE MESSAGES

	// This must stay last.
	NUM_ET_VCHATS
} eVChat;

// class: ET_VoiceMacros
class ET_VoiceMacros
{
public:

	static int GetVChatId(const char *_string);
	static void SendVoiceMacro(Client *_bot, int _msg);

protected:
};

#endif
