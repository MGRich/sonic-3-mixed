﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using ManiacEditor;
using Microsoft.Xna.Framework;
using RSDKv5;
using SystemColors = System.Drawing.Color;

namespace ManiacEditor.Entity_Renders
{
    public class Hatch : EntityRenderer
    {

        public override void Draw(DevicePanel d, SceneEntity entity, EditorEntity e, int x, int y, int Transparency, int index = 0, int previousChildCount = 0, int platformAngle = 0, EditorAnimations Animation = null, bool selected = false, AttributeValidater attribMap = null)
        {
            var widthPixels = (int)(entity.attributesMap["subOff2"].ValuePosition.X.High) * 16;
            var heightPixels = (int)(entity.attributesMap["subOff2"].ValuePosition.Y.High) * 16;
            var width = (int)widthPixels / 16;
            var height = (int)heightPixels / 16;
            int offsetX = (int)(entity.attributesMap["subOff1"].ValuePosition.X.High);
            int offsetY = (int)(entity.attributesMap["subOff1"].ValuePosition.Y.High);

            var editorAnim = e.EditorInstance.EditorEntity_ini.LoadAnimation2("EditorAssets", d, 0, 1, false, false, false);

            var editorAnim2 = e.EditorInstance.EditorEntity_ini.LoadAnimation2("Hatch", d, 0, 0, false, false, false);
            var editorAnim3 = e.EditorInstance.EditorEntity_ini.LoadAnimation2("Hatch", d, 0, 1, false, false, false);
            if (editorAnim2 != null && editorAnim2.Frames.Count != 0 && editorAnim3 != null && editorAnim3.Frames.Count != 0)
            {
                var frame = editorAnim2.Frames[0];
                var frame2 = editorAnim3.Frames[0];

                Animation.ProcessAnimation(frame.Entry.SpeedMultiplyer, frame.Entry.Frames.Count, frame.Frame.Delay);

                d.DrawBitmap(frame2.Texture,
                    x + frame2.Frame.PivotX,
                    y + frame2.Frame.PivotY,
                    frame2.Frame.Width, frame2.Frame.Height, false, Transparency);
                d.DrawBitmap(frame.Texture,
                    x + frame.Frame.PivotX,
                    y + frame.Frame.PivotY,
                    frame.Frame.Width, frame.Frame.Height, false, Transparency);

            }

            if (width != 0 && height != 0)
            {
                int x1 = x + offsetX + widthPixels / -2;
                int x2 = x + offsetX + widthPixels / 2 - 1;
                int y1 = y + offsetY + heightPixels / -2;
                int y2 = y + offsetY + heightPixels / 2 - 1;


                d.DrawLine(x1, y1, x1, y2, SystemColors.White);
                d.DrawLine(x1, y1, x2, y1, SystemColors.White);
                d.DrawLine(x2, y2, x1, y2, SystemColors.White);
                d.DrawLine(x2, y2, x2, y1, SystemColors.White);

                // draw corners
                for (int i = 0; i < 4; i++)
                {
                    bool right = (i & 1) > 0;
                    bool bottom = (i & 2) > 0;

                    editorAnim = e.EditorInstance.EditorEntity_ini.LoadAnimation2("EditorAssets", d, 0, 1, right, bottom, false);
                    if (editorAnim != null && editorAnim.Frames.Count != 0)
                    {
                        var frame = editorAnim.Frames[Animation.index];
                        Animation.ProcessAnimation(frame.Entry.SpeedMultiplyer, frame.Entry.Frames.Count, frame.Frame.Delay);
                        d.DrawBitmap(frame.Texture,
                            (x + offsetX + widthPixels / (right ? 2 : -2)) - (right ? frame.Frame.Width : 0),
                            (y + offsetY + heightPixels / (bottom ? 2 : -2) - (bottom ? frame.Frame.Height : 0)),
                            frame.Frame.Width, frame.Frame.Height, false, Transparency);

                    }
                }
            }
        }

        public override string GetObjectName()
        {
            return "Hatch";
        }
    }
}
