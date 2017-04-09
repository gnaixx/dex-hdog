package cc.gnaixx.hdog;

import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.TextView;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import cc.gnaixx.hdog.adapter.AppInfoAdapter;
import cc.gnaixx.hdog.model.AppInfo;
import cc.gnaixx.hdog.util.FileUtil;
import cc.gnaixx.hdog.util.RootUtil;
import cc.gnaixx.hdog.util.ZipUtil;

import static cc.gnaixx.hdog.common.Constant.SCRIPT_NAME;
import static cc.gnaixx.hdog.common.Constant.HDOG_PATH;
import static cc.gnaixx.hdog.common.Constant.TAG;
import static cc.gnaixx.hdog.common.Constant.scriptStorePath;

public class MainActivity extends AppCompatActivity implements ListView.OnItemClickListener{


    private ListView lvAppInfo;
    private TextView tvRootStatus;
    private boolean isRoot;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        lvAppInfo = (ListView) findViewById(R.id.lv_app_info);
        tvRootStatus = (TextView) findViewById(R.id.tv_root_status);
        showAppInfo();

        init();
    }

    private void init(){
        isRoot = RootUtil.isRoot();
        if(!isRoot){
            tvRootStatus.setVisibility(View.VISIBLE);
            return;
        }

        scriptStorePath = this.getCacheDir().getAbsolutePath();
        ZipUtil.unzip(this.getPackageCodePath(), SCRIPT_NAME, scriptStorePath);
        String cmd = "chmod 777 " + scriptStorePath + File.separator + Build.CPU_ABI + File.separator + SCRIPT_NAME;
        RootUtil.execRootCmd(cmd);
    }


    private void showAppInfo() {
        List<AppInfo> appInfos = new ArrayList<>();
        PackageManager packageMgr = this.getPackageManager();
        List<ApplicationInfo> applicationInfos = packageMgr.getInstalledApplications(0);

        for(ApplicationInfo applicationInfo : applicationInfos){
            String appName = packageMgr.getApplicationLabel(applicationInfo).toString();
            String packageName = applicationInfo.packageName;
            Drawable icon = applicationInfo.loadIcon(packageMgr);
            int flags = applicationInfo.flags;
            AppInfo appinfo = new AppInfo(appName, packageName, icon);
            if((flags & ApplicationInfo.FLAG_SYSTEM) != 1 && !packageName.equals(this.getPackageName())) {
                appInfos.add(appinfo);
            }
        }

        AppInfoAdapter adapter = new AppInfoAdapter(this, appInfos);
        lvAppInfo.setAdapter(adapter);
        lvAppInfo.setOnItemClickListener(this);
        adapter.notifyDataSetChanged();
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
        ListView listView = (ListView) parent;
        AppInfo appInfo = (AppInfo) listView.getItemAtPosition(position);
        String packageName = appInfo.getPackageName();
        Log.d(TAG, packageName);

        FileUtil.createPath(HDOG_PATH + packageName + "/dex");
        FileUtil.createPath(HDOG_PATH+ packageName + "/dey");
        String cmd = scriptStorePath + File.separator + Build.CPU_ABI + File.separator + SCRIPT_NAME + " " + packageName;
        String logMsg = RootUtil.execRootCmd(cmd);
        for (String log : logMsg.split("\n")) {
            Log.d(TAG, log);
        }
    }
}
