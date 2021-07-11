using System.Collections.Generic;
using System.Xml;
using System.Net;
using System;
using System.IO;
using System.Threading.Tasks;

public enum FILE_ERR
{
    FERR_NOFILEFOUND = 3,
    FERR_SIZEMISMATCH = 2,
    FERR_CHECKSUMMISMATCH = 1,
    FERR_OK = 0,
}

public class FileList
{
   public string name;
   public ulong checkSum;
   public ulong size;
}

static class ListExtensions
{
    public static void MoveItemAtIndexToFront<T>(this List<T> list, int index)
    {
        T item = list[index];
        for (int i = index; i > 0; i--)
            list[i] = list[i - 1];
        list[0] = item;
    }
}

namespace ZLauncher
{
    class XMLParse
    {
        public List<FileList> files = new List<FileList>();
        MemoryStream xmlPatch = null;

        /// <summary>
        /// read patch.xml, store values in List of files
        /// </summary>
        /// <returns></returns>
        public bool ParseXML()
        {
            WebClient client = new WebClient();
            client.Headers.Add("User-Agent", "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.33 Safari/537.36");
            try
            {
                try
                {
                    byte[] downloadData = client.DownloadData("https://updates.duelists.online/patch.xml");
                }
                catch(Exception ex)
                {
                    return false;
                }
              xmlPatch = new MemoryStream(client.DownloadData("https://updates.duelists.online/patch.xml"));
            }
            catch(ArgumentNullException e)
            {
                System.Windows.Forms.MessageBox.Show(e.Message);
                return false;
            }

            XmlDocument doc = new XmlDocument();
            try
            {
                doc.Load(xmlPatch);
            }
            catch (XmlException e)
            {
                System.Windows.Forms.MessageBox.Show(e.Message);
                return false;
            }

            XmlNode node = doc.FirstChild;
            foreach (XmlNode child in node.FirstChild)
            {
                if (child.Name == "PATCHFILE")
                {
                    FileList file = new FileList
                    {
                        name = child.Attributes.GetNamedItem("name").Value,
                        size = System.Convert.ToUInt64(child["SIZE"].InnerText),
                        checkSum = System.Convert.ToUInt64(child["CHECKSUM"].InnerText)
                        
                    };
                    files.Add(file);
                }
            }

            foreach (FileList file in files.ToArray())
            {
                if (file.name.Contains("ZLauncher"))
                    files.MoveItemAtIndexToFront(files.IndexOf(file));
            }
            return true;

        }

        public FILE_ERR DoesFileMatch(string fileName, ulong checkSum, ulong fileSize)
        {
            FileList file = GetFileByName(fileName);
            if (file == null)
                return FILE_ERR.FERR_NOFILEFOUND;
            if (file.checkSum != checkSum)
                return FILE_ERR.FERR_CHECKSUMMISMATCH;
            if (file.size != fileSize)
                return FILE_ERR.FERR_SIZEMISMATCH;

            return FILE_ERR.FERR_OK;
        }

        private FileList GetFileByName(string name)
        {
            if(name.Contains("PATCH\\"))
            {
                name = name.Replace("PATCH\\", "");
            }
            foreach(FileList file in files)
            {
                if (name == file.name)
                    return file;
            }
            return null;
        }
    }
}
