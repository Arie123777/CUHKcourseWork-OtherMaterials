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


package Team;

sub new {
    my $class = shift;
    my $self = {
        "NO" => shift,
        "fighter_list" => undef,
        "order" => undef,
        "fight_cnt" => 0
    };
    bless $self, $class;
    return $self;
}

sub set_fighter_list{
    my ($self) = shift;
    $self->{"fighter_list"} = $_[0];
}

sub get_fighter_list{
    my ($self) = shift;
    return $self->{"fighter_list"};
}

sub set_order{
    my ($self) = shift;
    $self->{"order"} = ();
    for my $a_order (@{$_[0]})
    {
        push(@{$self->{"order"}}, $a_order);
    }
    $self->{"fight_cnt"} = 0;
}

sub get_next_fighter{
    my ($self) = shift;
    if ($self->{"fight_cnt"} >= $#{$self->{"order"}} + 1)
    {
        return;
    }
    my $prev_fighter_idx = @{$self->{"order"}}[$self->{"fight_cnt"}];
    my $fighter;
    for my $my_fighter (@{$self->{"fighter_list"}})
    {
        if ($my_fighter->{"NO"} == $prev_fighter_idx)
        {
            $fighter = $my_fighter;
            last;
        }
    }
    $self->{"fight_cnt"} += 1;
    return $fighter;
}

1;