#!/usr/bin/python3
#
# Author:	Eric Power
# How To:	
#			1. Change the PORT variable to your PORT
#			2. Run ./myserver (in a separate terminal)
#			3. Run this program with '-h' for more information :)
"""
More complete testing for A2 of the Spring 2021 offering of CSC 360 at UVic.
"""

# IMPORT STATEMENTS
import os
import sys
from time import sleep
from colorama import Fore
from colorama import Style

# KEY VARIABLES
PORT="12371"

# CONTROL FLOW
def main():

	task2 = False
	meetup_first = True
	count = 3

	for num, i in enumerate(sys.argv):
		if num == 0:
			continue
		if i.lower() in ['-h', '--help']:
			print_help()
			exit(0)
		if i.lower() == 't1':
			test_t1()
			exit(0)
		elif i.lower() == 't2':
			task2 = True
		elif i.lower() == 'last':
			meetup_first = False
		else:
			try:
				count = int(i)
			except:
				print_error(f"Unknown argument {i}, try running with '-h'")
				exit(1)

	if task2:
		test_t2(meetup_first, count)
		exit(0)
	else:
		print_error("pytest requires at least one argument. Run with '-h' for help.")
		exit(1)
		
def test_t1():
	"""A test for task 1 of the assignment."""
	id = 0
	for i in range(15):
		pid = os.fork() 
		if pid == 0:
			id = i
			break
		sleep(0.5)

	if id != 0:
		if id % 2 == 0:
			#os.system(f'curl "localhost:{PORT}/?op=read"')
			os.system(f'echo "READER {id}: " $(curl -s "localhost:{PORT}/?op=read")')
			exit(0)
		else:
			os.system(f'echo "WRITER {id}:" $(curl -s "localhost:{PORT}/?op=write&val=curval_is_{id}")')
			exit(0)
	else:
		while True:
			sleep(3)
			try:
				os.wait()
			except ChildProcessError:
				break
	print("-- All children have ended.")


def test_t2(mf, count):
	"""A test for task 2 of the assignment."""
	id = 0
	for i in range(15):
		pid = os.fork() 
		if pid == 0:
			id = i
			break
		sleep(0.5)

	if id != 0:
		os.system(f'echo "BOBA FETT {id}: " $(curl -s "localhost:{PORT}/?op=meetup&val=boba_{id}")')
		exit(0)
	else:
		while True:
			sleep(3)
			try:
				os.wait()
			except ChildProcessError:
				break
	print("-- All children have ended.")

def print_help():
	print("This program tests the code written for A2 of CSC 360. Ensure that you have set the PORT")
	print("variable in the pytest file (at the start), and that you have your 'myserver' program ")
	print("running in a different terminal.")
	print()
	print("Pass either 't1' or 't2' as an argument to tell pytest to test task 1 or task 2 respectively. If ")
	print("you pass 't2' you can also pass an integer to change the generation size (default is 3), and/or ")
	print("the keyword 'last' to set it to run in MEETUP_LAST mode.")
	print()
	print("The output of 't1' tests is displayed in the order that the readers and writers returned, not")
	print("order they were called. Note, if you get a line that just says 'READER 3', or something similar")
	print("this is because the server did not register your request. Ensure it is running, and increase the")
	print("value of SLEEP_VAL (in seconds).")
	print()
	print("Specific examples:")
	print("     pytest t1          -- This will test task 1 with 7 readers and 7 writers.")
	print("     pytest t2          -- This will test task 2 with generations of size 3 in MEETUP_FIRST mode")
	print("     pytest t2 5        -- This will test task 2 with generations of size 5 in MEETUP_FIRST mode")
	print("     pytest t2 5 last   -- This will test task 2 with generations of size 5 in MEETUP_LAST mode")
	print("     pytest t2 last     -- This will test task 2 with generations of size 3 in MEETUP_LAST mode")

style_dict = {
	"blue":Fore.BLUE,
	"cyan":Fore.CYAN,
	"green":Fore.GREEN,
	"red":Fore.RED,
	"white":Fore.WHITE,
	"yellow":Fore.YELLOW,

	"bold":Style.BRIGHT,
	"none":Style.RESET_ALL
}

def print_error(msg):
	print(f"{style_dict['red']}{style_dict['bold']}ERROR: {Style.RESET_ALL}{msg}")


if __name__ == '__main__':
	main()
