#include <vector>
#include "h265_nalu_finder.hpp"

const uint8_t kNaluTypeMask = 0x7E;
uint32_t kNaluShortStartSequenceSize = 3;

std::vector<NaluIndex> FindNaluIndices(const uint8_t* buffer, size_t buffer_size) {
  // This is sorta like Boyer-Moore, but with only the first optimization step:
  // given a 3-byte sequence we're looking at, if the 3rd byte isn't 1 or 0,
  // skip ahead to the next 3-byte sequence. 0s and 1s are relatively rare, so
  // this will skip the majority of reads/checks.
  std::vector<NaluIndex> sequences;


  const size_t end = buffer_size - kNaluShortStartSequenceSize;
  for (size_t i = 0; i < end;) {
    if (buffer[i + 2] > 1) {
      i += 3;
    } else if (buffer[i + 2] == 1) {
      if (buffer[i + 1] == 0 && buffer[i] == 0) {
        // We found a start sequence, now check if it was a 3 of 4 byte one.
        NaluIndex index = {i, i + 3, 0};
        if (index.start_offset > 0 && buffer[index.start_offset - 1] == 0)
          --index.start_offset;

        // Update length of previous entry.
        auto it = sequences.rbegin();
        if (it != sequences.rend())
          it->payload_size = index.start_offset - it->payload_start_offset;

        sequences.push_back(index);
      }

      i += 3;
    } else {
      ++i;
    }
  }

  // Update length of last entry, if any.
  auto it = sequences.rbegin();
  if (it != sequences.rend())
    it->payload_size = buffer_size - it->payload_start_offset;

  return sequences;
}

NaluType ParseNaluType(uint8_t data) 
{
  return static_cast<NaluType>((data & kNaluTypeMask) >> 1);
}
