package cc.gnaixx.dex_hound;

import android.os.Build;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

import cc.gnaixx.dex_hound.util.JniUtil;
import cc.gnaixx.dex_hound.util.RootUtil;
import cc.gnaixx.dex_hound.util.ZipUtil;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "GNAIXX";

    private static String STORE_PATH;
    private static final String SCRIPT_NAME = "hound-e.so";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        STORE_PATH = this.getCacheDir().getAbsolutePath();

        Log.d(TAG, JniUtil.hunting("cc.gnaixx.sample"));
        Log.d(TAG, Build.CPU_ABI);

        ZipUtil.unzip(this.getPackageCodePath(), SCRIPT_NAME, STORE_PATH);

        Log.d(TAG, RootUtil.execRootCmd("chmod 777 "+ STORE_PATH + "/armeabi/" + SCRIPT_NAME));
        Log.d(TAG, RootUtil.execRootCmd("." + STORE_PATH + "/armeabi/" + SCRIPT_NAME));
    }
}
