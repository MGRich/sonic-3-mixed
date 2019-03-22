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
    public class UIInfoLabel : EntityRenderer
    {

        public override void Draw(DevicePanel d, SceneEntity entity, EditorEntity e, int x, int y, int Transparency, int index = 0, int previousChildCount = 0, int platformAngle = 0, EditorAnimations Animation = null, bool selected = false, AttributeValidater attribMap = null)
        {
            
            string text = entity.attributesMap["text"].ValueString;
            int width = (int)entity.attributesMap["size"].ValuePosition.X.High;
            int height = (int)entity.attributesMap["size"].ValuePosition.Y.High;
            int spacingAmount = 0;
            e.DrawUIButtonBack(d, x, y, width, height, width, height, Transparency);
            if (width == 0) width = 1;
            int x2 = x - (width / 4);
            foreach (char symb in text)
            {
                int frameID = GetFrameID(symb, e.EditorInstance.MenuChar_Small);
                var editorAnim = e.EditorInstance.EditorEntity_ini.LoadAnimation("UIElements", d, 4, frameID, false, false, false);
                if (editorAnim != null && editorAnim.Frames.Count != 0)
                {
                    var frame = editorAnim.Frames[Animation.index];
                    //Animation.ProcessAnimation(frame.Entry.SpeedMultiplyer, frame.Entry.Frames.Count, frame.Frame.Delay);
                    d.DrawBitmap(frame.Texture, x2 + frame.Frame.PivotX + spacingAmount, y + frame.Frame.PivotY,
                        frame.Frame.Width, frame.Frame.Height, false, Transparency);
                    spacingAmount = spacingAmount + frame.Frame.Width;
                }
            }

            
        }

        public int GetFrameID(char letter, char[] arry)
        {
            char[] symArray = arry;
            int position = 0;
            foreach (char sym in symArray)
            {
                if (sym == letter) return position;
                position++;
            }
            return position;
        }

        public override string GetObjectName()
        {
            return "UIInfoLabel";
        }
    }
}
