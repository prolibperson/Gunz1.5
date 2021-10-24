using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public enum PatchResult
{
    PR_OK = 0,
    PR_NOCONN = 1,
    PR_NEEDUPDATE = 2,
    PR_CANTUPDATEFILES = 3,
    PR_LAUNCHERUPDATENEEDED = 4,
    PR_END
}

public struct FileInfo
{
    public string name;
    public long size;
    public long checksum;
}

namespace ZLauncher
{
    public class Patcher
    {
        public bool isPatchNeeded = false;
        public List<string> filesNeedingUpdate = new List<string>();

        public Patcher()
        {

        }

        public async Task<PatchResult> Run()
        {
            PatchResult result = PatchResult.PR_OK;

            List<FileInfo> patchFiles = new List<FileInfo>();

            result = GetPatchFiles(ref patchFiles);
            if (result != PatchResult.PR_OK)
            {
                return result;
            }

            result = await CheckPatchToExistingFiles(patchFiles, filesNeedingUpdate);
            if (result == PatchResult.PR_NEEDUPDATE)
            {
                isPatchNeeded = true;
            }

            return result;
        }

        public PatchResult GetPatchFiles(ref List<FileInfo> patchFiles)
        {
            WebClient client = new WebClient();
            byte[] patchData = null;
            try
            {
                patchData = client.DownloadData("http://patch.tupac.gay/patch/patch.xml");
            }
            catch (WebException e)
            {
                if (e.Status == WebExceptionStatus.ConnectFailure)
                {
                    return PatchResult.PR_NOCONN;
                }
            }

            XmlDocument xmlPatch = new XmlDocument();
            string xmlText = Encoding.UTF8.GetString(patchData);
            xmlPatch.LoadXml(xmlText);
            XmlNode rootNode = xmlPatch.SelectSingleNode("/files");//.SelectSingleNode("/PATCHINFO");

            foreach (XmlNode childNode in rootNode)
            {
                if (childNode.Name == "file")
                {
                    FileInfo fileInfo = new FileInfo();
                    fileInfo.name = childNode.Attributes["name"].Value;
                    foreach (XmlNode node in childNode)
                    {
                        if (node.Name == "size")
                        {
                            fileInfo.size = Convert.ToInt64(node.InnerText);
                        }
                        if (node.Name == "hash")
                        {
                            fileInfo.checksum = Convert.ToInt64(node.InnerText);
                        }
                    }

                    patchFiles.Add(fileInfo);
                }
            }
            return PatchResult.PR_OK;
        }

        //TODO: try/catch for checking if directory exists
        public async Task<PatchResult> CheckPatchToExistingFiles(List<FileInfo> patchFiles, List<string> updateFiles)
        {
            PatchResult result = PatchResult.PR_OK;
            await Task.Run(() =>
            {
                foreach (FileInfo patchFile in patchFiles)
                {
                    string file = null;

                    try
                    {
                        file = Directory.GetFiles(Directory.GetCurrentDirectory(), patchFile.name, SearchOption.AllDirectories).FirstOrDefault();
                    }
                    catch (DirectoryNotFoundException exception)
                    {
                        result = PatchResult.PR_NEEDUPDATE;
                        updateFiles.Add(patchFile.name);
                        continue;
                    }
                    if (file != null)
                    {
                        FileStream fileData = File.OpenRead(file);
                        FileInfo existingFile = new FileInfo();
                        existingFile.name = file;
                        existingFile.size = fileData.Length;
                        byte[] array = new byte[fileData.Length];
                        fileData.Read(array, 0, array.Length);
                        existingFile.checksum = Force.Crc32.Crc32Algorithm.Compute(array);

                        if (existingFile.size != patchFile.size || existingFile.checksum != patchFile.checksum)
                        {
                            updateFiles.Add(patchFile.name);
                            result = PatchResult.PR_NEEDUPDATE;
                        }
                    }
                    else
                    {
                        updateFiles.Add(patchFile.name);
                        result = PatchResult.PR_NEEDUPDATE;
                    }
                }
            });
            return result;
        }
    }
}
