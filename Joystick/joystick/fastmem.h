/*
 * fastmem.h
 *
 * Fast versions of memset() and memcpy() that use unrolled loops, acheiving
 * maximum possible speed on the AVR8 architecture.
 */


#ifndef FASTMEM_H_
#define FASTMEM_H_

#include <string.h>

#ifndef __OPTIMIZE__
# warning "Compiler optimizations disabled; functions from fastmem.h won't work as designed"
#endif

#define MAX_UNROLL	511		// Limits maximum iterations to be unrolled before falling back
							// to avr-libc functions. Cannot be more than 511 unless you add
							// additional conditions below



static inline void * fmemset(void *str, uint16_t c, size_t n)
{
	if (n > MAX_UNROLL)
		return memset(str, c, n);

	void *p = str;

	typedef struct {
		char b[n];
	} block_t;

	block_t *blk_p = (block_t *)str;


#define fm_ms1		"st		%a[ptr]+, %[cb]"		"\n\t"
#define fm_ms8		fm_ms1 fm_ms1 fm_ms1 fm_ms1 fm_ms1 fm_ms1 fm_ms1 fm_ms1
#define fm_msz1		"st		%a[ptr]+, __zero_reg__"	"\n\t"
#define fm_msz8		fm_msz1 fm_msz1 fm_msz1 fm_msz1 fm_msz1 fm_msz1 fm_msz1 fm_msz1


	if (c == 0)
	{
		if (n >= 256)
		{
			asm volatile(
				fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8
				fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8
				fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8
				fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8
			: [ptr] "+e" (p), "=m" (*blk_p)
			);
			n -= 256;
		}

		if (n >= 128)
		{
			asm volatile(
				fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8
				fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8
			: [ptr] "+e" (p), "=m" (*blk_p)
			);
			n -= 128;
		}

		if (n >= 64)
		{
			asm volatile(
				fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8 fm_msz8
			: [ptr] "+e" (p), "=m" (*blk_p)
			);
			n -= 64;
		}

		if (n >= 32)
		{
			asm volatile(
				fm_msz8 fm_msz8 fm_msz8 fm_msz8
			: [ptr] "+e" (p), "=m" (*blk_p)
			);
			n -= 32;
		}

		if (n >= 16)
		{
			asm volatile(
				fm_msz8 fm_msz8
			: [ptr] "+e" (p), "=m" (*blk_p)
			);
			n -= 16;
		}

		if (n >= 8)
		{
			asm volatile(
				fm_msz8
			: [ptr] "+e" (p), "=m" (*blk_p)
			);
			n -= 8;
		}

		if (n >= 4)
		{
			asm volatile(
				fm_msz1 fm_msz1 fm_msz1 fm_msz1
			: [ptr] "+e" (p), "=m" (*blk_p)
			);
			n -= 4;
		}

		if (n >= 2)
		{
			asm volatile(
				fm_msz1 fm_msz1
			: [ptr] "+e" (p), "=m" (*blk_p)
			);
			n -= 2;
		}

		if (n >= 1)
		{
			asm volatile(
				fm_msz1
			: [ptr] "+e" (p), "=m" (*blk_p)
			);
			n -= 1;
		}

		return str;
	}

	// c != 0
	if (n >= 256)
	{
		asm volatile(
			fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8
			fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8
			fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8
			fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8
		: [ptr] "+e" (p), "=m" (*blk_p)
		: [cb] "r" ((unsigned char)c)
		);
		n -= 256;
	}

	if (n >= 128)
	{
		asm volatile(
			fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8
			fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8
		: [ptr] "+e" (p), "=m" (*blk_p)
		: [cb] "r" ((unsigned char)c)
		);
		n -= 128;
	}

	if (n >= 64)
	{
		asm volatile(
			fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8 fm_ms8
		: [ptr] "+e" (p), "=m" (*blk_p)
		: [cb] "r" ((unsigned char)c)
		);
		n -= 64;
	}

	if (n >= 32)
	{
		asm volatile(
			fm_ms8 fm_ms8 fm_ms8 fm_ms8
		: [ptr] "+e" (p), "=m" (*blk_p)
		: [cb] "r" ((unsigned char)c)
		);
		n -= 32;
	}

	if (n >= 16)
	{
		asm volatile(
			fm_ms8 fm_ms8
		: [ptr] "+e" (p), "=m" (*blk_p)
		: [cb] "r" ((unsigned char)c)
		);
		n -= 16;
	}

	if (n >= 8)
	{
		asm volatile(
			fm_ms8
		: [ptr] "+e" (p), "=m" (*blk_p)
		: [cb] "r" ((unsigned char)c)
		);
		n -= 8;
	}

	if (n >= 4)
	{
		asm volatile(
			fm_ms1 fm_ms1 fm_ms1 fm_ms1
		: [ptr] "+e" (p), "=m" (*blk_p)
		: [cb] "r" ((unsigned char)c)
		);
		n -= 4;
	}

	if (n >= 2)
	{
		asm volatile(
			fm_ms1 fm_ms1
		: [ptr] "+e" (p), "=m" (*blk_p)
		: [cb] "r" ((unsigned char)c)
		);
		n -= 2;
	}

	if (n >= 1)
	{
		asm volatile(
			fm_ms1
		: [ptr] "+e" (p), "=m" (*blk_p)
		: [cb] "r" ((unsigned char)c)
		);
		n -= 1;
	}

	return str;

#undef fm_ms1
#undef fm_ms8
#undef fm_msz1
#undef fm_msz8
}

