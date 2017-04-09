package cc.gnaixx.hdog.model;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;

/**
 * 名称: AppInfo
 * 描述:
 *
 * @author xiangqing.xue
 * @date 2017/4/9
 */

public class AppInfo {
    private String appName;
    private String packageName;
    private Bitmap icon;


    public AppInfo(String appName, String packageName, Drawable icon){
        setAppName(appName);
        setPackageName(packageName);
        setIcon(icon);
    }

    public String getAppName() {
        return appName;
    }

    public void setAppName(String appName) {
        this.appName = appName;
    }

    public String getPackageName() {
        return packageName;
    }

    public void setPackageName(String packageName) {
        this.packageName = packageName;
    }

    public Bitmap getIcon() {
        return icon;
    }

    public void setIcon(Drawable drawable) {
        if (drawable instanceof BitmapDrawable) {
            this.icon = ((BitmapDrawable)drawable).getBitmap();
        }

        Bitmap bitmap = Bitmap.createBitmap(drawable.getIntrinsicWidth(), drawable.getIntrinsicHeight(), Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(bitmap);
        drawable.setBounds(0, 0, canvas.getWidth(), canvas.getHeight());
        drawable.draw(canvas);
        this.icon = bitmap;
    }
}
