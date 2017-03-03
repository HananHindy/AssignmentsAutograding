/* See COPYRIGHT for copyright information. */
#ifndef FOS_KERN_TESTS_H
#define FOS_KERN_TESTS_H
#ifndef FOS_KERNEL
# error "This is a FOS kernel header; user programs should not #include it"
#endif

#include <kern/command_prompt.h>
#include <inc/stdio.h>
#include <inc/string.h>
#include <inc/memlayout.h>
#include <inc/assert.h>
#include <inc/x86.h>

void TestAssignment3();
int TestAss3Q1();
int TestAss3Q2();
int TestAss3Q3();
int TestAss3Q4();
int TestAss3B1();
int TestAss3B2();
int CheckBit(uint32 va, int bitIndex);

#endif /* !FOS_KERN_TESTS_H */
