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
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"sort.c"

typedef struct master
	{
		char name[21];
	    char number[17];
		char password[7];
		double balance;
	} Master;

typedef struct mytransaction
	{
		char number[17];
		char action;
		double amount;
	} Transaction;
	
void sorting_func();
void merging_func();
void update_func();
Master master_readline(char mystring[]);
Transaction transaction_readline(char mystring[]);
const char* master_writeline(Master my, int writemode);

int main()
{
	sorting_func();
	merging_func();
	update_func();
	return 0;
}

void sorting_func()
{
	sort_transaction("trans711.txt", "transac_sorted711.txt");
	sort_transaction("trans713.txt", "transac_sorted713.txt");
}

void merging_func()
{
	FILE *fp711;
	FILE *fp713;
	FILE *fp;
	fp711 = fopen("transac_sorted711.txt", "r");
	fp713 = fopen("transac_sorted713.txt", "r");
	fp = fopen("transac_sorted.txt", "w");
	char copy_string[256];
	while (fgets(copy_string, 99, fp711) != NULL)
	{
		fprintf(fp, "%s", copy_string);
	}
	while (fgets(copy_string, 99, fp713) != NULL)
		fprintf(fp, "%s", copy_string);
	fclose(fp711);
	fclose(fp713);
	fclose(fp);
	sort_transaction("transac_sorted.txt", "transac_sorted.txt");
}

void update_func()
{
	FILE *fpsort;
	FILE *fpmaster;
	FILE *fpmaster_updated;
	FILE *fpneg;
	char scanline[256];
	Transaction curr_transaction;
	Master curr_master;
	fpsort = fopen("transac_sorted.txt", "r");
	fpmaster = fopen("master.txt", "r");
	fpmaster_updated = fopen("master_updated.txt", "w");
	fpneg = fopen("negReport.txt", "w");
	fgets(scanline, 99, fpmaster);
	curr_master = master_readline(scanline);
	//printf("%s %s %s %lf\n", curr_master.name, curr_master.number, curr_master.password, curr_master.balance);
	while (fgets(scanline, 99, fpsort) != NULL)
	{
		curr_transaction = transaction_readline(scanline);
		//printf("%s %c %lf\n", curr_transaction.number, curr_transaction.action, curr_transaction.amount);
		while (strcmp(curr_transaction.number, curr_master.number) != 0)
		{
			if (curr_master.balance < 0)
				fprintf(fpneg, "%s\n", master_writeline(curr_master, 1));
			fprintf(fpmaster_updated, "%s\n", master_writeline(curr_master, 0));
			fgets(scanline, 99, fpmaster);
			curr_master = master_readline(scanline);
		}
		if (curr_transaction.action == 'D')
			curr_master.balance += curr_transaction.amount;
		if (curr_transaction.action == 'W')
			curr_master.balance -= curr_transaction.amount;
		//master_writeline(curr_master);
		//printf("%s %s %s %lf\n", curr_master.name, curr_master.number, curr_master.password, curr_master.balance);
	}
	do
	{
		if (curr_master.balance < 0)
			fprintf(fpneg, "%s\n", master_writeline(curr_master, 1));
		fprintf(fpmaster_updated, "%s\n", master_writeline(curr_master, 0));
	} while (fgets(scanline, 99, fpsort) != NULL);
	fclose(fpsort);
	fclose(fpmaster);
	fclose(fpmaster_updated);
	fclose(fpneg);
}

Master master_readline(char mystring[])
{
	Master my;
	char balance_string[17];
	strncpy(my.name, &mystring[0], 20);
	my.name[20] = '\0';
	strncpy(my.number, &mystring[20], 16);
	my.number[16] = '\0';
	strncpy(my.password, &mystring[36], 6);
	my.password[6] = '\0';
	strncpy(balance_string, &mystring[42], 16);
	balance_string[16] = '\0';
	my.balance = atoll(balance_string)/100.0;
	return my;
}

Transaction transaction_readline(char mystring[])
{
	Transaction curr_transaction;
	char amount_string[8];
	strncpy(curr_transaction.number, &mystring[0], 16);
	curr_transaction.number[16] = '\0';
	curr_transaction.action = mystring[16];
	strncpy(amount_string, &mystring[17], 7);
	amount_string[7] = '\0';
	curr_transaction.amount = atoi(amount_string)/100.0;
	return curr_transaction;
}

const char* master_writeline(Master my, int writemode)
{
	char* balance_string = malloc(sizeof(char) * 17);
	char* master_string = malloc(sizeof(char) * 100);
	master_string[0] = '\0';
	if (my.balance < 0)
		balance_string[0] = '-';
	else
		balance_string[0] = '+';
	long long int balance_int = (long long int) (my.balance * 100);
	if (balance_int < 0)
		balance_int = -balance_int;
	long long int pow10[15] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000
		, 10000000000, 100000000000, 1000000000000, 10000000000000, 100000000000000};
	for (int i = 1; i <= 15; i++)
		balance_string[i] = balance_int % pow10[16-i] / pow10[15-i] + '0';
	balance_string[16] = '\0';
	if (writemode == 0)
	{
		strcat(master_string, my.name);
		strcat(master_string, my.number);
		strcat(master_string, my.password);
		strcat(master_string, balance_string);
		master_string[58] = '\0';
	}
	//printf("%s\n", master_string);
	if (writemode == 1)
	{
		strcat(master_string, "Name: ");
		strcat(master_string, my.name);
		strcat(master_string, " Account Number: ");
		strcat(master_string, my.number);
		strcat(master_string, " Balance: ");
		strcat(master_string, balance_string);
		master_string[85] = '\0';
	}
	return master_string;
}
