﻿using System;
using System.Collections.Generic;
using System.Drawing;

/* Taxman sure loves leaving support for fileformats in his code */

namespace RSDKv2
{
    public class Video
    {

        /// <summary>
        /// the list of every frame
        /// </summary>
        public List<VideoFrame> Frames = new List<VideoFrame>();

        /// <summary>
        /// How many frames the video contains
        /// </summary>
        public ushort VideoInfo;
        /// <summary>
        /// How Wide the frames are (in pixels)
        /// </summary>
        public ushort Width;
        /// <summary>
        /// how Tall the frames are (in pixels)
        /// </summary>
        public ushort Height;

        public class VideoFrame
        {
            /// <summary>
            /// How Wide this frame is (in pixels)
            /// </summary>
            public ushort Width;
            /// <summary>
            /// how Tall this frame is (in pixels)
            /// </summary>
            public ushort Height;

            /// <summary>
            /// the file offset to the next file(?)
            /// </summary>
            public uint FilePos;

            /// <summary>
            /// extra bytes until "," is found
            /// </summary>
            List<byte> idkMan = new List<byte>();

            //GIF VALUES
            /// <summary>
            /// ImageLeft value (from the gif format, ignored by RSDK)
            /// </summary>
            public ushort ImageLeft;
            /// <summary>
            /// ImageTop value (from the gif format, ignored by RSDK)
            /// </summary>
            public ushort ImageTop;
            /// <summary>
            /// ImageWidth value, should be the same as Main Width (from the gif format, ignored by RSDK)
            /// </summary>
            public ushort ImageWidth;
            /// <summary>
            /// ImageHeight value, should be the same as Main Height (from the gif format, ignored by RSDK)
            /// </summary>
            public ushort ImageHeight;
            /// <summary>
            /// whether the image is loaded via interlacing or not
            /// </summary>
            public uint isInterlaced;
            /// <summary>
            /// various image flags
            /// </summary>
            public byte PaletteType;
            /// <summary>
            /// whether or not the image has 128 or 256 colours
            /// </summary>
            public bool FullPallete;

            /// <summary>
            /// the raw image data
            /// </summary>
            public byte[] ImageData;
            /// <summary>
            /// the raw (compressed) image data
            /// </summary>
            public byte[] CompressedImageData;
            int dataptr = 0;

            public bool ExtendedCodeTable = false;

            /// <summary>
            /// the palette for this frame
            /// </summary>
            public List<Color> FramePalette = new List<Color>();

            int startOffset = 0;
            int endOffset = 0;

            int blockLength = 0;
            int bitCache = 0;
            int bitCacheLength = 0;

            public VideoFrame()
            {

            }

            public VideoFrame(ushort width, ushort height)
            {
                Width = width;
                Height = height;

                ImageData = new byte[Width * Height];
            }

            public VideoFrame(Reader reader, ushort width, ushort height)
            {
                Width = width;
                Height = height;
                startOffset = (int)reader.Pos;
                ImageData = new byte[Width * Height];

                FilePos = (uint)(reader.ReadByte() + (reader.ReadByte() << 8) + (reader.ReadByte() << 16) + (reader.ReadByte() << 24));//reader.ReadUInt32();

                for (int i = 0; i < 128; i++)
                {
                    Color c;
                    byte r = reader.ReadByte(); //r
                    byte g = reader.ReadByte(); //g
                    byte b = reader.ReadByte(); //b
                    c = Color.FromArgb(255, r, g, b);
                    //Console.WriteLine("Colour " + i + " = " + c.R + " " + c.G + " " + c.B);
                    FramePalette.Add(c);
                }

                /*bool Next = false;

                while (!Next)
                {
                    byte tmp = reader.ReadByte();
                    idkMan.Add(tmp);
                    //Console.WriteLine("idk lol = " + tmp);
                    if (tmp == 44) { Next = true; } // AKA ','
                }

                ImageLeft = reader.ReadUInt16();
                ImageTop = reader.ReadUInt16();
                ImageWidth = reader.ReadUInt16();
                ImageHeight = reader.ReadUInt16();

                PaletteType = reader.ReadByte();

                //Console.WriteLine("Palette Type = " + PaletteType);

                isInterlaced = (uint)PaletteType << 25 >> 31;

                //Console.WriteLine("Interlaced? = " + isInterlaced);

                //Console.WriteLine("Use full Palette = " + (PaletteType >> 7));

                FullPallete = PaletteType >> 7 == 1;

                if (FullPallete) // Use extra colours?
                {
                    for (int i = 128; i < 256; i++)
                    {
                        Color c;
                        byte r = reader.ReadByte(); //r
                        byte g = reader.ReadByte(); //g
                        byte b = reader.ReadByte(); //b
                        c = Color.FromArgb(255, r, g, b);
                        //Console.WriteLine("(Extra) Colour " + i + " = " + c.R + "," + c.G + "," + c.B);
                        FramePalette.Add(c);
                    }
                }*/

                ReadGIFData(reader);
                Console.WriteLine("Loaded Video Frame!");
                //reader.Basereader.Position = FilePos + 6;
                endOffset = (int)reader.Pos;
            }

