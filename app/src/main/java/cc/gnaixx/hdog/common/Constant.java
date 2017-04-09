package cc.gnaixx.hdog.common;

import android.os.Environment;

/**
 * 名称: Constant
 * 描述:
 *
 * @author xiangqing.xue
 * @date 2017/4/9
 */

public class Constant {

    public static final String SCRIPT_NAME = "hound-e.so";
    public static final String HDOG_PATH   = Environment.getExternalStorageDirectory().getAbsolutePath() + "/Hdog";
    public static final String TYPE_DEX    = "dex";
    public static final String TYPE_DEY    = "dey";

    public static final String TAG         = "GNAIXX";

    public static String scriptStorePath;
}
