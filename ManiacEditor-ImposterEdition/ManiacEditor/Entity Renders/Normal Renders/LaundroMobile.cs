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
    public class LaundroMobile : EntityRenderer
    {
        //Shorthanding Setting Files
        Properties.Settings mySettings = Properties.Settings.Default;
        Properties.KeyBinds myKeyBinds = Properties.KeyBinds.Default;
        public override void Draw(DevicePanel d, SceneEntity entity, EditorEntity e, int x, int y, int Transparency, int index = 0, int previousChildCount = 0, int platformAngle = 0, EditorAnimations Animation = null, bool selected = false, AttributeValidater attribMap = null)
        {
            bool fliph = false;
            bool flipv = false;
            int type = (int)entity.attributesMap["type"].ValueUInt8;
            int blockID = 0;
            int randomNum = entity.SlotID % 3;
            switch (type)
            {
                case 4:
                    blockID = randomNum;
                    break;
                case 5:

                    blockID = randomNum+3;
                    break;
            }
            var editorAnim = e.EditorInstance.EditorEntity_ini.LoadAnimation2("LaundroMobile", d, 0, 0, fliph, flipv, false);
            var editorAnim2 = e.EditorInstance.EditorEntity_ini.LoadAnimation2("LaundroMobile", d, 9, blockID, fliph, flipv, false);
            var editorAnim3 = e.EditorInstance.EditorEntity_ini.LoadAnimation2("LaundroMobile", d, 1, -1, fliph, flipv, false);
            var editorAnim4 = e.EditorInstance.EditorEntity_ini.LoadAnimation2("LaundroMobile", d, 3, -1, fliph, flipv, false);
            var editorAnim5 = e.EditorInstance.EditorEntity_ini.LoadAnimation2("LaundroMobile", d, 8, -1, fliph, flipv, false);
            var editorAnim6 = e.EditorInstance.EditorEntity_ini.LoadAnimation2("LaundroMobile", d, 2, -1, fliph, flipv, false);
            var editorAnim7 = e.EditorInstance.EditorEntity_ini.LoadAnimation2("LaundroMobile", d, 0, 3, fliph, flipv, false);
            var editorAnim8 = e.EditorInstance.EditorEntity_ini.LoadAnimation2("LaundroMobile", d, 0, 2, fliph, flipv, false);
            var editorAnim9 = e.EditorInstance.EditorEntity_ini.LoadAnimation2("LaundroMobile", d, 0, 4, fliph, flipv, false);
            var editorAnimIcon = e.EditorInstance.EditorEntity_ini.LoadAnimation2("EditorIcons2", d, 0, 14, false, false, false);
            if (editorAnim != null && editorAnim.Frames.Count != 0 && editorAnim2 != null && editorAnim2.Frames.Count != 0 && editorAnim3 != null && editorAnim3.Frames.Count != 0 && editorAnim4 != null && editorAnim4.Frames.Count != 0 && editorAnim5 != null && editorAnim5.Frames.Count != 0 && editorAnim6 != null && editorAnim6.Frames.Count != 0 && editorAnim7 != null && editorAnim7.Frames.Count != 0 && editorAnim8 != null && editorAnim8.Frames.Count != 0 && editorAnim9 != null && editorAnim9.Frames.Count != 0)
            {
                var frame = editorAnim.Frames[0];
                var frameBlock = editorAnim2.Frames[0];
                var frameLaundry = editorAnim3.Frames[Animation.index];
                var framePropel = editorAnim4.Frames[Animation.index];
                var frameBomb = editorAnim5.Frames[Animation.index2];
                var frameLaundryCenter = editorAnim6.Frames[Animation.index];
                var frameLaundryCenterBottom = editorAnim7.Frames[0];
                var frameLaundryCenterTop = editorAnim8.Frames[0];
                var frameRockets = editorAnim9.Frames[0];


                if (type == 0) {
                    Animation.ProcessAnimation(framePropel.Entry.SpeedMultiplyer, framePropel.Entry.Frames.Count, framePropel.Frame.Delay);
                    d.DrawBitmap(frameRockets.Texture,
                        x + frameRockets.Frame.PivotX - 4,
                        y + frameRockets.Frame.PivotY - 27,
                        frameRockets.Frame.Width, frameRockets.Frame.Height, false, Transparency);
                    d.DrawBitmap(frame.Texture,
                        x + frame.Frame.PivotX,
                        y + frame.Frame.PivotY,
                        frame.Frame.Width, frame.Frame.Height, false, Transparency);
                    d.DrawBitmap(framePropel.Texture,
                        x + framePropel.Frame.PivotX,
                        y + framePropel.Frame.PivotY,
                        framePropel.Frame.Width, framePropel.Frame.Height, false, Transparency);
                    d.DrawBitmap(frameRockets.Texture,
                        x + frameRockets.Frame.PivotX - 18,
                        y + frameRockets.Frame.PivotY + 20,
                        frameRockets.Frame.Width, frameRockets.Frame.Height, false, Transparency);
                    d.DrawBitmap(frameRockets.Texture,
                        x + frameRockets.Frame.PivotX - 18,
                        y + frameRockets.Frame.PivotY - 27,
                        frameRockets.Frame.Width, frameRockets.Frame.Height, false, Transparency);
                }
                    else if (type == 1) {
                    Animation.ProcessAnimation3(frameBomb.Entry.SpeedMultiplyer, frameBomb.Entry.Frames.Count, frameBomb.Frame.Delay);
                    d.DrawBitmap(frameBomb.Texture,
                            x + frameBomb.Frame.PivotX,
                            y + frameBomb.Frame.PivotY,
                            frameBomb.Frame.Width, frameBomb.Frame.Height, false, Transparency);
                    }
                    else if (type == 2)
                    {
                    Animation.ProcessAnimation2(frameLaundry.Entry.SpeedMultiplyer, frameLaundry.Entry.Frames.Count, frameLaundry.Frame.Delay);
                    d.DrawBitmap(frameLaundry.Texture,
                        x + frameLaundry.Frame.PivotX,
                        y + frameLaundry.Frame.PivotY + 16,
                        frameLaundry.Frame.Width, frameLaundry.Frame.Height, false, Transparency);
                    d.DrawBitmap(frameLaundry.Texture,
                        x + frameLaundry.Frame.PivotX,
                        y + frameLaundry.Frame.PivotY + frameLaundry.Frame.PivotY + 16,
                        frameLaundry.Frame.Width, frameLaundry.Frame.Height, false, Transparency);
                    d.DrawBitmap(frameLaundry.Texture,
                        x + frameLaundry.Frame.PivotX,
                        y + frameLaundry.Frame.PivotY + -(frameLaundry.Frame.PivotY * 2) + 16,
                        frameLaundry.Frame.Width, frameLaundry.Frame.Height, false, Transparency);
                    d.DrawBitmap(frameLaundry.Texture,
                        x + frameLaundry.Frame.PivotX,
                        y - frameLaundry.Frame.PivotY + -(frameLaundry.Frame.PivotY * 2) + 16,
                        frameLaundry.Frame.Width, frameLaundry.Frame.Height, false, Transparency);

                    d.DrawBitmap(frameLaundryCenterBottom.Texture,
                        x + frameLaundryCenterBottom.Frame.PivotX,
                        y + frameLaundryCenterBottom.Frame.PivotY + frameLaundryCenterBottom.Frame.PivotY * 3 + 16,
                        frameLaundryCenterBottom.Frame.Width, frameLaundryCenterBottom.Frame.Height, false, Transparency);

                    d.DrawBitmap(frameLaundryCenterTop.Texture,
                        x + frameLaundryCenterTop.Frame.PivotX,
                        y + frameLaundryCenterTop.Frame.PivotY,
                        frameLaundryCenterTop.Frame.Width, frameLaundryCenterTop.Frame.Height, false, Transparency);

                    d.DrawBitmap(frameLaundryCenter.Texture,
                        x + frameLaundryCenter.Frame.PivotX,
                        y + frameLaundryCenter.Frame.PivotY + frameLaundryCenter.Frame.PivotY*2 + 16,
                        frameLaundryCenter.Frame.Width, frameLaundryCenter.Frame.Height, false, Transparency);
                    }
                    else if (type == 4 || type == 5) {
                    d.DrawBitmap(frameBlock.Texture,
                            x + frameBlock.Frame.PivotX,
                            y + frameBlock.Frame.PivotY,
                            frameBlock.Frame.Width, frameBlock.Frame.Height, false, Transparency);
                    }
                    else
                    { 
                        if (editorAnimIcon != null && editorAnimIcon.Frames.Count != 0)
                        {
                            var frameIcon = editorAnimIcon.Frames[0];
                            d.DrawBitmap(frameIcon.Texture, x + frameIcon.Frame.PivotX, y + frameIcon.Frame.PivotY,
                                frameIcon.Frame.Width, frameIcon.Frame.Height, false, Transparency);
                        }
                    }



            }
        }

        public override string GetObjectName()
        {
            return "LaundroMobile";
        }
    }
}
