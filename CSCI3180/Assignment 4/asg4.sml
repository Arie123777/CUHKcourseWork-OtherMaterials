(* CSCI3180 Principles of Programming Languages

 --- Declaration ---

 I declare that the assignment here submitted is original except for source
 material explicitly acknowledged. I also acknowledge that I am aware of
 University policy and regulations on honesty in academic work, and of the
 disciplinary guidelines and procedures applicable to breaches of such policy
 and regulations, as contained in the website
 http://www.cuhk.edu.hk/policy/academichonesty/

 Assignment 4
 Name : Lai Man Hin
 Student ID : 1155136167
 Email Addr : mhlai0@cse.cuhk.edu.hk
*)
(* data type *)
datatype rank = Jack | Queen | King | Ace | Num of int;

(* basic version: the point of Ace is 1 *)
(* magic version: the point of Ace is 1 or 11*)

(* Problem 1 *)
(* compute the point for basic version *)
(* input: a rank *)
(* output: the basic point of the rank *)
fun get_basic_point(Ace) = 1
| get_basic_point(Num x) = x
| get_basic_point(Jack) = 10
| get_basic_point(Queen) = 10
| get_basic_point(King) = 10;
    

(* Problem 2 *)
(* get the cards of Player 1 *)
(*
** input: the number of cards for player 1
**        the number of cards for player 2
**        the common rank list for two players
*)
(* output: (the drawn cards of player 1, the drawn cards of player 2) *)
(* n1 + n2 is smaller than the number of cards *)
(* n1 and n2 are positive *)
fun get_cards(n1, n2, mylist) =
let
    val total = n1+n2
    fun insert_item(list1, list2, 0, 0, total, curlist, turn) = (list1, list2)
    |   insert_item(list1, list2, n1, 0, x, curlist, turn) = insert_item(list1@[hd(curlist)], list2, n1-1, 0, x+1, tl(curlist), turn)
    |   insert_item(list1, list2, 0, n2, x, curlist, turn) = insert_item(list1, list2@[hd(curlist)], 0, n2-1, x+1, tl(curlist), turn)
    |   insert_item(list1, list2, n1, n2, x, curlist, 1) = insert_item(list1@[hd(curlist)], list2, n1-1, n2, x+1, tl(curlist),2)
    |   insert_item(list1, list2, n1, n2, x, curlist, _) = insert_item(list1, list2@[hd(curlist)], n1, n2-1, x+1, tl(curlist), 1)
    in
        insert_item([], [], n1, n2, 0, mylist, 1)
end;

(* Problem 3 *)
(* tanslate the sum of points to TP by objective function *)
(* the tool used by other functions *)
(* input: the sum of points for all cards *)
(* output: the TP computed by objective function *)
fun cal_TP 0 = 0
| cal_TP x = ((x-1) mod 21)+1 

(* Problem 4 *)
(* compute the TP for basic version *)
(* input: a rank list *)
(* output: the TP for basic version *)
fun get_basic_TP mylist =
let
    fun adding_num(nil, total) = cal_TP(total)
    |   adding_num(remaining_list, total) = adding_num(tl(remaining_list), total+get_basic_point(hd(remaining_list)))
    in adding_num(mylist, 0)
end;

(* Problem 5 *)
(* compute TP considering the different choices of Ace for magic version *)
(* input: a rank list *)
(* output: the TP for magic version *)
fun get_TP nil = 0
|   get_TP mylist =
let
    fun final_TP(TP, 0, mx_TP, true) = TP
    |   final_TP(TP, 0, mx_TP, false) = mx_TP
    |   final_TP(TP, no_ace, mx_TP, true) = final_TP((TP + 9) mod 21 + 1, no_ace-1, TP, (TP + 9) mod 21 + 1 > mx_TP)
    |   final_TP(TP, no_ace, mx_TP, false) = final_TP((TP + 9) mod 21 + 1, no_ace-1, mx_TP, (TP + 9) mod 21 + 1 > mx_TP)
    
    fun count_ace(TP, no_ace, nil) = final_TP(TP, no_ace, TP, false)
    |   count_ace(TP, no_ace, Ace::remaining_list) = count_ace(TP, no_ace+1, remaining_list)
    |   count_ace(TP, no_ace, _::remaining_list) = count_ace(TP, no_ace, remaining_list)
    
    in count_ace(get_basic_TP(mylist), 0, mylist)
