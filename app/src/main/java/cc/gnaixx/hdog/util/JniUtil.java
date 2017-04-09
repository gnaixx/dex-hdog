package cc.gnaixx.hdog.util;

/**
 * 名称: JniUtil
 * 描述:
 *
 * @author xiangqing.xue
 * @date 2017/3/31
 */

public class JniUtil {

    static {
        System.loadLibrary("hound-c");
    }

    public static native boolean isRunning(String packageName);
}
