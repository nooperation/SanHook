
using BitReaderPlayground;

class Program
{
    static void Main()
    {
        //   moveRight = (12) bits
        //   moveForward = (12) bits
        //   cameraYaw = (13) bits
        //   cameraPitch = (13) bits
        //   behaviorYawDelta = (11) bits
        //   behaviorPitchDelta = (11) bits
        //   characterForward = (15) bits
        //   cameraForward = (28) bits

        foreach (var bytes in CharacterControllerInputPayloads.SpinAroundData)
        {
            var bitReader = new BitReader(bytes);

            var moveRight = bitReader.ReadFloat(12);
            var moveForward = bitReader.ReadFloat(12);
            var cameraYaw = bitReader.ReadFloat(13);
            var cameraPitch = bitReader.ReadFloat(13);
            var behaviorYawDelta = bitReader.ReadFloat(11);
            var behaviorPitchDelta = bitReader.ReadFloat(11);
            var characterForward = bitReader.ReadFloat(15);
            var cameraForward = bitReader.ReadFloat(28);

            //Console.WriteLine($"moveRight = {moveRight}");
            //Console.WriteLine($"moveForward = {moveForward}");
            Console.WriteLine($"cameraYaw = {cameraYaw}");
            //Console.WriteLine($"cameraPitch = {cameraPitch}");
            //Console.WriteLine($"behaviorYawDelta = {behaviorYawDelta}");
            //Console.WriteLine($"behaviorPitchDelta = {behaviorPitchDelta}");
            //Console.WriteLine($"characterForward = {characterForward}");
            //Console.WriteLine($"cameraForward = {cameraForward}");
            // 2F FA -> [101111] 1111[1010]
            // 1011111010
            // break;
        }
    }
}