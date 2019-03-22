﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.CompilerServices;
using System.Diagnostics;
using RSDKv5;
using IronPython.Modules;
using Microsoft.Scripting.Utils;
using ManiacEditor.Interfaces;
using WPFGrid = Xceed.Wpf.Toolkit.PropertyGrid.PropertyGrid;
using Xceed.Wpf.Toolkit.PropertyGrid;

namespace ManiacEditor
{
	public partial class EntitiesToolbar : UserControl
	{
		public Action<int> SelectedEntity;
		public Action<Actions.IAction> AddAction;
		public Action<RSDKv5.SceneObject> Spawn;

		public bool multipleObjects = false;

		public WPFGrid entityProperties2;

		public Editor EditorInstance;

		private List<RSDKv5.SceneEntity> _entities;
		private List<int> _selectedEntitySlots = new List<int>();
		private BindingSource _bindingSceneObjectsSource = new BindingSource();

		private RSDKv5.SceneEntity currentEntity;

		public List<RSDKv5.SceneEntity> Entities
		{
			set
			{
				_entities = value.ToList();
				_entities.Sort((x, y) => x.SlotID.CompareTo(y.SlotID));
				UpdateEntitiesList();
			}
		}

		bool updateSelected = true;

		public List<RSDKv5.SceneEntity> SelectedEntities
		{
			set
			{
				UpdateEntitiesProperties(value);
			}
		}

		public bool NeedRefresh;

		/*private RSDKv5.SceneObject currentObject;*/

		public EntitiesToolbar(List<RSDKv5.SceneObject> sceneObjects, Editor instance)
		{
			InitializeComponent();

			EditorInstance = instance;

			if (Settings.mySettings.ExperimentalPropertyGridView)
			{
				entityProperties2 = new WPFGrid();
				entityProperties2.PropertyValueChanged += new Xceed.Wpf.Toolkit.PropertyGrid.PropertyValueChangedEventHandler(entityProperties2_PropertyValueChanged);
				elementHost1.Child = entityProperties2;
				entityProperties2.IsCategorized = true;

			}
			else
			{
				elementHost1.Enabled = false;
				elementHost1.Visible = false;
			}



			RefreshObjects(sceneObjects);

			defaultFilter.Items.Add("Mania (2)");
			defaultFilter.Items.Add("Encore (4)");
			defaultFilter.Items.Add("Both (1)");
			defaultFilter.Items.Add("Pinball (255)");
			defaultFilter.Items.Add("Other (0)");
			defaultFilter.SelectedIndex = 0;

			UpdateFilterNames();

		}

		public void UpdateFilterNames()
		{
			if (Properties.Settings.Default.useBitOperators)
			{
				maniaFilterCheck.Text = "Mania (0b0010)";
				encoreFilterCheck.Text = "Encore (0b0100)";
				otherFilterCheck.Text = "Other (0b1000)";
				bothFilterCheck.Text = "Both (0b0001)";
			}
			else
			{
				maniaFilterCheck.Text = "Mania (2)";
				encoreFilterCheck.Text = "Encore (4)";
				otherFilterCheck.Text = "Other (0)";
				bothFilterCheck.Text = "Both (1 and 5)";

			}

		}

		private void UpdateEntitiesList()
		{
			entitiesList.Items.Clear();
			entitiesList.ResetText();
			if (currentEntity != null && _entities.Contains(currentEntity))
			{
				entitiesList.SelectedText = String.Format("{0} - {1}", currentEntity.SlotID, currentEntity.Object.Name.Name.ToString());
				MoreButton.Enabled = true;
			}
			else if (currentEntity != null) MoreButton.Enabled = true;
			else MoreButton.Enabled = false;
		}

		public void RefreshObjects(List<RSDKv5.SceneObject> sceneObjects)
		{
			sceneObjects.Sort((x, y) => x.Name.ToString().CompareTo(y.Name.ToString()));
			var bindingSceneObjectsList = new BindingList<RSDKv5.SceneObject>(sceneObjects);
			_bindingSceneObjectsSource.DataSource = bindingSceneObjectsList;

			if (_bindingSceneObjectsSource != null && _bindingSceneObjectsSource.Count > 0)
			{
				cbSpawn.DataSource = _bindingSceneObjectsSource;
				cbSpawn.SelectedIndex = 0;
			}
		}



