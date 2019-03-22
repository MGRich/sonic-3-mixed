﻿using ManiacEditor.Entity_Renders;
using RSDKv5;
using SharpDX.Direct3D9;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.Serialization;
using System.Drawing;
using System.IO;
using System.Reflection;
using System.Linq;
using System.Linq.Expressions;
using System.Runtime.InteropServices.ComTypes;
using System.Windows.Forms;
using System.Diagnostics;
using MonoGame.UI.Forms;
using MonoGame.Extended;
using System.Runtime.Remoting.Metadata.W3cXsd2001;
using System.Runtime.InteropServices;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Text.RegularExpressions;
using ImageMagick;

namespace ManiacEditor
{
    [Serializable]
    public class EditorEntity : IDrawable
	{
		private bool is64Bit = false;

        public bool Selected;
        public bool InTempSelection = false;
        public bool TempSelected = false;

        public bool rotateImageLegacyMode = false;
		public bool PreLoadDrawing = false;

        //public static EditorEntity Instance;
        public EditorAnimations EditorAnimations;
        public AttributeValidater AttributeValidater;
        public EntityRenderer renderer;

        private SceneEntity entity;
        public bool filteredOut;
        public bool filteredOutByParent;
        public string uniqueKey = "";
		public bool useOtherSelectionVisiblityMethod = false; //Not Universal; Only for Renders that need it
		public bool drawSelectionBoxInFront = true;
		public bool renderNotFound = false;




		//Rotating/Moving Platforms
		public int platformAngle = 0;
        public int platformpositionX = 0;
        public int platformpositionY = 0;
        //bool platformdisableX = false;
        //bool platformdisableY = false;
        //bool platformreverse = false;



        //For Drawing Extra Child Objects

        public bool childDraw = false;
        public int childX = 0;
        public int childY = 0;
        public bool childDrawAddMode = true;
        public int previousChildCount = 0; //For Reseting ChildCount States
        public static bool Working = false;
        public DateTime lastFrametime;
        public int index = 0;
        public int layerPriority = 0;
        public SceneEntity Entity { get { return entity; }}

        public int PositionX = 0;
        public int PositionY = 0;
        public string Name = "";


        public Editor EditorInstance;

        public EditorEntity(SceneEntity entity, Editor instance)
        {
            EditorInstance = instance;
            this.entity = entity;
            PositionX = entity.Position.X.High;
            PositionY = entity.Position.Y.High;
            Name = entity.Object.Name.Name;
            lastFrametime = DateTime.Now;
            EditorAnimations = new EditorAnimations(instance);
            AttributeValidater = new AttributeValidater();
			is64Bit = Environment.Is64BitProcess;

            if (EditorInstance.EditorEntity_ini.EntityRenderers.Count == 0)
            {
                var types = GetType().Assembly.GetTypes().Where(t => t.BaseType == typeof(EntityRenderer)).ToList();
                foreach (var type in types)
                    EditorInstance.EditorEntity_ini.EntityRenderers.Add((EntityRenderer)Activator.CreateInstance(type));
            }

            if (EditorInstance.EditorEntity_ini.LinkedEntityRenderers.Count == 0)
            {
                var types = GetType().Assembly.GetTypes().Where(t => t.BaseType == typeof(LinkedRenderer)).ToList();
                foreach (var type in types)
                    EditorInstance.EditorEntity_ini.LinkedEntityRenderers.Add((LinkedRenderer)Activator.CreateInstance(type));

                foreach (LinkedRenderer render in EditorInstance.EditorEntity_ini.LinkedEntityRenderers)
                {
                    render.EditorInstance = instance;
                }
            }


        }

        public void Draw(Graphics g)
        {

        }

        public EditorEntity GetSelf()
        {
            return this;
        }

        public bool ContainsPoint(Point point)
        {
            if (filteredOut) return false;

            return GetDimensions().Contains(point);
        }

