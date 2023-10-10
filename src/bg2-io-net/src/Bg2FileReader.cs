namespace bg2io {

    class Bg2FileReader {
        
        public Bg2File open(string filePath) {
            try {
                BinaryReader reader = new BinaryReader(new FileStream(filePath, FileMode.Open));
                Bg2File bg2File = new Bg2File();

                ReadHeader(bg2File, reader);

                // TODO: Read file
                
                reader.Close();
                return bg2File;
            }
            catch (IOException err) {
                Console.WriteLine(err.Message);
                return null;
            }
        }

        private void ReadHeader(Bg2File bg2File, BinaryReader reader)
        {
            bg2File.endianess = reader.ReadByte();
            bg2File.majorVersion = reader.ReadByte();
            bg2File.minorVersion = reader.ReadByte();
            bg2File.revision = reader.ReadByte();
        }
    }
}