		public void EntitiesList_SelectedIndexChanged(object sender, EventArgs e)
		{
			String input = entitiesList.Text.ToString();
			string output = new string(input.TakeWhile(char.IsDigit).ToArray());
			int selectedIndex = Int32.Parse(output);
			int selectedSlotID = GetIndexOfSlotID(selectedIndex);


			if (updateSelected && !multipleObjects || !searchBox.Text.Contains("")) SelectedEntity?.Invoke(_entities[entitiesList.SelectedIndex].SlotID);
			if (updateSelected && multipleObjects || searchBox.Text.Contains("")) SelectedEntity?.Invoke(_entities[selectedSlotID].SlotID);

			if (currentEntity != null) MoreButton.Enabled = true;
			else MoreButton.Enabled = false;


		}

		public int GetIndexOfSlotID(int slotID)
		{
			int index = 0;
			for (int i = 0; i < _entities.Count; i++)
			{
				if (_entities[i].SlotID == (ushort)slotID)
				{
					index = i;
				}
			}
			return index;
		}

		private void AddProperty(LocalProperties properties, int category_index, string category, string name, string value_type, object value, bool read_only = false)
		{
			properties.Add(String.Format("{0},{1}", category, name), new LocalProperty(name, value_type, category_index, category, name, true, read_only, value, ""));
			//properties.Add(String.Format("{0},{1}", category, name), new LocalProperty(name, value_type, ""));

		}

		private System.Drawing.Color SetForeColor()
		{
			System.Drawing.Color ForeColor;
			if (currentEntity.attributesMap.ContainsKey("filter") && currentEntity.attributesMap["filter"].Type == AttributeTypes.UINT8)
			{
				int filter = currentEntity.attributesMap["filter"].ValueUInt8;
				switch (filter)
				{
					case 1:
						ForeColor = System.Drawing.Color.DarkBlue;
						break;
					case 2:
						ForeColor = System.Drawing.Color.DarkRed;
						break;
					case 4:
						ForeColor = System.Drawing.Color.DarkGreen;
						break;
					case 5:
						ForeColor = System.Drawing.Color.DarkBlue;
						break;
					default:
						ForeColor = System.Drawing.Color.SandyBrown;
						break;

				}
			}

			return System.Drawing.Color.Aqua;
		}

