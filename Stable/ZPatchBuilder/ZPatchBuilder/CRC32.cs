using System;
using System.IO;
namespace ZPatchBuilder
{
    class CRC32
    {
        static uint[] crc32_table = new uint[256];
        static uint ulPolynomial = 0x04c11db7;


        public uint GetCRC32(string name)
        {

            FileStream fs = null;
            try
            {
                fs = new FileStream(name, FileMode.Open, FileAccess.Read);
                if (fs == null)
                    return 0;

                byte[] buffer;


                int length = (int)fs.Length;  // get file length
                buffer = new byte[length];    // create buffer

                // read until Read method returns 0 (end of the stream has been reached)
                fs.Read(buffer, 0, length);
                fs.Close();

                InitCrcTable();

                uint crc = GetCRC(buffer, length);
                return crc;
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.Message);
            }
            return 0;
        }

        ///////////////////////////////////////////////////////////////////

        static void InitCrcTable()
        {

            // 256 values representing ASCII character codes.

            for (uint i = 0; i <= 0xFF; i++)
            {
                crc32_table[i] = Reflect(i, 8) << 24;

                for (uint j = 0; j < 8; j++)
                {
                    long val = crc32_table[i] & (1 << 31);

                    if (val != 0)
                        val = ulPolynomial;
                    else val = 0;

                    crc32_table[i] = (crc32_table[i] << 1) ^ (uint)val;
                }

                crc32_table[i] = Reflect(crc32_table[i], 32);
            }



        }

        //////////////////////////////////////////////////////////////
        // Reflection is a requirement for the official CRC-32 standard.
        // You can create CRCs without it, but they won't conform to the standard.
        //////////////////////////////////////////////////////////////////////////

        static uint Reflect(uint re, byte ch)
        {  // Used only by Init_CRC32_Table()

            uint value = 0;

            // Swap bit 0 for bit 7
            // bit 1 for bit 6, etc.
            for (int i = 1; i < (ch + 1); i++)
            {
                long tmp = re & 1;
                int v = ch - i;

                if (tmp != 0)
                    value |= (uint)1 << v; //(uint)(ch - i));

                re >>= 1;
            }

            return value;
        }

        ///////////////////////////////////////////////////////////////

        static uint GetCRC(byte[] buffer, int bufsize)
        {

            uint crc = 0xffffffff;
            int len = bufsize;
            // Save the text in the buffer.

            // Perform the algorithm on each character
            // in the string, using the lookup table values.

            for (uint i = 0; i < len; i++)
                crc = (crc >> 8) ^ crc32_table[(crc & 0xFF) ^ buffer[i]];


            // Exclusive OR the result with the beginning value.
            return crc ^ 0xffffffff;

        }
    }
}
