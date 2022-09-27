#include "h265_nalu_finder.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include "cio.h"
#include "nal_sps.h"
#include "nal_pps.h"
#include "nal_vps.h"
#include <string.h>
using namespace std;

#define buf_size 1024 * 1024
int main()
{
    const char *path = "./stream/video0.h265";
    FILE *fd;
    uint8_t buf[buf_size];
    uint32_t len = 0;
    fd = fopen(path, "rb");   
    if(fd == NULL)
    {
        cout << "open file failed" << endl;
    } 
    len = fread(buf, 1, buf_size, fd);
    cout << "read success len :" << len << endl;

    std::vector<NaluIndex> nalus;
    nalus =  FindNaluIndices(buf, len);
    for(int i = 0; i < nalus.size(); i ++)
    {
        NaluType naluType = ParseNaluType(buf[nalus[i].payload_start_offset]);
        std::string naluName;
        nal_buffer_t nalu;
        memset(&nalu, 0, sizeof(nal_buffer_t));
        nalu.posmax = nalus[i].payload_size;
        memcpy(nalu.data, &buf[nalus[i].payload_start_offset], nalus[i].payload_size);
        switch(naluType)
        {
            case kVps:
                naluName = "VPS";
                cout << "Nalu Type :" << naluName << endl;
                print_nal_vps_parse(&nalu);  
                break;
            case kPps:
                naluName = "PPS";
                cout << "Nalu Type :" << naluName << endl;
                print_nal_pps_parse(&nalu);  
                break;
            case kSps:
                naluName = "SPS";  
                cout << "Nalu Type :" << naluName << endl;             
                print_nal_sps_parse(&nalu);
                break;
            default:
                break;
        }
    }
    
    return 0;
}