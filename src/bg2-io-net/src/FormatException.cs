using System;

namespace bg2io {
    public class FormatException : Exception {
        public FormatException()
            : base("Invalid file format")
        {

        }

        public FormatException(string message)
            : base(message)
        {

        }

        public FormatException(string message, Exception inner)
            : base(message, inner)
        {
            
        }
    }
}