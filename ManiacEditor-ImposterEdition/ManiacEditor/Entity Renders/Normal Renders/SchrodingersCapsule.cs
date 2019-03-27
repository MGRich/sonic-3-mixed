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
    public class SchrodingersCapsule : EntityRenderer
    {

        public override void Draw(DevicePanel d, SceneEntity entity, EditorEntity e, int x, int y, int Transparency, int index = 0, int previousChildCount = 0, int platformAngle = 0, EditorAnimations Animation = null, bool selected = false, AttributeValidater attribMap = null)
        {
            bool fliph = false;
            bool flipv = false;
            var editorAnim = e.EditorInstance.EditorEntity_ini.LoadAnimation2("SchrodingersCapsule", d, 0, 0, fliph, flipv, false);
            var editorAnimInside = e.EditorInstance.EditorEntity_ini.LoadAnimation2("SchrodingersCapsule", d, 0, 2, fliph, flipv, false);
            var editorAnimExclamation = e.EditorInstance.EditorEntity_ini.LoadAnimation2("SchrodingersCapsule", d, 0, 3, fliph, flipv, false);
            var editorAnimButton = e.EditorInstance.EditorEntity_ini.LoadAnimation2("SchrodingersCapsule", d, 1, -1, fliph, flipv, false);

            if (editorAnim != null && editorAnim.Frames.Count != 0 && editorAnimButton != null && editorAnimButton.Frames.Count != 0 && editorAnimInside != null && editorAnimInside.Frames.Count != 0 && editorAnimExclamation != null && editorAnimExclamation.Frames.Count != 0)
            {
                var frame = editorAnim.Frames[0];
                var insideFrame = editorAnimInside.Frames[0];
                var exclamationFrame = editorAnimExclamation.Frames[0];
                var buttonFrame = editorAnimButton.Frames[0];


                d.DrawBitmap(buttonFrame.Texture,
                    x + buttonFrame.Frame.PivotX - (fliph ? (buttonFrame.Frame.Width - editorAnimButton.Frames[0].Frame.Width) : 0),
                    y + exclamationFrame.Frame.PivotY + (flipv ? (buttonFrame.Frame.Height - editorAnimButton.Frames[0].Frame.Height) : 0),
                    buttonFrame.Frame.Width, buttonFrame.Frame.Height, false, Transparency);
                d.DrawBitmap(exclamationFrame.Texture,
                    x + exclamationFrame.Frame.PivotX - (fliph ? (exclamationFrame.Frame.Width - editorAnimExclamation.Frames[3].Frame.Width) : 0),
                    y + exclamationFrame.Frame.PivotY + (flipv ? (exclamationFrame.Frame.Height - editorAnimExclamation.Frames[3].Frame.Height) : 0),
                    exclamationFrame.Frame.Width, exclamationFrame.Frame.Height, false, Transparency);
                d.DrawBitmap(insideFrame.Texture,
                    x + insideFrame.Frame.PivotX - (fliph ? (insideFrame.Frame.Width - editorAnimInside.Frames[2].Frame.Width) : 0),
                    y + insideFrame.Frame.PivotY + (flipv ? (insideFrame.Frame.Height - editorAnimInside.Frames[2].Frame.Height) : 0),
                    insideFrame.Frame.Width, insideFrame.Frame.Height, false, Transparency);



                var mightyAnim = e.EditorInstance.EditorEntity_ini.LoadAnimation2("SchrodingersCapsule", d, 3, -1, fliph, flipv, false);
                var rayAnim = e.EditorInstance.EditorEntity_ini.LoadAnimation2("SchrodingersCapsule", d, 4, -1, fliph, flipv, false);

                if (mightyAnim != null && mightyAnim.Frames.Count != 0 && rayAnim != null && rayAnim.Frames.Count != 0)
                {
                    var mightyFrame = mightyAnim.Frames[Animation.index2 < 6 ? Animation.index2 : 0];
                    var rayFrame = rayAnim.Frames[Animation.index];

                    Animation.ProcessAnimation(rayFrame.Entry.SpeedMultiplyer, rayFrame.Entry.Frames.Count, rayFrame.Frame.Delay);
                    //Animation.ProcessAnimation2(mightyFrame.Entry.SpeedMultiplyer, mightyFrame.Entry.Frames.Count, mightyFrame.Frame.Delay);

                    d.DrawBitmap(mightyFrame.Texture,
                        x + mightyFrame.Frame.PivotX - (fliph ? (mightyFrame.Frame.Width - mightyAnim.Frames[0].Frame.Width) : 0) + 15,
                        y + mightyFrame.Frame.PivotY + (flipv ? (mightyFrame.Frame.Height - mightyAnim.Frames[0].Frame.Height) : 0),
                        mightyFrame.Frame.Width, mightyFrame.Frame.Height, false, Transparency);
                    d.DrawBitmap(rayFrame.Texture,
                        x + rayFrame.Frame.PivotX - (fliph ? (rayFrame.Frame.Width - rayAnim.Frames[0].Frame.Width) : 0) - 15,
                        y + rayFrame.Frame.PivotY + (flipv ? (rayFrame.Frame.Height - rayAnim.Frames[0].Frame.Height) : 0),
                        rayFrame.Frame.Width, rayFrame.Frame.Height, false, Transparency);
                }

                var editorAnimGlass = e.EditorInstance.EditorEntity_ini.LoadAnimation2("SchrodingersCapsule", d, 2, -1, fliph, flipv, false);

                if (editorAnimGlass != null && editorAnimGlass.Frames.Count != 0)
                {
                    var glassFrame = editorAnimGlass.Frames[Animation.index2];

                    Animation.ProcessAnimation3(glassFrame.Entry.SpeedMultiplyer, glassFrame.Entry.Frames.Count, glassFrame.Frame.Delay);

                    d.DrawBitmap(glassFrame.Texture,
                        x + glassFrame.Frame.PivotX - (fliph ? (glassFrame.Frame.Width - editorAnimGlass.Frames[0].Frame.Width) : 0),
                        y + glassFrame.Frame.PivotY + (flipv ? (glassFrame.Frame.Height - editorAnimGlass.Frames[0].Frame.Height) : 0),
                        glassFrame.Frame.Width, glassFrame.Frame.Height, false, 50);
                }





                d.DrawBitmap(frame.Texture,
                    x + frame.Frame.PivotX - (fliph ? (frame.Frame.Width - editorAnim.Frames[0].Frame.Width) : 0),
                    y + frame.Frame.PivotY + (flipv ? (frame.Frame.Height - editorAnim.Frames[0].Frame.Height) : 0),
                    frame.Frame.Width, frame.Frame.Height, false, Transparency);
                d.DrawBitmap(frame.Texture,
                    x + frame.Frame.PivotX - (fliph ? (frame.Frame.Width - editorAnim.Frames[0].Frame.Width) : 0),
                    y + frame.Frame.PivotY + (flipv ? (frame.Frame.Height - editorAnim.Frames[0].Frame.Height) : 0),
                    frame.Frame.Width, frame.Frame.Height, false, Transparency);
            }



        }

        public override string GetObjectName()
        {
            return "SchrodingersCapsule";
        }
    }
}
