using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

/* EXAMPLE INPUT
00007FF610579998  00000000AB406734  
00007FF6105799A0  00007FF610416300  "AgentControllerMessages::ControlPoint"
00007FF6105799A8  0000000000000040  
00007FF6105799B0  0000000000000001  
00007FF6105799B8  00007FF60D610620  
00007FF6105799C0  0000000000000000  
00007FF6105799C8  000000002DF35CF3  
00007FF6105799D0  0000000000000000  
00007FF6105799D8  4190000041C00000  
00007FF6105799E0  6515D3F275C0AC6B  
00007FF6105799E8  0000000064E3BD79  
00007FF6105799F0  00007FF610430AB8  "AgentControllerMessages::WarpCharacter"
00007FF6105799F8  0000000000000040  
00007FF610579A00  0000000000000001  
00007FF610579A08  00007FF60D610690  sansarclient.sub_7FF60D610690
00007FF610579A10  0000000000000000  
00007FF610579A18  0000000075C0AC6B  
00007FF610579A20  0000000000000000  
00007FF610579A28  4210000042100000  
00007FF610579A30  6515D3F225C093E0  
00007FF610579A38  000000005F292BE3  
00007FF610579A40  00007FF610430A88  "AgentControllerMessages::RequestWarpCharacter"
00007FF610579A48  0000000000000030  
00007FF610579A50  0000000000000001  
00007FF610579A58  00007FF60D610700  sansarclient.sub_7FF60D610700
00007FF610579A60  0000000000000000  
00007FF610579A68  0000000025C093E0  
00007FF610579A70  0000000000000000  
00007FF610579A78  FFFFFFFFFFFFFFFE  
00007FF610579A80  6515D3F2FCA3EF20  
00007FF610579A88  000000005B3687E6  
*/

namespace PacketOffsetsParser
{
    class Program
    {
        class ParserData
        {
            public string Name { get; set; }
            public long SizeAligned { get; set; }
            public long Version { get; set; }
            public ulong Offset { get; set; }
            public long Value3 { get; set; }
            public ulong MessageId { get; set; }

            public override string ToString()
            {
                return string.Format($"{Name},{SizeAligned},{Version},0x{Offset:X},{Value3},0x{MessageId:X}");
            }
        }


        static void Main(string[] args)
        {
            Regex patternPacketName = new Regex("[0-9A-Z]{16}  (?<value>[0-9A-Z]{16})  (?<dump>.{8})  \\\"(?<name>[a-zA-Z]+::[a-zA-Z0-9]+)\\\"");
            Regex patternValueType = new Regex("(?<address>[0-9A-Z]{16})  (?<value>[0-9A-Z]{16})  (?<dump>.{8})  ");
            Regex patternSmallValueType = new Regex("[0-9A-Z]{16}  000000000000(?<value>[0-9A-Z]{4})  (?<dump>.{8})  ");
            Regex patternParserOffset = new Regex("[0-9A-Z]{16}  (?<value>[0-9A-Z]{16})  (?<dump>.{8})  ");

            List<ParserData> parsers = new List<ParserData>();

            Console.WriteLine("Address of first line of client code:");
            var codeAddressLine = Console.ReadLine();
            var codeAddress = ulong.Parse(codeAddressLine, System.Globalization.NumberStyles.HexNumber);

            Console.WriteLine("File offset of first line of client code:");
            var fileOffsetLine = Console.ReadLine();
            var fileOffset = ulong.Parse(fileOffsetLine, System.Globalization.NumberStyles.HexNumber);

            var totalOffset = codeAddress - fileOffset;

            Console.WriteLine("Input memory dump:");

            while (true)
            {
                var line = Console.ReadLine();
                if(line.Length == 0)
                {
                    break;
                }

                var matchPacketName = patternPacketName.Match(line);
                if(matchPacketName.Success)
                {
                    var packetName = matchPacketName.Groups["name"].Value;

                    var sizeAlignedLine = Console.ReadLine();
                    var sizeAlignedMatch = patternSmallValueType.Match(sizeAlignedLine);
                    if(!sizeAlignedMatch.Success)
                    {
                        continue;
                    }
                    var sizeAligned = long.Parse(sizeAlignedMatch.Groups["value"].Value, System.Globalization.NumberStyles.HexNumber);

                    var versionLine = Console.ReadLine();
                    var matchVersion = patternSmallValueType.Match(versionLine);
                    if (!matchVersion.Success)
                    {
                        continue;
                    }
                    var version = long.Parse(matchVersion.Groups["value"].Value, System.Globalization.NumberStyles.HexNumber);

                    var nextLineParserOffset = Console.ReadLine();
                    var matchParserOffset = patternParserOffset.Match(nextLineParserOffset);
                    if (!matchParserOffset.Success)
                    {
                        continue;
                    }
                    var offset = ulong.Parse(matchParserOffset.Groups["value"].Value, System.Globalization.NumberStyles.HexNumber);
                    if(offset < codeAddress)
                    {
                        continue;
                    }

                    offset -= totalOffset;

                    var nextLineValue3 = Console.ReadLine();
                    var matchValue3 = patternValueType.Match(nextLineValue3);
                    if (!matchValue3.Success)
                    {
                        continue;
                    }
                    var value3 = long.Parse(matchValue3.Groups["value"].Value, System.Globalization.NumberStyles.HexNumber);

                    var nextLineMessageId = Console.ReadLine();
                    var matchMessageId = patternValueType.Match(nextLineMessageId);
                    if (!matchMessageId.Success)
                    {
                        continue;
                    }

                    var messageId = ulong.Parse(matchMessageId.Groups["value"].Value, System.Globalization.NumberStyles.HexNumber);
                    if(messageId == 0)
                    {
                        continue;
                    }

                    parsers.Add(new ParserData() { 
                        Name = packetName,
                        SizeAligned = sizeAligned,
                        Version = version,
                        Offset = offset,
                        Value3 = value3,
                        MessageId = messageId
                    });
                }
            }

            Console.WriteLine("-----------------------------");
            Console.WriteLine("name,sizeAligned,version,offset,value3,messageId");
            foreach (var item in parsers)
            {
                Console.WriteLine(item);
            }
        }
    }
}
