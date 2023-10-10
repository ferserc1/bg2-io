using System;

namespace bg2io {
    public class UnexpectedBlockException : Exception {
        public UnexpectedBlockException()
            : base("Unexpected block found")
        {

        }

        public UnexpectedBlockException(string message)
            : base(message)
        {

        }

        public UnexpectedBlockException(string message, Exception inner)
            : base(message, inner)
        {
            
        }
    }
}