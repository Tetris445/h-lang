package com.example.happ;

import android.app.Activity;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.content.Context;
import android.content.Intent;
import android.hardware.camera2.CameraManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.os.Vibrator;
import android.provider.Settings;
import android.webkit.JavascriptInterface;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.webkit.WebChromeClient;
import android.webkit.JsResult;
import android.webkit.JsPromptResult;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

public class MainActivity extends Activity {

    private WebView webView;
    public static int notifyIcon = android.R.drawable.ic_dialog_info;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        requestAppPermissions();
        createNotificationChannel();

        webView = new WebView(this);
        webView.getSettings().setJavaScriptEnabled(true);
        webView.getSettings().setAllowFileAccess(true);
        webView.setWebViewClient(new WebViewClient());

        webView.addJavascriptInterface(new AndroidBridge(this, webView), "Android");

        webView.setWebChromeClient(new WebChromeClient() {
            @Override
            public boolean onJsAlert(WebView view, String url, String message, JsResult result) {
                return super.onJsAlert(view, url, message, result);
            }
            @Override
            public boolean onJsPrompt(WebView view, String url, String message, String defaultValue, JsPromptResult result) {
                return super.onJsPrompt(view, url, message, defaultValue, result);
            }
            @Override
            public boolean onJsConfirm(WebView view, String url, String message, JsResult result) {
                return super.onJsConfirm(view, url, message, result);
            }
        });