        public void DrawUIButtonBack(DevicePanel d, int x, int y, int width, int height, int frameH, int frameW, int Transparency)
        {
            width += 1;
            height += 1;

            bool wEven = width % 2 == 0;
            bool hEven = height % 2 == 0;

            int x2 = x;
            int y2 = y;
            if (width != 0) x2 -= width / 2;
            if (height != 0) y2 -= height / 2;

            d.DrawRectangle(x2, y2, x2 + width, y2 + height, System.Drawing.Color.FromArgb(Transparency, 0, 0, 0));

            System.Drawing.Color colur = System.Drawing.Color.FromArgb(Transparency, 0, 0, 0);
            //Left Triangle         
            for (int i = 1; i <= (height); i++)
            {
                d.DrawLine(x2 - height + i, y + (!hEven ? 1 : 0) + (height / 2) - i, x2, y + (!hEven ? 1 : 0) + (height / 2) - i, colur);
            }

            int x3 = x2 + width;
            int y3 = y2 + height;

            //Right Triangle
            for (int i = 1; i <= height; i++)
            {
                d.DrawLine(x3, y + (!hEven ? 1 : 0) + (height / 2) - i, x3 + height + i, y + (!hEven ? 1 : 0) + (height / 2) - i, colur);
            }
        }
        public void DrawTriangle(DevicePanel d, int x, int y, int width, int height, int frameH, int frameW, int state = 0, int Transparency = 0xFF)
        {

            bool wEven = width % 2 == 0;
            bool hEven = height % 2 == 0;

            System.Drawing.Color colur = System.Drawing.Color.FromArgb(Transparency, 0, 0, 0);
            if (state == 0)
            {
                //Left Triangle         
                for (int i = 1; i <= (height); i++)
                {
                    d.DrawLine(x - height + i, y + (!hEven ? 1 : 0) + (height / 2) - i, x, y + (!hEven ? 1 : 0) + (height / 2) - i, colur);
                }
            }
            else if (state == 1)
            {
                //Right Triangle
                for (int i = 1; i <= height; i++)
                {
                    d.DrawLine(x, y + (!hEven ? 1 : 0) + (height / 2) - i, x + height + i, y + (!hEven ? 1 : 0) + (height / 2) - i, colur);
                }
            }



        }

        public bool IsInArea(Rectangle area)
        {
            if (filteredOut) return false;

            return GetDimensions().IntersectsWith(area);
        }

        public void Move(Point diff, bool relative = true)
        {
            if (relative)
            {
                entity.Position.X.High += (short)diff.X;
                entity.Position.Y.High += (short)diff.Y;
            }
            else
            {
                entity.Position.X.High = (short)diff.X;
                entity.Position.Y.High = (short)diff.Y;
            }

            // Since the Editor can now update without the use of this render, I removed it
            //if (Properties.Settings.Default.AllowMoreRenderUpdates == true) EditorInstance.UpdateRender();
            if (EditorInstance.GameRunning && Properties.Settings.Default.EnableRealTimeObjectMovingInGame)
            {

                int ObjectStart = EditorInGame.ObjectStart[EditorInGame.GameVersion.IndexOf(EditorInGame.SelectedGameVersion)];
                int ObjectSize =  EditorInGame.ObjectSize[EditorInGame.GameVersion.IndexOf(EditorInGame.SelectedGameVersion)];

                // TODO: Find out if this is constent
                int ObjectAddress = ObjectStart + (ObjectSize * entity.SlotID);
                EditorInstance.GameMemory.WriteInt16(ObjectAddress + 2, entity.Position.X.High);
                EditorInstance.GameMemory.WriteInt16(ObjectAddress + 6, entity.Position.Y.High);
            }


        }

        public Rectangle GetDimensions()
        {
            return new Rectangle(entity.Position.X.High, entity.Position.Y.High, EditorConstants.ENTITY_NAME_BOX_WIDTH, EditorConstants.ENTITY_NAME_BOX_HEIGHT);
        }

        public bool SetFilter()
        {
			if (HasFilter())
			{
				int filter = entity.GetAttribute("filter").ValueUInt8;

				/**
                 * 1 or 5 = Both
                 * 2 = Mania
                 * 4 = Encore
				 * 255 = Pinball
                 * 
                 */
				filteredOut =
					((filter == 1 || filter == 5) && !Properties.Settings.Default.showBothEntities) ||
					(filter == 2 && !Properties.Settings.Default.showManiaEntities) ||
					(filter == 4 && !Properties.Settings.Default.showEncoreEntities) ||
					(filter == 255 && !Properties.Settings.Default.showPinballEntities) ||
					((filter < 1 || filter == 3 || filter > 5 && filter != 255) && !Properties.Settings.Default.showOtherEntities);
			}
			else
			{
				filteredOut = !Properties.Settings.Default.showPrePlusEntities;
			}


            if (EditorInstance.entitiesTextFilter != "" && !entity.Object.Name.Name.Contains(EditorInstance.entitiesTextFilter))
            {
                filteredOut = true;
            }

            return filteredOut;
        }

