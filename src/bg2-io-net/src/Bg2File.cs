using System;
using System.Collections.Generic;

namespace bg2io {
	public class Bg2File {
		public byte endianess;
		public byte majorVersion;
		public byte minorVersion;
		public byte revision;

		public Int32 numberOfPlist;

		public string materialDataString = "";

		public string jointDataString = "";

		public string componentsDataString = "";

		public List<PolyList> polyLists = new List<PolyList>();
	

		public string GetVersion()
		{
			return string.Format("{0}.{1}.{2}", majorVersion, minorVersion, revision);
		}
	}
}
