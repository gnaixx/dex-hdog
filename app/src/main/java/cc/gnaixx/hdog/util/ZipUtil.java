package cc.gnaixx.hdog.util;

import android.util.Log;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

public class ZipUtil{

    public static void unzip(String targetZipName, String targetName, String output){
        File entryPath;
        BufferedInputStream bis;
        BufferedOutputStream bos;
        byte[] buff = new byte[1024];
        try {
            ZipFile zipFile = new ZipFile(targetZipName);
            Enumeration<ZipEntry> zipEntrys = (Enumeration<ZipEntry>) zipFile.entries();
            ZipEntry entry;
            while(zipEntrys.hasMoreElements()){
                entry = zipEntrys.nextElement();
                String unzipPath = entry.getName();
                if(unzipPath.endsWith(targetName)){
                    unzipPath = unzipPath.replace("lib", output);
                    entryPath = new File(unzipPath.replace(targetName, ""));
                    if(!entryPath.exists()){
                        boolean suc = entryPath.mkdirs();
                        Log.d("GNAIXX", suc + "");
                    }

                    bos = new BufferedOutputStream(new FileOutputStream(unzipPath));
                    bis = new BufferedInputStream(zipFile.getInputStream(entry));
                    while(bis.read(buff, 0, buff.length) != -1){
                        bos.write(buff);
                    }
                    bos.flush();
                    bos.close();
                    bis.close();
                }
            }
            zipFile.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}