        public void TestIfPlayerObject()
        {
            if (entity.Object.Name.Name == "Player" && !EditorInstance.playerObjectPosition.Contains(entity))
            {
                EditorInstance.playerObjectPosition.Add(entity);
            }
        }
        public System.Drawing.Color GetFilterBoxColor()
        {
            System.Drawing.Color color = System.Drawing.Color.DarkBlue;
            if (HasSpecificFilter(1) || HasSpecificFilter(5))
            {
                color = System.Drawing.Color.DarkBlue;
            }
            else if (HasSpecificFilter(2))
            {
                color = System.Drawing.Color.DarkRed;
            }
            else if (HasSpecificFilter(4))
            {
                color = System.Drawing.Color.DarkGreen;
            }
            else if (HasSpecificFilter(255))
            {
                color = System.Drawing.Color.Purple;
            }
            else if (HasFilterOther())
            {
                color = System.Drawing.Color.Yellow;
            }
            else if (!HasFilter())
            {
                color = System.Drawing.Color.White;
            }
            return color;

        }
        public System.Drawing.Color GetBoxInsideColor()
        {
            if (InTempSelection)
            {
                return (TempSelected && EditorInstance.IsEntitiesEdit()) ? System.Drawing.Color.MediumPurple : System.Drawing.Color.MediumTurquoise;
            }
            else
            {
                return (Selected && EditorInstance.IsEntitiesEdit()) ? System.Drawing.Color.MediumPurple : System.Drawing.Color.MediumTurquoise;
            }
        }
        public int GetTransparencyLevel()
        {
            return (EditorInstance.EditLayer == null || EditorInstance.isExportingImage) ? 0xff : 0x32;
        }
        public int GetChildX()
        {
            return  (childDrawAddMode == false ? childX : entity.Position.X.High + (childDraw ? childX : 0));
        }
        public int GetChildY()
        {
            return (childDrawAddMode == false ? childY : entity.Position.Y.High + (childDraw ? childY : 0));
        }
        public bool ValidPriorityPlane(int priority)
		{
			bool validPlane = false;
			if (priority != 0) validPlane = AttributeValidater.PlaneFilterCheck(entity, priority);
			else validPlane = true;
			
			return validPlane;
		}
		public virtual void DrawBoxOnly(DevicePanel d)
		{
			int Transparency = (EditorInstance.EditLayer == null || EditorInstance.isExportingImage) ? 0xff : 0x32;
			int x = entity.Position.X.High;
			int y = entity.Position.Y.High;

			if (filteredOut && !EditorInstance.isPreRending) return;

            System.Drawing.Color color = GetBoxInsideColor();
            System.Drawing.Color color2 = GetFilterBoxColor();

            DrawSelectionBox(d, x, y, Transparency, color, color2);
		}
        public virtual void Draw(DevicePanel d)
        {
            if (filteredOut) return;
            if (EditorEntity_ini.LinkedRendersNames.Contains(entity.Object.Name.Name) && EditorInstance.UIModes.ShowEntityPathArrows)
            {
                try
                {
                    LinkedRenderer renderer = EditorInstance.EditorEntity_ini.LinkedEntityRenderers.Where(t => t.GetObjectName() == entity.Object.Name.Name.ToString()).FirstOrDefault();
                    if (renderer != null) renderer.Draw(d, entity, this);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Unable to load the linked render for " + entity.Object.Name.Name + "! " + ex.ToString());
                    EditorInstance.EditorEntity_ini.linkedrendersWithErrors.Add(entity.Object.Name.Name);

                }

            }
            else
            {
                DrawBase(d);
            }
        }
        public virtual void DrawBase(DevicePanel d)
        {
            TestIfPlayerObject();

            List<string> entityRenderList = EditorInstance.EditorEntity_ini.entityRenderingObjects;
            List<string> onScreenExlusionList = (Properties.Settings.Default.DisableRenderExlusions ? new List<string>() : EditorInstance.EditorEntity_ini.renderOnScreenExlusions);
         
            if (!onScreenExlusionList.Contains(entity.Object.Name.Name)) if (!this.IsObjectOnScreen(d)) return;
            System.Drawing.Color color = GetBoxInsideColor();
            System.Drawing.Color color2 = GetFilterBoxColor();
            int Transparency = GetTransparencyLevel();
            if (!Properties.Settings.Default.NeverLoadEntityTextures) EditorInstance.EditorEntity_ini.LoadNextAnimation(this);

            int x = entity.Position.X.High;
            int y = entity.Position.Y.High;
            int _ChildX = GetChildX();
            int _ChildY = GetChildY();
            bool fliph = false;
            bool flipv = false;
            bool rotate = false;
            var offset = GetRotationFromAttributes(ref fliph, ref flipv, ref rotate);
            string name = entity.Object.Name.Name;

			if (!drawSelectionBoxInFront && !EditorInstance.UIModes.EntitySelectionBoxesAlwaysPrioritized) DrawSelectionBox(d, x, y, Transparency, color, color2);

            if (!Properties.Settings.Default.NeverLoadEntityTextures)
            {
                if (entityRenderList.Contains(name)) PrimaryDraw(d, onScreenExlusionList);
                else FallbackDraw(d, x, y, _ChildX, _ChildY, Transparency, color);
            }

            if (drawSelectionBoxInFront && !EditorInstance.UIModes.EntitySelectionBoxesAlwaysPrioritized) DrawSelectionBox(d, x, y, Transparency, color, color2);
		}
        public virtual void PrimaryDraw(DevicePanel d, List<string> onScreenExlusionList)
        {
            if ((this.IsObjectOnScreen(d) || onScreenExlusionList.Contains(entity.Object.Name.Name)) && Properties.Settings.Default.UseAltEntityRenderMode)
            {
                EditorInstance.EditorEntity_ini.DrawOthers(d, entity, this, childX, childY, index, previousChildCount, platformAngle, EditorAnimations, Selected, AttributeValidater, childDrawAddMode);
            }
            else if (!Properties.Settings.Default.UseAltEntityRenderMode)
            {
                EditorInstance.EditorEntity_ini.DrawOthers(d, entity, this, childX, childY, index, previousChildCount, platformAngle, EditorAnimations, Selected, AttributeValidater, childDrawAddMode);
            }
        }
        public virtual void FallbackDraw(DevicePanel d, int x, int y, int _ChildX, int _ChildY, int Transparency, System.Drawing.Color color)
        {
            bool fliph = false;
            bool flipv = false;
            bool rotate = false;
            var offset = GetRotationFromAttributes(ref fliph, ref flipv, ref rotate);
            string name = entity.Object.Name.Name;
            var editorAnim = EditorInstance.EditorEntity_ini.LoadAnimation2(name, d, -1, -1, fliph, flipv, rotate);
            if (editorAnim != null && editorAnim.Frames.Count > 0)
            {
                renderNotFound = false;
                // Special cases that always display a set frame(?)
                if (EditorInstance.ShowAnimations.IsEnabled == true)
                {
                    if (entity.Object.Name.Name == "StarPost")
                        index = 1;
                }
                // Just incase
                if (index >= editorAnim.Frames.Count)
                    index = 0;
                var frame = editorAnim.Frames[index];

                if (entity.attributesMap.ContainsKey("frameID"))
                    frame = GetFrameFromAttribute(editorAnim, entity.attributesMap["frameID"]);

                if (frame != null)
                {
                    EditorAnimations.ProcessAnimation(frame.Entry.SpeedMultiplyer, frame.Entry.Frames.Count, frame.Frame.Delay);
                    // Draw the normal filled Rectangle but Its visible if you have the entity selected
                    d.DrawBitmap(frame.Texture, _ChildX + frame.Frame.PivotX + ((int)offset.X * frame.Frame.Width), _ChildY + frame.Frame.PivotY + ((int)offset.Y * frame.Frame.Height),
                        frame.Frame.Width, frame.Frame.Height, false, Transparency);
                }
                else
                { // No frame to render
                    if (EditorInstance.UIModes.ShowEntitySelectionBoxes) d.DrawRectangle(x, y, x + EditorConstants.ENTITY_NAME_BOX_WIDTH, y + EditorConstants.ENTITY_NAME_BOX_HEIGHT, System.Drawing.Color.FromArgb(Transparency, color));
                }
                //Failsafe?
                //DrawOthers(d);

            }
            else
            {
                renderNotFound = true;
            }
        }
        public void DrawSelectionBox(DevicePanel d, int x, int y, int Transparency, System.Drawing.Color color, System.Drawing.Color color2)
        {
            if (EditorInstance.UIModes.ShowEntitySelectionBoxes && !useOtherSelectionVisiblityMethod && this.IsObjectOnScreen(d))
            {
                    if (renderNotFound)
                    {
                        d.DrawRectangle(x, y, x + EditorConstants.ENTITY_NAME_BOX_WIDTH, y + EditorConstants.ENTITY_NAME_BOX_HEIGHT, System.Drawing.Color.FromArgb(Transparency, color));
                    }
                    else
                    {
                        d.DrawRectangle(x, y, x + EditorConstants.ENTITY_NAME_BOX_WIDTH, y + EditorConstants.ENTITY_NAME_BOX_HEIGHT, GetSelectedColor(color2));
                    }
                    d.DrawLine(x, y, x + EditorConstants.ENTITY_NAME_BOX_WIDTH, y, System.Drawing.Color.FromArgb(Transparency, color2));
                    d.DrawLine(x, y, x, y + EditorConstants.ENTITY_NAME_BOX_HEIGHT, System.Drawing.Color.FromArgb(Transparency, color2));
                    d.DrawLine(x, y + EditorConstants.ENTITY_NAME_BOX_HEIGHT, x + EditorConstants.ENTITY_NAME_BOX_WIDTH, y + EditorConstants.ENTITY_NAME_BOX_HEIGHT, System.Drawing.Color.FromArgb(Transparency, color2));
                    d.DrawLine(x + EditorConstants.ENTITY_NAME_BOX_WIDTH, y, x + EditorConstants.ENTITY_NAME_BOX_WIDTH, y + EditorConstants.ENTITY_NAME_BOX_HEIGHT, System.Drawing.Color.FromArgb(Transparency, color2));
                    if (Properties.Settings.Default.UseObjectRenderingImprovements == false)
                    {
                        if (EditorInstance.GetZoom() >= 1) d.DrawTextSmall(String.Format("{0} (ID: {1})", entity.Object.Name, entity.SlotID), x + 2, y + 2, EditorConstants.ENTITY_NAME_BOX_WIDTH - 4, System.Drawing.Color.FromArgb(Transparency, System.Drawing.Color.Black), true);
                    }             
            }
        }

