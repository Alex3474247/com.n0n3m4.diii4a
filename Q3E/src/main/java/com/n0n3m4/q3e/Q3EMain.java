/*
	Copyright (C) 2012 n0n3m4
	
    This file is part of Q3E.

    Q3E is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    Q3E is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Q3E.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.n0n3m4.q3e;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.graphics.Typeface;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.RelativeLayout;
import android.widget.Toast;

import com.n0n3m4.q3e.device.Q3EOuya;
import com.n0n3m4.q3e.gl.Q3EGL;
import com.n0n3m4.q3e.karin.KDebugTextView;
import com.n0n3m4.q3e.karin.KUncaughtExceptionHandler;
import com.n0n3m4.q3e.karin.KidTech4Command;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

public class Q3EMain extends Activity
{
    public static Q3ECallbackObj mAudio;
    public static Q3EView mGLSurfaceView;
    public static String datadir;
    // k
    private boolean m_hideNav = true;
    private int m_runBackground = 1;
    private int m_renderMemStatus = 0;
    public Q3EControlView mControlGLSurfaceView;
    private KDebugTextView memoryUsageText;
    private boolean m_coverEdges = true;

    public void ShowMessage(String s)
    {
        Toast.makeText(this, s, Toast.LENGTH_LONG).show();
    }

    public boolean checkGameFiles()
    {
        if (!new File(datadir).exists())
        {
            ShowMessage(Q3ELang.tr(this, R.string.game_files_weren_t_found_put_game_files_to) + datadir);
            this.finish();
            return false;
        }

        return true;
    }

    @SuppressLint("ResourceType")
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        requestWindowFeature(Window.FEATURE_NO_TITLE);

        KUncaughtExceptionHandler.HandleUnexpectedException(this);
        InitGlobalEnv();
        SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(this);

        Q3EGL.usegles20 = false; // Q3EUtils.q3ei.isD3 || Q3EUtils.q3ei.isQ1 || Q3EUtils.q3ei.isD3BFG;
        m_coverEdges = preferences.getBoolean(Q3EPreference.COVER_EDGES, true);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P && m_coverEdges)
        {
            WindowManager.LayoutParams lp = getWindow().getAttributes();
            lp.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            getWindow().setAttributes(lp);
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.GINGERBREAD) // 9
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);

        m_hideNav = preferences.getBoolean(Q3EPreference.HIDE_NAVIGATION_BAR, true);
        m_renderMemStatus = preferences.getInt(Q3EPreference.RENDER_MEM_STATUS, 0);
        String harm_run_background = preferences.getString(Q3EPreference.RUN_BACKGROUND, "1");
        if (null != harm_run_background)
            m_runBackground = Integer.parseInt(harm_run_background);
        else
            m_runBackground = 1;
        //k
        SetupUIFlags();
/*        if(m_hideNav)
        {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
				final View decorView = getWindow().getDecorView();
				decorView.setSystemUiVisibility(m_uiOptions);// This code will always hide the navigation bar
            decorView.setOnSystemUiVisibilityChangeListener(new View.OnSystemUiVisibilityChangeListener(){
                    @Override
                    public void  onSystemUiVisibilityChange(int visibility)
                    {
                        if((visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0)
                        {
                            decorView.setSystemUiVisibility(m_uiOptions);
                        }
                    }
                });
			}
        }*/

        super.onCreate(savedInstanceState);
        Q3ELang.Locale(this);
		
