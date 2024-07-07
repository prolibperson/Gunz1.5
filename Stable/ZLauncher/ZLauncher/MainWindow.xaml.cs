using System.Windows;
using System.Diagnostics;
using System;
using System.IO;
using System.Reflection;
using System.Windows.Input;
using System.Windows.Media.Effects;
using System.Net;

namespace ZLauncher
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        public void SetProgress(float f)
        {
            ProgressBar.Value = f;
        }

        private async void StartPatch(object sender, RoutedEventArgs e)
        {
            Patch.Visibility = Visibility.Hidden;
            ProgressBar.Visibility = Visibility.Visible;
            DownloadFile.Visibility = Visibility.Visible;
            DownloadFile.Text = "Scanning existing files...";
            Patcher patcher = new Patcher();
            PatchResult result = await patcher.Run();
            if(result != PatchResult.PR_OK)
            {
                PostErrorMsg(result);
            }
            if(patcher.isPatchNeeded == true)
            {
                DownloadManager downloadManager = new DownloadManager();
                result = await downloadManager.DownloadFilesAsync(patcher.filesNeedingUpdate, ProgressBar,DownloadFile);
                if(result != PatchResult.PR_OK && result != PatchResult.PR_LAUNCHERUPDATENEEDED)
                {
                    PostErrorMsg(result);
                }

                if(result == PatchResult.PR_LAUNCHERUPDATENEEDED)
                {
                    UpdateLauncher();
                }
            }

            if(result == PatchResult.PR_OK)
            {
                Launch.Visibility = Visibility.Visible;
            }
        }

        private void StartGame(object sender, RoutedEventArgs e)
        {
            var myProcess = new Process { StartInfo = new ProcessStartInfo("Gunz.exe")};
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

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if(e.LeftButton == MouseButtonState.Pressed)
            {
                DragMove();
            }
        }

        private void Menu_Click(object sender, RoutedEventArgs e)
        {
            if(SettingsPanel.Visibility == Visibility.Visible)
            {
                SettingsPanel.Visibility = Visibility.Collapsed;
            }
            else
            {
                SettingsPanel.Visibility = Visibility.Visible;
            }
        }

        private void Config_Click(object sender, RoutedEventArgs e)
        {
            Effect = new BlurEffect();
            Config config = new Config();
            config.Owner = this;
            config.Show();
        }

        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void PostErrorMsg(PatchResult result)
        {
            if (result == PatchResult.PR_NOCONN)
            {
                System.Windows.Forms.MessageBox.Show("Error reaching the patchserver, please try again later");
                Patch.Visibility = Visibility.Visible;
                DownloadFile.Text = "";
                return;
            }
            if (result == PatchResult.PR_CANTUPDATEFILES)
            {
                System.Windows.Forms.MessageBox.Show("Error updating files, please run as adminsitrator and try again.");
                Patch.Visibility = Visibility.Visible;
                DownloadFile.Text = "";
                return;
            }
        }

        private void UpdateLauncher()
        {
            string exePath = AppDomain.CurrentDomain.BaseDirectory + "\\ZUpdater.exe";

            var assembly = Assembly.GetExecutingAssembly();
            using (Stream stream = assembly.GetManifestResourceStream("ZLauncher.ZUpdater.exe"))
            {
                byte[] bytes = new byte[(int)stream.Length];
                stream.Read(bytes, 0, bytes.Length);
                File.WriteAllBytes(exePath, bytes);
                System.Diagnostics.Process.Start(exePath);
                Application.Current.Shutdown();
            }
        }
    }
}