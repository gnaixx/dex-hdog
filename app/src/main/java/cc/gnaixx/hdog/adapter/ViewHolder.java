package cc.gnaixx.hdog.adapter;

import android.util.SparseArray;
import android.view.View;

/**
 * 名称: ViewHolder
 * 描述:
 *
 * @author xiangqing.xue
 * @date 16/8/24
 */
public class ViewHolder {
    private SparseArray<View> views = new SparseArray<>();
    private View convertView;

    public ViewHolder(View view) {
        this.convertView = view;
    }

    public <T extends View> T getView(int resId) {
        View view = views.get(resId);
        if (view == null) {
            view = convertView.findViewById(resId);
            views.put(resId, view);
        }
        return (T) view;
    }
}
