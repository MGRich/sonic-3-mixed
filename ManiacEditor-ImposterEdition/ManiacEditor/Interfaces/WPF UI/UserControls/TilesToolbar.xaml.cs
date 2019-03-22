﻿using System;
using System.Windows;
using System.Windows.Controls;
using RSDKv5;
using System.Diagnostics;

namespace ManiacEditor
{
	/// <summary>
	/// Interaction logic for TilesToolbar.xaml
	/// </summary>
	public partial class TilesToolbar : UserControl
	{
		public Editor EditorInstance;

		bool disposing = false;
		RSDKv5.GIF TileGridImage;
		string TilesImagePath;
		public Action<int> TileDoubleClick { get; set; } 

		bool TilesFlipHorizontal = false;
		bool TilesFlipVertical = false;

		public System.Windows.Forms.Panel tilePanel;
		public string CurrentColorPalette;

		public Interfaces.RetroEDTileList ChunkList;
		private Interfaces.RetroEDTileList TilesList;

		public System.Windows.Forms.Integration.WindowsFormsHost host;
		public System.Windows.Forms.Integration.WindowsFormsHost host3;

		public Action<int, bool> TileOptionChanged;

		CheckBox[] selectTileOptionsCheckboxes = new CheckBox[6];
		CheckBox[] tileOptionsCheckboxes = new CheckBox[4];
		bool setCheckboxes;

		public int SelectedTile
		{
			get
			{
				return TilesList.SelectedIndex;
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
			selectTileOptionsCheckboxes[option].IsChecked = state;
		}

		public void SetTileOptionState(int option, TileOptionState state)
		{
			setCheckboxes = true;
			switch (state)
			{
				case TileOptionState.Disabled:
					tileOptionsCheckboxes[option].IsEnabled = false;
					tileOptionsCheckboxes[option].IsChecked = false;
					break;
				case TileOptionState.Checked:
					tileOptionsCheckboxes[option].IsEnabled = true;
					tileOptionsCheckboxes[option].IsChecked = true;
					break;
				case TileOptionState.Unchcked:
					tileOptionsCheckboxes[option].IsEnabled = true;
					tileOptionsCheckboxes[option].IsChecked = false;
					break;
				case TileOptionState.Indeterminate:
					tileOptionsCheckboxes[option].IsEnabled = true;
					tileOptionsCheckboxes[option].IsChecked = null;
					break;
			}
			setCheckboxes = false;
		}

		public TilesToolbar(StageTiles tiles, String data_directory, String Colors, Editor instance)
		{
            try
            {
                InitializeComponent();
                SetupTilesList(instance);

                EditorInstance = instance;

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

                TilesImagePath = data_directory + "\\16x16Tiles.gif";
                TileGridImage = new GIF((TilesImagePath), Colors);

                UpdateShortcuts();

                ChunksReload();
                TilesReload(Colors);
            }
            catch (Exception ex)
            {
                Debug.Print(ex.ToString());
            }

		}

		public void SetupTilesList(Editor instance)
		{
			host = new System.Windows.Forms.Integration.WindowsFormsHost();
			host3 = new System.Windows.Forms.Integration.WindowsFormsHost();
			this.ChunkList = new ManiacEditor.Interfaces.RetroEDTileList(instance);

			this.tilePanel = new System.Windows.Forms.Panel();
			// 
			// ChunkList
			// 
			this.ChunkList.BackColor = System.Drawing.SystemColors.Window;
			this.ChunkList.Dock = System.Windows.Forms.DockStyle.Fill;
			this.ChunkList.ImageHeight = 16 * 8;
			this.ChunkList.ImageSize = 16 * 8;
			this.ChunkList.ImageWidth = 16 * 8;
			this.ChunkList.Location = new System.Drawing.Point(3, 3);
			this.ChunkList.Name = "ChunkList";
			this.ChunkList.ScrollValue = 0;
			this.ChunkList.SelectedIndex = -1;
			this.ChunkList.Size = new System.Drawing.Size(234, 290);
			this.ChunkList.TabIndex = 1;
			this.ChunkList.MouseClick += ChunkList_Click;
			this.ChunkList.SelectedIndexChanged += ChunkList_SelectedIndexChanged;

			// 
			// TileList
			//
			this.TilesList = new Interfaces.RetroEDTileList(instance);
			this.TilesList.BackColor = System.Drawing.SystemColors.Window;
			this.TilesList.Dock = System.Windows.Forms.DockStyle.Fill;
			this.TilesList.ImageHeight = 16 * 1;
			this.TilesList.ImageSize = 16 * 1;
			this.TilesList.ImageWidth = 16 * 1;
			this.TilesList.Location = new System.Drawing.Point(3, 3);
			this.TilesList.Name = "TilesList";
			this.TilesList.ScrollValue = 0;
			this.TilesList.SelectedIndex = -1;
			this.TilesList.Size = new System.Drawing.Size(234, 290);
			this.TilesList.TabIndex = 1;
			this.TilesList.MouseClick += TilesList_MouseClick; ;
			this.TilesList.SelectedIndexChanged += TilesListList_SelectedIndexChanged;
			this.TilesList.MouseMove += TilesList_MouseMove;
			this.TilesList.MouseDoubleClick += TilesList_MouseDoubleClick;
			this.tilePanel.Controls.Add(this.TilesList);

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
			this.tilePanel.Dock = System.Windows.Forms.DockStyle.Fill;


			host.Child = tilePanel;
			host3.Child = ChunkList;
			TileViewer.Children.Add(host);
			ChunksPage.Children.Add(host3);
		}

		private void TilesList_MouseDoubleClick(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (disposing) return;
			if (e.Button == System.Windows.Forms.MouseButtons.Left)
			{
				if (SelectedTile != -1 && TileDoubleClick != null)
				{
					TileDoubleClick(SelectedTile);
				}
			}
		}

		private void TilesList_MouseClick(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			TilesList.editCollisionToolStripMenuItem.Enabled = false;
			if (e.Button == System.Windows.Forms.MouseButtons.Right)
			{
				if (TilesList.SelectedIndex != -1)
				{
					TilesList.editCollisionToolStripMenuItem.Enabled = true;
					TilesList.editCollisionToolStripMenuItem.Text = string.Format("Edit Collision of Tile {0} with Tile Maniac", SelectedTile);

					TilesList.Tiles16ToolStrip.Show(TilesList, e.Location);
				}
				else
				{
					TilesList.editCollisionToolStripMenuItem.Enabled = false;
					TilesList.editCollisionToolStripMenuItem.Text = string.Format("Edit Collision of Tile {0} with Tile Maniac", "N/A");

					TilesList.Tiles16ToolStrip.Show(TilesList, e.Location);
				}
			}
		}

		private void TileListDoubleClick()
		{

		}

		private void TilesList_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (disposing) return;
			if (e.Button == System.Windows.Forms.MouseButtons.Left)
			{
				if (SelectedTile != -1)
				{
					Int32 val = SelectedTile;
					TilesList.DoDragDrop(val, System.Windows.Forms.DragDropEffects.Move);
				}
			}
		}