            uint ReadCode(Reader reader, int codeSize)
            {
                if (blockLength == 0)
                    blockLength = reader.ReadByte();

                while (bitCacheLength <= codeSize && blockLength > 0)
                {
                    uint bytedata = reader.ReadByte();
                    (blockLength)--;
                    bitCache |= (int)(bytedata << bitCacheLength);
                    bitCacheLength += 8;

                    if (blockLength == 0)
                    {
                        blockLength = reader.ReadByte();
                    }
                }
                uint result = (uint)(bitCache & ((1 << codeSize) - 1));
                bitCache >>= codeSize;
                bitCacheLength -= codeSize;

                return result;
            }

            /*public void ReadGIFData(Reader reader)
            {
                //I have no idea how to load GIF data

                CompressedImageData = new byte[Width * Height + 1];

                byte bitSize = reader.ReadByte();
                ExtendedCodeTable = bitSize == 8;

                bool notEnd = true;

                int c = 0;

                while (notEnd)
                {
                    byte BlockSize = reader.ReadByte();
                    byte clearCode = reader.ReadByte(); //just read the clearcode

                    if (BlockSize == 0)
                    {
                        break;
                    }

                    if (!ExtendedCodeTable)
                    {

                        while ((c = reader.ReadByte()) != EndCode)
                        {

                            if (c == ClearCode)
                            {
                                //do clearcode shit

                                while ((c = reader.ReadByte()) == ClearCode)
                                {
                                    //skip
                                }

                                if (c == EndCode)
                                {
                                    notEnd = false;
                                    break;
                                }

                            }
                            else
                            {
                                //Process data
                            }
                        }
                    }
                    else
                    {
                        Console.WriteLine("wtf no");
                        break;
                        //gonna haveta do special stuff for 256 colour images
                    }
                }
            }*/

            struct Entry
            {
                public byte Used;
                public ushort Length;
                public ushort Prefix;
                public byte Suffix;
            };

