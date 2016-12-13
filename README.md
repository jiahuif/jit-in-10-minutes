JIT in 10 Minutes
=============

Introduction
-------------
This repository introduces a simple language called +1s and makes a JIT runtime for it. 

Prerequisites
-------------
No knowledge in Compiler Design course is assumed. However you will need some basic awareness of assembly language to understand how machine code is generated. Some experience of programming in Unix like operating systems is required.

You will need a POSIX-compatible x86-64 (a.k.a. amd64) operating system. All popular Linux distributions or macOS on a recent apple computer should work. All code in this repository depends on x86-64 architecture, especially the registers. Due to the significant difference in function call conventions between Windows and POSIX-compatible operating systems, it is going to be costly to make the same piece of code run under Windows. 

Executable Code in Memory
-------------
To make our system secure, the operating system generally allow code execution only in certain memory pages. To change the access control flag, or canonically `protection flag` ( see `mprotect(2)` ).

The +1s Language
-------------
We need to create a simple language for this experiment. We want this language to be as simple as possible to avoid creating syntax or lexical analyzer, since they are not the point of this repository. Let's name our little funny language +1s, a language for the time accumulator.

At the beginning of our program, we set the global accumulator to zero. For each `+1s` statement, we increase our accumulator by one second. For example `+1s +1s +1s +1s +1s` increases the accumulator by 5 seconds.

To accumnulate a large amount of time, we need to do some crowd-funding of time. Or in other word, for-loop. The structure of for-loop looks like this.
```
N people do
	+1s
done
```
For example if we want to repeat +1s for 5 times, another solution besides that metioned above is
```
5 people do
	+1s
done
```