		private void UpdateEntitiesProperties(List<RSDKv5.SceneEntity> selectedEntities)
		{
			//TODO: Allow to change multiple entities
			/*bool first_entity = true;
            RSDKv5.SceneObject commonObject = null;

            foreach (var entity in selectedEntities)
            {
                if (first_entity) commonObject = entity.Object;
                else if (entity.Object != commonObject) commonObject = null;
            }

            if (commonObject != currentObject)
            {
                currentObject = commonObject;*/

			multipleObjects = false;
			bool isCommonObjects = false;

			if (selectedEntities.Count != 1)
			{
				if (Settings.mySettings.ExperimentalPropertyGridView) entityProperties2.SelectedObject = null;
				else entityProperties.SelectedObject = null;
				currentEntity = null;
				entitiesList.ResetText();
				_selectedEntitySlots.Clear();
				if (selectedEntities.Count > 1)
				{
					isCommonObjects = true;
					entitiesList.SelectedText = String.Format("{0} entities selected", selectedEntities.Count);
					entitiesList.Items.Clear();
					entitiesList.DisplayMember = "Text";
					entitiesList.Tag = "Value";
					foreach (RSDKv5.SceneEntity selectedEntity in selectedEntities)
					{

						entitiesList.Items.Add(new { Text = (String.Format("{1} - {0}", (searchBox.Text != "" ? selectedEntity.Object.Name.Name.Contains(searchBox.Text) ? selectedEntity.Object.Name.Name.ToString() : "" : selectedEntity.Object.Name.Name.ToString()), selectedEntity.SlotID)), Value = selectedEntity.SlotID });
						_selectedEntitySlots.Add(selectedEntity.SlotID);
					}
					//entitiesList.Items.AddRange(selectedEntities.Select(x => String.Format("{0} - {1}", x.SlotID, x.Object.Name)).ToArray());
					multipleObjects = true;
					//_selectedEntitySlots.AddRange(selectedEntities.Select(x => (int)x.SlotID).ToList());
					string commonObject = selectedEntities[0].Object.Name.Name;
					foreach (RSDKv5.SceneEntity selectedEntity in selectedEntities)
					{
						if (selectedEntity.Object.Name.Name != commonObject)
						{
							isCommonObjects = false;
						}
					}

				}
				if (isCommonObjects == true)
				{
					//Keep Going (if Implemented; which it's not)
					return;
				}
				else
				{
					return;
				}

			}


			RSDKv5.SceneEntity entity = selectedEntities[0];

			if (entity == currentEntity) return;
			currentEntity = entity;

			if (entitiesList.SelectedIndex >= 0 && entitiesList.SelectedIndex < _entities.Count && _entities[entitiesList.SelectedIndex] == currentEntity)
			{
				// Than it is called from selected item in the menu, so changeing the text will remove it, we don't want that
			}
			else
			{
				entitiesList.ResetText();
				entitiesList.SelectedText = String.Format("{1} - {0}", currentEntity.Object.Name.Name.ToString(), currentEntity.SlotID);
				//entitiesList.SelectedIndex = entities.IndexOf(entity);
			}


			LocalProperties objProperties = new LocalProperties();
			int category_index = 2 + entity.Attributes.Count;
			AddProperty(objProperties, category_index, "object", "name", "string", entity.Object.Name.ToString(), false);
			AddProperty(objProperties, category_index, "object", "entitySlot", "ushort", entity.SlotID, false);
			--category_index;
			AddProperty(objProperties, category_index, "position", "x", "float", entity.Position.X.High + ((float)entity.Position.X.Low / 0x10000));
			AddProperty(objProperties, category_index, "position", "y", "float", entity.Position.Y.High + ((float)entity.Position.Y.Low / 0x10000));
			--category_index;


			foreach (var attribute in entity.Object.Attributes)
			{
				string attribute_name = attribute.Name.ToString();
				var attribute_value = currentEntity.GetAttribute(attribute_name);
				switch (attribute.Type)
				{
					case RSDKv5.AttributeTypes.UINT8:
						AddProperty(objProperties, category_index, attribute_name, "uint8", "byte", attribute_value.ValueUInt8);
						break;
					case RSDKv5.AttributeTypes.UINT16:
						AddProperty(objProperties, category_index, attribute_name, "uint16", "ushort", attribute_value.ValueUInt16);
						break;
					case RSDKv5.AttributeTypes.UINT32:
						AddProperty(objProperties, category_index, attribute_name, "uint32", "uint", attribute_value.ValueUInt32);
						break;
					case RSDKv5.AttributeTypes.INT8:
						AddProperty(objProperties, category_index, attribute_name, "int8", "sbyte", attribute_value.ValueInt8);
						break;
					case RSDKv5.AttributeTypes.INT16:
						AddProperty(objProperties, category_index, attribute_name, "int16", "short", attribute_value.ValueInt16);
						break;
					case RSDKv5.AttributeTypes.INT32:
						AddProperty(objProperties, category_index, attribute_name, "int32", "int", attribute_value.ValueInt32);
						break;
					case RSDKv5.AttributeTypes.VAR:
						AddProperty(objProperties, category_index, attribute_name, "var", "uint", attribute_value.ValueVar);
						break;
					case RSDKv5.AttributeTypes.BOOL:
						AddProperty(objProperties, category_index, attribute_name, "bool", "bool", attribute_value.ValueBool);
						break;
					case RSDKv5.AttributeTypes.STRING:
						AddProperty(objProperties, category_index, attribute_name, "string", "string", attribute_value.ValueString);
						break;
					case RSDKv5.AttributeTypes.POSITION:
						AddProperty(objProperties, category_index, attribute_name, "x", "float", attribute_value.ValuePosition.X.High + ((float)attribute_value.ValuePosition.X.Low / 0x10000));
						AddProperty(objProperties, category_index, attribute_name, "y", "float", attribute_value.ValuePosition.Y.High + ((float)attribute_value.ValuePosition.Y.Low / 0x10000));
						break;
					case RSDKv5.AttributeTypes.COLOR:
						var color = attribute_value.ValueColor;
						AddProperty(objProperties, category_index, attribute_name, "color", "color", System.Drawing.Color.FromArgb(255 /* color.A */, color.R, color.G, color.B));
						break;
				}
				--category_index;
			}
			if (Settings.mySettings.ExperimentalPropertyGridView) entityProperties2.SelectedObject = new LocalPropertyGridObject(objProperties);
			else entityProperties.SelectedObject = new LocalPropertyGridObject(objProperties);


		}

