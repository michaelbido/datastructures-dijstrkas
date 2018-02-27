# *********************************************************************
#        CLASS:  CSCE 3110
#   ASSIGNMENT:  Program 4
#         NAME:  Michael Bido-Chavez (euid: mb0501)
#
# DESCRICPTION:  This is the makefile for the following source file(s).
#                type "make" to compile
#
# REQUIRED SOURCE FILE(S):
#    program4_Bido-Chavez.cpp
#
# *********************************************************************
CC=g++
#CFLAGS=-Wall -g
prog: program4_Bido-Chavez.cpp
	$(CC) -o greed.out -g program4_Bido-Chavez.cpp
run:
	./greed.out
clean:
	$(RM) greed.out
