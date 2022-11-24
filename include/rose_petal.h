#ifndef __ROSE_PETAL_H_
#define __ROSE_PETAL_H_

/* 	************************************
 * 	* ~The Rose Petal Memory Debugger~ *
 * 	************************************
 *
 * 	This is just a memory debugger I made so I can better keep track of memory
 * 	throughout a program's lifetime. It's really small for the time being,
 * 	but I'll probably extend it at some point, idk.
 *
 * 	Anyways, in order to use this, make sure that you write
 * 	"#define ROSE_PETAL_IMPL"
 * 	before you include the "rose_pedal.h" header file, otherwise none of the
 * 	functions will exist and you'll just be wasting your damn time. lmao
*/

#include "stddef.h"

void rp_memory_init(void);
void *rp_malloc(const size_t size, const size_t line, const char *file);
void *rp_realloc(void *ptr, const size_t size,
		const size_t line, const char *file);
void rp_free(void *pointer, const size_t line, const char *file);
void rp_memory_print(void);
void rp_memory_terminate(void);

#ifdef ROSE_PETAL_IMPL

#define RP_PREFIX "ROSE PETAL: "

#include <malloc.h>
#include <assert.h>

typedef struct {
	void *data;
	size_t alloc_size, line;
	const char *file;
} RPData;

static size_t rp_alloc_count = 0;
static size_t rp_free_count = 0;
static size_t rp_data_size = 0;
static RPData *rp_data;

void rp_memory_init(void) {
	assert(!rp_data);
	rp_data = malloc(0);
}

void *rp_malloc(const size_t size, const size_t line, const char *file) {
	assert(rp_data);

	rp_alloc_count++;
	rp_data = realloc(rp_data, rp_data_size +
			rp_alloc_count * sizeof(RPData));
	size_t index = rp_alloc_count - 1;
	rp_data[index] = (RPData){malloc(size), size, line, file};
	rp_data_size += size;
	printf("%sMalloced %lu bytes at %p in %s at line %lu. [%lu]\n", 
			RP_PREFIX, size, rp_data[index].data, file, line, index);

	return rp_data[index].data;
}

void *rp_realloc(void *ptr, const size_t size,
		const size_t line, const char *file) {
	assert(rp_data);
	assert(ptr);
	for(size_t i = 0; i < rp_alloc_count; i++) {
		if(ptr == rp_data[i].data) {
			rp_data_size -= rp_data[i].alloc_size;
			rp_data_size += size;
			printf("%sRealloced %lu bytes at %p to %lu in %s at"
					"line %lu. [%lu]\n", RP_PREFIX, rp_data[i].alloc_size,
					rp_data[i].data, size, file, line, i);
			rp_data[i].data = realloc(ptr, size);
			rp_data[i].alloc_size = size;

			return rp_data[i].data;
		}
	}

	assert(0);
}

void rp_free(void *pointer, const size_t line, const char *file) {
	assert(pointer);
	for(size_t i = 0; i < rp_alloc_count; i++) {
		if(pointer == rp_data[i].data) {
			printf("%sFreed %lu bytes at %p in %s at line %lu. [%lu]\n",
					RP_PREFIX, rp_data[i].alloc_size, rp_data[i].data, file,
					line, i);
			free(rp_data[i].data);
			rp_data[i].data = NULL;
			rp_free_count++;
			return;
		}
	}
}

void rp_memory_print(void) {
	printf("\nROSE PETAL MEMORY TALLY:\n");
	printf("Total malloc calls: %lu\nTotal free calls: %lu\n",
			rp_alloc_count, rp_free_count);
	if(rp_alloc_count == rp_free_count) {
		printf("GOOD JOB!\n");
	} else {
		printf("YOU FUCKED UP!\nYou forgot to free...\n");
		for(size_t i = 0; i < rp_alloc_count; i++) {
			if(rp_data[i].data) {
				printf("Pointer %p (alloced in %s at line %lu). [%lu]\n",
						rp_data[i].data, rp_data[i].file, rp_data[i].line, i);
			}
		}
	}
}

void rp_memory_terminate(void) {
	free(rp_data);
	rp_data = NULL;
	rp_alloc_count = 0;
	rp_free_count = 0;
	rp_data_size = 0;
}

#endif

#define malloc(x) rp_malloc(x, __LINE__, __FILE__)
#define realloc(x, y) rp_realloc(x, y, __LINE__, __FILE__)
#define free(x) rp_free(x, __LINE__, __FILE__)

#endif
