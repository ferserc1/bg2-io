namespace bg2io {

    class Bg2FileReader {
        
        public Bg2File open(string filePath) {
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
            // TODO: Parse plists
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
    }
}