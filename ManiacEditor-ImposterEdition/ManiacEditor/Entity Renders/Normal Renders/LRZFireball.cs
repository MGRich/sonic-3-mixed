﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using ManiacEditor;
using Microsoft.Xna.Framework;
using RSDKv5;

namespace ManiacEditor.Entity_Renders
{
    public class LRZFireball : EntityRenderer
    {

        public override void Draw(DevicePanel d, SceneEntity entity, EditorEntity e, int x, int y, int Transparency, int index = 0, int previousChildCount = 0, int platformAngle = 0, EditorAnimations Animation = null, bool selected = false, AttributeValidater attribMap = null)
        {

            int rotation = (int)(entity.attributesMap["rotation"].ValueInt32 / 1.42);
            int pageID = GetRotationFrame(rotation);

            bool fliph = FlippedH(rotation);
            bool flipv = FlippedV(rotation);



            var editorAnim = e.EditorInstance.EditorEntity_ini.LoadAnimation("LRZFireball", d, 1, 0, fliph, flipv, false, rotation, true, false, false, true);

            if (editorAnim != null && editorAnim.Frames.Count != 0)
            {
                var frame = editorAnim.Frames[0];
                int thickness = (pageID == 1 ? frame.Frame.Width : frame.Frame.Height);
                int offset = thickness / 2;

                d.DrawBitmap(frame.Texture,
                    x - (fliph ? offset : -offset) - (int)(frame.ImageWidth / 2),
                    y - (flipv ? -offset : 0)  - (int)(frame.ImageHeight / 2),
                    frame.ImageWidth, frame.ImageHeight, false, Transparency);

            }
        }

        public enum Orientation : int
        {
            Horizontal = 0,
            Vertical = 1,
            HorizontalFlipped = 2,
            VerticalFlipped = 3,
            Unknown = -1
        }

        public int GetRotationFrame(int rotation)
        {
            Orientation Orientation = GetOrientation(rotation);
            if (Orientation == Orientation.Horizontal || Orientation == Orientation.HorizontalFlipped)
            {
                return 0;
            }
            else return 1;
        }

        public bool FlippedV(int rotation)
        {
            Orientation Orientation = GetOrientation(rotation);
            if (Orientation == Orientation.VerticalFlipped) return true;
            else return false;
        }

        public bool FlippedH(int rotation)
        {
            Orientation Orientation = GetOrientation(rotation);
            if (Orientation == Orientation.HorizontalFlipped) return true;
            else return false;
        }

        public Orientation GetOrientation(int rotation)
        {
            bool isNegative = false;
            if (rotation < 0) isNegative = true;
            if (isNegative) rotation = -rotation;

            while (rotation > 360) rotation -= 360;

            if (!isNegative)
            {
                if (Enumerable.Range(0, 90).Contains(rotation)) return Orientation.Vertical;
                else if (Enumerable.Range(90, 180).Contains(rotation)) return Orientation.HorizontalFlipped;
                else if (Enumerable.Range(180, 270).Contains(rotation)) return Orientation.VerticalFlipped;
                else if (Enumerable.Range(270, 360).Contains(rotation)) return Orientation.Horizontal;
                else return Orientation.Unknown;
            }
            else
            {
                if (Enumerable.Range(0, 90).Contains(rotation)) return Orientation.Vertical;
                else if (Enumerable.Range(90, 180).Contains(rotation)) return Orientation.Horizontal;
                else if (Enumerable.Range(180, 270).Contains(rotation)) return Orientation.VerticalFlipped;
                else if (Enumerable.Range(270, 360).Contains(rotation)) return Orientation.HorizontalFlipped;
                else return Orientation.Unknown;
            }

        }

        public override string GetObjectName()
        {
            return "LRZFireball";
        }
    }
}
