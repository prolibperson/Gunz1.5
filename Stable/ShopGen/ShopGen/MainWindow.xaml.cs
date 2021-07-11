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
using System.Xml;

namespace ShopGen
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

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            XmlReaderSettings readerSettings = new XmlReaderSettings();
            readerSettings.IgnoreComments = true;
            XmlReader reader = XmlReader.Create("zitem.xml", readerSettings);
            XmlDocument doc = new XmlDocument();
            doc.Load(reader);
            XmlWriterSettings writerSettings = new XmlWriterSettings();
            writerSettings.Indent = true;
            writerSettings.IndentChars = "\t";
            writerSettings.NewLineHandling = NewLineHandling.Entitize;
            writerSettings.NamespaceHandling = NamespaceHandling.Default;
            XmlWriter writer = XmlWriter.Create("shop.xml",writerSettings);
            writer.WriteStartDocument();
            writer.WriteStartElement("XML");
            writer.WriteAttributeString("id", "shop");
            XmlElement rootNode = doc.DocumentElement;

            foreach(XmlNode child in rootNode.ChildNodes)
            {
                if(child.Name == "ITEM")
                {
                    XmlAttribute currencyType = child.Attributes["currency"];
                    if (currencyType != null)
                    {
                       if(currencyType.Value != "bounty")
                            continue;
                    }
                    if(child.Attributes["iscashitem"] != null)
                    {
                        if (child.Attributes["iscashitem"].Value == "true")
                            continue;
                    }
                    if (Convert.ToInt32(child.Attributes["id"].Value) >= 300011 && Convert.ToInt32(child.Attributes["id"].Value) <= 300039)
                        continue;

                    writer.WriteStartElement("SELL");
                    writer.WriteAttributeString("itemid", child.Attributes["id"].Value);
                    writer.WriteEndElement();

                    string outPut = "Added " + child.Attributes["id"].Value + " to shop xml";

                    genlistBox.Items.Add(outPut);
                }
            }
            reader = XmlReader.Create("zitem_locale.xml", readerSettings);
            doc = new XmlDocument();
            doc.Load(reader);
            rootNode = doc.DocumentElement;
            foreach (XmlNode child in rootNode.ChildNodes)
            {
                if (child.Name == "ITEM")
                {
                    XmlAttribute iscashItem = child.Attributes["iscashitem"];
                    if (iscashItem != null)
                    {
                        if (iscashItem.Value == "true")
                            continue;

                        writer.WriteStartElement("SELL");
                        writer.WriteAttributeString("itemid", child.Attributes["id"].Value);
                        writer.WriteEndElement();

                        string outPut = "Added " + child.Attributes["id"].Value + " to shop xml";

                        genlistBox.Items.Add(outPut);
                    }
                }
            }

            reader = XmlReader.Create("zquestitem.xml", readerSettings);
            doc = new XmlDocument();
            doc.Load(reader);
            rootNode = doc.DocumentElement;
            foreach (XmlNode child in rootNode.ChildNodes)
            {
                if (child.Name == "ITEM")
                {
                    writer.WriteStartElement("SELL");
                    writer.WriteAttributeString("itemid", child.Attributes["id"].Value);
                    writer.WriteEndElement();

                    string outPut = "Added " + child.Attributes["id"].Value + " to shop xml";

                    genlistBox.Items.Add(outPut);
                }
            }
            writer.WriteEndElement();
            writer.WriteEndDocument();
            writer.Close();

 

            genlistBox.Items.Add("DONE");
        }
    }
}
