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

from base_version.Team import Team
from base_version.Tournament import Tournament
from .AdvancedFighter import AdvancedFighter
import advanced_version.AdvancedFighter as AdvancedFighterFile


class AdvancedTournament(Tournament):
    def __init__(self):
        super(AdvancedTournament, self).__init__()
        self.team1 = None
        self.team2 = None
        self.round_cnt = 1
        self.defeat_record = []

    def update_fighter_properties_and_award_coins(self, fighter, flag_defeat=False, flag_rest=False):
        original_coin = AdvancedFighterFile.coins_to_obtain
        if (flag_rest == True):
            AdvancedFighterFile.coins_to_obtain /= 2
            AdvancedFighterFile.delta_attack = 1
            AdvancedFighterFile.delta_defense = 1
            AdvancedFighterFile.delta_speed = 1
        else:
            if (len(fighter.history_record) == 3 and fighter.history_record[0] == fighter.history_record[1] and fighter.history_record[0] == fighter.history_record[2]):
                if (fighter.history_record[0] == "W"):
                    AdvancedFighterFile.coins_to_obtain = 20 * 1.1
                    AdvancedFighterFile.delta_attack = 1
                    AdvancedFighterFile.delta_defense = -2
                    AdvancedFighterFile.delta_speed = 1
                    fighter.history_record.clear()
                else:
                    if (fighter.history_record[0] == "L"):
                        AdvancedFighterFile.coins_to_obtain = 20 * 1.1
                        AdvancedFighterFile.delta_attack = -2
                        AdvancedFighterFile.delta_defense = 2
                        AdvancedFighterFile.delta_speed = 2
                        fighter.history_record.clear()
            if (flag_defeat == True):
                AdvancedFighterFile.delta_attack += 1
                AdvancedFighterFile.coins_to_obtain *= 2
        fighter.obtain_coins()
        fighter.update_properties()
        AdvancedFighterFile.coins_to_obtain = original_coin
        AdvancedFighterFile.delta_attack = -1
        AdvancedFighterFile.delta_defense = -1
        AdvancedFighterFile.delta_speed = -1

    def input_fighters(self, team_NO):
        print("Please input properties for fighters in Team {}".format(team_NO))
        fighter_list_team = []
        for fighter_idx in range(4 * (team_NO - 1) + 1, 4 * (team_NO - 1) + 5):
            while True:
                properties = input().split(" ")
                properties = [int(prop) for prop in properties]
                HP, attack, defence, speed = properties
                if HP + 10 * (attack + defence + speed) <= 500:
                    fighter = AdvancedFighter(fighter_idx, HP, attack, defence, speed)
                    fighter_list_team.append(fighter)
                    break
                print("Properties violate the constraint")
        return fighter_list_team
        
    def play_one_round(self):
        fight_cnt = 1
        print("Round {}:".format(self.round_cnt))

        while True:
            team1_fighter = self.team1.get_next_fighter()
            team2_fighter = self.team2.get_next_fighter()

            if team1_fighter is None or team2_fighter is None:
                break

            team1_fighter.buy_prop_upgrade()
            team2_fighter.buy_prop_upgrade()
            
            fighter_first = team1_fighter
            fighter_second = team2_fighter
            if team1_fighter.properties["speed"] < team2_fighter.properties["speed"]:
                fighter_first = team2_fighter
                fighter_second = team1_fighter

            properties_first = fighter_first.properties
            properties_second = fighter_second.properties

            damage_first = max(properties_first["attack"] - properties_second["defense"], 1)
            fighter_second.reduce_HP(damage_first)

            damage_second = None
            if not fighter_second.check_defeated():
                damage_second = max(properties_second["attack"] - properties_first["defense"], 1)
                fighter_first.reduce_HP(damage_second)

            winner_info = "tie"
            if damage_second is None:
                winner_info = "Fighter {} wins".format(fighter_first.properties["NO"])
                fighter_first.record_fight("W")
                fighter_second.record_fight("L")
            else:
                if damage_first > damage_second:
                    winner_info = "Fighter {} wins".format(fighter_first.properties["NO"])
                    fighter_first.record_fight("W")
                    fighter_second.record_fight("L")
                elif damage_second > damage_first:
                    winner_info = "Fighter {} wins".format(fighter_second.properties["NO"])
                    fighter_second.record_fight("W")
                    fighter_first.record_fight("L")
                else:
                    fighter_first.record_fight("T")
                    fighter_second.record_fight("T")
            if (len(fighter_first.history_record) == 4):
                fighter_first.history_record.remove(fighter_first.history_record[0])
            if (len(fighter_second.history_record) == 4):
                fighter_second.history_record.remove(fighter_first.history_record[0])
            print("Duel {}: Fighter {} VS Fighter {}, {}".format(fight_cnt, team1_fighter.properties["NO"],
                    team2_fighter.properties["NO"], winner_info))
            team1_fighter.print_info()
            team2_fighter.print_info()
            self.update_fighter_properties_and_award_coins(team1_fighter, team2_fighter.check_defeated(), 0)
            self.update_fighter_properties_and_award_coins(team2_fighter, team1_fighter.check_defeated(), 0)
            fight_cnt += 1

        print("Fighters at rest:")
        for team in [self.team1, self.team2]:
            if team is self.team1:
                team_fighter = team1_fighter
            else:
                team_fighter = team2_fighter
            while True:
                if team_fighter is not None:
                    team_fighter.print_info()
                    self.update_fighter_properties_and_award_coins(team_fighter, 0, 1)
                else:
                    break
                team_fighter = team.get_next_fighter()

        self.round_cnt += 1
