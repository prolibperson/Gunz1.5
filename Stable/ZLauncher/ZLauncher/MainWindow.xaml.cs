using System.Windows;
using System.Diagnostics;
using System;
using System.Linq;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using System.Reflection;
using System.Security.Cryptography;
using System.Text;
using System.Windows.Navigation;
using System.Windows.Input;
using Microsoft.Web.WebView2;
using System.Xml;
using System.Windows.Media.Effects;

namespace ZLauncher
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    ///
    public class ExistingFile
    {
        public string fileName;
        public ulong checkSum;
        public ulong fileSize;
        public ExistingFile(string name, ulong checksum)
        {
           fileName = name;
           checkSum = checksum;
            using (var file = new FileStream(name, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
            {
                fileSize = Convert.ToUInt64(file.Length);
            }
        }
    }

    public partial class MainWindow : Window
    {
        public bool isLauncherPatchNeeded = false;
        DownloadManager downloader;
        List<string> filesToDownLoad = new List<string>();
        XMLParse parser = new XMLParse();

        public MainWindow()
        {
            InitializeComponent();
            //StartGame.Visibility = Visibility.Hidden;
            //CancelDownload.Visibility = Visibility.Hidden;

            //if(File.Exists("ZUpdater.exe"))
            //    File.Delete("ZUpdater.exe");


        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);

            Application.Current.Shutdown();
        }

        /// <summary>
        /// get the checksum of the current file
        /// </summary>
        /// <param name="fileName"></param>
        /// <returns>checksum</returns>
        public ulong GetCheckSum(string fileName)
        {
            CRC32 crc = new CRC32();
            return crc.GetCRC32(fileName);
        }
        /// <summary>
        ///Initialiaze patch, if all files match, then show startgame button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private async void Button_Click(object sender, RoutedEventArgs e)
        {
            Start.Visibility = Visibility.Hidden;
            DownloadFile.Text = "Scanning existing files...";

            if (!parser.ParseXML())
            {
                System.Windows.Forms.MessageBox.Show("Error reaching the patchserver, please try again later");
                Start.Visibility = Visibility.Visible;
                DownloadFile.Text = "";
                return;
            }


            await CheckFiles(parser.files);

            if (filesToDownLoad.Count > 0)
            {

                Directory.CreateDirectory("PATCH");
                downloader = new DownloadManager();
                Cancel.Visibility = Visibility.Visible;
                try
                {
                    await downloader.DownloadFiles(filesToDownLoad);
                }
                catch (Exception)
                {
                }

                if (!downloader.IsCancelled())
                    MoveFilesToBaseDir(filesToDownLoad);

            }
            else
            {
                DownloadFile.Text = "Patching Complete";
                Launch.Visibility = Visibility.Visible;
                Cancel.Visibility = Visibility.Hidden;
                Start.Visibility = Visibility.Hidden;
                return;
            }

            if (isLauncherPatchNeeded == true)
            {
                string exePath = AppDomain.CurrentDomain.BaseDirectory + "\\ZUpdater.exe";

                var assembly = Assembly.GetExecutingAssembly();// ("ZUpdater.exe");
                using (Stream stream = assembly.GetManifestResourceStream("ZLauncher.ZUpdater.exe"))
                {
                    byte[] bytes = new byte[(int)stream.Length];
                    stream.Read(bytes, 0, bytes.Length);
                    File.WriteAllBytes(exePath, bytes);
                    System.Diagnostics.Process.Start(exePath);
                    Application.Current.Shutdown();
                }
            }

            if (downloader.isComplete)
            {
                DownloadFile.Text = "Patching Complete";
                Launch.Visibility = Visibility.Visible;
                Cancel.Visibility = Visibility.Hidden;
                Start.Visibility = Visibility.Hidden;
            }
            else
            {
                DownloadFile.Text = "Patching Cancelled";
                Launch.Visibility = Visibility.Hidden;
                Cancel.Visibility = Visibility.Hidden;
                Start.Visibility = Visibility.Visible;
            }

        }

        public async Task CheckFiles(List<FileList> files)
        {
            await Task.Run(() =>
            {
                string[] filesInDirectory = System.IO.Directory.GetFiles(AppDomain.CurrentDomain.BaseDirectory, "*", SearchOption.AllDirectories);

                foreach (FileList file in files)
                {
                    string absolutePath = file.name.Replace("/", "\\");
                    string PatchPatch = "PATCH\\" + absolutePath;

                    if (!filesInDirectory.Contains(AppDomain.CurrentDomain.BaseDirectory + absolutePath) && !filesInDirectory.Contains(AppDomain.CurrentDomain.BaseDirectory + PatchPatch))
                    {
                        filesToDownLoad.Add(file.name);
                    }
                    ///check if the patch folder contains files
                    else if (filesInDirectory.Contains(AppDomain.CurrentDomain.BaseDirectory + PatchPatch))
                    {
                        ExistingFile existingFile = new ExistingFile(PatchPatch, GetCheckSum(PatchPatch));
                        FILE_ERR err = parser.DoesFileMatch(existingFile.fileName, existingFile.checkSum, existingFile.fileSize);
                        if (err != FILE_ERR.FERR_OK)
                        {
                            switch (err)
                            {
                                case FILE_ERR.FERR_CHECKSUMMISMATCH:
                                case FILE_ERR.FERR_SIZEMISMATCH:
                                    {
                                        filesToDownLoad.Add(file.name);
                                    }
                                    break;
                                case FILE_ERR.FERR_NOFILEFOUND:
                                    {
                                        using (System.IO.StreamWriter writer = new System.IO.StreamWriter("dlErr.txt", true))
                                        {
                                            string error = "Error Downloading " + existingFile.fileName + " , " + "file does not exist in the patcher.";
                                            writer.WriteLine(error);
                                        }
                                    }
                                    break;
                            }
                        }
                    }

                    else if (filesInDirectory.Contains(AppDomain.CurrentDomain.BaseDirectory + absolutePath))
                    {
                        ExistingFile existingFile = new ExistingFile(file.name, GetCheckSum(file.name));
                        FILE_ERR err = parser.DoesFileMatch(existingFile.fileName, existingFile.checkSum, existingFile.fileSize);
                        if (err != FILE_ERR.FERR_OK)
                        {
                            switch (err)
                            {
                                case FILE_ERR.FERR_CHECKSUMMISMATCH:
                                case FILE_ERR.FERR_SIZEMISMATCH:
                                    {
                                        filesToDownLoad.Add(file.name);
                                    }
                                    break;
                                case FILE_ERR.FERR_NOFILEFOUND:
                                    {
                                        using (System.IO.StreamWriter writer = new System.IO.StreamWriter("dlErr.txt", true))
                                        {
                                            writer.WriteLine("Error Downloading %s, file does not exist in the patcher.", existingFile.fileName);
                                        }
                                    }
                                    break;
                            }
                        }
                    }
                }
            });
        }
        /// <summary>
        /// Once file are downloaded, move them to the proper directories
        /// </summary>
        /// <param name="filesToDownLoad"></param>
        public void MoveFilesToBaseDir(List<string> filesToDownLoad)
        {
            //if (downloader.IsCancelled())
            //    return;
            Task.Run(() =>
            {
                foreach (string name in filesToDownLoad)
                {
                    string fileToMove = "PATCH\\" + name.Replace("/", "\\");
                    string targetDir = AppDomain.CurrentDomain.BaseDirectory + name;
                    string absolutetargetDir = targetDir.Replace("/", "\\");
                    string actualPath = fileToMove.Replace("PATCH\\", "");
                    ///TODO: If file is launcher, add append _ to new filename, then, handle somehow
                    ///
                    if (name.Contains("ZLauncher"))
                    {
                        isLauncherPatchNeeded = true;
                        return;
                    }
                    if (!File.Exists(fileToMove))
                    {
                        ///<summary>
                        /// if file doesn't exist, move on
                        ///</summary>
                        continue;
                    }
                    new System.IO.FileInfo(actualPath).Directory.Create();


                    if (File.Exists(absolutetargetDir))
                        File.Delete(absolutetargetDir);


                    try
                    {
                        File.Move(fileToMove, absolutetargetDir);
                    }
                    catch (Exception e)
                    {
                        System.Windows.Forms.MessageBox.Show(e.Message);
                        System.Windows.Forms.MessageBox.Show(e.InnerException.Message);
                    }
                }


                if (Directory.Exists("PATCH"))
                    Directory.Delete("PATCH", true);
            }).Wait();
        }

        private void ProgressBar_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {

        }

        /// <summary>
        /// this is the actual start game button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        /// 

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            var myProcess = new Process { StartInfo = new ProcessStartInfo("Gunz.exe", "01001001 00100000 01001100 01101111 01110110 01100101 00100000 01000100 01100001 01100101 01101101 01101111 01101110 01110011 01010010 01101001 01101110 01100111 00001010 ") };
            try
            {
                if (myProcess.Start())
                {
                    System.Windows.Application.Current.Shutdown();
                }
            }
            catch (Exception exception)
            {
                System.Windows.Forms.MessageBox.Show(exception.Message);
            }
        }

        /// <summary>
        /// This is the cancel download button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
           if (!downloader.IsCancelled())
            {
                downloader.Cancel();
                Start.Visibility = Visibility.Visible;
                Cancel.Visibility = Visibility.Hidden;
                Launch.Visibility = Visibility.Hidden;
            }
        }

        private void webView_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            webView.ExecuteScriptAsync("startPatch");
        }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if(e.LeftButton == MouseButtonState.Pressed)
            {
                DragMove();
            }
        }

        private void Config_Click(object sender, RoutedEventArgs e)
        {
            XmlDocument doc = new XmlDocument();
            try
            {
                doc.Load("config.xml");
            }
            catch (IOException ex)
            {
                MessageBox.Show("Error Opening Config.xml");
               // return;
            }

            Effect = new BlurEffect();
            Config configWindow = new Config();
            configWindow.Owner = this;
          //  configWindow.LoadConfig(doc);
            configWindow.Show();
        }

        /// <summary>
        /// Optional search bar, I wouldn't recommend it
        /// </summary>
    }
}