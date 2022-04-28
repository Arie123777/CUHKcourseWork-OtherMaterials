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
# Duck Typing
class Trap:
    def __init__(self, row, col):
        self._row = row
        self._col = col
        self._occupying = None
        self._name = 'Trap'

    # TODO: _occupying get and setter (done)
    def getOccupying(self):
        return self._occupying
    
    def setOccupying(self, cell):
        self._occupying = cell

    # TODO: _name getter (done)
    def getName(self):
        return self._name
    
    def interact_with(self, comer):
        # TODO: Add game logic. (done)
        self._occupying.remove_occupant()
        if comer._name == 'Goblin':
            print('\033[1;31;43mA goblin entered a trap at (%d, %d)and died.\033[0;0m' % (self._row, self._col))
            # TODO: Add game logic. (done)
            comer.setActive(False)
            return False
        elif comer._name == 'Player':
            print('\033[1;31;43mYou entered a trap at (%d, %d)! HP - 1.\033[0;0m' % (self._row, self._col))
            # TODO: Add game logic. (done)
            comer.setHP(comer.getHP() - 1)
            comer.setOxygen(comer.getOxygen() - 1)
            return True
        else:
            return False


    def display(self):
        # TODO: Add display logic. (done)
        return "\033[2;97m "
