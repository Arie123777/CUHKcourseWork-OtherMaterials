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
from Map import Map
from Cell import Plain, Mountain, Swamp
from GameCharacter import Player, Goblin
from Trap import Trap 
from Volcano import Volcano
class NewEngine:
    def __init__(self, data_file):
        self._actors = []
        self._map = None 
        self._player = None 
        with open(data_file, 'r') as fp:
            line = fp.readline()
            if not line:
                return None
            else:
                items = line.split()
                if len(items) != 7:
                    print('INVALID DATA FILE.')
                    return None
                num_of_row = int(items[0])
                num_of_col = int(items[1])
                p_ox = int(items[2])
                p_hp = int(items[3])
                num_of_goblins = int(items[4])
                num_of_traps = int(items[5])
                num_of_volcanoes = int(items[6])

            self._map = Map(num_of_row, num_of_col)
            
            # TODO: initialize each cell of the map object 
            #       using the build_cell method (done)
            for i in range(0, num_of_row):
                line = fp.readline()
                splitArr = []
                splitArr = line.split()
                for j in range(0, len(splitArr)):
                    if (splitArr[j] == 'P'):
                        self._map.build_cell(i, j, Plain(i, j))
                    elif (splitArr[j] == 'M'):
                        self._map.build_cell(i, j, Mountain(i, j))
                    elif (splitArr[j] == 'S'):
                        self._map.build_cell(i, j, Swamp(i, j))

            # END TODO
            
            self._player = Player(num_of_row - 1, 0, p_hp, p_ox)
            
            # TODO: initilize the position of the player (done)
            #       using the set_occupant and occupying setter;
            #       add the player to _actors array
            initCell = self._map.get_cell(num_of_row - 1, 0)
            initCell.set_occupant(self._player)
            self._player.setOccupying(initCell)
            self._actors.append(self._player)

            for gno in range(num_of_goblins):
                # TODO: initilize each Goblin on the map
                #       using the set_occupant and occupying setter;
                #       add each Goblin to _actors array (done)
                tempString = fp.readline()
                splitArr = tempString.split()
                goblinRow = int(splitArr[0])
                goblinCol = int(splitArr[1])
                goblinActions = []
                for i in range(2, len(splitArr)):
                    goblinActions += splitArr[i][0]
                gob = Goblin(goblinRow, goblinCol, goblinActions)
                self._actors.append(gob)
                initCell = self._map.get_cell(goblinRow, goblinCol)
                initCell.set_occupant(gob)
                gob.setOccupying(initCell)
                # END TODO 

            for tno in range(num_of_traps):
                # TODO: initilize each Trap on the map (done)
                #       using the set_occupant and occupying setter;
                tempString = fp.readline()
                splitArr = tempString.split()
                trapRow = int(splitArr[0])
                trapCol = int(splitArr[1])
                trap = Trap(trapRow, trapCol)
                initCell = self._map.get_cell(trapRow, trapCol)
                initCell.set_occupant(trap)
                trap.setOccupying(initCell)
                # END TODO 

            for vno in range(num_of_volcanoes):
                # TODO: initilize each Volcano of the map object 
                #       using the build_cell method 
                #       add each volcano to _actors array 
                tempString = fp.readline()
                splitArr = tempString.split()
                volcanoRow = int(splitArr[0])
                volcanoCol = int(splitArr[1])
                volcanoFraq = int(splitArr[2])
                vol = Volcano(volcanoRow, volcanoCol, volcanoFraq)
                self._actors.append(vol)
                self._map.build_cell(volcanoRow, volcanoCol, vol)
                # END TODO 

    def run(self):
        # main rountine of the game
        self.print_info()
        while not self.state():            
            for obj in self._actors:
                if obj._active: 
                    obj.act(self._map)
            self.print_info()
            self.clean_up()
        self.print_result()

    def clean_up(self):
        # TODO: remove all objects in _actors which is not active (half done)
        it = iter(self._actors)
        while (True):
            o = next(it, None)
            if (o == None):
                break
            elif (isinstance(o, Player) or isinstance(o, Goblin)):
                if (not o.getActive()):
                    self._actors.remove(o)
        # END TODO 

    # check if the game ends and return if the player win or not.
    def state(self):
        # TODO: check if the game ends and 
        #       return an integer for the game status (done)
        if (self._player.getHP() <= 0 or self._player.getOxygen() <= 0):
            return -1
        elif (self._player.getRow() == 0 and self._player.getCol() == self._map.getCols() - 1):
            return 1
        else:
            return 0
        # END TODO 

    def print_info(self):
        self._map.display()
        # TODO: display the remaining oxygen, HP 
        #       and the number of traps surrounding the player (done)
        trapNum = 0
        cells = self._map.get_neighbours(self._player.getRow(), self._player.getCol())
        for i in range (0, len(cells)):
            occ = cells[i].getOccupant()
            if (isinstance(occ, Trap)):
                trapNum += 1 
        print('Oxygen: %d, HP: %d, Trap: %d' % (self._player.getOxygen(), self._player.getHP(), trapNum))
        # END TODO

    def print_result(self):
        if self.state() == 1:
            print('\033[1;33;41mCongrats! You win!\033[0;0m')
        if self.state() == -1:
            print('\033[1;33;41mBad Luck! You lose.\033[0;0m')

        
