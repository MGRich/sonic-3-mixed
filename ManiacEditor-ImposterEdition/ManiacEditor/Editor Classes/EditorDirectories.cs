﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using IniParser;
using IniParser.Model;

namespace ManiacEditor
{
	public class EditorDirectories
	{
		public Editor Instance;
		IniData ModPackInfo;
		public List<Tuple<string, List<Tuple<string, string>>>> ModListInformation;
		public EditorDirectories(Editor instance)
		{
			Instance = instance;
			LoadFile();
		}

        public List<string> DataPackNamesToList()
        {
            List<string> PackNames = new List<string>();
            foreach (var config in ModListInformation)
            {
                PackNames.Add(config.Item1);
            }
            return PackNames;
        }

		public void LoadFile()
		{
			if (GetFile() == false)
			{
				var ModListFile = File.Create(Path.Combine(Environment.CurrentDirectory, "Resources", "ModPackLists.ini"));
				ModListFile.Close();
				if (GetFile() == false) return;
			}
			InterpretInformation();
		}

		public void InterpretInformation()
		{
			ModListInformation = new List<Tuple<string, List<Tuple<string, string>>>>();
			foreach (var section in ModPackInfo.Sections)
			{
				List<Tuple<string, string>> Keys = new List<Tuple<string, string>>();
				foreach (var key in section.Keys)
				{
					Keys.Add(new Tuple<string, string>(key.KeyName, key.Value));
				}
				ModListInformation.Add(new Tuple<string, List<Tuple<string, string>>>(section.SectionName, Keys));
			}

        }

		public void PrintInformation()
		{
			var n = Environment.NewLine;
			string fullInfo = "";
			foreach(var pair in ModListInformation)
			{
				fullInfo += String.Format("[{0}]", pair.Item1) + n;
				foreach (var key in pair.Item2)
				{
					fullInfo += String.Format("   {0}={1}", key.Item1, key.Item2) + n;
				}
			}
			RSDKrU.MessageBox.Show(fullInfo);
		}

		public void SaveFile()
		{
			IniData SaveData = new IniData();
			foreach (var pair in ModListInformation)
			{
				SectionData section = new SectionData(pair.Item1);
				foreach (var key in pair.Item2)
				{
					section.Keys.AddKey(key.Item1, key.Item2);
				}
				SaveData.Sections.Add(section);
			}
			string path = Path.Combine(Environment.CurrentDirectory, "Resources", "ModPackLists.ini");
			var parser = new FileIniDataParser();
			parser.WriteFile(path, SaveData);
		}

		public static FileStream GetModPackList(string path)
		{
			if (!File.Exists(path)) return null;
			return new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read);
		}

		public bool GetFile()
		{
			var parser = new FileIniDataParser();
            if (!File.Exists(Path.Combine(Environment.CurrentDirectory, "Resources", "ModPackLists.ini")))
            {
                return false;
            }
			else
			{
                IniData file = parser.ReadFile(Path.Combine(Environment.CurrentDirectory, "Resources", "ModPackLists.ini"));
                ModPackInfo = file;
			}
			return true;
		}
	}
}
