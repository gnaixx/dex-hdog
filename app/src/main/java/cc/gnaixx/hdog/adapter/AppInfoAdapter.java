package cc.gnaixx.hdog.adapter;

import android.content.Context;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.List;

import cc.gnaixx.hdog.R;
import cc.gnaixx.hdog.model.AppInfo;

/**
 * 名称: AppInfoAdapter
 * 描述:
 *
 * @author xiangqing.xue
 * @date 2017/4/9
 */

public class AppInfoAdapter extends SimpleAdapter<AppInfo>{

    public AppInfoAdapter(Context context, List<AppInfo> data) {
        super(context, data);
    }

    @Override
    public int getItemResource() {
        return R.layout.app_info;
    }

    @Override
    public View getItemView(int position, View convertView, ViewHolder viewHolder) {
        ImageView ivIcon = viewHolder.getView(R.id.iv_app_icon);
        TextView tvAppName = viewHolder.getView(R.id.tv_app_name);
        TextView tvPackageName = viewHolder.getView(R.id.tv_app_package);

        AppInfo appInfo = (AppInfo) getItem(position);
        ivIcon.setImageBitmap(appInfo.getIcon());
        tvAppName.setText(appInfo.getAppName());
        tvPackageName.setText(appInfo.getPackageName());
        return convertView;
    }
}
