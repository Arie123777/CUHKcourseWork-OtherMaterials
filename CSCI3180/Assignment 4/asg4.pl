%
% CSCI3180 Principles of Programming Languages
%
% --- Declaration ---
%
% I declare that the assignment here submitted is original except for source
% material explicitly acknowledged. I also acknowledge that I am aware of
% University policy and regulations on honesty in academic work, and of the
% disciplinary guidelines and procedures applicable to breaches of such policy
% and regulations, as contained in the website
% http://www.cuhk.edu.hk/policy/academichonesty/
%
% Assignment 4
% Name : Lai Man Hin
% Student ID : 1155136167
% Email Addr : mhlai0@cse.cuhk.edu.hk

% Sample trees.
% bt(bt(bt(nil,0,nil),s(0), bt(nil,s(s(s(0))),nil)), s(s(s(s(0)))), bt(nil,s(s(0)),nil))
% bt(bt(bt(nil,0,nil),s(0), bt(nil,s(s(s(0))),nil)), s(s(s(s(0)))), bt(nil,s(s(s(s(s(0))))),nil))

% 1. is_binary_tree(T)
% | ?- is_binary_tree(bt(bt(bt(nil,0,nil),s(0), bt(nil,s(s(s(0))),nil)), s(s(s(s(0)))), bt(nil,s(s(0)),nil))).
% yes

is_binary_tree(nil).
is_binary_tree(bt(X, _, Z)) :-
    is_binary_tree(X),
    is_binary_tree(Z).



%2. lt(X,Y) and bs_tree(T). 
% | ?- bs_tree(bt(bt(bt(nil,0,nil),s(0), bt(nil,s(s(s(0))),nil)), s(s(s(s(0)))), bt(nil,s(s(0)),nil))).
% no
% | ?- bs_tree(bt(bt(bt(nil,0,nil),s(0), bt(nil,s(s(s(0))),nil)), s(s(s(s(0)))), bt(nil,s(s(s(s(s(0))))),nil))).
% yes

lt(0, s(_)).
lt(s(X), s(Y)) :-
    lt(X, Y).

bs_tree(bt(nil, _, nil)).
bs_tree(bt(bt(A, X, B), Y, bt(C, Z, D))) :-
    is_binary_tree(bt(bt(A, X, B), Y, bt(C, Z, D))),
    lt(X, Y),
    lt(Y, Z),
    bs_tree(bt(A, X, B)),
    bs_tree(bt(C, Z, D)).



% 3. parent(T, P, C).
% | ?- parent(bt(bt(bt(nil,0,nil),s(0), bt(nil,s(s(s(0))),nil)), s(s(s(s(0)))), bt(nil,s(s(0)), nil)), s(s(s(s(0)))), C).
% C = s(0) ? ;
% C = s(s(0)) ? ;
% no


% | ?- parent(bt(bt(bt(nil,0,nil),s(0), bt(nil,s(s(s(0))),nil)), s(s(s(s(0)))), bt(nil,s(s(0)), nil)), P, 0).
% P = s(0) ? ;
% no

parent(bt(bt(_, X, _), Y, bt(_, _, _)), Y, X).
parent(bt(bt(_, _, _), Y, bt(_, Z, _)), Y, Z).
parent(bt(X, _, _), P, C) :-
    parent(X, P, C).
parent(bt(_, _, Z), P, C) :-
    parent(Z, P, C).


% 4. descendent(T, A, D).
% | ?- descendent(bt(bt(bt(nil,0,nil),s(0), bt(nil,s(s(s(0))),nil)), s(s(s(s(0)))), bt(nil,s(s(0)), nil)), s(s(s(s(0)))), D).
% D = s(0) ? ;
% D = s(s(0)) ? ;
% D = 0 ? ;
% D = s(s(s(0))) ? ;
% no

% | ?- descendent(bt(bt(bt(nil,0,nil),s(0), bt(nil,s(s(s(0))),nil)), s(s(s(s(0)))), bt(nil,s(s(0)), nil)), P, 0).
% P = s(0) ? ;
% P = s(s(s(s(0)))) ? ;
% no

descendent(bt(bt(_, X, _), Y, bt(_, _, _)), Y, X).
descendent(bt(bt(_, _, _), Y, bt(_, Z, _)), Y, Z).
% if found not same, go find child
descendent(bt(bt(E, X, F), _, _), Z, D) :-
    descendent(bt(E, X, F), Z, D).
% if found it is same, print ALL child
descendent(bt(bt(E, X, F), Y, _), Y, D) :-
    descendent(bt(E, X, F), X, D).
% if found not same, go find child
descendent(bt(_, _, bt(E, X, F)), Z, D) :-
    descendent(bt(E, X, F), Z, D).
% if found it is same, print ALL child
descendent(bt(_, Y, bt(E, X, F)), Y, D) :-
    descendent(bt(E, X, F), X, D).



% 5. count_nodes(T, X).
% | ?- count_nodes(bt(bt(bt(nil,0,nil),s(0), bt(nil,s(s(s(0))),nil)), s(s(s(s(0)))), bt(nil,s(s(0)), nil)), X).
% X = s(s(s(s(s(0))))) ? ;
% no

sum(0, X, X).
sum(s(X), Y, s(Z)) :- sum(X, Y, Z).

count_nodes(nil, 0).
count_nodes(bt(A, _, C), s(X)) :-
      count_nodes(A, Y),
      count_nodes(C, Z),
      sum(Y, Z, X).





% 6. sum_nodes(T, X).
% | ?- sum_nodes(bt(bt(nil, s(0), nil), s(s(s(s(0)))), bt(nil,s(s(0)), nil)), X).
% X = s(s(s(s(s(s(s(0))))))) ? ;
% no

% | ?- sum_nodes(T, s(s(s(s(s(s(s(0)))))))).
% T = bt(nil,s(s(s(s(s(s(s(0))))))),nil) ? ;
% T = bt(nil,s(s(s(s(s(s(s(0))))))),bt(nil,0,nil)) ? ;
% T = bt(nil,s(s(s(s(s(s(0)))))),bt(nil,s(0),nil)) ? ;
% T = bt(nil,s(s(s(s(s(0))))),bt(nil,s(s(0)),nil)) ? ;
% T = bt(nil,s(s(s(s(0)))),bt(nil,s(s(s(0))),nil)) ? ;
% T = bt(nil,s(s(s(0))),bt(nil,s(s(s(s(0)))),nil)) ?
% yes

sum_nodes(nil, 0).
sum_nodes(bt(A, B, C), X) :-
    sum_nodes(A, Y),
    sum_nodes(C, Z),
    sum(Y, Z, W),
    sum(W, B, X).



% 7. preorder(T, X).
% ?- preorder(bt(bt(bt(nil,0,nil),s(0), bt(nil,s(s(s(0))),nil)), s(s(s(s(0)))), bt(nil,s(s(0)),nil)), X).
% X = [s(s(s(s(0)))),s(0),0,s(s(s(0))),s(s(0))] ? ;
% no

append([],L,L).
append([H|T],L2,[H|L3]):-append(T,L2,L3).

preorder(bt(nil, X, nil), [X]).
preorder(bt(bt(A, B, C), D, bt(E, F, G)), X) :-
    preorder(bt(A, B, C), Y),
	preorder(bt(E, F, G), Z),
    append([D], W, X),
    append(Y, [F|_], W),
    append(Y, Z, W).