        public System.Drawing.Color GetSelectedColor(System.Drawing.Color color)
        {
            if (InTempSelection)
            {
                return System.Drawing.Color.FromArgb(TempSelected && EditorInstance.IsEntitiesEdit() ? 0x60 : 0x00, color);
            }
            else
            {
                return System.Drawing.Color.FromArgb(Selected && EditorInstance.IsEntitiesEdit() ? 0x60 : 0x00, color);
            }
        }

		public EditorEntity_ini.EditorAnimation.EditorFrame GetFrameFromAttribute(EditorEntity_ini.EditorAnimation anim, AttributeValue attribute, string key = "frameID")
        {
            int frameID = -1;
            switch (attribute.Type)
            {
                case AttributeTypes.UINT8:
                    frameID = entity.attributesMap[key].ValueUInt8;
                    break;
                case AttributeTypes.INT8:
                    frameID = entity.attributesMap[key].ValueInt8;
                    break;
                case AttributeTypes.VAR:
                    frameID = (int)entity.attributesMap[key].ValueVar;
                    break;
            }
            if (frameID >= anim.Frames.Count)
                frameID = (anim.Frames.Count - 1) - (frameID % anim.Frames.Count + 1);
            if (frameID < 0)
                frameID = 0;
            if (frameID >= 0 && frameID < int.MaxValue)
                return anim.Frames[frameID % anim.Frames.Count];
            else
                return null; // Don't Render the Animation
        }
        /// <summary>
        /// Guesses the rotation of an entity
        /// </summary>
        /// <param name="attributes">List of all Attributes</param>
        /// <param name="fliph">Reference to fliph</param>
        /// <param name="flipv">Reference to flipv</param>
        /// <returns>AnimationID Offset</returns>
        public SharpDX.Vector2 GetRotationFromAttributes(ref bool fliph, ref bool flipv, ref bool rotate)
        {
            AttributeValue attribute = null;
            var attributes = entity.attributesMap;
            int dir = 0;
            var offset = new SharpDX.Vector2();
            if (attributes.ContainsKey("orientation"))
            {
                attribute = attributes["orientation"];
                switch (attribute.Type)
                {
                    case AttributeTypes.UINT8:
                        dir = attribute.ValueUInt8;
                        break;
                    case AttributeTypes.INT8:
                        dir = attribute.ValueInt8;
                        break;
                    case AttributeTypes.VAR:
                        dir = (int) attribute.ValueVar;
                        break;
                }
                if (dir == 0) // Up
                {
                }
                else if (dir == 1) // Down
                {
                    fliph = true;
                    offset.X = 1;
                    flipv = true;
                    offset.Y = 1;
                }
                else if (dir == 2) // Right
                {
                    flipv = true;
                    rotate = true;
                    offset.Y = 1;
                    //animID = 1;
                }
                else if (dir == 3) // Left
                {
                    flipv = true;
                    rotate = true;
                    offset.Y = 1;
                    //animID = 1;
                }
            }
            if (attributes.ContainsKey("direction") && dir == 0)
            {
                attribute = attributes["direction"];
                switch (attribute.Type)
                {
                    case AttributeTypes.UINT8:
                        dir = attribute.ValueUInt8;
                        break;
                    case AttributeTypes.INT8:
                        dir = attribute.ValueInt8;
                        break;
                    case AttributeTypes.VAR:
                        dir = (int)attribute.ValueVar;
                        break;
                }
                if (dir == 0) // Right
                {
                }
                else if (dir == 1) // left
                {
                    fliph = true;
                    offset.X = 0;
                    rotate = true;
                }
                else if (dir == 2) // Up
                {
                    flipv = true;
                }
                else if (dir == 3) // Down
                {
                    flipv = true;
                    //offset.Y = 1;
                }
            }
            return offset;
        }
        public bool IsObjectOnScreen(DevicePanel d)
        {
            
            int x = entity.Position.X.High + childX;
            int y = entity.Position.Y.High + childY;
            if (childDrawAddMode == false)
            {
                x = childX;
                y = childY;
            }
            int Transparency = (EditorInstance.EditLayer == null) ? 0xff : 0x32;

            bool isObjectVisibile = false;

            
			if (!filteredOut)
			{
                if (renderer == null || renderer.GetObjectName() != entity.Object.Name.Name) renderer = EditorInstance.EditorEntity_ini.EntityRenderers.Where(t => t.GetObjectName() == entity.Object.Name.Name).FirstOrDefault();
                if (renderer != null)
				{
					isObjectVisibile = renderer.isObjectOnScreen(d, entity, null, x, y, 0);
				}
				else
				{
					isObjectVisibile = d.IsObjectOnScreen(x, y, 20, 20);
				}
			}

            isObjectVisibile = d.IsObjectOnScreen(x, y, 20, 20);
            return isObjectVisibile;


        }
        public bool HasFilter()
        {
            return entity.attributesMap.ContainsKey("filter") && entity.attributesMap["filter"].Type == AttributeTypes.UINT8;
        }

