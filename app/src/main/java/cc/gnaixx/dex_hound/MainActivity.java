package cc.gnaixx.dex_hound;

import android.os.Build;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

import cc.gnaixx.dex_hound.util.FileUtil;
import cc.gnaixx.dex_hound.util.JniUtil;
import cc.gnaixx.dex_hound.util.RootUtil;
import cc.gnaixx.dex_hound.util.ZipUtil;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "GNAIXX";

    private static String STORE_PATH;
    private static final String SCRIPT_NAME = "hound-e.so";
    private static final String PACKAGE_NAME = "com.vipbcw.bcwmall";
    //private static final String PACKAGE_NAME = " cc.gnaixx.sample";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        STORE_PATH = this.getCacheDir().getAbsolutePath();
        RootUtil.execRootCmd("");

        Log.d(TAG, JniUtil.hunting("cn.tongdun.android.demo306"));
        //Log.d(TAG, Build.CPU_ABI);
        //Log.d(TAG, RootUtil.isRoot() + "");

        ZipUtil.unzip(this.getPackageCodePath(), SCRIPT_NAME, STORE_PATH);
        FileUtil.createPath("/sdcard/Hdog/" + PACKAGE_NAME);

        Log.d(TAG, RootUtil.execRootCmd("chmod 777 "+ STORE_PATH + "/armeabi/" + SCRIPT_NAME));
        String logMsg = RootUtil.execRootCmd("." + STORE_PATH + "/armeabi/" + SCRIPT_NAME + " " +PACKAGE_NAME);
        for(String log : logMsg.split("\n")) {
            Log.d(TAG, log);
        }
    }
}
