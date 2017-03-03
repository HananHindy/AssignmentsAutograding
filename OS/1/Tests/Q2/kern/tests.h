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

void TestAssignment1();
int TestAss1Q1();
int TestAss1Q2();
int TestAss1Q3();

#endif /* !FOS_KERN_TESTS_H */