            public void ReadGIFData(Reader reader)
            {
                int eighthHeight = Height >> 3;
                int quarterHeight = Height >> 2;
                int halfHeight = Height >> 1;
                int bitsWidth = 0;
                int width = 0;

                Entry[] codeTable = new Entry[0x10000];
                ImageData = new byte[Width * Height + 1];

                for (int i = 0; i < 0x10000; i++)
                {
                    codeTable[i] = new Entry();
                }

                // Get frame
                byte type, subtype, temp;
                type = reader.ReadByte();

                while (type != 0)
                {
                    bool tableFull, interlaced;
                    int codeSize, initCodeSize;
                    int clearCode, eoiCode, emptyCode;
                    int blockLength, bitCache, bitCacheLength;
                    int codeToAddFrom, mark, str_len = 0, frm_off = 0;
                    uint currentCode;

                    switch (type)
                    {
                        // Extension
                        case 0x21:
                            subtype = reader.ReadByte();
                            switch (subtype)
                            {
                                // Graphics Control Extension
                                case 0xF9:
                                    reader.ReadBytes(0x06);
                                    // temp = reader.ReadByte();  // Block Size [byte] (always 0x04)
                                    // temp = reader.ReadByte();  // Packed Field [byte] //
                                    // temp16 = reader.ReadUInt16(); // Delay Time [short] //
                                    // temp = reader.ReadByte();  // Transparent Color Index? [byte] //
                                    // temp = reader.ReadByte();  // Block Terminator [byte] //
                                    break;
                                // Plain Text Extension
                                case 0x01:
                                // Comment Extension
                                case 0xFE:
                                // Application Extension
                                case 0xFF:
                                    temp = reader.ReadByte(); // Block Size
                                                               // Continue until we run out of blocks
                                    while (temp != 0)
                                    {
                                        // Read block
                                        reader.ReadBytes(temp);
                                        temp = reader.ReadByte(); // next block Size
                                    }
                                    break;
                                default:
                                    Console.WriteLine("GIF LOAD FAILED");
                                    return;
                            }
                            break;
                        // Image descriptor
                        case 0x2C:
                            // temp16 = reader.ReadUInt16(); // Destination X
                            // temp16 = reader.ReadUInt16(); // Destination Y
                            // temp16 = reader.ReadUInt16(); // Destination Width
                            // temp16 = reader.ReadUInt16(); // Destination Height
                            reader.ReadBytes(8);
                            temp = reader.ReadByte();    // Packed Field [byte]

                            // If a local color table exists,
                            if ((temp & 0x80) != 0)
                            {
                                int size = 2 << (temp & 0x07);
                                // Load all colors
                                reader.ReadBytes(3 * size); // reader.ReadBytesTo(buffer, 3 * size);
                            }

                            interlaced = (temp & 0x40) == 0x40;
                            if (interlaced)
                            {
                                bitsWidth = 0;
                                while (width != 0)
                                {
                                    width >>= 1;
                                    bitsWidth++;
                                }
                                width = Width - 1;
                            }

                            codeSize = reader.ReadByte();

                            clearCode = 1 << codeSize;
                            eoiCode = clearCode + 1;
                            emptyCode = eoiCode + 1;

                            codeSize++;
                            initCodeSize = codeSize;

                            // Init table
                            for (int i = 0; i <= eoiCode; i++)
                            {
                                codeTable[i].Length = 1;
                                codeTable[i].Prefix = 0xFFF;
                                codeTable[i].Suffix = (byte)i;
                            }

                            blockLength = 0;
                            bitCache = 0b00000000;
                            bitCacheLength = 0;
                            tableFull = false;

                            currentCode = ReadCode(reader, codeSize);

                            codeSize = initCodeSize;
                            emptyCode = eoiCode + 1;
                            tableFull = false;

                            Entry entry = new Entry();
                            entry.Suffix = 0;

                            while (blockLength != 0)
                            {
                                codeToAddFrom = -1;
                                mark = 0;

                                if (currentCode == clearCode)
                                {
                                    codeSize = initCodeSize;
                                    emptyCode = eoiCode + 1;
                                    tableFull = false;
                                }
                                else if (!tableFull)
                                {
                                    codeTable[emptyCode].Length = (ushort)(str_len + 1);
                                    codeTable[emptyCode].Prefix = (ushort)currentCode;
                                    codeTable[emptyCode].Suffix = entry.Suffix;
                                    emptyCode++;

                                    // Once we reach highest code, increase code size
                                    if ((emptyCode & (emptyCode - 1)) == 0)
                                        mark = 1;
                                    else
                                        mark = 0;

                                    if (emptyCode >= 0x1000)
                                    {
                                        mark = 0;
                                        tableFull = true;
                                    }
                                }

                                currentCode = ReadCode(reader, codeSize);

                                if (currentCode == clearCode) continue;
                                if (currentCode == eoiCode) return;
                                if (mark == 1) codeSize++;

                                entry = codeTable[currentCode];
                                str_len = entry.Length;

                                while (true)
                                {
                                    int p = frm_off + entry.Length - 1;
                                    if (interlaced)
                                    {
                                        int row = p >> bitsWidth;
                                        if (row < eighthHeight)
                                            p = (p & width) + ((((row) << 3) + 0) << bitsWidth);
                                        else if (row < quarterHeight)
                                            p = (p & width) + ((((row - eighthHeight) << 3) + 4) << bitsWidth);
                                        else if (row < halfHeight)
                                            p = (p & width) + ((((row - quarterHeight) << 2) + 2) << bitsWidth);
                                        else
                                            p = (p & width) + ((((row - halfHeight) << 1) + 1) << bitsWidth);
                                    }

                                    ImageData[p] = entry.Suffix;
                                    if (entry.Prefix != 0xFFF)
                                        entry = codeTable[entry.Prefix];
                                    else
                                        break;
                                }
                                frm_off += str_len;
                                if (currentCode < emptyCode - 1 && !tableFull)
                                    codeTable[emptyCode - 1].Suffix = entry.Suffix;
                            }
                            break;
                    }

                    type = reader.ReadByte();

                    if (type == 0x3B) break;
                }
            }

        }
        public Video(string filepath) : this(new Reader(filepath))
        {

        }

        public Video(System.IO.Stream stream) : this(new Reader(stream))
        {

        }

        public Video(Reader reader)
        {
            VideoInfo = reader.ReadUInt16();

            Width = reader.ReadUInt16();

            Height = reader.ReadUInt16();

            Console.WriteLine("VideoInfo = " + VideoInfo);
            Console.WriteLine("Width = " + Width);
            Console.WriteLine("Height = " + Height);

            //GET IMAGE DATA LOADING
            for (int f = 0; f < VideoInfo; f++)
            {
                LoadVideoFrame(reader);
                Bitmap b = new Bitmap(Frames[f].Width, Frames[f].Height);

                for (int i = 0; i < Frames[f].Height; i++)
                {
                    for (int ii = 0; ii < Frames[f].Width; ii++)
                    {
                        Color c = new Color();
                        c = Frames[f].FramePalette[Frames[f].ImageData[(i * Frames[f].Width) + ii]];
                        b.SetPixel(ii, i, c);
                    }
                }
                b.Save("Frames/Frame" + f + ".gif", System.Drawing.Imaging.ImageFormat.Gif);
            }

            Console.WriteLine("Reader Position = " + reader.Pos + " FileSize = " + reader.BaseStream.Length + " Data Left = " + (reader.BaseStream.Length - reader.Pos));
            reader.Close();
        }

        void LoadVideoFrame(Reader reader)
        {
            Frames.Add(new VideoFrame(reader, Width, Height));
        }

        public void Write(string filename)
        {
            using (Writer writer = new Writer(filename))
                this.Write(writer);
        }

        public void Write(System.IO.Stream reader)
        {
            using (Writer writer = new Writer(reader))
                this.Write(writer);
        }

        internal void Write(Writer writer)
        {

            writer.Close();
        }

    }
}
