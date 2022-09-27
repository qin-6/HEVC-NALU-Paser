#ifndef H265_NALU_FINDER_HPP
#define H265_NALU_FINDER_HPP

#include <stdio.h>
#include <vector>
#include <stdint.h>
using namespace std;

enum NaluType : uint8_t {
  kTrailN = 0,
  kTrailR = 1,
  kTsaN = 2,
  kTsaR = 3,
  kStsaN = 4,
  kStsaR = 5,
  kRadlN = 6,
  kRadlR = 7,
  kBlaWLp = 16,
  kBlaWRadl = 17,
  kBlaNLp = 18,
  kIdrWRadl = 19,
  kIdrNLp = 20,
  kCra = 21,
  kRsvIrapVcl23 = 23,
  kVps = 32,
  kSps = 33,
  kPps = 34,
  kAud = 35,
  kPrefixSei = 39,
  kSuffixSei = 40,
  kAP = 48,
  kFU = 49
};

typedef struct NaluIndex {
  // Start index of NALU, including start sequence.
  size_t start_offset;
  // Start index of NALU payload, typically type header.
  size_t payload_start_offset;
  // Length of NALU payload, in bytes, counting from payload_start_offset.
  size_t payload_size;
};


std::vector<NaluIndex> FindNaluIndices(const uint8_t* buffer, size_t buffer_size);
NaluType ParseNaluType(uint8_t data);

#endif