using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Net;
using System.Threading.Tasks;

namespace ZLauncher
{
    class DownloadManager
    {
        public DownloadManager()
        {

        }
        public async Task<PatchResult> DownloadFilesAsync(List<string> filesToDownload)
        {
            WebClient client = new WebClient();
            foreach (string fileName in filesToDownload)
            {
                try
                {
                    string webPath = "https://todo.todo" + fileName;
                    if (fileName.Contains("/"))
                    {
                        string folderPath = Path.GetDirectoryName(fileName);
                        if (Directory.Exists(folderPath) == false)
                        {
                            System.IO.Directory.CreateDirectory(fileName.Substring(0, fileName.LastIndexOf('/')));
                        }
                    }
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
    }
}
