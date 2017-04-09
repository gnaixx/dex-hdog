package cc.gnaixx.hdog.util;

import android.os.Handler;
import android.os.Message;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

/**
 * 名称: RootUtil
 * 描述:
 *
 * @author xiangqing.xue
 * @date 2017/4/1
 */

public class RootUtil {
    public static String execRootCmd(String cmd){
        return execRootCmd(cmd, null);
    }


    // 执行命令并且输出结果
    public static String execRootCmd(String cmd, Handler handler) {
        String result = "";
        DataOutputStream dos = null;
        DataInputStream dis = null;

        try {
            Process p = Runtime.getRuntime().exec("su");// 经过Root处理的android系统即有su命令
            dos = new DataOutputStream(p.getOutputStream());
            dis = new DataInputStream(p.getInputStream());

            dos.writeBytes(cmd + "\n");
            dos.flush();
            dos.writeBytes("exit\n");
            dos.flush();
            String line = null;
            while ((line = dis.readLine()) != null) {
                result += line + "\n";
                if(handler != null){
                    Message msg = handler.obtainMessage();
                    msg.what = 0;
                    msg.obj = line + "\n";
                    handler.sendMessage(msg);
                }
            }
            p.waitFor();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (dos != null) {
                try {
                    dos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (dis != null) {
                try {
                    dis.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return result;
    }

    public static boolean isRoot(){
        String result = execRootCmd("id");
        if(result.contains("uid=0") && result.contains("root")){
            return true;
        }else{
            return false;
        }
    }
}
