using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Net;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace ZLauncher
{
    class DownloadManager
    {
        ProgressBar progressBar;
        TextBlock dlText;
        string dlFileName;
        public DownloadManager()
        {

        }
        public async Task<PatchResult> DownloadFilesAsync(List<string> filesToDownload,ProgressBar prog,TextBlock downloadText)
        {
            WebClient client = new WebClient();
            progressBar = prog;
            dlText = downloadText;
            client.DownloadProgressChanged += Client_DownloadProgressChanged;
            foreach (string fileName in filesToDownload)
            {
                try
                {
                    string webPath = "http://patch.tupac.gay/patch/client/" + fileName;
                    if (fileName.Contains("/"))
                    {
                        string folderPath = Path.GetDirectoryName(fileName);
                        if (Directory.Exists(folderPath) == false)
                        {
                           Directory.CreateDirectory(fileName.Substring(0, fileName.LastIndexOf('/')));
                        }
                    }
                    //downloadText.Text = "Downloading: " + fileName;
                    dlFileName = "Downloading : " + fileName + " ";
                    await client.DownloadFileTaskAsync(webPath, fileName);
                }
                catch (WebException e)
                {
                    switch (e.Status)
                    {
                        case WebExceptionStatus.ConnectionClosed:
                        case WebExceptionStatus.ConnectFailure:
                            {
                                return PatchResult.PR_NOCONN;
                            }
                    }
                }
            }
            return PatchResult.PR_OK;
        }

        private void Client_DownloadProgressChanged(object sender, DownloadProgressChangedEventArgs e)
        {
            dlText.Text = dlFileName + e.BytesReceived / 1024 + "KB/" + e.TotalBytesToReceive / 1024 + "KB";
            progressBar.Value = e.ProgressPercentage;
        }
    }
}
