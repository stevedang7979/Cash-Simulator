#include "cache.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "bits.h"
#include "cpu.h"
#include "lru.h"

char *make_block(int block_size) {
  // TODO:
  //   Make and initialize a block's accessed bits given the block_size.
  //
  // HINT: if it wasn't clear already, this gets put in the "accessed" field
	char *blockPtr = (char*)malloc(block_size*sizeof(char));
	for(int i =0; i<block_size; i++){
		 blockPtr[i] = NULL;
	}
	return blockPtr;
}

Line *make_lines(int line_count, int block_size) {
  // TODO:
  //   Make and initialize the lines given the line count. Then
  //   make and initialize the blocks.
  //
  //   HINT: this will be placed in the "accessed" field of a Line
	Line *linePtr = (Line*)malloc(line_count*sizeof(Line));
        for(int i =0; i<line_count; i++){
                linePtr[i].valid = NULL;
		linePtr[i].tag = NULL;
		linePtr[i].accessed = make_block(block_size);
		linePtr[i].block_size =block_size;
        }
        return linePtr;
}

Set *make_sets(int set_count, int line_count, int block_size) {
  // TODO:
  //   Make and initialize the sets given the set count. Then
  //   make and initialize the line and blocks.
  //
	Set *setPtr = (Set*)malloc(set_count*sizeof(Set));
        for(int i =0; i<set_count; i++){
                setPtr[i].lines= make_lines(line_count,block_size);
		setPtr[i].line_count= line_count;
		setPtr[i].lru_queue= NULL;
        }
        return setPtr;
}

Cache *make_cache(int set_bits, int line_count, int block_bits) {
  Cache *cache = NULL;
  // TODO:
  //   Make and initialize the cache, sets, lines, and blocks.
  //   You should use the `exp2` function to determine the
  //   set_count and block_count from the set_bits and block_bits
  //   respectively (use `man exp2` from the command line).
  //
  // ADD YOUR CODE HERE:
	cache = (Cache*)malloc(sizeof(Cache));
	cache->sets= make_sets(exp2(set_bits),line_count,exp2(block_bits));
	cache->set_count=exp2(set_bits);
	cache->line_count=line_count;
	cache->block_size=exp2(block_bits);
	cache->set_bits = set_bits;
	cache->block_bits= block_bits;
  // END TODO

  // Create LRU queues for sets:
  if (cache != NULL) {
    lru_init(cache);
  }

  return cache;
}

void delete_block(char *accessed) { free(accessed); }

void delete_lines(Line *lines, int line_count) {
  for (int i = 0; i < line_count; i++) {
    delete_block(lines[i].accessed);
  }
  free(lines);
}

void delete_sets(Set *sets, int set_count) {
  for (int i = 0; i < set_count; i++) {
    delete_lines(sets[i].lines, sets[i].line_count);
  }
  free(sets);
}

void delete_cache(Cache *cache) {
  lru_destroy(cache);
  delete_sets(cache->sets, cache->set_count);
  free(cache);
}

AccessResult cache_access(Cache *cache, TraceLine *trace_line) {
  unsigned int s = get_set(cache, trace_line->address);
  unsigned int t = get_line(cache, trace_line->address);
  unsigned int b = get_byte(cache, trace_line->address);

  // Get the set:
  Set *set = &cache->sets[s];

  // Get the line:
  LRUResult result;
  lru_fetch(set, t, &result);
  Line *line = result.line;

  // If it was a miss we will clear the accessed bits:
  if (result.access != HIT) {
    for (int i = 0; i < cache->block_size; i++) {
      line->accessed[i] = 0;
    }
  }

  // Then set the accessed byte to 1:
  line->accessed[b] = 1;

  return result.access;
}
