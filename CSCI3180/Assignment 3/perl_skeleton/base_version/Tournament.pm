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

package Tournament;
use base_version::Team;
use base_version::Fighter;

sub new{
    my $class = shift;
    my $self = {
                "team1" => undef,
                "team2" => undef,
                "round_cnt" => 1
                };
    bless $self, $class;
    return $self;
}

sub set_teams{
    my ($self) = shift;
    $self->{"team1"} = $_[0];
    $self->{"team2"} = $_[1];
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
        }
        else
        {
            if ($damage_first > $damage_second)
            {
                $winner_info = "Fighter $fighter_first->{'NO'} wins";
            }
            else 
            {
                if ($damage_second > $damage_first)
                {
                    $winner_info = "Fighter $fighter_second->{'NO'} wins";
                }
            }
        }
        print "Duel $fight_cnt: Fighter $team1_fighter->{'NO'} VS $team2_fighter->{'NO'}, $winner_info\n";
        $team1_fighter->print_info();
        $team2_fighter->print_info();
        $fight_cnt += 1;
    }
    print "Fighters at rest:\n";
    while (defined $team1_fighter)
    {
        $team1_fighter->print_info();
        $team1_fighter = $self->{"team1"}->get_next_fighter();
    }
    while (defined $team2_fighter)
    {
        $team2_fighter->print_info();
        $team2_fighter = $self->{"team2"}->get_next_fighter();
    }
    $self->{"round_cnt"} += 1;
}

sub check_winner{
    my ($self) = shift;
    my $team1_defeated = 1;
    my $team2_defeated = 1;

    my $fighter_list1 = $self->{"team1"}->{"fighter_list"};
    my $fighter_list2 = $self->{"team2"}->{"fighter_list"};

    for my $i (@{$fighter_list1})
    {
        if (!$i->check_defeated())
        {
            $team1_defeated = 0;
            last;
        }
    }

    for my $i (@{$fighter_list2})
    {
        if (!$i->check_defeated())
        {
            $team2_defeated = 0;
            last;
        }
    }

    my $stop = 0;
    my $winner = 0;
    if ($team1_defeated == 1)
    {
        $stop = 1;
        $winner = 2;
    }
    else
    {
        if ($team2_defeated == 1)
        {
            $stop = 1;
            $winner = 1;
        }
    }
    return ($stop, $winner);
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
                my $fighter = new Fighter($fighter_idx, $HP, $attack, $defense, $speed);
                push @fighter_list_team, $fighter;
                last;
            }
            print "Properties violate the constraint\n";
        }
    }
    return \@fighter_list_team;
}

sub play_game{
    my ($self) = shift;
    my $fighter_list_team1 = $self->input_fighters(1);
    my $fighter_list_team2 = $self->input_fighters(2);

    my $team1 = new Team(1);
    my $team2 = new Team(2);
    $team1->{"fighter_list"} = $fighter_list_team1;
    $team2->{"fighter_list"} = $fighter_list_team2;
    
    $self->set_teams($team1, $team2);

    my $stop;
    my $winner;

    print "===========\n";
    print "Game Begins\n";
    print "===========\n\n";

    while (1)
    {
        print "Team 1: please input order\n";
        my @order1;
        my @order2;
        while (1)
        {
            my $order1 = <STDIN>;
            chomp $order1;
            @order1 = split(" ", $order1);

            my $flag_valid = 1;
            my $undefeated_number = 0;
            for my $order (@order1)
            {
                if ($order < 1 || $order > 4)
                {
                    $flag_valid = 0;
                }
                else 
                {
                    if (@{$self->{"team1"}->{"fighter_list"}}[$order - 1]->check_defeated())
                    {
                        $flag_valid = 0;
                    }
                }
            }
            my %temp_set;
            for my $order (@order1)
            {                               
                $temp_set{$order} = 1;
            }
            if ($#order1 + 1 != scalar keys %temp_set)
            {
                $flag_valid = 0;
            }
            for my $i (0..3)
            {
                if (@{$self->{"team1"}->{"fighter_list"}}[$i]->check_defeated() == 0)
                {
                    $undefeated_number += 1;
                }
            }
            if ($undefeated_number != $#order1 + 1)
            {
                $flag_valid = 0;
            }
            if ($flag_valid == 1)
            {
                last;
            }
            else
            {
                print "Invalid input order\n";
            }
        }

        print "Team 2: please input order\n";
        while (1)
        {
            my $order2 = <STDIN>;
            chomp $order2;
            @order2 = split(" ", $order2);
            my $flag_valid = 1;
            my $undefeated_number = 0;
            for my $order (@order2)
            {
                if ($order < 5 || $order > 8)
                {
                    $flag_valid = 0;
                }
                else
                {
                    if (@{$self->{"team2"}->{"fighter_list"}}[$order - 1 - 4]->check_defeated())
                    {
                        $flag_valid = 0;
                    }
                }                    
            }
            my %temp_set;
            for my $order (@order2)
            {
                $temp_set{$order} = 1;
            }
            if ($#order2 + 1 != scalar keys %temp_set)
            {
                $flag_valid = 0;
            }
            for my $i (0..3)
            {
                if (@{$self->{"team2"}->{"fighter_list"}}[$i]->check_defeated() == 0)
                {
                    $undefeated_number += 1;
                }
            }
            if ($undefeated_number != $#order2 + 1)
            {
                $flag_valid = 0;
            }
            if ($flag_valid == 1)
            {
                last;
            }
            else
            {
                print "Invalid input order\n";
            }
        }

        $self->{"team1"}->set_order(\@order1);
        $self->{"team2"}->set_order(\@order2);

        $self->play_one_round();
        ($stop, $winner) = $self->check_winner();
        if ($stop)
        {
            last;
        }
    }
    print "Team $winner wins\n";
}
1;