/*//k		if (Q3EUtils.q3ei==null)
		{			
			finish();
			try
			{
				//k startActivity(new Intent(this,Class.forName(getPackageName()+".GameLauncher")));//Dirty hack	
				startActivity(new Intent(this,Class.forName("com.n0n3m4.DIII4A.GameLauncher")));//Dirty hack
			}			
			catch (Exception e){e.printStackTrace();};			
			return;
		}*/

        if (checkGameFiles())
        {
            if (!Q3EOuya.Init(this))
                Q3EUtils.isOuya = false;

            if (mAudio == null)
            {
                mAudio = new Q3ECallbackObj();
            }
            Q3EUtils.q3ei.callbackObj = mAudio;
            Q3EJNI.setCallbackObject(mAudio);
            if (mGLSurfaceView == null)
                mGLSurfaceView = new Q3EView(this);
            if (mControlGLSurfaceView == null)
                mControlGLSurfaceView = new Q3EControlView(this);
            mAudio.vw = mControlGLSurfaceView;
            mControlGLSurfaceView.EnableGyroscopeControl(Q3EUtils.q3ei.view_motion_control_gyro);
            float gyroXSens = preferences.getFloat(Q3EPreference.pref_harm_view_motion_gyro_x_axis_sens, Q3EControlView.GYROSCOPE_X_AXIS_SENS);
            float gyroYSens = preferences.getFloat(Q3EPreference.pref_harm_view_motion_gyro_y_axis_sens, Q3EControlView.GYROSCOPE_Y_AXIS_SENS);
            if (Q3EUtils.q3ei.view_motion_control_gyro && (gyroXSens != 0.0f || gyroYSens != 0.0f))
                mControlGLSurfaceView.SetGyroscopeSens(gyroXSens, gyroYSens);
            mControlGLSurfaceView.RenderView(mGLSurfaceView);
            RelativeLayout mainLayout = new RelativeLayout(this);
            RelativeLayout.LayoutParams params;

            params = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);

            mainLayout.addView(mGLSurfaceView, params);

            //mControlGLSurfaceView.setZOrderOnTop();
            mControlGLSurfaceView.setZOrderMediaOverlay(true);
            mainLayout.addView(mControlGLSurfaceView, params);

            if (Q3EUtils.q3ei.function_key_toolbar)
            {
                params = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, getResources().getDimensionPixelSize(R.dimen.toolbarHeight));
                View key_toolbar = mControlGLSurfaceView.CreateToolbar();
                mainLayout.addView(key_toolbar, params);
            }

            if (m_renderMemStatus > 0) //k
            {
                memoryUsageText = new KDebugTextView(mainLayout.getContext());
                params = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
                mainLayout.addView(memoryUsageText, params);
                memoryUsageText.setTypeface(Typeface.MONOSPACE);
            }

            setContentView(mainLayout);

            mControlGLSurfaceView.requestFocus();
        }
        else
        {
            finish();
        }
    }

    @Override
    public void onAttachedToWindow()
    {
        super.onAttachedToWindow();

        if (mControlGLSurfaceView != null)
        {
            View toolbar = mControlGLSurfaceView.Toolbar();
            if (toolbar != null)
            {
                if (m_coverEdges)
                {
                    int x = Q3EUtils.GetEdgeHeight(this, true);
                    if (x != 0)
                        toolbar.setX(x);
                }
                int[] size = Q3EUtils.GetNormalScreenSize(this);
                ViewGroup.LayoutParams layoutParams = toolbar.getLayoutParams();
                layoutParams.width = size[0];
                toolbar.setLayoutParams(layoutParams);
                //mainLayout.requestLayout();
            }
        }
    }

    @Override
    protected void onDestroy()
    {
        if (null != mGLSurfaceView)
            mGLSurfaceView.Shutdown();
        super.onDestroy();
        if (null != mAudio)
            mAudio.OnDestroy();
    }

    @Override
    protected void onPause()
    {
        super.onPause();

        //k
        if (memoryUsageText != null)
            memoryUsageText.Stop();

        if (m_runBackground < 2)
            if (mAudio != null)
            {
                mAudio.pause();
            }

        if (mGLSurfaceView != null)
        {
            mGLSurfaceView.Pause();
        }

        if (mControlGLSurfaceView != null)
        {
            mControlGLSurfaceView.Pause();
        }
        Q3EUtils.CloseVKB();
    }

    @Override
    protected void onResume()
    {
        super.onResume();

        //k
        if (memoryUsageText != null/* && m_renderMemStatus > 0*/)
            memoryUsageText.Start(m_renderMemStatus * 1000);

        //k if(m_runBackground < 1)
        if (mGLSurfaceView != null)
        {
            mGLSurfaceView.Resume();
        }
        if (mControlGLSurfaceView != null)
        {
            mControlGLSurfaceView.Resume();
        }

        //k if(m_runBackground < 2)
        if (mAudio != null)
        {
            mAudio.resume();
        }
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus)
    {
        super.onWindowFocusChanged(hasFocus);
        SetupUIFlags();
    }

    private void SetupUIFlags()
    {
        final View decorView = getWindow().getDecorView();
        if (m_hideNav)
            decorView.setSystemUiVisibility(Q3EUtils.UI_FULLSCREEN_HIDE_NAV_OPTIONS);
        else
            decorView.setSystemUiVisibility(Q3EUtils.UI_FULLSCREEN_OPTIONS);
    }

    private void InitGlobalEnv()
    {
        SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(this);

        if (!Q3EUtils.q3ei.IsInitGame()) // not from GameLauncher::startActivity
        {
            Q3EKeyCodes.InitD3Keycodes();

            Q3EUtils.q3ei.InitD3();

            Q3EUtils.q3ei.InitDefaultsTable();

            Q3EUtils.q3ei.default_path = Environment.getExternalStorageDirectory() + "/diii4a";

            Q3EUtils.q3ei.SetupGame(preferences.getString(Q3EPreference.pref_harm_game, Q3EGlobals.GAME_DOOM3));

            Q3EUtils.q3ei.LoadTypeAndArgTablePreference(this);

            String extraCommand = "";
            if (preferences.getBoolean(Q3EPreference.pref_harm_skip_intro, false))
                extraCommand += " +disconnect";
            if(Q3EUtils.q3ei.IsIdTech4())
            {
                if (preferences.getBoolean(Q3EPreference.pref_harm_auto_quick_load, false))
                    extraCommand += " +loadGame QuickSave";
            }
            Q3EUtils.q3ei.start_temporary_extra_command = extraCommand.trim();
        }
        Log.i(Q3EGlobals.CONST_Q3E_LOG_TAG, "Run " + Q3EUtils.q3ei.game_name);

        Q3EUtils.q3ei.joystick_release_range = preferences.getFloat(Q3EPreference.pref_harm_joystick_release_range, 0.0f);
        Q3EUtils.q3ei.joystick_inner_dead_zone = preferences.getFloat(Q3EPreference.pref_harm_joystick_inner_dead_zone, 0.0f);
        Q3EUtils.q3ei.SetupEngineLib(); //k setup engine library here again
        Q3EUtils.q3ei.view_motion_control_gyro = preferences.getBoolean(Q3EPreference.pref_harm_view_motion_control_gyro, false);
        Q3EUtils.q3ei.multithread = preferences.getBoolean(Q3EPreference.pref_harm_multithreading, false);
        Q3EUtils.q3ei.function_key_toolbar = preferences.getBoolean(Q3EPreference.pref_harm_function_key_toolbar, true);
        Q3EUtils.q3ei.joystick_unfixed = preferences.getBoolean(Q3EPreference.pref_harm_joystick_unfixed, false);
        Q3EUtils.q3ei.joystick_smooth = preferences.getBoolean(Q3EPreference.pref_analog, true);
        Q3EUtils.q3ei.VOLUME_UP_KEY_CODE = Q3EKeyCodes.GetRealKeyCode(preferences.getInt(Q3EPreference.VOLUME_UP_KEY, Q3EKeyCodes.KeyCodesGeneric.K_F3));
        Q3EUtils.q3ei.VOLUME_DOWN_KEY_CODE = Q3EKeyCodes.GetRealKeyCode(preferences.getInt(Q3EPreference.VOLUME_DOWN_KEY, Q3EKeyCodes.KeyCodesGeneric.K_F2));
        // DOOM 3: hardscorps mod template disable smooth joystick
        /*if(Q3EUtils.q3ei.joystick_smooth)
        {
            if(!Q3EUtils.q3ei.isQ4 && !Q3EUtils.q3ei.isPrey && !Q3EUtils.q3ei.isQ2)
            {
                String game = preferences.getString(Q3EUtils.q3ei.GetGameModPreferenceKey(), "");
                if("hardscorps".equals(game))
                    Q3EUtils.q3ei.joystick_smooth = false;
            }
        }*/

        Q3EUtils.q3ei.SetAppStoragePath(this);

        datadir = preferences.getString(Q3EPreference.pref_datapath, Q3EUtils.q3ei.default_path);
        if (null == datadir)
            datadir = Q3EUtils.q3ei.default_path;
        if ((datadir.length() > 0) && (datadir.charAt(0) != '/'))//lolwtfisuserdoing?
        {
            datadir = "/" + datadir;
            preferences.edit().putString(Q3EPreference.pref_datapath, datadir).commit();
        }

        String cmd = preferences.getString(Q3EUtils.q3ei.GetGameCommandPreferenceKey(), Q3EUtils.q3ei.libname);
        if(null == cmd)
            cmd = Q3EGlobals.GAME_EXECUABLE;
        if(preferences.getBoolean(Q3EPreference.pref_harm_find_dll, false))
        {
            KidTech4Command command = new KidTech4Command(cmd);
            String fs_game = command.Prop("fs_game");
            if(null == fs_game || fs_game.isEmpty())
            {
                switch (Q3EUtils.q3ei.game)
                {
                    case Q3EGlobals.GAME_PREY:
                        fs_game = "preybase";
                        break;
                    case Q3EGlobals.GAME_QUAKE4:
                        fs_game = "q4base";
                        break;
                    case Q3EGlobals.GAME_QUAKE2:
                        fs_game = "baseq2";
                        break;
                    case Q3EGlobals.GAME_DOOM3:
                    default:
                        fs_game = "base";
                        break;
                }
            }
            String dll = FindDLL(fs_game);
            if(null != dll)
                command.SetProp("harm_fs_gameLibPath", dll);
            cmd = command.toString();
        }
        cmd = datadir + "/" + cmd + " " + Q3EUtils.q3ei.start_temporary_extra_command/* + " +set harm_fs_gameLibDir " + lib_dir*/;
        Q3EUtils.q3ei.cmd = cmd;
    }

    private String FindDLL(String fs_game)
    {
        String DLLPath = datadir + File.separator + fs_game + File.separator; // /sdcard/diii4a/<fs_game>
        String Suffix = "game" + Q3EJNI.ARCH + ".so"; // gameaarch64.so(64) / gamearm.so(32)
        String[] guess = {
                Suffix,
                "lib" + Suffix,
        };
        String res = null;
        String targetDir = getCacheDir() + File.separator; // /data/user/<package_name>/cache/
        for (String f : guess)
        {
            String p = DLLPath + f;
            File file = new File(p);
            if(!file.isFile() || !file.canRead())
                continue;
            Log.i(Q3EGlobals.CONST_Q3E_LOG_TAG, "Found user game library file: " + p);
            String cacheFile = targetDir + Suffix;
            long r = Q3EUtils.cp(p, cacheFile);
            if(r > 0)
            {
                Log.i(Q3EGlobals.CONST_Q3E_LOG_TAG, "Load user game library: " + cacheFile);
                res = cacheFile;
            }
            else
                Log.e(Q3EGlobals.CONST_Q3E_LOG_TAG, "Upload user game library fail: " + cacheFile);
        }
        return res;
    }
}
