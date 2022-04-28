# CSCI3180 Principles of Programming Languages
#
# --- Declaration ---
#
# I declare that the assignment here submitted is original except for source
# material explicitly acknowledged. I also acknowledge that I am aware of
# University policy and regulations on honesty in academic work, and of the
# disciplinary guidelines and procedures applicable to breaches of such policy
# and regulations, as contained in the website
# http://www.cuhk.edu.hk/policy/academichonesty/
#
# Assignment 2
# Name : Lai Man Hin
# Student ID : 1155136167
# Email Addr : mhlai0@cse.cuhk.edu.hk
#
import sys 

if __name__ == '__main__':
	if len(sys.argv) > 1:
		if sys.argv[1] == 'basic':
			from Engine import Engine
			eng = Engine('map-basic.txt')
			if eng != None:
				eng.run()
		elif sys.argv[1] == 'extension':
			from NewEngine import NewEngine
			eng = NewEngine('map-extension.txt')
			if eng != None:
				eng.run()
	else:
		print('usage: python3 StrangePlaent.py [basic/extension]')