		private void TilePanel_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
		{
			EditorInstance.UIControl.GraphicPanel_OnKeyDown(sender, e);
		}

		private void TilePanel_KeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
		{
			EditorInstance.UIControl.GraphicPanel_OnKeyUp(sender, e);
		}

		private void ChunkList_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (EditorInstance.IsChunksEdit())
			{
				EditorInstance.TilesToolbar.SelectedTileLabel.Content = "Selected Chunk: " + ChunkList.SelectedIndex.ToString();

			}
		}

		private void TilesListList_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (EditorInstance.IsTilesEdit())
			{
				EditorInstance.TilesToolbar.SelectedTileLabel.Content = "Selected Tile: " + TilesList.SelectedIndex.ToString();
			}
		}

		private void ChunkList_Click(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			ChunkList.moveChunkDownToolStripMenuItem.Enabled = false;
			ChunkList.moveChunkUpToolStripMenuItem.Enabled = false;
			if (e.Button == System.Windows.Forms.MouseButtons.Right)
			{
				if (ChunkList.SelectedIndex != -1)
				{
					ChunkList.removeChunkToolStripMenuItem.Enabled = true;
					ChunkList.duplicateChunkToolStripMenuItem.Enabled = true;

					ChunkList.Chunks128ToolStrip.Show(ChunkList, e.Location);
				}
				else
				{
					ChunkList.removeChunkToolStripMenuItem.Enabled = false;
					ChunkList.duplicateChunkToolStripMenuItem.Enabled = false;

					ChunkList.Chunks128ToolStrip.Show(ChunkList, e.Location);
				}
			}
		}

		public void RemoveChunk(int index)
		{
			EditorInstance.EditorChunk.StageStamps.StampList.RemoveAt(index);
			ChunkList.Images.Clear();
			EditorInstance.EditorChunk.DisposeTextures();
			if (index != 0) ChunkList.SelectedIndex = index--;
			else ChunkList.SelectedIndex = -1;



			ChunksReload();
		}

		public void DuplicateChunk(int index)
		{
			EditorInstance.EditorChunk.StageStamps.StampList.Add(EditorInstance.EditorChunk.StageStamps.StampList[index]);

			ChunksReload();
		}

		private void trackBar1_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
		{
			if (disposing) return;
			int scale = 16 << (int)trackBar1.Value;
			TilesList.ImageSize = scale;
		}

		public void Reload(string colors = null)
		{
			if (disposing) return;
			TilesReload(colors);
			ChunksReload();
		}

		public void TilesReload(string colors = "")
		{
			if (colors != "") CurrentColorPalette = colors;
			else if (CurrentColorPalette != null) colors = CurrentColorPalette;

			if (disposing) return;
			TilesList.Images.Clear();
			TileGridImage = new GIF((TilesImagePath), colors);

			for (int i = 0; i < 1024; i++)
			{
				TilesList.Images.Add(TileGridImage.GetBitmap(new System.Drawing.Rectangle(0,16*i,16,16) , TilesFlipHorizontal, TilesFlipVertical));
			}
			int indexStorage = TilesList.SelectedIndex;
			TilesList.SelectedIndex = -1;
			TilesList.SelectedIndex = indexStorage;
			TilesList.Refresh();
		}

		public void ChunksReload()
		{
			if (disposing) return;
			ChunkList.Images.Clear();
			if (EditorInstance.EditorChunk != null)
			{
				EditorInstance.EditorChunk.DisposeTextures();

				int indexStorage = ChunkList.SelectedIndex;
				ChunkList.SelectedIndex = -1;
				if (EditorInstance.EditorChunk.StageStamps != null)
				{
					for (int i = 0; i < EditorInstance.EditorChunk.StageStamps.StampList.Count; i++)
					{
						ChunkList.Images.Add(EditorInstance.EditorChunk.GetChunkTexture(i));
					}
					ChunkList.SelectedIndex = indexStorage;
				}
				ChunkList.Refresh();
			}

		}

		public void Dispose()
		{
			disposing = true;
			if (tilePanel != null)
			{
				tilePanel.Controls.Clear();
				tilePanel.Dispose();
				tilePanel = null;
			}
			if (TilesList != null)
			{
				TilesList.Controls.Clear();
				TilesList.Dispose();
				TilesList = null;
			}
			if (ChunkList != null)
			{
				ChunkList.Controls.Clear();
				ChunkList.Dispose();
				ChunkList = null;
			}
			if (host != null)
			{
				host.Child.Dispose();
				host = null;
			}
			if (host3 != null)
			{
				host3.Child.Dispose();
				host3 = null;
			}


		}

		private void TilesToolbar_Load(object sender, RoutedEventArgs e)
		{
			Debug.WriteLine("Send to debug output.");
			trackBar1.Value = Properties.Settings.Default.tileToolbarDefaultZoomLevel;

		}

		private void option1CheckBox_CheckedChanged(object sender, RoutedEventArgs e)
		{
			TilesFlipHorizontal = option1CheckBox.IsChecked.Value;
			TilesReload();
		}

		private void option2CheckBox_CheckedChanged(object sender, RoutedEventArgs e)
		{
			TilesFlipVertical = option2CheckBox.IsChecked.Value;
			TilesReload();
		}

		private void tileOption1_CheckedChanged(object sender, RoutedEventArgs e)
		{
			if (!setCheckboxes) TileOptionChanged?.Invoke(0, tileOption1.IsChecked.Value);
		}

		private void tileOption2_CheckedChanged(object sender, RoutedEventArgs e)
		{
			if (!setCheckboxes) TileOptionChanged?.Invoke(1, tileOption2.IsChecked.Value);
		}

		private void tileOption3_CheckedChanged(object sender, RoutedEventArgs e)
		{
			if (!setCheckboxes) TileOptionChanged?.Invoke(2, tileOption3.IsChecked.Value);
		}

		private void tileOption4_CheckedChanged(object sender, RoutedEventArgs e)
		{
			if (!setCheckboxes) TileOptionChanged?.Invoke(3, tileOption4.IsChecked.Value);
		}

		private void UpdateShortcuts(bool show = false)
		{
			if (show)
			{
				option1CheckBox.Content = "Flip Horizontal " + Environment.NewLine + EditorInstance.UIControl.KeyBindPraser("FlipHTiles", true);
				option2CheckBox.Content = "Flip Vertical " + Environment.NewLine + EditorInstance.UIControl.KeyBindPraser("FlipVTiles", true);
			}
			else
			{
				option1CheckBox.Content = "Flip Horizontal" + Environment.NewLine + string.Format("({0} - Selected Only)", EditorInstance.UIControl.KeyBindPraser("FlipH"));
				option2CheckBox.Content = "Flip Vertical" + Environment.NewLine + string.Format("({0} - Selected Only)", EditorInstance.UIControl.KeyBindPraser("FlipV"));
			}
		}

		private void trackBar1_Scroll(object sender, EventArgs e)
		{

		}
		public void RefreshTileSelected()
		{
			EditorInstance.TilesToolbar.SelectedTileLabel.Content = "Selected Tile: " + EditorInstance.ToolbarSelectedTile;
		}

		private void tabControl1_SelectedIndexChanged(object sender, SelectionChangedEventArgs e)
		{
			if (e.Source is TabControl)
			{
				if (TabControl.SelectedIndex == 0) EditorInstance.ChunksToolButton.IsChecked = false;
				else
				{
					EditorInstance.ChunksToolButton.IsChecked = true;
					EditorInstance.TilesToolbar.SelectedTileLabel.Content = "Selected Chunk: " + ChunkList.SelectedIndex.ToString();
					TileChunkOptionsMenu.IsEnabled = true;
				}
			}
			if (TabControl.SelectedIndex == 0)
			{
				TilesReload();
				EditorInstance.TilesToolbar.SelectedTileLabel.Content = "Selected Tile: " + TilesList.SelectedIndex.ToString();
				TileChunkOptionsMenu.IsEnabled = false;
			}

		}

		private void TilesToolbar_Resize(object sender, EventArgs e)
		{


		}

		private void selectedTileLabel_Click(object sender, EventArgs e)
		{

		}

		private void UserControl_Unloaded(object sender, RoutedEventArgs e)
		{
			//this.Dispose();
		}

		private void TabItem_MouseUp(object sender, System.Windows.Input.MouseButtonEventArgs e)
		{
			if (TabControl.SelectedIndex == 0)
			{
				//tilesList.graphicPanel.Render();
			}
		}

		private void SaveChunksManually_Click(object sender, RoutedEventArgs e)
		{
			try
			{
				if (EditorInstance.EditorChunk.StageStamps?.loadstate == Stamps.LoadState.Upgrade)
				{
					MessageBoxResult result = MessageBox.Show("This Editor Chunk File needs to be updated to a newer version of the format. This will happen almost instantly, however you will be unable to use your chunks in a previous version of maniac on this is done. Would you like to continue?" + Environment.NewLine + "(Click Yes to Save, Click No to Continue without Saving Your Chunks)", "Chunk File Format Upgrade Required", MessageBoxButton.YesNo, MessageBoxImage.Warning);
					if (result == MessageBoxResult.Yes)
					{
						EditorInstance.EditorChunk.StageStamps?.Write(EditorInstance.EditorPath.Stamps_Source);
					}
				}
				else
				{
					EditorInstance.EditorChunk.StageStamps?.Write(EditorInstance.EditorPath.Stamps_Source);
				}
			}
			catch (Exception ex)
			{
				EditorInstance.ShowError($@"Failed to save StageStamps to file '{EditorInstance.EditorPath.Stamps_Source}'
Error: {ex.Message}");
			}

			TileChunkOptionsMenu.IsOpen = false;
		}

        private void AutoGenerateChunks_Click(object sender, RoutedEventArgs e)
        {
            if (EditorInstance.EditLayerA != null && EditorInstance.EditLayerB != null)
            {
                EditorInstance.EditorChunk.AutoGenerateChunks(EditorInstance.EditLayerA, EditorInstance.EditLayerB);
                ChunksReload();
            }
        }

        private void TrackBar2_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (ChunkList != null && trackBar2 != null)
            {
                if (disposing) return;
                int scale = 32 << (int)trackBar2.Value;
                ChunkList.ImageSize = scale;
            }

        }
    }
}
