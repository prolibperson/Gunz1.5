using System;
using System.Windows;
using System.Xml;

namespace ZLauncher
{
    /// <summary>
    /// Interaction logic for Config.xaml
    /// </summary>
    public partial class Config : Window
    {
        public Config()
        {
            InitializeComponent();
            InitializeDefaultConfigValues();
        }

        private void InitializeDefaultConfigValues()
        {
            //TODO:
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            Owner.Effect = null;
        }

        private void Resolution_Loaded(object sender, RoutedEventArgs e)
        {
            DisplayModecs.DEVMODE mode  = new DisplayModecs.DEVMODE();
            int i = 0;
            while (DisplayModecs.EnumDisplaySettings(null, i, ref mode))
            {
                if(mode.dmPelsWidth < 800 && mode.dmPelsHeight < 600)
                {
                    ++i;
                    continue;
                }
                string res = mode.dmPelsWidth + "x" + mode.dmPelsHeight;
                if(Resolution.Items.Contains(res))
                {
                    ++i;
                    continue;
                }
                Resolution.Items.Add(res);
                ++i;
            }
            Resolution.SelectedIndex = 0;
        }

        private void Save_Click(object sender, RoutedEventArgs e)
        {
            XmlDocument document = new XmlDocument();
            document.Load("config.xml");

            string[] selectedRes = Resolution.SelectedItem.ToString().Split('x');

            XmlNode resNode = document.SelectSingleNode("/XML/VIDEO/WIDTH");
            if(resNode != null)
            {
                resNode.InnerText = selectedRes[0];
            }
            resNode = document.SelectSingleNode("/XML/VIDEO/HEIGHT");
            if (resNode != null)
            {
                resNode.InnerText = selectedRes[1];
            }

            XmlNode fullScreenNode = document.SelectSingleNode("/XML/VIDEO/SCREENMODE");
            if(fullScreenNode != null)
            {
                fullScreenNode.InnerText = FullScreen.IsChecked == true ? "true" : "false";
            }

            XmlNode d3d9ex = document.SelectSingleNode("/XML/CUSTOM/D3D9EX");
            if(d3d9ex != null)
            {
                d3d9ex.InnerText = Direct3D9Ex.IsChecked == true ? "true" : "false";
            }

            XmlNode x64Mode = document.SelectSingleNode("/XML/CUSTOM/BIT64");
            if(x64Mode != null)
            {
                x64Mode.InnerText = Bit64.IsChecked == true ? "TRUE" : "FALSE";
            }

            document.Save("config.xml");
            Close();
        }

        private void Close_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}
