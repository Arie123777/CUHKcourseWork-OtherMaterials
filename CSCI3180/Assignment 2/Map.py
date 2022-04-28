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
from Cell import Cell


class Map:
    def __init__(self, height, width):
        self._rows = height
        self._cols = width
        self._cells = [[Cell() for x in range(width)] for y in range(height)]
    
    #TODO: rows getter (done)
    def getCols(self):
        return self._cols

    #TODO: cols getter (done)
    def getRows(self):
        return self._rows

    def get_cell(self, row, col):
        # TODO: check whether the position is out of boundary (done)
        #       if not, return the cell at (row, col)
        #       return None otherwise 
        if (row < 0 or row >= self._rows or col < 0 or col >= self._cols): 
            print('\033[1;31;46mNext move is out of boundary!\033[0;0m')
            return None 
        else:
            # return a cell 
            return self._cells[row][col]
        # END TODO 

    def build_cell(self, row, col, cell):
        # TODO: check whether the position is out of boundary (done)
        #       if not, add a cell (row, col) and return True
        #       return False otherwise 
        if (not(row >= 0 and row < self._rows and col >= 0 and col < self._cols)):
            print('\033[1;31;46mThe position (%d, %d) is out of boundary!\033[0;0m' %(row, col))
            return False 
        else:
            # add a cell
            self._cells[row][col] = cell
            return True 
        # END TODO

    # return a list of cells which are neighbours of cell (row, col) 
    def get_neighbours(self, row, col):
        return_cells = []
        # TODO: return a list of neighboring cells of cell (row, col) (done)
        for i in range (max(0, row - 1), min(row + 2, self._rows)):
            for j in range(max(0, col - 1), min(col + 2, self._cols)):
                if (i == row and j == col):
                    continue
                return_cells.append(self._cells[i][j])
        return return_cells
        # END TODO
        

    def display(self):
        # TODO: print the map by calling diplay of each cell (done)
        print("   ", end="")
        for i in range(0, self._cols):
            print("%d     " % i, end = "")
        print()
        for i in range(0, self._rows):
            print("%d " % i, end="")
            for j in range(0, self._cols):
                self._cells[i][j].display()
            print("\n")
        # END TODO