end; 

(* Problem 6 *)
(* judge winner by the given rank lists of players *)
(*
** input: (the drawn cards of player 1, the drawn cards of player 2)
*)
(*
** output: 1 (player 1 wins) or
**         2 (player 2 wins) or
**         0 (tie)
*)
fun judge_winner_basic(player1, player2) =
let
    val p1_point = get_basic_TP(player1)
    val p2_point = get_basic_TP(player2)
    fun compare(0, equal) = 0
    |   compare(_, true) = 1
    |   compare(_, false) = 2
    in
        compare(p1_point-p2_point, p1_point>p2_point)
end;


(* Problem 7 *)
(* judge winner by the given rank lists of players *)
(*
** input: (the drawn cards of player 1, the drawn cards of player 2)
*)
(*
** output: 1 (player 1 wins) or
**         2 (player 2 wins) or
**         0 (tie)
*)
fun judge_winner(player1, player2) =
let
    val p1_point = get_TP(player1)
    val p2_point = get_TP(player2)
    fun compare(0, equal) = 0
    |   compare(_, true) = 1
    |   compare(_, false) = 2
    in
        compare(p1_point-p2_point, p1_point>p2_point)
end;

(* Problem 8 *)
(* judge winner by the number of drawn cards and the rank list *)
(* n1 + n2 is smaller than the number of cards *)
(* n1 and n2 are positive *)
(*
** input: the number of cards for player 1
**        the number of cards for player 2
**        the common rank list for two players
*)
(*
** output: 1 (player 1 wins) or
**         2 (player 2 wins) or
**         0 (tie)
*)
fun basic_result(n1, n2, mylist) = judge_winner_basic(get_cards(n1, n2, mylist));

(* Problem 9 *)
(* judge winner by the number of drawn cards and the rank list *)
(* n1 + n2 is smaller than the number of cards *)
(* n1 and n2 are positive *)
(*
** input: the number of cards for player 1
**        the number of cards for player 2
**        the common rank list for two players
*)
(*
** output: 1 (player 1 wins) or
**         2 (player 2 wins) or
**         0 (tie)
*)
fun result(n1, n2, mylist) = judge_winner(get_cards(n1, n2, mylist));

(* Problem 10 *)
(* magic version *)
(* judge the largest TP which can be gotten by player 2 with any legal number of drawn cards *)
(*
** input: the number of cards for player 1
**        the common rank list for two players
*)
(* output: the largest TP *)
fun cal_largest_TP(n1, mylist) =
let
    fun get_p2_TP(p1_list, p2_list) = get_TP(p2_list)
    
    fun find_largest_TP(n1, mylist, mx_TP, cur_TP, 0, true) = cur_TP
    |   find_largest_TP(n1, mylist, mx_TP, cur_TP, 0, false) = mx_TP
    |   find_largest_TP(n1, mylist, mx_TP, cur_TP, count, true) = find_largest_TP(n1, mylist, cur_TP, get_p2_TP(get_cards(n1, count, mylist)), count-1, get_p2_TP(get_cards(n1, count, mylist)) > mx_TP)
    |   find_largest_TP(n1, mylist, mx_TP, cur_TP, count, false) = find_largest_TP(n1, mylist, mx_TP, get_p2_TP(get_cards(n1, count, mylist)), count-1, get_p2_TP(get_cards(n1, count, mylist)) > mx_TP)
    in
        find_largest_TP(n1, mylist, get_p2_TP(get_cards(n1, length(mylist)-n1, mylist)), get_p2_TP(get_cards(n1, length(mylist)-n1, mylist)), length(mylist)-n1, false)
end;















