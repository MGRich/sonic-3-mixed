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
    public class JuggleSaw : EntityRenderer
    {

        public override void Draw(DevicePanel d, SceneEntity entity, EditorEntity e, int x, int y, int Transparency, int index = 0, int previousChildCount = 0, int platformAngle = 0, EditorAnimations Animation = null, bool selected = false, AttributeValidater attribMap = null)
        {
            int direction = (int)entity.attributesMap["direction"].ValueUInt8;
            bool hasSaw = entity.attributesMap["hasSaw"].ValueBool;
            bool fliph = false;
            bool flipv = false;
            int animID;
            if (direction == 2)
            {
                if (hasSaw)
                {
                    animID = 4;
                }
                else
                {
                    animID = 3;
                }

            }
            else if (direction == 3)
            {
                fliph = true;
                if (hasSaw)
                {
                    animID = 4;
                }
                else
                {
                    animID = 3;
                }

            }
            else
            {
                if (hasSaw)
                {
                    animID = 1;
                }
                else
                {
                    animID = 0;
                }
            }
            if (direction == 1)
            {
                flipv = true;
            }
            var editorAnim = e.EditorInstance.EditorEntity_ini.LoadAnimation2("JuggleSaw", d, animID, -1, fliph, flipv, false);
            if (editorAnim != null && editorAnim.Frames.Count != 0 && animID >= 0)
            {
                var frame = editorAnim.Frames[Animation.index];

                Animation.ProcessAnimation(frame.Entry.SpeedMultiplyer, frame.Entry.Frames.Count, frame.Frame.Delay);

                d.DrawBitmap(frame.Texture,
                    x + frame.Frame.PivotX - (fliph ? (frame.Frame.Width - editorAnim.Frames[0].Frame.Width + (hasSaw ? (38) : 16)) : 0),
                    y + frame.Frame.PivotY + (flipv ? (frame.Frame.Height - editorAnim.Frames[0].Frame.Height + (hasSaw ? (37) : 15)) : 0),
                    frame.Frame.Width, frame.Frame.Height, false, Transparency);
            }
        }

        public override string GetObjectName()
        {
            return "JuggleSaw";
        }
    }
}