		public void UpdateCurrentEntityProperites()
		{
			object selectedObject = (Settings.mySettings.ExperimentalPropertyGridView ? entityProperties2.SelectedObject : entityProperties.SelectedObject);
			if (selectedObject is LocalPropertyGridObject obj)
			{
				obj.setValue("position,x", currentEntity.Position.X.High + ((float)currentEntity.Position.X.Low / 0x10000));
				obj.setValue("position,y", currentEntity.Position.Y.High + ((float)currentEntity.Position.Y.Low / 0x10000));
				foreach (var attribute in currentEntity.Object.Attributes)
				{
					string attribute_name = attribute.Name.ToString();
					var attribute_value = currentEntity.GetAttribute(attribute_name);
					switch (attribute.Type)
					{
						case RSDKv5.AttributeTypes.UINT8:
							obj.setValue(String.Format("{0},{1}", attribute_name, "uint8"), attribute_value.ValueUInt8);
							break;
						case RSDKv5.AttributeTypes.UINT16:
							obj.setValue(String.Format("{0},{1}", attribute_name, "uint16"), attribute_value.ValueUInt16);
							break;
						case RSDKv5.AttributeTypes.UINT32:
							obj.setValue(String.Format("{0},{1}", attribute_name, "uint32"), attribute_value.ValueUInt32);
							break;
						case RSDKv5.AttributeTypes.INT8:
							obj.setValue(String.Format("{0},{1}", attribute_name, "int8"), attribute_value.ValueInt8);
							break;
						case RSDKv5.AttributeTypes.INT16:
							obj.setValue(String.Format("{0},{1}", attribute_name, "int16"), attribute_value.ValueInt16);
							break;
						case RSDKv5.AttributeTypes.INT32:
							obj.setValue(String.Format("{0},{1}", attribute_name, "int32"), attribute_value.ValueInt32);
							break;
						case RSDKv5.AttributeTypes.VAR:
							obj.setValue(String.Format("{0},{1}", attribute_name, "var"), attribute_value.ValueVar);
							break;
						case RSDKv5.AttributeTypes.BOOL:
							obj.setValue(String.Format("{0},{1}", attribute_name, "bool"), attribute_value.ValueBool);
							break;
						case RSDKv5.AttributeTypes.STRING:
							obj.setValue(String.Format("{0},{1}", attribute_name, "string"), attribute_value.ValueString);
							break;
						case RSDKv5.AttributeTypes.POSITION:
							obj.setValue(String.Format("{0},{1}", attribute_name, "x"), attribute_value.ValuePosition.X.High + ((float)attribute_value.ValuePosition.X.Low / 0x10000));
							obj.setValue(String.Format("{0},{1}", attribute_name, "y"), attribute_value.ValuePosition.Y.High + ((float)attribute_value.ValuePosition.Y.Low / 0x10000));
							break;
						case RSDKv5.AttributeTypes.COLOR:
							var color = attribute_value.ValueColor;
							obj.setValue(String.Format("{0},{1}", attribute_name, "color"), System.Drawing.Color.FromArgb(255 /* color.A */, color.R, color.G, color.B));
							break;
					}
				}
				NeedRefresh = true;
			}
		}

