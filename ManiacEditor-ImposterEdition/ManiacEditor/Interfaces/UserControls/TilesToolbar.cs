﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using RSDKv5;
using System.Diagnostics;
using TileManiac;

namespace ManiacEditor
{
    public partial class TilesToolbarOld : UserControl
    {
        public Editor EditorInstance;

        private TilesList tilesList;
		private System.Windows.Forms.TrackBar trackBar1;
		public System.Windows.Forms.Panel tilePanel;
		public Interfaces.RetroEDTileList retroEDTileList1;
		public ManiacEditor.Interfaces.Controls.TilesTabControl TabControl;
		public Action<int> TileDoubleClick
        {
            get
            {
                return tilesList.TileDoubleClick;
            }
            set
            {
                tilesList.TileDoubleClick = value;
            }
        }

        public Action<int, bool> TileOptionChanged;

        CheckBox[] selectTileOptionsCheckboxes = new CheckBox[6];
        CheckBox[] tileOptionsCheckboxes = new CheckBox[4];
        bool setCheckboxes;

        public int SelectedTile
        {
            get {
                int res = tilesList.SelectedTile;
                for (int i = 0; i < selectTileOptionsCheckboxes.Length; ++i)
                    if (selectTileOptionsCheckboxes[i].Checked) res |= 1 << (10 + i);
                return res;

            }
        }
        
        public bool ShowShortcuts { set { UpdateShortcuts(value); } }

        public enum TileOptionState
        {
            Disabled,
            Checked,
            Unchcked,
            Indeterminate
        }

        public void SetSelectTileOption(int option, bool state)
        {
            selectTileOptionsCheckboxes[option].Checked = state;
        }

        public void SetTileOptionState(int option, TileOptionState state)
        {
            setCheckboxes = true;
            switch (state)
            {
                case TileOptionState.Disabled:
                    tileOptionsCheckboxes[option].AutoCheck = false;
					tileOptionsCheckboxes[option].ForeColor = SystemColors.GrayText;
					tileOptionsCheckboxes[option].Checked = false;
                    break;
                case TileOptionState.Checked:
                    tileOptionsCheckboxes[option].Enabled = true;
					tileOptionsCheckboxes[option].AutoCheck = true;
					tileOptionsCheckboxes[option].ForeColor = SystemColors.ControlText;
					tileOptionsCheckboxes[option].CheckState = CheckState.Checked;
                    break;
                case TileOptionState.Unchcked:
                    tileOptionsCheckboxes[option].Enabled = true;
					tileOptionsCheckboxes[option].AutoCheck = true;
					tileOptionsCheckboxes[option].ForeColor = SystemColors.ControlText;
					tileOptionsCheckboxes[option].CheckState = CheckState.Unchecked;
                    break;
                case TileOptionState.Indeterminate:
                    tileOptionsCheckboxes[option].Enabled = true;
					tileOptionsCheckboxes[option].AutoCheck = true;
					tileOptionsCheckboxes[option].ForeColor = SystemColors.ControlText;
					tileOptionsCheckboxes[option].CheckState = CheckState.Indeterminate;
                    break;
            }
            setCheckboxes = false;
        }

        public TilesToolbarOld(StageTiles tiles, String data_directory, String Colors, Editor instance)
        {
            InitializeComponent();
			SetupTilesList(instance);

			EditorInstance = instance;

            if (Properties.Settings.Default.NightMode)
            {
                trackBar1.BackColor = Editor.darkTheme1;
                tilesList.BackColor = Editor.darkTheme1;
                groupBox1.ForeColor = Editor.darkTheme3;
            }

            tileOptionsCheckboxes[0] = tileOption1;
            tileOptionsCheckboxes[1] = tileOption2;
            tileOptionsCheckboxes[2] = tileOption3;
            tileOptionsCheckboxes[3] = tileOption4;

            selectTileOptionsCheckboxes[0] = option1CheckBox;
            selectTileOptionsCheckboxes[1] = option2CheckBox;
            selectTileOptionsCheckboxes[2] = option3CheckBox;
            selectTileOptionsCheckboxes[3] = option4CheckBox;
            selectTileOptionsCheckboxes[4] = option5CheckBox;
            selectTileOptionsCheckboxes[5] = option6CheckBox;

            RSDKv5.GIF tileGridImage = new GIF((data_directory + "\\16x16Tiles.gif"), Colors);
            tilesList.TilesImage = tileGridImage;
            tilesList.TileScale = 1 << trackBar1.Value;

            UpdateShortcuts();

            ChunksReload();
        }

