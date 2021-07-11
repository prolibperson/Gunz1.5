using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Net;
using System.ComponentModel;
using System;
using System.IO;
using System.Threading;

namespace ZLauncher
{
    public static class DownLoader
    {
        public static Task DownloadFileTaskAsync(this WebClient client, string address, string filename,
                   CancellationToken token)
        {
            token.Register(() => client.CancelAsync());

            return client.DownloadFileTaskAsync(address, filename);
        }
    };
    public class DownloadManager : MainWindow
    {
        public bool isComplete = false;

        MainWindow mainWindow = System.Windows.Application.Current.MainWindow as MainWindow;
        System.Threading.CancellationTokenSource cancel = new System.Threading.CancellationTokenSource();
        public async Task DownloadFiles(List<string> files)
        {
            // .net 4.0 w/Extensions
            foreach (string file in files)
            {
                WebClient client = new WebClient();
                string targetpath = "";

                TaskEx.Run(() =>
                {
                    client.Headers.Add("User-Agent", "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.33 Safari/537.36");
                    if (!cancel.IsCancellationRequested)
                    {
                        client.DownloadProgressChanged += new DownloadProgressChangedEventHandler(ProgressChanged);
                        if (file.Contains('\\'))
                        {
                            if(!Directory.Exists("PATCH\\" + file))
                            Directory.CreateDirectory(System.IO.Path.GetDirectoryName("PATCH\\" + file));
                            targetpath = "PATCH\\" + file;
                        }
                        else
                            targetpath = "PATCH\\" + file;
                    }
                }).Wait(200,cancel.Token);
                mainWindow.DownloadFile.Text = "Downloading : " + System.IO.Path.GetFileName(targetpath);
                if (cancel.IsCancellationRequested == true)
                {
                    isComplete = false;
                    return;
                }

                try
                {
                    if (!cancel.IsCancellationRequested)
                        await DownLoader.DownloadFileTaskAsync(client, "http://updates.duelists.online/" + file, targetpath, cancel.Token);
                    else
                    {
                        isComplete = false;
                        return;
                    }
                }
                catch (WebException e)
                {
                    System.Windows.MessageBox.Show(e.Message);
                    System.Windows.MessageBox.Show(e.InnerException.Message);
                    isComplete = false;
                }

            }
            isComplete = true;
        }

        public void ProgressChanged(object sender, DownloadProgressChangedEventArgs e)
        {
            if (!cancel.IsCancellationRequested)
            {
                mainWindow.progBar1.Value = e.ProgressPercentage;
                double percentage = Math.Round(Convert.ToDouble(e.BytesReceived) / Convert.ToDouble(e.TotalBytesToReceive) * 100);
                mainWindow.progPercentage.Clear();
                mainWindow.progPercentage.Text = percentage.ToString();
            }
            else
            {
                mainWindow.progBar1.Value = 0;
                mainWindow.progPercentage.Clear();
            }
        }

        public void Cancel()
        {
            if (cancel != null)
            {
                cancel.Cancel();
                isComplete = false;
            }
        }

        public bool IsCancelled()
        {
            return cancel.IsCancellationRequested;
        }
    }
}