		public void PropertiesRefresh()
		{
			if (Settings.mySettings.ExperimentalPropertyGridView) entityProperties2.Update();
			else entityProperties.Refresh();
			NeedRefresh = false;
		}
		private void setEntitiyProperty(RSDKv5.SceneEntity entity, string tag, object value, object oldValue)
		{
			string[] parts = tag.Split(',');
			string category = parts[0];
			string name = parts[1];
			if (category == "position")
			{
				float fvalue = (float)value;
				if (fvalue < Int16.MinValue || fvalue > Int16.MaxValue)
				{
					// Invalid
					var obj = (Settings.mySettings.ExperimentalPropertyGridView ? entityProperties2.SelectedObject as LocalPropertyGridObject : entityProperties.SelectedObject as LocalPropertyGridObject);
					obj.setValue(tag, oldValue);
					return;
				}
				var pos = entity.Position;
				if (name == "x")
				{
					pos.X.High = (short)fvalue;
					pos.X.Low = (ushort)(fvalue * 0x10000);
				}
				else if (name == "y")
				{
					pos.Y.High = (short)fvalue;
					pos.Y.Low = (ushort)(fvalue * 0x10000);
				}
				entity.Position = pos;
				if (entity == currentEntity)
					UpdateCurrentEntityProperites();
			}
			else if (category == "object")
			{
				if (name == "name" && oldValue != value)
				{
					var info = RSDKv5.Objects.GetObjectInfo(new RSDKv5.NameIdentifier(value as string));
					if (info == null)
					{
						System.Windows.Forms.MessageBox.Show("Unknown Object", "", MessageBoxButtons.OK, MessageBoxIcon.Error);
						return;
					}
					var objects = ((BindingList<RSDKv5.SceneObject>)_bindingSceneObjectsSource.DataSource).ToList();
					var obj = objects.FirstOrDefault(t => t.Name.Name == value as string);
					if (obj != null)
					{
						entity.Attributes.Clear();
						entity.attributesMap.Clear();
						foreach (var attb in info.Attributes)
						{
							var attributeValue = new RSDKv5.AttributeValue(attb.Type);
							entity.Attributes.Add(attributeValue);
							entity.attributesMap.Add(attb.Name.Name, attributeValue);
						}
						entity.Object.Entities.Remove(entity);
						entity.Object = obj;
						obj.Entities.Add(entity);
					}
					else
					{
						// The new object
						var sobj = new RSDKv5.SceneObject(info.Name, info.Attributes);

						entity.Attributes.Clear();
						entity.attributesMap.Clear();
						foreach (var attb in info.Attributes)
						{
							var attributeValue = new RSDKv5.AttributeValue(attb.Type);
							entity.Attributes.Add(attributeValue);
							entity.attributesMap.Add(attb.Name.Name, attributeValue);
						}
						entity.Object.Entities.Remove(entity);
						entity.Object = sobj;
						sobj.Entities.Add(entity);
						_bindingSceneObjectsSource.Add(sobj);
					}
				}
				if (name == "entitySlot" && oldValue != value)
				{
					ushort newSlot = (ushort)value;
					// Check if slot has been used
					var objects = ((BindingList<RSDKv5.SceneObject>)_bindingSceneObjectsSource.DataSource).ToList();
					foreach (var obj in objects)
					{
						if (obj.Entities.Any(t => t.SlotID == newSlot))
						{
							System.Windows.Forms.MessageBox.Show($"Slot {newSlot} is currently being used by a {obj.Name.ToString()}",
								"Slot in use!", MessageBoxButtons.OK, MessageBoxIcon.Error);
							return;
						}
					}
					// Passed
					entity.SlotID = newSlot;
				}
				// Update Properties
				currentEntity = null;
				UpdateEntitiesProperties(new List<RSDKv5.SceneEntity>() { entity });
			}
			else
			{
				var attribute = entity.GetAttribute(category);
				switch (attribute.Type)
				{
					case RSDKv5.AttributeTypes.UINT8:
						attribute.ValueUInt8 = (byte)value;
						break;
					case RSDKv5.AttributeTypes.UINT16:
						attribute.ValueUInt16 = (ushort)value;
						break;
					case RSDKv5.AttributeTypes.UINT32:
						attribute.ValueUInt32 = (uint)value;
						break;
					case RSDKv5.AttributeTypes.INT8:
						attribute.ValueInt8 = (sbyte)value;
						break;
					case RSDKv5.AttributeTypes.INT16:
						attribute.ValueInt16 = (short)value;
						break;
					case RSDKv5.AttributeTypes.INT32:
						attribute.ValueInt32 = (int)value;
						break;
					case RSDKv5.AttributeTypes.VAR:
						attribute.ValueVar = (uint)value;
						break;
					case RSDKv5.AttributeTypes.BOOL:
						attribute.ValueBool = (bool)value;
						break;
					case RSDKv5.AttributeTypes.STRING:
						attribute.ValueString = (string)value;
						break;
					case RSDKv5.AttributeTypes.POSITION:
						float fvalue = (float)value;
						if (fvalue < Int16.MinValue || fvalue > Int16.MaxValue)
						{
							// Invalid
							var obj = (Settings.mySettings.ExperimentalPropertyGridView ? entityProperties2.SelectedObject as LocalPropertyGridObject : entityProperties.SelectedObject as LocalPropertyGridObject);
							obj.setValue(tag, oldValue);
							return;
						}
						var pos = attribute.ValuePosition;
						if (name == "x")
						{
							pos.X.High = (short)fvalue;
							pos.X.Low = (ushort)(fvalue * 0x10000);
						}
						else if (name == "y")
						{
							pos.Y.High = (short)fvalue;
							pos.Y.Low = (ushort)(fvalue * 0x10000);
						}
						attribute.ValuePosition = pos;
						if (entity == currentEntity)
							UpdateCurrentEntityProperites();
						break;
					case RSDKv5.AttributeTypes.COLOR:
						System.Drawing.Color c = (System.Drawing.Color)value;
						attribute.ValueColor = new RSDKv5.Color(c.R, c.G, c.B, c.A);
						break;
				}
			}
		}