static inline void * fmemcpy(void *dest, const void *src, size_t n)
{
	if (n > MAX_UNROLL)
		return memcpy(dest, src, n);

	void *pd = dest;
	void *ps = (void *)src;

	typedef struct {
		char b[n];
	} block_t;

	block_t *blk_p = (block_t *)dest;


#define fm_cp1		"ld		__tmp_reg__, %a[src]+"	"\n\t" \
					"st		%a[dst]+, __tmp_reg__"	"\n\t"
#define fm_cp8		fm_cp1 fm_cp1 fm_cp1 fm_cp1 fm_cp1 fm_cp1 fm_cp1 fm_cp1


	if (n >= 256)
	{
		asm volatile(
			fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8
			fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8
			fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8
			fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8
		: [src] "+e" (ps), [dst] "+e" (pd), "=m" (*blk_p)
		);
		n -= 256;
	}

	if (n >= 128)
	{
		asm volatile(
			fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8
			fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8
		: [src] "+e" (ps), [dst] "+e" (pd), "=m" (*blk_p)
		);
		n -= 128;
	}

	if (n >= 64)
	{
		asm volatile(
			fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8 fm_cp8
		: [src] "+e" (ps), [dst] "+e" (pd), "=m" (*blk_p)
		);
		n -= 64;
	}

	if (n >= 32)
	{
		asm volatile(
			fm_cp8 fm_cp8 fm_cp8 fm_cp8
		: [src] "+e" (ps), [dst] "+e" (pd), "=m" (*blk_p)
		);
		n -= 32;
	}

	if (n >= 16)
	{
		asm volatile(
			fm_cp8 fm_cp8
		: [src] "+e" (ps), [dst] "+e" (pd), "=m" (*blk_p)
		);
		n -= 16;
	}

	if (n >= 8)
	{
		asm volatile(
			fm_cp8
		: [src] "+e" (ps), [dst] "+e" (pd), "=m" (*blk_p)
		);
		n -= 8;
	}

	if (n >= 4)
	{
		asm volatile(
			fm_cp1 fm_cp1 fm_cp1 fm_cp1
		: [src] "+e" (ps), [dst] "+e" (pd), "=m" (*blk_p)
		);
		n -= 4;
	}

	if (n >= 2)
	{
		asm volatile(
			fm_cp1 fm_cp1
		: [src] "+e" (ps), [dst] "+e" (pd), "=m" (*blk_p)
		);
		n -= 2;
	}

	if (n >= 1)
	{
		asm volatile(
			fm_cp1
		: [src] "+e" (ps), [dst] "+e" (pd), "=m" (*blk_p)
		);
		n -= 1;
	}

	return dest;

#undef fm_cp1
#undef fm_cp8
}



#endif /* FASTMEM_H_ */