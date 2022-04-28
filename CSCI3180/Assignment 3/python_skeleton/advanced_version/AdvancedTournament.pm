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

use strict;
use warnings;

package AdvancedTournament;
use base_version::Team;
use advanced_version::AdvancedFighter;
use base_version::Tournament;
use List::Util qw(sum);

our @ISA = qw(Tournament); 


sub new{
    my ($class) = shift;
    my $self = $class->SUPER::new();
    $self->{"team1"} = undef;
    $self->{"team2"} = undef;
    $self->{"round_cnt"} = 1;
    $self->{"defeat_record"} = [];
    bless $self, $class;
    return $self;
}

sub play_one_round{
    my ($self) = shift;
    my $fight_cnt = 1;
    my $team1_fighter;
    my $team2_fighter;
    print "Round $self->{'round_cnt'}:\n";

    while (1)
    {
        $team1_fighter = $self->{"team1"}->get_next_fighter();
        $team2_fighter = $self->{"team2"}->get_next_fighter();

        if (!(defined $team1_fighter) || !(defined $team2_fighter))
        {
            last;
        }
        
        # ask whether they buy abilities here
        $team1_fighter->buy_prop_upgrade();
        $team2_fighter->buy_prop_upgrade();

        my $fighter_first = $team1_fighter;
        my $fighter_second = $team2_fighter;
        if ($team1_fighter->{"speed"} < $team2_fighter->{"speed"})
        {
            $fighter_first = $team2_fighter;
            $fighter_second = $team1_fighter;
        }

        my %properties_first = $fighter_first->get_properties();
        my %properties_second = $fighter_second->get_properties();

        my $damage_first = $properties_first{"attack"}-$properties_second{"defense"};
        if ($damage_first < 1)
        {
            $damage_first = 1;
        }
        $fighter_second->reduce_HP($damage_first);
        my $damage_second;
        if ($fighter_second->check_defeated() == 0)
        {
            $damage_second = $properties_second{"attack"}-$properties_first{"defense"};
            if ($damage_second < 1)
            {
                $damage_second = 1;
            }
            $fighter_first->reduce_HP($damage_second);
        }

        my $winner_info = "tie";
        if (!(defined $damage_second))
        {
            $winner_info = "Fighter $fighter_first->{'NO'} wins";
            $fighter_first->record_fight("W");
            $fighter_second->record_fight("L");
        }
        else
        {
            if ($damage_first > $damage_second)
            {
                $winner_info = "Fighter $fighter_first->{'NO'} wins";
                $fighter_first->record_fight("W");
                $fighter_second->record_fight("L");
            }
            else 
            {
                if ($damage_second > $damage_first)
                {
                    $winner_info = "Fighter $fighter_second->{'NO'} wins";
                    $fighter_second->record_fight("W");
                    $fighter_first->record_fight("L");
                }
                else
                {
                    $fighter_first->record_fight("T");
                    $fighter_second->record_fight("T");
                }
            }
        }
        if ($#{$fighter_first->{"history_record"}} + 1 == 4)
        {
            shift @{$fighter_first->{"history_record"}};
        }
        if ($#{$fighter_second->{"history_record"}} + 1 == 4)
        {
            shift @{$fighter_second->{"history_record"}};
        }
        print "Duel $fight_cnt: Fighter $team1_fighter->{'NO'} VS $team2_fighter->{'NO'}, $winner_info\n";
        $team1_fighter->print_info();
        $team2_fighter->print_info();
        $self->update_fighter_properties_and_award_coins($team1_fighter, $team2_fighter->check_defeated(), 0);
        $self->update_fighter_properties_and_award_coins($team2_fighter, $team1_fighter->check_defeated(), 0);
        $fight_cnt += 1;


    }
    print "Fighters at rest:\n\n";
    while (defined $team1_fighter)
    {
        $team1_fighter->print_info();
        $self->update_fighter_properties_and_award_coins($team1_fighter, 0, 1);
        $team1_fighter = $self->{"team1"}->get_next_fighter();
    }
    while (defined $team2_fighter)
    {
        $team2_fighter->print_info();
        $self->update_fighter_properties_and_award_coins($team2_fighter, 0, 1);
        $team2_fighter = $self->{"team2"}->get_next_fighter();
    }
    $self->{"round_cnt"} += 1;
}

sub update_fighter_properties_and_award_coins{
    my ($self) = shift;
    my $fighter = $_[0];
    my $flag_defeat = $_[1];
    my $flag_rest = $_[2];
    local $AdvancedFighter::coins_to_obtain = $AdvancedFighter::coins_to_obtain;
    local $AdvancedFighter::delta_attack = -1;
    local $AdvancedFighter::delta_defense = -1;
    local $AdvancedFighter::delta_speed = -1;
    if ($flag_rest == 1)
    {
        $AdvancedFighter::coins_to_obtain /= 2;
        $AdvancedFighter::delta_attack = 1;
        $AdvancedFighter::delta_defense = 1;
        $AdvancedFighter::delta_speed = 1;
    }
    else
    {
        if ($#{$fighter->{"history_record"}} + 1 == 3 && @{$fighter->{"history_record"}}[0] eq @{$fighter->{"history_record"}}[1] && @{$fighter->{"history_record"}}[0] eq @{$fighter->{"history_record"}}[2])
        {
            if (@{$fighter->{"history_record"}}[0] eq "W")
            {
                $AdvancedFighter::coins_to_obtain *= 1.1;
                $AdvancedFighter::delta_attack = 1;
                $AdvancedFighter::delta_defense = -2;
                $AdvancedFighter::delta_speed = 1;
                $fighter->{"history_record"} = [];
            }
            else
            {
                if (@{$fighter->{"history_record"}}[0] eq "L")
                {
                    $AdvancedFighter::coins_to_obtain *= 1.1;
                    $AdvancedFighter::delta_attack = -2;
                    $AdvancedFighter::delta_defense = 2;
                    $AdvancedFighter::delta_speed = 2;
                    $fighter->{"history_record"} = [];
                }
            }
        }
        if ($flag_defeat == 1)
        {
            $AdvancedFighter::delta_attack += 1;
            $AdvancedFighter::coins_to_obtain *= 2;
        }
    }
    $fighter->obtain_coins();
    $fighter->update_properties();
}

sub input_fighters{
    my ($self) = shift;
    my $team_NO = int($_[0]);
    print "Please input properties for fighters in Team $team_NO\n";
    my @fighter_list_team = ();
    for my $fighter_idx ((4 * ($team_NO - 1) + 1) .. (4 * ($team_NO - 1) + 4))
    {
        while (1)
        {
            my $properties = <STDIN>;
            chomp $properties;
            my @properties = split(" ", $properties);
            my $HP = int($properties[0]);
            my $attack = int($properties[1]);
            my $defense = int($properties[2]);
            my $speed = int($properties[3]);
            if ($HP + 10 * ($attack + $defense + $speed) <= 500)
            {
                my $fighter = new AdvancedFighter($fighter_idx, $HP, $attack, $defense, $speed);
                push(@fighter_list_team, $fighter);
                last;
            }
            print "Properties violate the constraint\n";
        }
    }
    return \@fighter_list_team;
}


1;