using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using Microsoft.Win32;

namespace GunzItemLister
{
	/// <summary>
	/// Form1에 대한 요약 설명입니다.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TextBox tbGunzItem;
		private System.Windows.Forms.TextBox tbGunzString;
		private System.Windows.Forms.Button btnGunzItemBrowse;
		private System.Windows.Forms.Button btnGunzStringBrowse;
		private System.Windows.Forms.Button btnBuild;
		private System.Windows.Forms.Label lblOutput;
		private System.Windows.Forms.CheckBox chkUTF8;
		/// <summary>
		/// 필수 디자이너 변수입니다.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form1()
		{
			//
			// Windows Form 디자이너 지원에 필요합니다.
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent를 호출한 다음 생성자 코드를 추가합니다.
			//
			LoadConfigFromRegistry();
		}

		/// <summary>
		/// 사용 중인 모든 리소스를 정리합니다.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			SaveConfigToRegistry();

			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form 디자이너에서 생성한 코드
		/// <summary>
		/// 디자이너 지원에 필요한 메서드입니다.
		/// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
		/// </summary>
		private void InitializeComponent()
		{
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btnGunzItemBrowse = new System.Windows.Forms.Button();
            this.tbGunzItem = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.tbGunzString = new System.Windows.Forms.TextBox();
            this.btnGunzStringBrowse = new System.Windows.Forms.Button();
            this.btnBuild = new System.Windows.Forms.Button();
            this.lblOutput = new System.Windows.Forms.Label();
            this.chkUTF8 = new System.Windows.Forms.CheckBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btnGunzItemBrowse);
            this.groupBox1.Controls.Add(this.tbGunzItem);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.tbGunzString);
            this.groupBox1.Controls.Add(this.btnGunzStringBrowse);
            this.groupBox1.Location = new System.Drawing.Point(8, 8);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(448, 88);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "groupBox1";
            // 
            // btnGunzItemBrowse
            // 
            this.btnGunzItemBrowse.Location = new System.Drawing.Point(384, 24);
            this.btnGunzItemBrowse.Name = "btnGunzItemBrowse";
            this.btnGunzItemBrowse.Size = new System.Drawing.Size(56, 23);
            this.btnGunzItemBrowse.TabIndex = 3;
            this.btnGunzItemBrowse.Text = "Browse";
            this.btnGunzItemBrowse.MouseClick += new System.Windows.Forms.MouseEventHandler(this.btnGunzItemBrowse_MouseClick);
            // 
            // tbGunzItem
            // 
            this.tbGunzItem.AllowDrop = true;
            this.tbGunzItem.Location = new System.Drawing.Point(80, 24);
            this.tbGunzItem.Name = "tbGunzItem";
            this.tbGunzItem.Size = new System.Drawing.Size(296, 21);
            this.tbGunzItem.TabIndex = 2;
            this.tbGunzItem.Text = "textBox1";
            this.tbGunzItem.DragDrop += new System.Windows.Forms.DragEventHandler(this.tbGunzItem_DragDrop);
            this.tbGunzItem.DragEnter += new System.Windows.Forms.DragEventHandler(this.tbGunzItem_DragEnter);
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(8, 56);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(64, 23);
            this.label2.TabIndex = 1;
            this.label2.Text = "String.xml";
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(8, 24);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(64, 23);
            this.label1.TabIndex = 0;
            this.label1.Text = "ZItem.xml";
            // 
            // tbGunzString
            // 
            this.tbGunzString.AllowDrop = true;
            this.tbGunzString.Location = new System.Drawing.Point(80, 56);
            this.tbGunzString.Name = "tbGunzString";
            this.tbGunzString.Size = new System.Drawing.Size(296, 21);
            this.tbGunzString.TabIndex = 2;
            this.tbGunzString.Text = "textBox1";
            this.tbGunzString.DragDrop += new System.Windows.Forms.DragEventHandler(this.tbGunzString_DragDrop);
            this.tbGunzString.DragEnter += new System.Windows.Forms.DragEventHandler(this.tbGunzString_DragEnter);
            // 
            // btnGunzStringBrowse
            // 
            this.btnGunzStringBrowse.Location = new System.Drawing.Point(384, 56);
            this.btnGunzStringBrowse.Name = "btnGunzStringBrowse";
            this.btnGunzStringBrowse.Size = new System.Drawing.Size(56, 23);
            this.btnGunzStringBrowse.TabIndex = 3;
            this.btnGunzStringBrowse.Text = "Browse";
            this.btnGunzStringBrowse.MouseClick += new System.Windows.Forms.MouseEventHandler(this.btnGunzStringBrowse_MouseClick);
            // 
            // btnBuild
            // 
            this.btnBuild.Location = new System.Drawing.Point(80, 104);
            this.btnBuild.Name = "btnBuild";
            this.btnBuild.Size = new System.Drawing.Size(75, 23);
            this.btnBuild.TabIndex = 1;
            this.btnBuild.Text = "Build";
            this.btnBuild.Click += new System.EventHandler(this.btnBuild_Click);
            // 
            // lblOutput
            // 
            this.lblOutput.Location = new System.Drawing.Point(168, 104);
            this.lblOutput.Name = "lblOutput";
            this.lblOutput.Size = new System.Drawing.Size(280, 23);
            this.lblOutput.TabIndex = 2;
            this.lblOutput.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // chkUTF8
            // 
            this.chkUTF8.Location = new System.Drawing.Point(16, 104);
            this.chkUTF8.Name = "chkUTF8";
            this.chkUTF8.Size = new System.Drawing.Size(56, 24);
            this.chkUTF8.TabIndex = 3;
            this.chkUTF8.Text = "UTF8";
            // 
            // Form1
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
            this.ClientSize = new System.Drawing.Size(464, 134);
            this.Controls.Add(this.chkUTF8);
            this.Controls.Add(this.lblOutput);
            this.Controls.Add(this.btnBuild);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form1";
            this.Text = "GunzItemLister";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// 해당 응용 프로그램의 주 진입점입니다.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());
		}

		private void tbGunzItem_DragEnter(object sender, System.Windows.Forms.DragEventArgs e)
		{
			if (e.Data.GetDataPresent(DataFormats.FileDrop)) 
			{
				e.Effect = DragDropEffects.All;
			} 
			else 
			{
				e.Effect = DragDropEffects.None;
			}
		}

		protected void LoadConfigFromRegistry()
		{
			try
			{						
				RegistryKey keyGunz = Registry.LocalMachine.OpenSubKey(@"Software\GunzItemLister");
				
				string a = keyGunz.GetValue("ZItemPath").ToString();

				tbGunzItem.Text = keyGunz.GetValue("ZItemPath").ToString();
				tbGunzString.Text = keyGunz.GetValue("ZStringPath").ToString();

				keyGunz.Close();
			}
			catch(Exception e)
			{
				MessageBox.Show(e.ToString());
			}
		}

		protected void SaveConfigToRegistry()
		{
			try
			{
				RegistryKey keySW = Registry.LocalMachine.OpenSubKey("Software", true);
				RegistryKey keyGunz = keySW.CreateSubKey("GunzItemLister");
				keyGunz.SetValue("ZItemPath", tbGunzItem.Text);
				keyGunz.SetValue("ZStringPath", tbGunzString.Text);

				Registry.LocalMachine.Flush();
				keyGunz.Close();
				keySW.Close();
			}
			catch(Exception e)
			{
				MessageBox.Show(e.ToString());
			}		
		}

		public void OutputMessage()
		{
		}
        
		private void tbGunzItem_DragDrop(object sender, System.Windows.Forms.DragEventArgs e)
		{
			if (e.Data.GetDataPresent(DataFormats.FileDrop)) 
			{
				string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);

				tbGunzItem.Text = files[0];
			}		
		}

		private void tbGunzString_DragEnter(object sender, System.Windows.Forms.DragEventArgs e)
		{
			if (e.Data.GetDataPresent(DataFormats.FileDrop)) 
			{
				e.Effect = DragDropEffects.All;
			} 
			else 
			{
				e.Effect = DragDropEffects.None;
			}
		}

		private void tbGunzString_DragDrop(object sender, System.Windows.Forms.DragEventArgs e)
		{
			if (e.Data.GetDataPresent(DataFormats.FileDrop)) 
			{
				string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);

				tbGunzString.Text = files[0];
			}		
		}

		private void btnBuild_Click(object sender, System.EventArgs e)
		{
			GunzStringBuilder builder = new GunzStringBuilder();
			builder.Build(tbGunzItem.Text, tbGunzString.Text, chkUTF8.Checked, lblOutput);
		}

        private void btnGunzItemBrowse_MouseClick(object sender, MouseEventArgs e)
        {
            System.Diagnostics.Trace.WriteLine("Browse1 Clicked");
        }

        private void btnGunzStringBrowse_MouseClick(object sender, MouseEventArgs e)
        {
            System.Diagnostics.Trace.WriteLine("Browse2 Clicked");
        }
	}
}