		private void entityProperties_PropertyValueChanged(object s, System.Windows.Forms.PropertyValueChangedEventArgs e)
		{
			if (!Settings.mySettings.ExperimentalPropertyGridView)
			{
				string tag = e.ChangedItem.PropertyDescriptor.Name;
				AddAction?.Invoke(new Actions.ActionEntityPropertyChange(currentEntity, tag, e.OldValue, e.ChangedItem.Value, new Action<RSDKv5.SceneEntity, string, object, object>(setEntitiyProperty)));
				setEntitiyProperty(currentEntity, tag, e.ChangedItem.Value, e.OldValue);
			}


		}

		private void entityProperties2_PropertyValueChanged(object s, Xceed.Wpf.Toolkit.PropertyGrid.PropertyValueChangedEventArgs e)
		{
			if (Settings.mySettings.ExperimentalPropertyGridView)
			{
				string tag = e.OriginalSource.ToString();
				AddAction?.Invoke(new Actions.ActionEntityPropertyChange(currentEntity, tag, e.OldValue, e.NewValue, new Action<RSDKv5.SceneEntity, string, object, object>(setEntitiyProperty)));
				setEntitiyProperty(currentEntity, tag, e.NewValue, e.OldValue);
			}
		}

		private void entitiesList_DropDown(object sender, EventArgs e)
		{
			if (multipleObjects == false)
			{
				// It is slow to update the list, so lets generate it when the menu opens
				entitiesList.Items.Clear();
				entitiesList.Items.AddRange(_entities.Select(x => String.Format("{0} - {1}", x.SlotID, x.Object.Name.Name)).Where(x => x.Contains(searchBox.Text != "" ? searchBox.Text : x)).ToArray());
			}


		}

