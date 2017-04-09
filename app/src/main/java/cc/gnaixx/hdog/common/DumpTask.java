package cc.gnaixx.hdog.common;

import android.os.Build;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import cc.gnaixx.hdog.util.RootUtil;

import static cc.gnaixx.hdog.common.Constant.SCRIPT_NAME;
import static cc.gnaixx.hdog.common.Constant.scriptStorePath;
import static cc.gnaixx.hdog.common.Constant.TAG;

/**
 * 名称: DumpTask
 * 描述:
 *
 * @author xiangqing.xue
 * @date 2017/4/9
 */

public class DumpTask{
    private Handler handler;
    private String packageName;
    private List<String> dumpeds;

    public DumpTask(Handler handler, String packageName){
        this.handler = handler;
        this.packageName = packageName;
        this.dumpeds = new ArrayList<>();
    }

    public void run(){
        new Thread(new Runnable() {
            @Override
            public void run() {
                for(int i=0; i<10; i++){
                    try {
                        Thread.sleep(200);

                        String cmd = scriptStorePath + File.separator + Build.CPU_ABI + File.separator + SCRIPT_NAME + " " + packageName;
                        String logMsg = RootUtil.execRootCmd(cmd, handler);
                        for (String log : logMsg.split("\n")) {
                            if(log.contains("Dump success")){
                                String dumped = log.split("->")[1];
                                if(!dumpeds.contains(dumped)){
                                    dumpeds.add(dumped);
                                }
                            }
                            Log.d(TAG, log);
                        }

                        if(dumpeds.size() > 1) break;
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                Message msg = handler.obtainMessage();
                msg.what = 1;
                msg.obj = dumpeds;
                handler.sendMessage(msg);
            }
        }).start();
    }
}
