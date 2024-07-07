using System;
using System.IO;
using System.Collections;
using System.Windows.Forms;
using System.Xml;
using System.Text;

namespace GunzItemLister
{
	/// <summary>
	/// GunzStringBuilder에 대한 요약 설명입니다.
	/// </summary>
	public class GunzStringBuilder
	{
        ZIndex  m_Index = new ZIndex();

        // Attribute Filter
        Hashtable m_htAttrFilter = new Hashtable();
        Hashtable m_htGunzString = new Hashtable();


        public class ZIndex
        {
            int m_nIndex;

            public int Use()
            {
                return m_nIndex++;
            }

            public int Get()
            {
                return m_nIndex;
            }
        }

		public GunzStringBuilder()
		{
			//
			// TODO: 여기에 생성자 논리를 추가합니다.
			//

            InitAttributeFilter();
		}

        private void InitAttributeFilter()
        {
            m_htAttrFilter.Add("id", m_Index.Use());
            m_htAttrFilter.Add("name", m_Index.Use());
            //m_htAttrFilter.Add("mesh_name", m_Index.Use());
            //m_htAttrFilter.Add("totalpoint", m_Index.Use());
            m_htAttrFilter.Add("type", m_Index.Use());
            m_htAttrFilter.Add("res_sex", m_Index.Use());
            m_htAttrFilter.Add("res_level", m_Index.Use());
            m_htAttrFilter.Add("slot", m_Index.Use());
            m_htAttrFilter.Add("weapon", m_Index.Use());
            m_htAttrFilter.Add("weight", m_Index.Use());
            m_htAttrFilter.Add("bt_price", m_Index.Use());
            m_htAttrFilter.Add("delay", m_Index.Use());
            m_htAttrFilter.Add("damage", m_Index.Use());
            m_htAttrFilter.Add("range", m_Index.Use());
            m_htAttrFilter.Add("ctrl_ability", m_Index.Use());
            m_htAttrFilter.Add("magazine", m_Index.Use());
            m_htAttrFilter.Add("reloadtime", m_Index.Use());
            //m_htAttrFilter.Add("slug_output", m_Index.Use());
            //m_htAttrFilter.Add("gadget_id", m_Index.Use());
            m_htAttrFilter.Add("hp", m_Index.Use());
            m_htAttrFilter.Add("ap", m_Index.Use());
            m_htAttrFilter.Add("maxwt", m_Index.Use());
            //m_htAttrFilter.Add("sf", m_Index.Use());
            //m_htAttrFilter.Add("fr", m_Index.Use());
            //m_htAttrFilter.Add("cr", m_Index.Use());
            //m_htAttrFilter.Add("pr", m_Index.Use());
            //m_htAttrFilter.Add("lr", m_Index.Use());
            //m_htAttrFilter.Add("color", m_Index.Use());
            //m_htAttrFilter.Add("image_id", m_Index.Use());
            //m_htAttrFilter.Add("bullet_image_id", m_Index.Use());
            //m_htAttrFilter.Add("magazine_image_id", m_Index.Use());
            m_htAttrFilter.Add("desc", m_Index.Use());
        }

        private void InitStringXMLTable(string strGunzStringFile)
        {
            try
            {
                XmlTextReader reader = new XmlTextReader(strGunzStringFile);
                while (reader.Read())
                {
                    switch (reader.NodeType)
                    {
                        case XmlNodeType.Element:
                            string strURI = reader.NamespaceURI;
                            string strName = reader.Name;
                            if (strName == "STR" && reader.HasAttributes)
                            {
                                string strStringID = reader.GetAttribute("id");
                                reader.MoveToContent();
                                string strStringValue = reader.ReadString();

                                m_htGunzString.Add(strStringID, strStringValue);
                                //System.Diagnostics.Trace.WriteLine(strItemID+':'+strItemName); 
                            }
                            break;
                        //case XmlNodeType.EndElement: 
                        //case XmlNodeType.Text: 
                        default:
                            break;
                    }
                }
            }
            catch (XmlException e)
            {
                System.Diagnostics.Trace.WriteLine("error occured: " + e.Message);
            }
        }

