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
    public class TVVan : EntityRenderer
    {

        public override void Draw(DevicePanel d, SceneEntity entity, EditorEntity e, int x, int y, int Transparency, int index = 0, int previousChildCount = 0, int platformAngle = 0, EditorAnimations Animation = null, bool selected = false, AttributeValidater attribMap = null)
        {
            int type = (int)entity.attributesMap["type"].ValueUInt8;
            bool allowToRender = false;
            int objType = 0;
            bool fliph = false;
            bool flipv = false;
            switch (type)
            {
                case 0:
                    objType = 0;
                    break;
                case 1:
                    objType = 1;
                    fliph = true;
                    break;
                case 2:
                    objType = 2;
                    break;
                case 3:
                    objType = 3;
                    break;
                case 4:
                    objType = 4;
                    break;
                case 5:
                    objType = 5;
                    break;
                case 6:
                    objType = 6;
                    break;
                case 7:
                    objType = 7;
                    break;
                case 8:
                    objType = 8;
                    break;
                case 9:
                    objType = 9;
                    break;
                case 10:
                    objType = 10;
                    break;
                case 11:
                    objType = 11;
                    break;
                case 12:
                    objType = 12;
                    break;
                case 13:
                    objType = 13;
                    break;
                case 14:
                    objType = 14;
                    break;
                default:
                    objType = 14;
                    break;

            }
            var editorAnim = e.EditorInstance.EditorEntity_ini.LoadAnimation2("TVVan", d, 0, -1, fliph, flipv, false);
            var editorAnim2 = e.EditorInstance.EditorEntity_ini.LoadAnimation2("TVVan", d, 0, -1, true, flipv, false);
            var editorAnim10 = e.EditorInstance.EditorEntity_ini.LoadAnimation2("TVVan", d, 0, 9, fliph, flipv, false);
            var editorAnim11 = e.EditorInstance.EditorEntity_ini.LoadAnimation2("TVVan", d, 3, 3, fliph, flipv, false);
            var editorAnim12 = e.EditorInstance.EditorEntity_ini.LoadAnimation2("TVVan", d, 4, 3, fliph, flipv, false);
            var editorAnim13 = e.EditorInstance.EditorEntity_ini.LoadAnimation2("TVVan", d, 6, 0, fliph, flipv, false);
            var editorAnim14 = e.EditorInstance.EditorEntity_ini.LoadAnimation2("TVVan", d, 6, 1, fliph, flipv, false);
            var editorAnim15 = e.EditorInstance.EditorEntity_ini.LoadAnimation2("TVVan", d, 6, 2, fliph, flipv, false);
            var editorAnim16 = e.EditorInstance.EditorEntity_ini.LoadAnimation2("TVVan", d, 15, -1, fliph, flipv, false);
            var normalSataliteReversedHV = e.EditorInstance.EditorEntity_ini.LoadAnimation2("TVVan", d, 6, 0, true, true, false);
            var normalSataliteReversedV = e.EditorInstance.EditorEntity_ini.LoadAnimation2("TVVan", d, 6, 0, false, true, false);
            var normalSataliteReversedH = e.EditorInstance.EditorEntity_ini.LoadAnimation2("TVVan", d, 6, 0, true, false, false);
            var downwardsSatalite = e.EditorInstance.EditorEntity_ini.LoadAnimation2("TVVan", d, 6, 1, fliph, true, false);

            if (editorAnim != null && editorAnim.Frames.Count != 0)
            {
                if (true)
                {
                    if (true)
                    {
                        if (true)
                        {
                            if (true && editorAnim10 != null && editorAnim10.Frames.Count != 0)
                            {
                                if (editorAnim11 != null && editorAnim11.Frames.Count != 0 && editorAnim12 != null && editorAnim12.Frames.Count != 0)
                                {
                                    if (editorAnim13 != null && editorAnim13.Frames.Count != 0 && editorAnim14 != null && editorAnim14.Frames.Count != 0)
                                    {
                                        if (editorAnim15 != null && editorAnim15.Frames.Count != 0 && editorAnim16 != null && editorAnim16.Frames.Count != 0)
                                        {
                                            if (normalSataliteReversedHV != null && normalSataliteReversedHV.Frames.Count != 0 && normalSataliteReversedV != null && normalSataliteReversedV.Frames.Count != 0 && normalSataliteReversedH != null && normalSataliteReversedH.Frames.Count != 0)
                                            {
                                                if (downwardsSatalite != null && downwardsSatalite.Frames.Count != 0)
                                                {
                                                    allowToRender = true;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (allowToRender == true)

            {
                var TVVan = editorAnim.Frames[0];
                var HighLabel = editorAnim.Frames[1];
                var InsideTVs = editorAnim.Frames[2];
                var ramp = editorAnim.Frames[3];
                var backsideTire = editorAnim.Frames[4];
                var frontTireL = editorAnim.Frames[5];
                var frontTireR = editorAnim.Frames[6];
                var Window = editorAnim.Frames[7];
                var VanSatalite = editorAnim.Frames[8];
                var frame10 = editorAnim10.Frames[0];
                var frame11 = editorAnim11.Frames[0];
                var frame12 = editorAnim12.Frames[0];
                var normalSatalite = editorAnim13.Frames[0];
                var upwardsSatalite = editorAnim14.Frames[0];
                var sataliteHook = editorAnim15.Frames[0];
                var frame16 = editorAnim16.Frames[Animation.index];
                var normalSatalite2 = normalSataliteReversedHV.Frames[0];
                var normalSataliteH = normalSataliteReversedH.Frames[0];
                var normalSataliteV = normalSataliteReversedV.Frames[0];
                var downwardsFaceSatalite = downwardsSatalite.Frames[0];

                Animation.ProcessAnimation(frame16.Entry.SpeedMultiplyer, frame16.Entry.Frames.Count, frame16.Frame.Delay);

                if (objType == 0 || objType == 12 || objType == 13) // Normal (TV Van)
                {
                    d.DrawBitmap(Window.Texture,
                    x + Window.Frame.PivotX - (fliph ? (Window.Frame.Width - editorAnim.Frames[0].Frame.Width) : 0),
                    y + Window.Frame.PivotY + (flipv ? (Window.Frame.Height - editorAnim.Frames[0].Frame.Height) : 0),
                    Window.Frame.Width, Window.Frame.Height, false, Transparency);
                    d.DrawBitmap(backsideTire.Texture,
                    x + backsideTire.Frame.PivotX - (fliph ? (backsideTire.Frame.Width - editorAnim.Frames[0].Frame.Width) : 0),
                    y + backsideTire.Frame.PivotY + (flipv ? (backsideTire.Frame.Height - editorAnim.Frames[0].Frame.Height) : 0),
                    backsideTire.Frame.Width, backsideTire.Frame.Height, false, Transparency);
                    d.DrawBitmap(TVVan.Texture,
                    x + TVVan.Frame.PivotX - (fliph ? (TVVan.Frame.Width - editorAnim.Frames[0].Frame.Width) : 0),
                    y + TVVan.Frame.PivotY + (flipv ? (TVVan.Frame.Height - editorAnim.Frames[0].Frame.Height) : 0),
                    TVVan.Frame.Width, TVVan.Frame.Height, false, Transparency);
                    d.DrawBitmap(HighLabel.Texture,
                    x + HighLabel.Frame.PivotX - (fliph ? (HighLabel.Frame.Width - editorAnim.Frames[0].Frame.Width) : 0),
                    y + HighLabel.Frame.PivotY + (flipv ? (HighLabel.Frame.Height - editorAnim.Frames[0].Frame.Height) : 0),
                    HighLabel.Frame.Width, HighLabel.Frame.Height, false, Transparency);
                    d.DrawBitmap(InsideTVs.Texture,
                    x + InsideTVs.Frame.PivotX - (fliph ? (InsideTVs.Frame.Width - editorAnim.Frames[0].Frame.Width) : 0),
                    y + InsideTVs.Frame.PivotY + (flipv ? (InsideTVs.Frame.Height - editorAnim.Frames[0].Frame.Height) : 0),
                    InsideTVs.Frame.Width, InsideTVs.Frame.Height, false, Transparency);
                    d.DrawBitmap(ramp.Texture,
                    x + ramp.Frame.PivotX - (fliph ? (ramp.Frame.Width - editorAnim.Frames[0].Frame.Width) : 0),
                    y + ramp.Frame.PivotY + (flipv ? (ramp.Frame.Height - editorAnim.Frames[0].Frame.Height) : 0),
                    ramp.Frame.Width, ramp.Frame.Height, false, Transparency);
                    d.DrawBitmap(frontTireL.Texture,
                    x + frontTireL.Frame.PivotX - (fliph ? (frontTireL.Frame.Width - editorAnim.Frames[0].Frame.Width) : 0),
                    y + frontTireL.Frame.PivotY + (flipv ? (frontTireL.Frame.Height - editorAnim.Frames[0].Frame.Height) : 0),
                    frontTireL.Frame.Width, frontTireL.Frame.Height, false, Transparency);
                    d.DrawBitmap(frontTireR.Texture,
                    x + frontTireR.Frame.PivotX - (fliph ? (frontTireR.Frame.Width - editorAnim.Frames[0].Frame.Width) : 0),
                    y + frontTireR.Frame.PivotY + (flipv ? (frontTireR.Frame.Height - editorAnim.Frames[0].Frame.Height) : 0),
                    frontTireR.Frame.Width, frontTireR.Frame.Height, false, Transparency);
                    d.DrawBitmap(VanSatalite.Texture,
                    x + VanSatalite.Frame.PivotX - (fliph ? (VanSatalite.Frame.Width - editorAnim.Frames[0].Frame.Width) : 0),
                    y + VanSatalite.Frame.PivotY + (flipv ? (VanSatalite.Frame.Height - editorAnim.Frames[0].Frame.Height) : 0),
                    VanSatalite.Frame.Width, VanSatalite.Frame.Height, false, Transparency);
                    d.DrawBitmap(frame11.Texture,
                    x + frame11.Frame.PivotX - (fliph ? (frame11.Frame.Width - editorAnim11.Frames[0].Frame.Width) : 0),
                    y + frame11.Frame.PivotY + (flipv ? (frame11.Frame.Height - editorAnim11.Frames[0].Frame.Height) : 0),
                    frame11.Frame.Width, frame11.Frame.Height, false, Transparency);
                    d.DrawBitmap(frame12.Texture,
                    x + frame12.Frame.PivotX - (fliph ? (frame12.Frame.Width - editorAnim12.Frames[0].Frame.Width) : 0),
                    y + frame12.Frame.PivotY + (flipv ? (frame12.Frame.Height - editorAnim12.Frames[0].Frame.Height) : 0),
                    frame12.Frame.Width, frame12.Frame.Height, false, Transparency);
                }

                if (objType == 1) // Reverse (TV Van)
                {
                    d.DrawBitmap(Window.Texture,
                    x - Window.Frame.PivotX - Window.Frame.Width - (false ? (Window.Frame.Width - editorAnim.Frames[0].Frame.Width) : 0),
                    y + Window.Frame.PivotY + (flipv ? (Window.Frame.Height - editorAnim.Frames[0].Frame.Height) : 0),
                    Window.Frame.Width, Window.Frame.Height, false, Transparency);
                    d.DrawBitmap(backsideTire.Texture,
                    x - backsideTire.Frame.PivotX - backsideTire.Frame.Width - (false ? (backsideTire.Frame.Width - editorAnim.Frames[0].Frame.Width) : 0),
                    y + backsideTire.Frame.PivotY + (flipv ? (backsideTire.Frame.Height - editorAnim.Frames[0].Frame.Height) : 0),
                    backsideTire.Frame.Width, backsideTire.Frame.Height, false, Transparency);
                    d.DrawBitmap(TVVan.Texture,
                    x + TVVan.Frame.PivotX + (true ? (TVVan.Frame.Width - editorAnim.Frames[0].Frame.Width) : 0),
                    y + TVVan.Frame.PivotY + (flipv ? (TVVan.Frame.Height - editorAnim.Frames[0].Frame.Height) : 0),
                    TVVan.Frame.Width, TVVan.Frame.Height, false, Transparency);
                    d.DrawBitmap(HighLabel.Texture,
                    x + HighLabel.Frame.PivotX + HighLabel.Frame.Width * 2 - (true ? (HighLabel.Frame.Width - editorAnim.Frames[0].Frame.Width) : 0),
                    y + HighLabel.Frame.PivotY + (flipv ? (HighLabel.Frame.Height - editorAnim.Frames[0].Frame.Height) : 0),
                    HighLabel.Frame.Width, HighLabel.Frame.Height, false, Transparency);
                    d.DrawBitmap(InsideTVs.Texture,
                    x + InsideTVs.Frame.PivotX + InsideTVs.Frame.Width - (true ? (InsideTVs.Frame.Width - editorAnim.Frames[0].Frame.Width) : 0),
                    y + InsideTVs.Frame.PivotY + (flipv ? (InsideTVs.Frame.Height - editorAnim.Frames[0].Frame.Height) : 0),
                    InsideTVs.Frame.Width, InsideTVs.Frame.Height, false, Transparency);
                    d.DrawBitmap(ramp.Texture,
                    x - ramp.Frame.PivotX - ramp.Frame.Width - (false ? (ramp.Frame.Width - editorAnim.Frames[0].Frame.Width) : 0),
                    y + ramp.Frame.PivotY + (flipv ? (ramp.Frame.Height - editorAnim.Frames[0].Frame.Height) : 0),
                    ramp.Frame.Width, ramp.Frame.Height, false, Transparency);
                    d.DrawBitmap(frontTireL.Texture,
                    x - frontTireL.Frame.PivotX - frontTireL.Frame.Width - (false ? (frontTireL.Frame.Width - editorAnim.Frames[0].Frame.Width) : 0),
                    y + frontTireL.Frame.PivotY + (flipv ? (frontTireL.Frame.Height - editorAnim.Frames[0].Frame.Height) : 0),
                    frontTireL.Frame.Width, frontTireL.Frame.Height, false, Transparency);
                    d.DrawBitmap(frontTireR.Texture,
                    x - frontTireR.Frame.PivotX - frontTireR.Frame.Width - (false ? (frontTireR.Frame.Width - editorAnim.Frames[0].Frame.Width) : 0),
                    y + frontTireR.Frame.PivotY + (flipv ? (frontTireR.Frame.Height - editorAnim.Frames[0].Frame.Height) : 0),
                    frontTireR.Frame.Width, frontTireR.Frame.Height, false, Transparency);
                    d.DrawBitmap(VanSatalite.Texture,
                    x - VanSatalite.Frame.PivotX - VanSatalite.Frame.Width - (false ? (VanSatalite.Frame.Width - editorAnim.Frames[0].Frame.Width) : 0),
                    y + VanSatalite.Frame.PivotY + (flipv ? (VanSatalite.Frame.Height - editorAnim.Frames[0].Frame.Height) : 0),
                    VanSatalite.Frame.Width, VanSatalite.Frame.Height, false, Transparency);
                    d.DrawBitmap(frame11.Texture,
                    x - frame11.Frame.PivotX - frame11.Frame.Width - (true ? (frame11.Frame.Width - editorAnim11.Frames[0].Frame.Width) : 0),
                    y + frame11.Frame.PivotY + (flipv ? (frame11.Frame.Height - editorAnim11.Frames[0].Frame.Height) : 0),
                    frame11.Frame.Width, frame11.Frame.Height, false, Transparency);
                    d.DrawBitmap(frame12.Texture,
                    x - frame12.Frame.PivotX - frame12.Frame.Width - (true ? (frame12.Frame.Width - editorAnim12.Frames[0].Frame.Width) : 0),
                    y + frame12.Frame.PivotY + (flipv ? (frame12.Frame.Height - editorAnim12.Frames[0].Frame.Height) : 0),
                    frame12.Frame.Width, frame12.Frame.Height, false, Transparency);
                }

                if (objType >= 14) //Game Gear TV
                {
                    d.DrawBitmap(frame16.Texture,
                    x + frame16.Frame.PivotX - (fliph ? (frame16.Frame.Width - editorAnim16.Frames[0].Frame.Width) : 0),
                    y + frame16.Frame.PivotY + (flipv ? (frame16.Frame.Height - editorAnim16.Frames[0].Frame.Height) : 0),
                    frame16.Frame.Width, frame16.Frame.Height, false, Transparency);
                    d.DrawBitmap(frame10.Texture,
                    x + frame10.Frame.PivotX - (fliph ? (frame10.Frame.Width - editorAnim10.Frames[0].Frame.Width) : 0),
                    y + frame10.Frame.PivotY + (flipv ? (frame10.Frame.Height - editorAnim10.Frames[0].Frame.Height) : 0),
                    frame10.Frame.Width, frame10.Frame.Height, false, Transparency);
                }

                if (objType == 2 || objType == 5 || objType == 6 || objType == 11) //Satalite Normal
                {
                    d.DrawBitmap(normalSatalite.Texture,
                    x + normalSatalite.Frame.PivotX - (fliph ? (normalSatalite.Frame.Width - editorAnim13.Frames[0].Frame.Width) : 0),
                    y + normalSatalite.Frame.PivotY + (flipv ? (normalSatalite.Frame.Height - editorAnim13.Frames[0].Frame.Height) : 0),
                    normalSatalite.Frame.Width, normalSatalite.Frame.Height, false, Transparency);
                }
                if (objType == 3 || objType == 4 || objType == 6 || objType == 9) //Satalite Flipped H
                {
                    d.DrawBitmap(normalSataliteH.Texture,
                    x - normalSataliteH.Frame.PivotX - normalSataliteH.Frame.Width - (true ? (normalSataliteH.Frame.Width - normalSataliteReversedH.Frames[0].Frame.Width) : 0),
                    y + normalSataliteH.Frame.PivotY + (false ? (normalSataliteH.Frame.Height - normalSataliteReversedH.Frames[0].Frame.Height) : 0),
                    normalSataliteH.Frame.Width, normalSataliteH.Frame.Height, false, Transparency);
                }
                if (objType == 3 || objType == 5 || objType == 7 || objType == 10) //Satalite Flipped V
                {
                    d.DrawBitmap(normalSataliteV.Texture,
x + normalSataliteV.Frame.PivotX - (false ? (normalSataliteV.Frame.Width - normalSataliteReversedV.Frames[0].Frame.Width) : 0),
y + normalSataliteV.Frame.PivotY + normalSataliteV.Frame.Height + (true ? (normalSataliteV.Frame.Height - normalSataliteReversedV.Frames[0].Frame.Height) : 0),
normalSataliteV.Frame.Width, normalSataliteV.Frame.Height, false, Transparency);
                }
                if (objType == 2 || objType == 4 || objType == 7 || objType == 8) //Satalite Flipped VH
                {
                    d.DrawBitmap(normalSatalite2.Texture,
x - normalSatalite2.Frame.PivotX - normalSatalite2.Frame.Width - (true ? (normalSatalite2.Frame.Width - editorAnim13.Frames[0].Frame.Width) : 0),
y + normalSatalite2.Frame.PivotY + normalSatalite2.Frame.Height + (true ? (normalSatalite2.Frame.Height - editorAnim13.Frames[0].Frame.Height) : 0),
normalSatalite2.Frame.Width, normalSatalite2.Frame.Height, false, Transparency);
                }
                if (objType == 8 || objType == 10) //Satalite Upward
                {
                    d.DrawBitmap(upwardsSatalite.Texture,
                    x + upwardsSatalite.Frame.PivotX - (fliph ? (upwardsSatalite.Frame.Width - editorAnim14.Frames[0].Frame.Width) : 0),
                    y + upwardsSatalite.Frame.PivotY + (flipv ? (upwardsSatalite.Frame.Height - editorAnim14.Frames[0].Frame.Height) : 0),
                    upwardsSatalite.Frame.Width, upwardsSatalite.Frame.Height, false, Transparency);
                }
                if (objType == 9 || objType == 11) //Satalite Downward
                {
                    d.DrawBitmap(downwardsFaceSatalite.Texture,
                    x + downwardsFaceSatalite.Frame.PivotX - (fliph ? (downwardsFaceSatalite.Frame.Width - downwardsSatalite.Frames[0].Frame.Width) : 0),
                    y + downwardsFaceSatalite.Frame.PivotY + downwardsFaceSatalite.Frame.Width + sataliteHook.Frame.Height / 2 + (true ? (downwardsFaceSatalite.Frame.Height - downwardsSatalite.Frames[0].Frame.Height) : 0),
                    downwardsFaceSatalite.Frame.Width, downwardsFaceSatalite.Frame.Height, false, Transparency);
                }
                if (objType <= 11 && objType >= 2) //Satalite Center
                {
                    d.DrawBitmap(sataliteHook.Texture,
x + sataliteHook.Frame.PivotX - (fliph ? (sataliteHook.Frame.Width - editorAnim15.Frames[0].Frame.Width) : 0),
y + sataliteHook.Frame.PivotY + (flipv ? (sataliteHook.Frame.Height - editorAnim15.Frames[0].Frame.Height) : 0),
sataliteHook.Frame.Width, sataliteHook.Frame.Height, false, Transparency);
                }
            }
        }

        public override string GetObjectName()
        {
            return "TVVan";
        }
    }
}
