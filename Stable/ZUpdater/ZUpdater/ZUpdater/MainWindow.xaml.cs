using System;
using System.Collections.Generic;
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
using System.IO;
using System.Diagnostics;

namespace ZUpdater
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            this.WindowStartupLocation = WindowStartupLocation.CenterScreen;
            this.WindowState = WindowState.Normal;
            Show();
            
            System.Threading.Thread.Sleep(5000);
            Process[] pname = Process.GetProcessesByName("ZLauncher");
            if (pname.Length != 0)
                try
                {
                    pname[0].Kill();
                }
                catch(Exception e)
                {
                    ///do nothing
                }

            string targetFile = "PATCH\\" + "ZLauncher.exe";
            if (File.Exists("ZLauncher.exe"))
                File.Delete("ZLauncher.exe");

            string targetDest = AppDomain.CurrentDomain.BaseDirectory + "\\" + "ZLauncher.exe"; 
            File.Move(targetFile, targetDest);

            PatcherTextBox.Text = "Patch Complete, opening Launcher.";
            var process = new Process { StartInfo = new ProcessStartInfo("ZLauncher.exe") };
            process.Start();

            Application.Current.Shutdown();
        }
    }
}