        webView.loadUrl("file:///android_asset/index.html");
        setContentView(webView);
    }

    private void requestAppPermissions() {
        if (Build.VERSION.SDK_INT >= 33) {
            if (checkSelfPermission("android.permission.POST_NOTIFICATIONS")
                    != android.content.pm.PackageManager.PERMISSION_GRANTED) {
                requestPermissions(new String[]{"android.permission.POST_NOTIFICATIONS"}, 100);
            }
        }
        if (Build.VERSION.SDK_INT >= 30) {
            if (!Environment.isExternalStorageManager()) {
                try {
                    Intent intent = new Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION);
                    intent.setData(Uri.parse("package:" + getPackageName()));
                    startActivity(intent);
                } catch (Exception e) {
                    Intent intent = new Intent(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION);
                    startActivity(intent);
                }
            }
        }
    }

    private void createNotificationChannel() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            NotificationChannel channel = new NotificationChannel(
                    "happ_channel",
                    "hApp",
                    NotificationManager.IMPORTANCE_DEFAULT
            );
            NotificationManager manager = getSystemService(NotificationManager.class);
            if (manager != null) manager.createNotificationChannel(channel);
        }
    }

    public static class AndroidBridge {
        private final Context context;
        private final WebView webView;
        private CameraManager cameraManager;
        private String cameraId;

        public AndroidBridge(Context context, WebView webView) {
            this.context = context;
            this.webView = webView;
        }

        // ============ ФОНАРИК ============
        @JavascriptInterface
        public void flash(final String action) {
            new Handler(Looper.getMainLooper()).post(new Runnable() {
                @Override public void run() {
                    try {
                        if (cameraManager == null) {
                            cameraManager = (CameraManager) context.getSystemService(Context.CAMERA_SERVICE);
                            cameraId = cameraManager.getCameraIdList()[0];
                        }
                        if (action.equals("on")) {
                            cameraManager.setTorchMode(cameraId, true);
                        } else if (action.equals("off")) {
                            cameraManager.setTorchMode(cameraId, false);
                        }
                    } catch (Exception e) {
                        Toast.makeText(context, "Ошибка фонарика: " + e.getMessage(), Toast.LENGTH_SHORT).show();
                    }
                }
            });
        }

        // ============ ИКОНКА УВЕДОМЛЕНИЯ ============
        @JavascriptInterface
        public void setNotifyIcon(final String name) {
            new Handler(Looper.getMainLooper()).post(new Runnable() {
                @Override public void run() {
                    if (name.equals("default")) {
                        notifyIcon = android.R.drawable.ic_dialog_info;
                        Toast.makeText(context, "Иконка: стандартная", Toast.LENGTH_SHORT).show();
                    } else {
                        int resId = context.getResources().getIdentifier(
                                name, "drawable", context.getPackageName());
                        if (resId != 0) {
                            notifyIcon = resId;
                            Toast.makeText(context, "Иконка: " + name, Toast.LENGTH_SHORT).show();
                        } else {
                            Toast.makeText(context, "Не найдена: " + name, Toast.LENGTH_SHORT).show();
                        }
                    }
                }
            });
        }

        // ============ УВЕДОМЛЕНИЕ ============
        @JavascriptInterface
        public void notify(final String title, final String text) {
            new Handler(Looper.getMainLooper()).post(new Runnable() {
                @Override public void run() {
                    try {
                        NotificationManager manager =
                                (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
                        Notification.Builder builder;
                        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                            builder = new Notification.Builder(context, "happ_channel");
                        } else {
                            builder = new Notification.Builder(context);
                        }
                        builder.setContentTitle(title)
                               .setContentText(text)
                               .setSmallIcon(notifyIcon)
                               .setAutoCancel(true);
                        manager.notify((int) System.currentTimeMillis(), builder.build());
                    } catch (Exception e) {
                        Toast.makeText(context, "Ошибка: " + e.getMessage(), Toast.LENGTH_LONG).show();
                    }
                }
            });
        }

        // ============ ФАЙЛЫ ============
        @JavascriptInterface
        public void saveFile(final String filename, final String content) {
            new Handler(Looper.getMainLooper()).post(new Runnable() {
                @Override public void run() {
                    try {
                        File dir = new File(Environment.getExternalStorageDirectory(), "hApp/hfiles");
                        if (!dir.exists()) dir.mkdirs();
                        FileOutputStream fos = new FileOutputStream(new File(dir, filename));
                        fos.write(content.getBytes("UTF-8"));
                        fos.close();
                        Toast.makeText(context, "Сохранено: " + filename, Toast.LENGTH_SHORT).show();
                    } catch (Exception e) {
                        Toast.makeText(context, "Ошибка: " + e.getMessage(), Toast.LENGTH_LONG).show();
                    }
                }
            });
        }

        @JavascriptInterface
        public void readFile(final String filename) {
            new Handler(Looper.getMainLooper()).post(new Runnable() {
                @Override public void run() {
                    try {
                        File file = new File(new File(Environment.getExternalStorageDirectory(), "hApp/hfiles"), filename);
                        FileInputStream fis = new FileInputStream(file);
                        byte[] buf = new byte[(int) file.length()];
                        fis.read(buf);
                        fis.close();
                        String content = new String(buf, "UTF-8");
                        webView.evaluateJavascript("onFileRead(" + JSONObject.quote(filename) + "," + JSONObject.quote(content) + ");", null);
                    } catch (Exception e) {
                        webView.evaluateJavascript("onFileError(" + JSONObject.quote(filename) + "," + JSONObject.quote(e.getMessage()) + ");", null);
                    }
                }
            });
        }

        @JavascriptInterface
        public void listFiles() {
            new Handler(Looper.getMainLooper()).post(new Runnable() {
                @Override public void run() {
                    try {
                        File dir = new File(Environment.getExternalStorageDirectory(), "hApp/hfiles");
                        if (!dir.exists()) dir.mkdirs();
                        File[] files = dir.listFiles();
                        JSONArray arr = new JSONArray();
                        if (files != null) for (File f : files) arr.put(f.getName());
                        webView.evaluateJavascript("onFilesList(" + arr.toString() + ");", null);
                    } catch (Exception e) {
                        webView.evaluateJavascript("onFilesList([]);", null);
                    }
                }
            });
        }

        @JavascriptInterface
        public void deleteFile(final String filename) {
            new Handler(Looper.getMainLooper()).post(new Runnable() {
                @Override public void run() {
                    try {
                        new File(new File(Environment.getExternalStorageDirectory(), "hApp/hfiles"), filename).delete();
                    } catch (Exception e) {}
                }
            });
        }

        @JavascriptInterface
        public void vibrate(long ms) {
            Vibrator v = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
            if (v != null && v.hasVibrator()) v.vibrate(ms);
        }

        @JavascriptInterface
        public void toast(final String message) {
            new Handler(Looper.getMainLooper()).post(new Runnable() {
                @Override public void run() {
                    Toast.makeText(context, message, Toast.LENGTH_SHORT).show();
                }
            });
        }
    }
}
