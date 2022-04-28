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
from abc import abstractmethod
from operator import truediv


class Cell:
    def __init__(self, row=0, col=0):
        self._row = row
        self._col = col
        self._occupant = None
        self._color = None 
        self._hours = 0
    
    # TODO: hours getter and setter (done)
    def getHours(self):
        return self._hours

    # TODO: occupant getter (done)
    def getOccupant(self):
        return self._occupant

    def set_occupant(self, obj):
        # TODO: set occupant for the Plain cell 
        #       return whether success or not (done)
        if (self.getOccupant() == None or self._occupant.interact_with(obj)):
            self._occupant = obj
            return True
        else:
            return False
        # END TODO

    def remove_occupant(self):
        # TODO: remove the occupant  (done)
        self._occupant = None
        # END TODO

    def display(self):
        # TODO: print a string to display the cell 
        #       and the occupant in the cell (done)
        if (self._occupant != None):
            #print("%shaha %s%s \033[0m   " % (self._color, (self._occupant).display(), self._color), end = "")
            print(self._color, end="")
            print(" ", end="")
            print(self._occupant.display(), end="")
            print(self._color, end="")
            print(" \033[0m   ", end="")
        else:
            print(self._color, end="")
            print("   \033[0m   ", end="")
        # END TO

class Plain(Cell):
    def __init__(self, row, col):
        Cell.__init__(self, row, col)
        self._color = '\033[1;32;42m'
        self._hours = 1

class Mountain(Cell):
    def __init__(self, row, col):
        Cell.__init__(self, row, col)
        self._color = '\033[1;37;47m'

    def set_occupant(self, obj):
        # TODO: return False (done)
        return False
        # END TODO
    
class Swamp(Cell):
    def __init__(self, row, col):
        Cell.__init__(self, row, col)
        self._color = '\033[1;34;44m'
        self._hours = 2