        public void SetupTilesList(Editor instance)
        {
			TabControl = new Interfaces.Controls.TilesTabControl();
			elementHost1.Child = TabControl;

			this.trackBar1 = new System.Windows.Forms.TrackBar();
			this.retroEDTileList1 = new ManiacEditor.Interfaces.RetroEDTileList();
			this.tilePanel = new System.Windows.Forms.Panel();
			// 
			// retroEDTileList1
			// 
			this.retroEDTileList1.BackColor = System.Drawing.SystemColors.Window;
			this.retroEDTileList1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.retroEDTileList1.ImageHeight = 128;
			this.retroEDTileList1.ImageSize = 128;
			this.retroEDTileList1.ImageWidth = 128;
			this.retroEDTileList1.Location = new System.Drawing.Point(3, 3);
			this.retroEDTileList1.Name = "retroEDTileList1";
			this.retroEDTileList1.ScrollValue = 0;
			this.retroEDTileList1.SelectedIndex = -1;
			this.retroEDTileList1.Size = new System.Drawing.Size(234, 290);
			this.retroEDTileList1.TabIndex = 1;

			// 
			// trackBar1
			// 
			this.trackBar1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
			| System.Windows.Forms.AnchorStyles.Right)));
			this.trackBar1.BackColor = System.Drawing.SystemColors.Control;
			this.trackBar1.LargeChange = 1;
			this.trackBar1.Location = new System.Drawing.Point(0, 0);
			this.trackBar1.Maximum = 3;
			this.trackBar1.Name = "trackBar1";
			this.trackBar1.Size = new System.Drawing.Size(242, 45);
			this.trackBar1.TabIndex = 1;
			this.trackBar1.TickStyle = System.Windows.Forms.TickStyle.Both;
			this.trackBar1.Value = 1;
			this.trackBar1.Scroll += new System.EventHandler(this.trackBar1_Scroll);
			this.trackBar1.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
			this.trackBar1.Dock = System.Windows.Forms.DockStyle.Fill;

			// 
			// tilePanel
			// 
			this.tilePanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
			| System.Windows.Forms.AnchorStyles.Left)
			| System.Windows.Forms.AnchorStyles.Right)));
			this.tilePanel.BackColor = System.Drawing.SystemColors.Control;
			this.tilePanel.Location = new System.Drawing.Point(0, 45);
			this.tilePanel.Name = "tilePanel";
			this.tilePanel.Size = new System.Drawing.Size(241, 253);
			this.tilePanel.TabIndex = 2;
			this.tilePanel.Dock = DockStyle.Fill;
			System.Windows.Forms.Integration.WindowsFormsHost host = new System.Windows.Forms.Integration.WindowsFormsHost();
			System.Windows.Forms.Integration.WindowsFormsHost host2 = new System.Windows.Forms.Integration.WindowsFormsHost();
			System.Windows.Forms.Integration.WindowsFormsHost host3 = new System.Windows.Forms.Integration.WindowsFormsHost();

			host.Child = tilePanel;
			host2.Child = trackBar1;
			host3.Child = retroEDTileList1;

			TabControl.TileSlider.Children.Add(host2);
			TabControl.TileViewer.Children.Add(host);

			TabControl.ChunksPage.Children.Add(host3);

			this.tilesList = new ManiacEditor.TilesList(instance);
            this.tilesList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