		public void Build(string strGunzItemFile, string strGunzStringFile, bool bUTF8, Label lblOutput)
		{
            InitStringXMLTable(strGunzStringFile);
			
			StreamWriter fileWriter;

            try
			{
                fileWriter = new StreamWriter("Output.csv", false, Encoding.UTF8);  // UTF8 BOM 삽입위해 명시해야한다
			}
			catch(Exception)
			{
				MessageBox.Show("Can't create Output.csv");
				return;
			}

			try 
			{
                bool bHeaderPrinted = false;
				XmlTextReader reader = new XmlTextReader(strGunzItemFile); 
				while (reader.Read()) 
				{ 
					switch (reader.NodeType) 
					{ 
						case XmlNodeType.Element: 
							Hashtable attributes = new Hashtable(); 
							string strURI= reader.NamespaceURI; 
							string strName= reader.Name; 
							if (strName=="ITEM" && reader.HasAttributes) 
							{
                                //SimpleItemWrite(reader, fileWriter);
                                //FullItemWrite(reader, fileWriter);
                                if (bHeaderPrinted == false)
                                {
                                    bHeaderPrinted = true;
                                    FullItemWriteCSVHeader(reader, fileWriter);
                                }                                
                                FullItemWriteCSV(reader, fileWriter);
							} 
							break; 
						//case XmlNodeType.EndElement: 
						//case XmlNodeType.Text: 
						default: 
							break; 
					} 
				} 
			}
			catch (XmlException e) 
			{ 
				System.Diagnostics.Trace.WriteLine("error occured: " + e.Message); 
			}

			// Close Output File
			fileWriter.Close();

			lblOutput.Text = "Output.csv built";
		}

        void SimpleItemWrite(XmlTextReader reader, StreamWriter fileWriter)
        {
			string strItemID = reader.GetAttribute("id");
			string strItemName = reader.GetAttribute("name");
			strItemName = strItemName.Substring(4);
			string strItemNameSolved;
			if (m_htGunzString[strItemName] == null)
				strItemNameSolved = "NO DATA";
			else
				strItemNameSolved = m_htGunzString[strItemName].ToString();

			string strOutput = strItemID+','+strItemNameSolved;
			//System.Diagnostics.Trace.WriteLine(strOutput); 

			fileWriter.WriteLine(strOutput);
        }

        void FullItemWrite(XmlTextReader reader, StreamWriter fileWriter)
        {
            if (reader.MoveToFirstAttribute() == true)
            {
                do
                {
                    if ( m_htAttrFilter[reader.Name] != null)
                    {
                        string strOutput = reader.Name + '=';
                        if (reader.Name == "name" || reader.Name == "desc")
                        {
                            string strI18Nid = reader.Value.Substring(4);
                            string strI18NSolved;

                            if (m_htGunzString[strI18Nid] == null)
                                strI18NSolved = "NO DATA";
                            else
                                strI18NSolved = m_htGunzString[strI18Nid].ToString();
                            strOutput += strI18NSolved;
                        }
                        else
                        {
                            strOutput += reader.Value;
                        }
                        
                        System.Diagnostics.Trace.WriteLine(strOutput);
                    }
                } while (reader.MoveToNextAttribute() == true);
            }
        }

        void FullItemWriteCSVHeader(XmlTextReader reader, StreamWriter fileWriter)
        {
            if (reader.MoveToFirstAttribute() == true)
            {
                string strOutput = "";
                do
                {
                    if (m_htAttrFilter[reader.Name] != null)
                    {
                        if (strOutput.Length > 0)
                            strOutput += " ,";

                        strOutput += reader.Name;
                    }
                } while (reader.MoveToNextAttribute() == true);
                
                //System.Diagnostics.Trace.WriteLine(strOutput);
                fileWriter.WriteLine(strOutput);
            }
        }

        void FullItemWriteCSV(XmlTextReader reader, StreamWriter fileWriter)
        {
            //            string strAttrID;
            //            string strAttrValue;

            if (reader.MoveToFirstAttribute() == true)
            {
                string[] arrayColumn = new string[m_Index.Get()];

                do
                {
                    if (m_htAttrFilter[reader.Name] != null)
                    {
                        int nCol = (int)m_htAttrFilter[reader.Name];
                        if (reader.Name == "name" || reader.Name == "desc")
                        {
                            string strI18Nid = reader.Value.Substring(4);
                            string strI18NSolved;

                            if (m_htGunzString[strI18Nid] == null)
                                strI18NSolved = "NO DATA";
                            else
                                strI18NSolved = m_htGunzString[strI18Nid].ToString();
                            arrayColumn[nCol] = strI18NSolved;
                        }
                        else
                        {
                            arrayColumn[nCol] = reader.Value;
                        }
                    }
                } while (reader.MoveToNextAttribute() == true);

                string strOutput = "";
                for (int i = 0; i < m_Index.Get(); i++)
                {
                    strOutput += arrayColumn[i];
                    if (i < m_Index.Get())
                        strOutput += ", ";
                }
                
                fileWriter.WriteLine(strOutput);
            }
        }
	}
}
