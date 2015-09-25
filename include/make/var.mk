INCLUDE=-I ./  -I $(TOPDIR)/include -I $(TOPDIR)/libc/include

C_FLAGS=-pipe  -nostdinc  -fno-builtin -fomit-frame-pointer -fno-defer-pop -Wall $(INCLUDE) -O3
AR_FLAGS= -r 
