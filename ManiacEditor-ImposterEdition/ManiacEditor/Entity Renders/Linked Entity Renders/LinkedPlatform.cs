﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

namespace ManiacEditor.Entity_Renders
{
    public class LinkedPlatform : LinkedRenderer
    {
        public override void Draw(DevicePanel d, RSDKv5.SceneEntity currentEntity, EditorEntity ObjectInstance)
        {
            ushort slotID = currentEntity.SlotID;
            uint childCount = currentEntity.GetAttribute("childCount").ValueVar;
            ushort[] targetSlotIDs = new ushort[childCount];
            for (int i = 0; i < childCount; i++)
            {
                targetSlotIDs[i] = (ushort)(slotID + (i+1));
            }


            var tagged = ObjectInstance.EditorInstance.entities.Entities.Where(e => targetSlotIDs.Contains(e.Entity.SlotID));

            if (tagged != null && tagged.Any())
            {
                foreach (var t in tagged)
                {
                    DrawCenteredLinkArrow(d, currentEntity, t.Entity);
                }
            }
            ObjectInstance.DrawBase(d);
        }

        public override string GetObjectName()
        {
            return "Platform";
        }
    }
}