		private void ColorizeList()
		{
		}

		private void btnSpawn_Click(object sender, EventArgs e)
		{
			if (cbSpawn?.SelectedItem != null
				&& cbSpawn.SelectedItem is RSDKv5.SceneObject)
			{
				switch (Properties.Settings.Default.DefaultFilter[0])
				{
					case 'M':
						EditorInstance.entities.DefaultFilter = 2;
						break;
					case 'E':
						EditorInstance.entities.DefaultFilter = 4;
						break;
					case 'B':
						EditorInstance.entities.DefaultFilter = 1;
						break;
					case 'P':
						EditorInstance.entities.DefaultFilter = 255;
						break;
					default:
						EditorInstance.entities.DefaultFilter = 0;
						break;
				}
				Spawn?.Invoke(cbSpawn.SelectedItem as RSDKv5.SceneObject);
			}
		}


		private void maniaFilterCheck_CheckedChanged(object sender, EventArgs e)
		{
			EditorInstance.entities.FilterRefreshNeeded = true;
		}

		private void encoreFilterCheck_CheckedChanged(object sender, EventArgs e)
		{
			EditorInstance.entities.FilterRefreshNeeded = true;
		}

		private void bothFilterCheck_CheckedChanged(object sender, EventArgs e)
		{
			EditorInstance.entities.FilterRefreshNeeded = true;
		}

		private void otherFilterCheck_CheckedChanged(object sender, EventArgs e)
		{
			EditorInstance.entities.FilterRefreshNeeded = true;
		}

		// Temporary solution to "filter" attribute being missing in old Scenes
		// TODO: Replace with proper Object/Entity Manager
		private void addFilterButton_Click(object sender, EventArgs e)
		{
			if (currentEntity != null && !currentEntity.attributesMap.ContainsKey("filter"))
			{
				currentEntity.AddAttributeToObject("filter", RSDKv5.AttributeTypes.UINT8);
				PropertiesRefresh();
			}
		}

		private void addAllFiltersButton_Click(object sender, EventArgs e)
		{
			foreach (RSDKv5.SceneEntity entity in _entities)
				if (entity != null && !entity.attributesMap.ContainsKey("filter"))
					entity.AddAttributeToObject("filter", RSDKv5.AttributeTypes.UINT8);
			PropertiesRefresh();
		}

		private void cbSpawn_KeyDown(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Enter)
			{
				btnSpawn_Click(sender, e);
			}
		}

		private void defaultFilter_SelectedIndexChanged(object sender, EventArgs e)
		{

		}

		private void cbSpawn_SelectedIndexChanged(object sender, EventArgs e)
		{

		}

		private void groupBox1_Enter(object sender, EventArgs e)
		{

		}

		private void gbEditor_Enter(object sender, EventArgs e)
		{

		}

		private void gbSpawn_Enter(object sender, EventArgs e)
		{

		}

		private void entitiesList_TextChanged(object sender, EventArgs e)
		{

		}

		private void searchBox_TextChanged(object sender, EventArgs e)
		{

		}

		private void entitiesList_DropDownClosed(object sender, EventArgs e)
		{

		}

		private void entityProperties_MouseHover(object sender, EventArgs e)
		{
			var obj = (Settings.mySettings.ExperimentalPropertyGridView ? entityProperties2.SelectedObject as LocalPropertyGridObject : entityProperties.SelectedObject as LocalPropertyGridObject);
			if (obj != null) MessageBox.Show(obj.ToString());
		}

		private void entityProperties_Click(object sender, EventArgs e)
		{

		}

		private void entityProperties_PropertyTabChanged(object s, PropertyTabChangedEventArgs e)
		{

		}

		private void button2_Click(object sender, EventArgs e)
		{
			contextMenuStrip1.Show(MoreButton, new Point(0, MoreButton.Size.Height));
		}

		private void goToThisEntityToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (currentEntity != null)
			{
				int x = currentEntity.Position.X.High;
				int y = currentEntity.Position.Y.High;
				EditorInstance.GoToPosition(x, y);
			}
		}
	}
}
