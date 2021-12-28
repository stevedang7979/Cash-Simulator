#include "bits.h"
#include "cache.h"

int get_set(Cache *cache, address_type address) {
  // TODO:
  //  Extract the set bits from a 32-bit address.
  //
	int set_bits = cache->set_bits;
	int block_bits = cache->block_bits;
	int tag_bits= 32-set_bits-block_bits;
	address= (address << tag_bits) >> (32-set_bits);
  return address;
}

int get_line(Cache *cache, address_type address) {
  // TODO:
  // Extract the tag bits from a 32-bit address.
  //
	int set_bits = cache->set_bits;
        int block_bits = cache->block_bits;
        int tag_bits= 32-set_bits-block_bits;
        address= (address >> (32-tag_bits));
  return address;
}

int get_byte(Cache *cache, address_type address) {
  // TODO
  // Extract the block offset (byte index) bits from a 32-bit address.
  //
	int set_bits = cache->set_bits;
        int block_bits = cache->block_bits;
        int tag_bits= 32-set_bits-block_bits;
        address= (address << (tag_bits + set_bits)) >> (32 - block_bits);
  return address;
}
