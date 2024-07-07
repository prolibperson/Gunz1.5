using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Xml;
using System.Reflection;

namespace ZPatchBuilder
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        public ulong GetCheckSum(string fileName)
        {
            CRC32 crc = new CRC32();
            return crc.GetCRC32(fileName);
        }

        /// <summary>
        /// TODO: parse each file and write to xml
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            ZFileList fileList = new ZFileList();
            XmlWriterSettings writerSettings = new XmlWriterSettings();
            writerSettings.Indent = true;
            writerSettings.IndentChars = "\t";
            writerSettings.NewLineHandling = NewLineHandling.Entitize;
            writerSettings.NamespaceHandling = NamespaceHandling.Default;
            XmlWriter writer = XmlWriter.Create("patch.xml", writerSettings);
            writer.WriteStartDocument();
            writer.WriteStartElement("XML");
            writer.WriteStartElement("PATCHINFO");
            foreach (string file in fileList.files)
            {
                FileInfo fileInfo = new FileInfo(file);
                if (fileInfo == null)
                    continue;
                if (fileInfo.Name.Contains("patch.xml") || fileInfo.Name.Contains("ZPatchBuilder.exe"))
                    continue;

                string absolutePath = fileInfo.DirectoryName + "\\" + fileInfo.Name;
                string realPath = absolutePath.Replace(AppDomain.CurrentDomain.BaseDirectory,"");

                string listItem = $"Added {realPath} to PatchFile";
                ListBox1.Items.Add(listItem);
                writer.WriteStartElement("PATCHFILE");
                writer.WriteAttributeString("name", realPath);
                writer.WriteStartElement("SIZE");
                writer.WriteValue(fileInfo.Length.ToString());
                writer.WriteEndElement();
                writer.WriteStartElement("CHECKSUM");
                writer.WriteValue(GetCheckSum(absolutePath).ToString());
                writer.WriteEndElement();
                writer.WriteEndElement();
            }
            writer.WriteEndElement();
            writer.WriteEndElement();
            writer.WriteEndDocument();
            writer.Close();
        }
    }
}
