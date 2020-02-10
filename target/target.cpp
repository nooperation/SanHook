#include <winsock2.h>


#include <iostream>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")


const char hex_lookup[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

void DumpPacket(const char *buff, int len)
{
    if(len <= 0)
    {
        return;
    }

    printf("DumpPacket - %d\n", len);

    char output_buffer[65536 * 3 + 1];
    std::size_t output_index = 0;
    // output_buffer[0] = 0;

    for(int i = 0; i < len; ++i)
    {
        auto low = ((uint8_t)buff[i] & 0xF0) >> 4;
        auto high = (uint8_t)buff[i] & 0x0F;

        output_buffer[output_index++] = hex_lookup[low];
        output_buffer[output_index++] = hex_lookup[high];
        output_buffer[output_index++] = ' ';

        /*
        char temp[16];
        sprintf_s(temp, "%02X ", (uint8_t)buff[i]);
        strcat_s(output_buffer, temp);
        */
    }
    output_buffer[output_index++] = 0;

    if(!output_buffer)
    {
        printf("Bad output buffer\n");
    }
    else
    {
        printf("[%d] %s\n", len, output_buffer);
    }
}

int main() {
	
  //  printf("Waiting...\n");

	//auto x = getchar();
	//Beep(100, 250);

   // sockaddr from = {};
    //int fromLength;
   // char buff[128] = {};

   // auto result = recvfrom(0xBEEF, buff, sizeof(buff), 123, &from, &fromLength);

    /*
    SOCKET sock;
    
    send(123, "asdf", 4, 11);

    char buff[128] = {};
    recvfrom(0, buff, 128, 0, nullptr, nullptr);
    printf("Buff = %s\n", buff);

    */
    std::string packet = "Hello world";
    DumpPacket(packet.c_str(), packet.length());

	//printf("recvfrom returned %d with buffer: '%s'", result, buff);
	return 0;
}
