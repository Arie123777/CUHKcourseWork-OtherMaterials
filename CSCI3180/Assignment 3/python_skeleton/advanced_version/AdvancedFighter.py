#
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
# Assignment 3
# Name : Lai Man Hin
# Student ID : 1155136167
# Email Addr : 1155136167@link.cuhk.edu.hk

from base_version.Fighter import Fighter

coins_to_obtain = 20
delta_attack = -1
delta_defense = -1
delta_speed = -1


class AdvancedFighter(Fighter):
    def __init__(self, NO, HP, attack, defense, speed):
        super(AdvancedFighter, self).__init__(NO, HP, attack, defense, speed)
        self.coins = 0
        self.history_record = []

    def obtain_coins(self):
        self.coins += coins_to_obtain
        
    def buy_prop_upgrade(self):
        inp = ''
        while (self.coins >= 50 and inp != 'N'):
            inp = input("Do you want to upgrade properties for Fighter {}? A for attack. D for defense. S for speed. N for no.\n".format(self.NO));
            if (inp == 'A'):
                self.coins -= 50
                self.attack += 1
            if (inp == 'D'):
                self.coins -= 50
                self.defense += 1
            if (inp == 'S'):
                self.coins -= 50
                self.speed += 1
            
    def update_properties(self):
        self.attack = max(self.attack + delta_attack, 1)
        self.defense = max(self.defense + delta_defense, 1)
        self.speed = max(self.speed + delta_speed, 1)

    def record_fight(self, fight_result):
        self.history_record.append(fight_result)