| System.Windows.Forms.AnchorStyles.Left)
| System.Windows.Forms.AnchorStyles.Right)));
            this.tilesList.BackColor = System.Drawing.SystemColors.Window;
            this.tilesList.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tilesList.FlipHorizontal = false;
            this.tilesList.FlipVertical = false;
            this.tilesList.ForeColor = System.Drawing.SystemColors.ControlText;
            this.tilesList.Location = new System.Drawing.Point(0, 0);
            this.tilesList.Name = "tilesList";
            this.tilesList.Size = tilePanel.Size;
            this.tilesList.Dock = DockStyle.Fill;
            this.tilesList.TabIndex = 0;
            this.tilesList.TileScale = 2;
            this.tilePanel.Controls.Add(this.tilesList);
        }

        private void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            tilesList.TileScale = 1 << trackBar1.Value;
        }

        public void Reload(string colors = null)
        {
            tilesList.Reload(colors);
            ChunksReload();
        }

        public void ChunksReload()
        {
            retroEDTileList1.Images.Clear();
            EditorInstance.EditorChunk.DisposeTextures();

            for (int i = 0; i < EditorInstance.EditorChunk.StageStamps.StampList.Count; i++)
            {
                retroEDTileList1.Images.Add(EditorInstance.EditorChunk.GetChunkTexture(i));
            }
            retroEDTileList1.Refresh();
        }

        public new void Dispose()
        {
            retroEDTileList1.Dispose();
            tilesList.Dispose();
            base.Dispose();
        }

        private void TilesToolbar_Load(object sender, EventArgs e)
        {
            Debug.WriteLine("Send to debug output.");
            trackBar1.Value = Properties.Settings.Default.tileToolbarDefaultZoomLevel;

        }

        private void option1CheckBox_CheckedChanged(object sender, EventArgs e)
        {
            tilesList.FlipHorizontal = option1CheckBox.Checked;
        }

        private void option2CheckBox_CheckedChanged(object sender, EventArgs e)
        {
            tilesList.FlipVertical = option2CheckBox.Checked;
        }

        private void tileOption1_CheckedChanged(object sender, EventArgs e)
        {
            if (!setCheckboxes) TileOptionChanged?.Invoke(0, tileOption1.Checked);
        }

        private void tileOption2_CheckedChanged(object sender, EventArgs e)
        {
            if (!setCheckboxes) TileOptionChanged?.Invoke(1, tileOption2.Checked);
        }

        private void tileOption3_CheckedChanged(object sender, EventArgs e)
        {
            if (!setCheckboxes) TileOptionChanged?.Invoke(2, tileOption3.Checked);
        }

        private void tileOption4_CheckedChanged(object sender, EventArgs e)
        {
            if (!setCheckboxes) TileOptionChanged?.Invoke(3, tileOption4.Checked);
        }

        private void UpdateShortcuts(bool show=false)
        {
            if (show)
            {
                option1CheckBox.Text = "Flip Horizontal " + Environment.NewLine + EditorInstance.EditorControls.KeyBindPraser("FlipHTiles", true);
				option2CheckBox.Text = "Flip Vertical " + Environment.NewLine + EditorInstance.EditorControls.KeyBindPraser("FlipVTiles", true);
			}
            else
            {
                option1CheckBox.Text = "Flip Horizontal" + Environment.NewLine + string.Format("({0} - Selected Only)", EditorInstance.EditorControls.KeyBindPraser("FlipH"));
				option2CheckBox.Text = "Flip Vertical" + Environment.NewLine + string.Format("({0} - Selected Only)", EditorInstance.EditorControls.KeyBindPraser("FlipV"));
			}
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {

        }
        public void RefreshTileSelected()
        {
            EditorInstance.TilesToolbar.selectedTileLabel.Text = "Selected Tile: " + EditorInstance.ToolbarSelectedTile;
        }

        private void editTileInTileManiacToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (EditorInstance.mainform.IsDisposed) EditorInstance.mainform = new TileManiac.Mainform();
            if (!EditorInstance.mainform.Visible)
            {
                EditorInstance.mainform.Show();
            }
            if (EditorInstance.TilesConfig != null && EditorInstance.StageTiles != null)
            {
                if (!EditorInstance.mainform.Visible || EditorInstance.mainform.tcf == null)
                {
                    EditorInstance.mainform.LoadTileConfigViaIntergration(EditorInstance.TilesConfig, EditorInstance.SceneFilepath, SelectedTile);
                }
                else
                {
                    EditorInstance.mainform.SetCollisionIndex(SelectedTile);
                    EditorInstance.mainform.Activate();
                }

            }

        }

        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (TabControl.TabIndex == 0) EditorInstance.ChunksToolButton.IsChecked = false;
            else EditorInstance.ChunksToolButton.IsChecked = true;
        }

        private void TilesToolbar_Resize(object sender, EventArgs e)
        {


        }

        private void selectedTileLabel_Click(object sender, EventArgs e)
        {
		
        }
    }

}
