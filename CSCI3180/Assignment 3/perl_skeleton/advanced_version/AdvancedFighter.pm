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

package AdvancedFighter;

use base_version::Fighter;
use List::Util qw(sum);

our @ISA = qw(Fighter); 

our $coins_to_obtain = 20;
our $delta_attack = -1;
our $delta_defense = -1;
our $delta_speed = -1;


sub new{
    my ($class) = shift;
    my $self = $class->SUPER::new($_[0], $_[1], $_[2], $_[3], $_[4]);
    $self->{"coins"} = 0;
    $self->{"history_record"} = [];
    bless $self, $class;
    return $self;
}

sub obtain_coins{
    my ($self) = shift;
    $self->{"coins"} += $coins_to_obtain;
}

sub buy_prop_upgrade{
    my ($self) = shift;
    my $inp;
    while ($self->{"coins"} >= 50 && (!(defined $inp) || $inp ne "N"))
    {
        print "Do you want to upgrade properties for fighter $self->{'NO'}? A for attack. D for defense. S for speed. N for no.\n";
        $inp = <STDIN>;
        chomp $inp;
        if ($inp eq "A")
        {
            $self->{"coins"} -= 50;
            $self->{"attack"} += 1;
        }
        if ($inp eq "D")
        {
            $self->{"coins"} -= 50;
            $self->{"defense"} += 1;
        }
        if ($inp eq "S")
        {
            $self->{"coins"} -= 50;
            $self->{"speed"} += 1;
        }
    }
}

sub record_fight{
    my ($self) = shift;
    my $fight_result = shift;
    push @{$self->{"history_record"}}, $fight_result;
}


sub update_properties{
    my ($self) = shift;
    $self->{"attack"} += $delta_attack;
    if ($self->{"attack"} < 1)
    {
        $self->{"attack"} = 1;
    }
    $self->{"defense"} += $delta_defense;
    if ($self->{"defense"} < 1)
    {
        $self->{"defense"} = 1;
    }
    $self->{"speed"} += $delta_speed;
    if ($self->{"speed"} < 1)
    {
        $self->{"speed"} = 1;
    }
}

1;