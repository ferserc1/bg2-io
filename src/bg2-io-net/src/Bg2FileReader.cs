using System;
using System.IO;
using System.Collections.Generic;

namespace bg2io {

	class Bg2FileReader {
		
		public Bg2File Open(string filePath) {
			BinaryReader reader = new BinaryReader(new FileStream(filePath, FileMode.Open));
			Bg2File bg2File = new Bg2File();

			ReadHeader(bg2File, reader);
			ReadPlist(bg2File, reader);
			
			reader.Close();
			
			return bg2File;
		}

		private void ReadHeader(Bg2File bg2File, BinaryReader reader)
		{
			bg2File.endianess = reader.ReadByte();
			bg2File.majorVersion = reader.ReadByte();
			bg2File.minorVersion = reader.ReadByte();
			bg2File.revision = reader.ReadByte();

			CheckBlock(reader, "hedr");
		
			// Number of polyList elements
			bg2File.numberOfPlist = ReadInt32(reader);

			// Materials
			CheckBlock(reader, "mtrl");
			bg2File.materialDataString = ReadString(reader);

			// Joints
			string block = ReadBlock(reader);
			if (block == "proj") {
				// Deprecated: this section only skips the data
				Int32 shadowTextSize = ReadInt32(reader);
				reader.ReadBytes(shadowTextSize);

				// Attenuation: one float
				reader.ReadBytes(4);

				// Projection matrix: 16 float
				reader.ReadBytes(4 * 16);

				// View matrix
				reader.ReadBytes(4 * 16);

				// Next block
				block = ReadBlock(reader); 
			}

			if (block == "join") {
				// joint data
				bg2File.jointDataString = ReadString(reader);
			}
			else {
				throw new UnexpectedBlockException();
			}
		}

		private void ReadPlist(Bg2File bg2File, BinaryReader reader)
		{
			CheckBlock(reader, "plst");

			for (int i = 0; i < bg2File.numberOfPlist; ++i) {
				bg2File.polyLists.Add(new PolyList());
			}


			bool done = false;
			int currentPlistIndex = 0;
			PolyList currentPlist = bg2File.polyLists[currentPlistIndex];
			while (!done)
			{
				string block = ReadBlock(reader);
				if (block == "pnam") {          // PolyListName
					currentPlist.name = ReadString(reader);
				}
				else if (block == "mnam") {     // MaterialName
					currentPlist.materialName = ReadString(reader);
				}
				else if (block == "varr") {     // VertexArray
					currentPlist.vertex = ReadFloatArray(reader);
				}
				else if (block == "narr") {     // NormalArray
					currentPlist.normal = ReadFloatArray(reader);
				}
				else if (block == "t0ar") {    // TexCoord0Array
					currentPlist.t0Coord = ReadFloatArray(reader);
				}
				else if (block == "t1ar") {    // TexCoord1Array
					currentPlist.t1Coord = ReadFloatArray(reader);
				}
				else if (block == "t2ar") {    // TexCoord2Array
					currentPlist.t2Coord = ReadFloatArray(reader);
				}
				else if (block == "indx") {     // IndexArray
					currentPlist.index = ReadIntArray(reader);
				}
				else if (block == "plst" || block == "endf") {  // End file or next plist
					if (block == "endf") {  // End file
						var cmpsBlock = ReadBlock(reader);  // Check for component data
						if (cmpsBlock == "cmps") {
							// Components
							var components = ReadString(reader);
							bg2File.componentsDataString = components;
						}

						done = true;
					}
					else {
						++currentPlistIndex;
						currentPlist = bg2File.polyLists[currentPlistIndex];
					}
				}
				else {
					throw new FormatException("Corrupted poly list data");
				}
			}
		}

		private string ReadBlock(BinaryReader reader)
		{
			char b0 = reader.ReadChar();
			char b1 = reader.ReadChar();
			char b2 = reader.ReadChar();
			char b3 = reader.ReadChar();

			return string.Format("{0}{1}{2}{3}", b0, b1, b2, b3);
		}

		private void CheckBlock(BinaryReader reader, string expected)
		{
			if (ReadBlock(reader) != expected)
			{
				throw new UnexpectedBlockException();
			}
		}

		private Int32 ReadInt32(BinaryReader reader)
		{
			var data = reader.ReadBytes(4);
			Array.Reverse(data);
			return BitConverter.ToInt32(data, 0);
		}

		private string ReadString(BinaryReader reader)
		{
			var strLen = ReadInt32(reader);
			var buffer = reader.ReadBytes(strLen);
			return System.Text.Encoding.ASCII.GetString(buffer);
		}

		private float[] ReadFloatArray(BinaryReader reader)
		{
			List<float> result = new List<float>();
			Int32 size = ReadInt32(reader);
			for (Int32 i = 0; i < size; ++i) {
				result.Add(ReadFloat(reader));
			}

			return size > 0 ? result.ToArray() : null;
		}

		private float ReadFloat(BinaryReader reader)
		{
			var data = reader.ReadBytes(4);
			Array.Reverse(data);
			return BitConverter.ToSingle(data);
		}

		private int[] ReadIntArray(BinaryReader reader)
		{
			List<int> result = new List<int>();
			Int32 size = ReadInt32(reader);
			for (Int32 i = 0; i < size; ++i) {
				result.Add(ReadInt32(reader));
			}

			return size > 0 ? result.ToArray() : null;
		}
	}
}