        public bool HasSpecificFilter(uint input, bool checkHigher = false)
        {
            if (entity.attributesMap.ContainsKey("filter") && entity.attributesMap["filter"].Type == AttributeTypes.UINT8)
            {
                if (entity.attributesMap["filter"].ValueUInt8 == input && checkHigher == false)
                {
                    return true;
                }
                else if (entity.attributesMap["filter"].ValueUInt8 >= input && checkHigher)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        public bool HasFilterOther()
        {
            if (entity.attributesMap.ContainsKey("filter") && entity.attributesMap["filter"].Type == AttributeTypes.UINT8)
            {
                int filter = entity.attributesMap["filter"].ValueUInt8;
                if (filter < 1 || filter == 3 || filter > 5)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        public void PrepareForExternalCopy()
        {
            entity.PrepareForExternalCopy();
        }

        public bool IsExternal()
        {
            return entity.IsExternal();
        }

        internal void Flip(FlipDirection flipDirection)
        {
            if (entity.attributesMap.ContainsKey("flipFlag"))
            {
                if (flipDirection == FlipDirection.Horizontal)
                {
                    entity.attributesMap["flipFlag"].ValueVar ^= 0x01;
                }
                else
                {
                    entity.attributesMap["flipFlag"].ValueVar ^= 0x02;
                }
            }
        }

        public void Dispose()
        {

        }
	}
}
