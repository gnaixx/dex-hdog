package cc.gnaixx.dex_hound.util;

import android.util.Log;

import java.io.File;

/**
 * 名称: FileUtil
 * 描述:
 *
 * @author xiangqing.xue
 * @date 2017/4/6
 */

public class FileUtil {
    public static boolean createPath(String pathName){
        File file = new File(pathName);
        if(!file.exists()){
            boolean suc = file.mkdirs();
            return suc;
        }else{
            return true;
        }
    }
}
