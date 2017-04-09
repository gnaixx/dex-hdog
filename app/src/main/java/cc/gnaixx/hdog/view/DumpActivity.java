package cc.gnaixx.hdog.view;

import android.os.Build;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.widget.TextView;

import java.io.File;
import java.util.List;

import cc.gnaixx.hdog.R;
import cc.gnaixx.hdog.common.DumpTask;

import static cc.gnaixx.hdog.common.Constant.SCRIPT_NAME;
import static cc.gnaixx.hdog.common.Constant.scriptStorePath;

public class DumpActivity extends AppCompatActivity {

    private TextView tvLog;
    private TextView tvResult;

    private Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            int what = msg.what;
            if (what == 0) {
                tvLog.append((String) msg.obj);
                int offset = tvLog.getLineCount() * tvLog.getLineHeight();
                if (offset - tvLog.getHeight() > 0) {
                    tvLog.scrollTo(0, offset - tvLog.getHeight());
                }
            } else if (what == 1) {
                List<String> dumpeds = (List<String>) msg.obj;
                for (int i = 0; i < dumpeds.size(); i++) {
                    String dumped = dumpeds.get(i);
                    dumped = dumped.replaceAll(" ", "");
                    tvResult.append(">>[" + (i+1) + "]<<" + dumped + "\n\n");
                }
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_dump);

        tvLog = (TextView) findViewById(R.id.tv_log);
        tvLog.setMovementMethod(ScrollingMovementMethod.getInstance());
        tvResult = (TextView) findViewById(R.id.tv_res);
        tvResult.setMovementMethod(ScrollingMovementMethod.getInstance());

        String packageName = getIntent().getStringExtra("package_name");
        new DumpTask(mHandler, packageName).run();
    }
}
