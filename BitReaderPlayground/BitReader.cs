using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BitReaderPlayground
{
    public class BitReader
    {
        public List<byte> Bytes { get; set; }
        public int BitOffset { get; set; }
        public int ByteOffset { get; set; }

        public BitReader(IEnumerable<byte> bytes)
        {
            this.Bytes = new List<byte>(bytes);
        }

        public int ReadBit()
        {
            var bit = Bytes[ByteOffset] & (1 << BitOffset);
            BitOffset++;
            if(BitOffset >= 8)
            {
                BitOffset = 0;
                ByteOffset++;
            }

            return bit;
        }

        public int ReadInt32(int numBits)
        {
            int result = 0;
            for (int i = 0; i < numBits; i++)
            {
                var bit = ReadBit();
                result |= (bit << i);
            }

            return result;
        }

        public float ReadFloat12()
        {
            var baseValue = ReadInt32(12);

            var mask = Math.Pow(2, 12) - 1;

            var result = (baseValue - mask) / mask;

            return (float)result;
        }

        public float ReadFloat(int numBits)
        {
            var baseValue = ReadInt32(numBits);

            var mask = Math.Pow(2, numBits-1) - 1;
            var floatMask = 1.0f / mask;
            
            var result = (baseValue - mask) * floatMask;

            return (float)result;
        }
    }
}
