/*
CSCI3180 Principles of Programming Languages

--- Declaration ---

I declare that the assignment here submitted is original except for source
material explicitly acknowledged. I also acknowledge that I am aware of
University policy and regulations on honesty in academic work, and of the
disciplinary guidelines and procedures applicable to breaches of such policy
and regulations, as contained in the website
http://www.cuhk.edu.hk/policy/academichonesty/

Assignment 1
Name : Lai Man Hin
Student ID : 1155136167
Email Addr : mhlai0@cse.cuhk.edu.hk
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct account
	{
		char name[21];
	    char number[17];
		char password[7];
		char balance[17];
	} Account;
	
Account readline(char mystring[], Account my);
Account account_login_func();
char service_select_func();
void deposit_func(char atmnum, Account a, char myservice, int time);
void withdrawal_func(char atmnum, Account a, char myservice, int time);
void transaction_func(char atmnum, Account a, char myservice, int time);
void addrecord(char atmnum, char accnum1[], char accnum2[], char action, double amount, int time);
char ask_continue_func();
int main()
{
	char input[256];
	char atmnum = '0';
	double myamount = 0.0;
	char myservice = '?';
	int time = 0;
	char ask_continue = 'Y';
	Account a;
	while (ask_continue == 'Y')
	{
		printf("##############################################\n");
		printf("##         Gringotts Wizarding Bank         ##\n");
		printf("##                 Welcome                  ##\n");
		printf("##############################################\n");
		atmnum = '0';
		while (atmnum != '1' && atmnum != '2')
		{
			printf("=> PLEASE CHOOSE THE ATM\n");
			printf("=> PRESS 1 FOR ATM 711\n");
			printf("=> PRESS 2 FOR ATM 713\n");
			scanf("\n%s", input);
			if (strcmp(input, "1") == 0)
				atmnum = '1';
			else if (strcmp(input, "2") == 0)
				atmnum = '2';
			else
				printf("=> INVALID INPUT\n");
		}
		
		a = account_login_func();
		myservice = service_select_func();
		if (myservice == 'D')
		{
			deposit_func(atmnum, a, myservice, time);
			time++;
		}
		else if (myservice == 'W')
		{
			withdrawal_func(atmnum, a, myservice, time);
			time++;
		}
		else
		{
			transaction_func(atmnum, a, myservice, time);
			time += 2;
		}
		ask_continue = ask_continue_func();
	}
	
	return 0;	
}

Account account_login_func()
{
	FILE *fp;
	char input_account[256], input[256], scanline[256];
	Account a;
	int account_check = 0;
	while (account_check == 0)
	{
		printf("=> ACCOUNT\n");
		scanf("\n%s", input_account);
		printf("=> PASSWORD\n");
		scanf("\n%s", input);
		fp = fopen("master.txt", "r");
		while (fgets(scanline, 99, fp) != NULL && account_check == 0)
		{
			a = readline(scanline, a);
			if (strcmp(a.number, input_account) == 0 && strcmp(a.password, input) == 0)
				if (atof(a.balance) <= 0)
					account_check = -1;
				else
					account_check = 1;
			//printf("%s", scanline);
		}
		if (account_check == 0)
		{
			printf("=> INVALID ACCOUNT/PASSWORD\n");
			strcpy(input_account, "NaN");
		}
		if (account_check == -1)
		{
			printf("=> NEGATIVE REMAINS TRANSECTION ABORT\n");
			account_check = 0;
		}
		fclose(fp);
	}
	return a;
}

char service_select_func()
{
	char myservice = '?';
	char input[256];
	while (myservice == '?')
	{
		printf("=> PLEASE CHOOSE YOUR SERVICE\n");
		printf("=> PRESS D FOR DEPOSIT\n");
		printf("=> PRESS W FOR WITHDRAWAL\n");
		printf("=> PRESS T FOR TRANSFER\n");
		scanf("%s", input);
		if (strcmp(input, "D") == 0)
			myservice = 'D';
		else if (strcmp(input, "W") == 0)
			myservice = 'W';
		else if (strcmp(input, "T") == 0)
			myservice = 'T';
		if (myservice == '?')
			printf("=> INVALID INPUT\n");
	}
	return myservice;
}

Account readline(char mystring[], Account my)
{
	strncpy(my.name, &mystring[0], 20);
	my.name[20] = '\0';
	strncpy(my.number, &mystring[20], 16);
	my.number[16] = '\0';
	strncpy(my.password, &mystring[36], 6);
	my.password[6] = '\0';
	strncpy(my.balance, &mystring[42], 16);
	my.balance[16] = '\0';
	return my;
}

void deposit_func(char atmnum, Account a, char myservice, int time)
{
	double myamount = 0.0;
	char input[256];
	while (! (myamount > 0 && myamount <= 99999.99)) 
	{
		printf("=> AMOUNT\n");
		scanf("\n%s", input);
		myamount = atof(input);
		myamount = (int) (myamount * 100)/100.00;
		if (! (myamount > 0 && myamount <= 99999.99))
			printf("=> INVALID VALUE\n");
	}
	addrecord(atmnum, a.number, a.number, myservice, myamount, time);
}

void withdrawal_func(char atmnum, Account a, char myservice, int time)
{
	double myamount = 0.0;
	char input[256];
	while (! (myamount > 0 && myamount <= 99999.99 && myamount <= atof(a.balance)/100)) 
	{
		printf("=> AMOUNT\n");
		scanf("\n%s", input);
		myamount = atof(input);
		myamount = (int) (myamount * 100)/100.00;
		if (!(myamount <= atof(a.balance)/100))
			printf("=> INSUFFICIENT BALANCE\n");
		else if (! (myamount > 0 && myamount <= 99999.99))
			printf("=> INVALID VALUE\n");
	}
	addrecord(atmnum, a.number, a.number, myservice, myamount, time);
}

void transaction_func(char atmnum, Account a, char myservice, int time)
{
	int account_check = 0;
	Account b;
	FILE *fp;
	char input[256], scanline[256];
	double myamount = 0.0;
	while (account_check == 0)
	{
		printf("=> TARGET ACCOUNT\n");
		scanf("\n%s", input);
		if (strcmp(input, a.number) == 0)
			printf("=> YOU CANNOT TRANSFER TO YOURSELF\n");
		else
		{
			fp = fopen("master.txt", "r");
			while (fgets(scanline, 99, fp) != NULL && account_check == 0)
			{
				b = readline(scanline, b);
				if (strcmp(b.number, input) == 0)
					account_check = 1;
			}
			if (account_check == 0)
				printf("=> TARGET ACCOUNT DOES NOT EXIST\n");
			fclose(fp);
		}
	}
			
	while (! (myamount > 0 && myamount <= 99999.99 && myamount <= atof(a.balance)/100)) 
	{
		printf("=> AMOUNT\n");
		scanf("\n%s", input);
		myamount = atof(input);
		myamount = (int) (myamount * 100)/100.00;
		//printf("%lf %lf %d\n", myamount, atof(a.balance)/100), !myamount <= atof(a.balance)/100;
		if (!(myamount <= atof(a.balance)/100))
			printf("=> INSUFFICIENT BALANCE\n");
		else if (! (myamount > 0 && myamount <= 99999.99))
			printf("=> INVALID VALUE\n");
	}
	addrecord(atmnum, a.number, b.number, 'W', myamount, time);
	addrecord(atmnum, b.number, a.number, 'D', myamount, time + 1);	
}

void addrecord(char atmnum, char accnum1[], char accnum2[], char action, double amount, int time)
{
		char output_amount[8];
		int pow10[8] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000};
		int intamount = (int) (amount * 100);
		for (int i = 0; i < 7; i++)
		{
			output_amount[i] = (intamount % pow10[7-i] / pow10[6-i]) + '0';
		}
		output_amount[7] = '\0';
		FILE *fp;
		if (atmnum == '1')
			fp = fopen("trans711.txt", "a");
		else
			fp = fopen("trans713.txt", "a");
		fprintf(fp, "%s%c%s%d%d%d%d%d\n", accnum1, action, output_amount, time/10000, time%10000/1000, time%1000/100, time%100/10, time%10);
		fclose(fp);
}

char ask_continue_func()
{
	char ask_continue = '?';
	while (ask_continue != 'Y' && ask_continue != 'N')
	{	
		printf("=> CONTINUE?\n");
		printf("=> N FOR NO\n");
		printf("=> Y FOR YES\n");
		scanf("\n%c", &ask_continue);
		if (ask_continue != 'Y' && ask_continue != 'N')
			printf("=> INVALID INPUT\n");
	}
	return ask_continue;
}
