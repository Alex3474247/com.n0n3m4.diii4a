package com.karin.idTech4Amm.sys;

import com.karin.idTech4Amm.misc.TextHelper;

import java.util.Arrays;

/**
 * Constants define
 */
public final class Constants
{
    public static final int CONST_UPDATE_RELEASE = 33;
    public static final String CONST_RELEASE = "2023-10-01";
    public static final String CONST_EMAIL = "beyondk2000@gmail.com";
    public static final String CONST_DEV = "Karin";
    public static final String CONST_CODE = "Harmattan";
    public static final String CONST_APP_NAME = "idTech4A++"; // "DIII4A++";
    public static final String CONST_NAME = "DOOM III/Quake 4/Prey(2006) for Android(Harmattan Edition)";
	public static final String CONST_MAIN_PAGE = "https://github.com/glKarin/com.n0n3m4.diii4a";
    public static final String CONST_TIEBA = "https://tieba.baidu.com/p/6825594793";
	public static final String CONST_DEVELOPER = "https://github.com/glKarin";
    public static final String CONST_DEVELOPER_XDA = "https://forum.xda-developers.com/member.php?u=10584229";
    public static final String CONST_PACKAGE = "com.karin.idTech4Amm";
    public static final String CONST_FDROID = "https://f-droid.org/packages/com.karin.idTech4Amm/";
	public static final String CONST_CHECK_FOR_UPDATE_URL = "https://raw.githubusercontent.com/glKarin/com.n0n3m4.diii4a/master/CHECK_FOR_UPDATE.json";
    public static final String CONST_LICENSE_URL = "https://raw.githubusercontent.com/glKarin/com.n0n3m4.diii4a/master/LICENSE";
	public static final String[] CONST_CHANGES = {
            "Add shadow mapping soft shadow support(testing, has some incorrect rendering), using `r_useShadowMapping` to change from `shadow mapping` or `stencil shadow`.",
            "In Quake4, remove Bot FakeClient in multiplayer-game, and add SABot-a9 mod support in multiplayer-game(need extract resource first).",
            "Fix Setting's tab GUI in Prey2006.",
            "Add `full-body awareness` mod in Quake4. Set bool cvar `harm_pm_fullBodyAwareness` to 1 enable, and using `harm_pm_fullBodyAwarenessOffset` setup offset(also change to third-person mode), and using `harm_pm_fullBodyAwarenessHeadJoint` setup head joint name(view position).",
            "Support max FPS limit(cvar `harm_r_maxFPS`).",
            "Support obj/dae static model, and fix png image load.",
            "Add skip intro support.",
	        "Add simple CVar editor.",
            "Change OpenGL vertex index size to 4 bytes for large model.",
            "Add GLES3.0 support, can choose in `Graphics` tab.",
	};

	// Launcher preference keys
    public static final class PreferenceKey {
        public static final String LAUNCHER_ORIENTATION = "harm_launcher_orientation";
        public static final String HIDE_AD_BAR = "harm_hide_ad_bar";
		//public static final String OPEN_QUAKE4_HELPER = "harm_open_quake4_helper";
        
        private PreferenceKey() {}
    }
    
	private Constants() {}
}
