#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
	

#ifdef _WIN32
#include <windows.h> // Ðoi voi Windows
#else
#include <unistd.h> // Ðoi voi Unix/Linux
#endif


#define WIDTH_CONSOLE 120
#define MAX_ACCOUNTS 100
#define MAX_LOGIN_ATTEMPTS 5
#define MAX_TRANSACTIONS 10 // Gioi han so lan giao dich
#define FILENAME_SIZE 150 
#define FILENAME_SIZE 150  // Kích thu?c c?a chu?i d? luu tên file và du?ng d?n
#define MAX_AMOUNTS 20000000
#define MIN_AMOUNTS 50000
#define MAX_LINE_LENGTH 256 //Kich thuoc toi da cua 1 line trong file

int start = 0;
char password_ad[] = "0000";

typedef struct {
    char username[50];
    char password[50];
    char nameUser[50];
    float balance;
    int isLocked;
    int loginAttempts;
    int transactionCount; // S? l?n giao d?ch
} Account;

// Struct de luu tru thông tin cua moi giao dich
typedef struct {
    char account_number[50];
    char type[50];
    float amount;
} Transaction;

void title();
void titleEnd();
void main_bgr();
void showMenu(); 
void showBlance(Account *accounts, int i);
void waitForClear();
void enterAccount(char *username);
void enterPassword(char *password);
void continue_gd();
void pressEnterForClear();
void clearInputBuffer();
void replaceUnderscoresWithSpace(char *str);
void deReplaceUnderscoresWithSpace(char *str);

int checkPassword_admin();
int check_maxLogin(int count);
void readAccountFile(Account *accounts, int *numAccounts);
void writeAccountFile(Account *accounts, int numAccounts);

void performTransaction(Account *accounts, Transaction *transactions,int *numTransactions,int numAccounts,char *username,char *password,int *reloadUser);

void menuAmount();
int chooseAmount(float *amount);
void getInputAmount(float* amount);
float getValidAmountForWithdraw(float maxAmount);
float getValidAmountForDeposit();

void deposit(Account *accounts, Transaction *transactions,int *numTransactions, int numAccounts,char *username,char* password,int *reloadUser);
void withdraw(Account *accounts, Transaction *transactions,int *numTransactions, int numAccounts,char *username,char *password,int *reloadUser);
void checkBalance(Account *accounts, Transaction *transactions,int *numTransactions, int numAccounts,char *username,char* password,int *reloadUser);
void save_transactions(Transaction *transactions, int num_transactions);
void addTransaction(Transaction *transactions, int *numTransactions, char *username, char *type, float amount);
int isEndTransaction();

int is_admin(char* username);
int is_trueUser(Account accounts[], int low, int high, char* username);
int is_truePass(Account *accounts, char *password, int index);
int check_username(Account accounts[], char* username,int numAccounts,int *index);
int check_password(Account *accounts, char *password,int numAccounts, int *index,int* reloadUser);
int check_account(Account *accounts,char *username, char *password, int numAccounts,int *reloadUser);
void resetUserAccount(char *username, char* password);

int main(){
	int numTransactions = 0;
	int numAccounts = 0;  
	char username[50];
	char password[50];
	
	Account *accounts = malloc(100 * sizeof(Account)); // Cap phat bo nho cho 100 tai khoan ban dau
	Transaction *transactions=malloc(sizeof(Transaction)*10);
	
	title();
    // Nhap mat khau nhan vien IT de khoi dong may
	checkPassword_admin();
    // Doc file
    readAccountFile(accounts, &numAccounts);
	//Thuc hien giao dich 
    while(start!=2){
    	int reloadUser = 1;
    	check_account(accounts,username,password,numAccounts,&reloadUser);
    	while(reloadUser && start!=2)
    		performTransaction(accounts, transactions,&numTransactions, numAccounts, username, password,&reloadUser);
//    	showMenuAndProcessChoice(accounts, numAccounts, username, password);
		resetUserAccount(username,password);
		
		printf("user %s",username);
		printf("\npass %s",password);
		system("cls");
		titleEnd();
	}
	printf("\n so %d",numTransactions); 
	save_transactions(transactions,numTransactions);
    return 0;
} 

//Giao dien ban dau
void title() {
	printf("\n\n");
	printf("\033[1;34m");
	printf("                    %c%c%c%c%c                                                                       %c%c%c%c%c                   \n",201,205,205,205,205,205,205,205,205,187);
	printf("                    %c                                                                               %c\n\n",186,186);
	printf("                                              PBL1: DO AN LAP TRINH TINH TOAN                        \n");
    printf("                                DE TAI 701: XAY DUNG UNG DUNG THUC HIEN GIAO DICH TREN ATM           \n");
	printf("                                                                                                     \n\n");
    printf("                                          Giang Vien Huong Dan:   Tran Ho Thuy Tien                  \n");
    printf("                                          Sinh Vien Thuc Hien :   Huynh Thao Nhi                     \n");
    printf("                                                                  Nguyen Thi Trang                   \n");
    printf("                                                                  Nguyen Le Minh Vu                  \n\n");
	printf("                    %c                                                                               %c\n",186,186);
	printf("                    %c%c%c%c%c                                                                       %c%c%c%c%c\n",200,205,205,205,205,205,205,205,205,188);
	printf("");
	printf("                    _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _          \n\n");
	printf("\033[0m");
	printf("\n\n");

    printf("\n");
    printf("\n\n");  
    printf("                                          VUI LONG NHAP MAT KHAU DE KHOI DONG ATM                      \n");
    printf("                         ---                  Enter it password to start ATM                  ---  \n\n\n\n");
    printf("\n                         Password:");
    
}
// Giao dien khi thuc hien cac giao dich
void main_bgr(){
	printf("\033[1;33m");
	printf("                       _____________________    ________  .__________________ ___  .____  __._____.___.\n");
	printf("                      /  _  \\__    ___/     \\   \\______ \\ |   \\_   _____/    |   \\ |    |/ _|\\__  |   |\n");
	printf("                     /  /_\\  \\|    | /  \\ /  \\   |    |  \\|   ||    __)_|    |   / |      <   /   |   |\n");
	printf("                    /    |    \\    |/    Y    \\  |    `   \\   ||        \\    |  /  |    |  \\  \\____   |\n");
	printf("                    \\____|__  /____|\\____|__  / /_______  /___/_______  /______/   |____|__ \\ / ______|\n")  ;
	printf("                            \\/              \\/          \\/            \\/                   \\/ \\/       \n")  ;
	printf("\033[0m");
    printf("\n\n"); 
}
//Ham cho xoa man hinh
void waitForClear(){
    #ifdef _WIN32
	Sleep(1500); // Trên Windows, thoi gian tinh bang mili giay (ms)
	#else
	sleep(1.5); // Trên Unix/Linux, thoi gian tinh bang giay
	#endif
    system("cls");
}
//Nhap so tai khoan 
void enterAccount(char *username){
    scanf("%s", username);
}
//Nhap mat khau
void enterPassword(char *password){
    char ch;
    int i = 0;
	
    // In ki tu nhap va
    while (1) {
        ch = getch(); // Lay ky tu ma khong hien
        if (ch == 13) // 13 là mã ASCII cua phím Enter
            break;
        else if (ch == 8 && i > 0) { // 8 la ma ASCII cua phim Backspace
            printf("\b \b"); // Xoa ki tu da nhap va dich con tro ve phia truoc
            i--;
        }
        else if (i < 50 - 1) { // Cho phep nhap toi da 
            password[i++] = ch;
            printf("*"); //Hien * thay vi ky tu chu
        }
    }
    password[i] = '\0';
}

int checkPassword_admin(){
    //Ham xac nhan admin
	char pass_check[50];
	enterPassword(pass_check);
    while(strcmp(pass_check, password_ad)){
    	system("cls");	
		printf("\n\n\n");
		printf("                                    *- - - - - - - - - - - -  - - - - - - - - - - -*\n");
		printf("                                    |                                              |\n");
	    printf("                                    *  MAT KHAU NHAP VAO SAI!!!Vui long nhap lai.  *\n");
	    printf("                                    *   INCORRECT PASSWORD!!! Please re-enter      *\n");
	    printf("                                    |                                              |\n");
    	printf("                                    *- - - - - - - - - - - -  - - - - - - - - - - -*\n");
    	printf("\n                                                         Password:");
		enterPassword(pass_check);
	}
	
	++start; //Neu mat khau dung thi tang bien dem dung chuong trinh len 
	system("cls");
		printf("\n\n\n");
		printf("                                    *- - - - - - - - - - - -  - - - - - - - - - - -* \n");
		printf("                                    |                                              |\n");
	    printf("                                    *  MAT KHAU NHAP VAO DUNG!!! Dang khoi dong... *\n");
	    printf("                                    *       CORRECT PASSWORD!!! Starting...        *\n");
	    printf("                                    |                                              |\n");
    	printf("                                    *- - - - - - - - - - - -  - - - - - - - - - - -* \n");
	waitForClear();  
}
void showMenu(){
	printf("\033[1;33m");
	printf("\n");
	printf("                                                                                                     \n");
	printf("                                               VUI LONG LUA CHON GIAO DICH                          \n");
    printf("                                     ---             Transaction Menu            ---           \n");
	printf("                                                                                                     \n\n\n\n");
	printf("");
	printf("\033[0m");
    printf("                           1. Withdraw                                            2. Deposit\n");
    printf("\n");
    printf("                           3. Check Balance                                       0. Exit\n");
    printf("\n");
    printf("                           ");
} 

int check_maxLogin(int count){
	return count >= MAX_LOGIN_ATTEMPTS;
}

int is_admin(char* username){
	return strcmp(username,"0000");
}

int is_trueUser(Account accounts[], int low, int high, char* username) {
    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        // So sanh ten nguoi dung tai vi tri mid voi username
        int cmp = strcmp(accounts[mid].username, username);
        
        // Neu tim thay tra ve vi tri nguoi dung
        if (cmp == 0) {
            return mid;
        }
        
		//Neu ten nguoi dung tai mid lon hon username, tiep tuc tim phia ben trai
	        else if (cmp > 0) {
	            high = mid - 1;
	        }
        // Neu ten nguoi dung tai vi tri mid nho hon user name, tiep tuc tim phia ben phai 
		        else {
		            low = mid + 1;
		        }
    }   
    // Tra ve -1 neu khong tim thay
    return -1;
}
int is_truePass(Account *accounts, char *password, int index){
	return strcmp(accounts[index].password,password);
}

int check_username(Account accounts[], char* username, int numAccounts, int *index) {
    // Kiem tra admin
    if (!is_admin(username)) {
        ++start;
        return 0;
    }
    do {
        // Kiem tra admin khi nhap lai
        if (!is_admin(username)) {
            ++start;
            return 0;
        }
        if(!strcmp(username,"end")) return -1;
        // Kiem tra nguoi dung co ton tai khong
        *index = is_trueUser(accounts, 0, numAccounts - 1, username);
        if (*index != -1) {
            // Tim thay
            return 1;
        }
        // Khong tim thay
        printf("\n\033[0;31m                         Khong tim thay tai khoan!!! Vui long nhap lai\n");
        printf("                         Hoac nhap end de ket thuc giao dich\n\033[0m");
        printf("\n                         Username: ");
        enterAccount(username);

    } while (*index == -1);

    return 1;
}


int check_password(Account *accounts, char *password, int numAccounts, int *index,int *reloadUser){
	if(*index != -1){
		int count = 1;
		while(is_truePass(accounts,password,*index) && count<=4){
			printf("\n\n\033[0;31m                         Mat khau sai!!! Vui long nhap lai\n");
			printf("                         Hoac nhap end de ket thuc giao dich\n\033[0m");
        	printf("\n                         Password: ");
			enterPassword(password);
			if(!strcmp(password,"end")) return -1;
			count++;
		}
		if (check_maxLogin(count)) {
            accounts[*index].isLocked = 1;
            *reloadUser = 0;
            system("cls");
            
            writeAccountFile(accounts, numAccounts);
//            waitForClear();
            return 0;
        }
		accounts[*index].loginAttempts = 0; // Reset so lan dang nhap sai
		return 1;
	}
}
int check_account(Account *accounts,char *username, char *password, int numAccounts,int* reloadUser){
	restart_user:
		system("cls");
		main_bgr();
		printf("\033[1;33m");
		printf("\n\n");  
	    printf("                                        XAC NHAN TAI KHOAN DE THUC HIEN GIAO DICH                      \n");
	    printf("                         ---          Confirm your account to make the transaction          ---  \n\n\n\n");
		printf("\033[0m");
		int index;	
		printf("\n                         Username: ");
		enterAccount(username);
		if(check_username(accounts,username,numAccounts,&index) == -1) {
			system("cls");
			titleEnd();
//			waitForClear();
			goto restart_user;
		}
		
		if(start == 2) return 0;//Neesu la admin thi thoat
		
		printf("\n                         Password: ");
	    enterPassword(password);
		if(check_password(accounts,password,numAccounts,&index,reloadUser) == -1) {
			system("cls");
			titleEnd();
//			waitForClear();
			goto restart_user;
		}
		if(accounts[index].isLocked){
			*reloadUser = 0;	
			printf("\n\n\033[0;31m                         Tai khoan bi khoa, vui long lien he ho tro khach hang.\n\033[0m");
			waitForClear();
		}
		index = 0;	
		system("cls");
		return 1;
}

int isEndTransaction(){
	int choice;
	continue_gd();
	do
	{
	scanf("%d",&choice);
	if(choice == 0) return 0;
	if(choice == 1) return 1;
	printf("\n\033[0;31m                     Lua chon khong hop le, vui long thu lai.\n\n\033[0m");
	printf("                               ");
	}while(choice!=1 && choice!=0);
}

void readAccountFile(Account *accounts, int *numAccounts) {
    char temp_username[50]; 
    char temp_password[50];
    char temp_nameUser[50];
    float temp_balance; 
    int temp_isLocked;
    char line[MAX_LINE_LENGTH];
	
    FILE *file = fopen("accounts.dat", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    *numAccounts = 0;
    int capacity = MAX_ACCOUNTS; // Dung luong ban dau cua mang
    int increment = 5; // So luong tai khoan cap phat them moi lan

    // Doc thong tin tu file va cap phat khi can thiet
    while (fscanf(file, "%49s %49s %49s %f %d", temp_username, temp_nameUser, temp_password, &temp_balance, &temp_isLocked) == 5) {
        if (*numAccounts == capacity) {
            // Mo rong mang tai khoan bang relloc
            capacity += increment;
            accounts = realloc(accounts, capacity * sizeof(Account));
            if (accounts == NULL) {
                printf("Memory allocation failed.\n");
                exit(1);
            }
        }

        // Luu thong tin tai khoan vao mang
        strcpy(accounts[*numAccounts].username, temp_username);
        strcpy(accounts[*numAccounts].nameUser, temp_nameUser);
        replaceUnderscoresWithSpace(accounts[*numAccounts].nameUser);
        strcpy(accounts[*numAccounts].password, temp_password);
        accounts[*numAccounts].balance = temp_balance;
        accounts[*numAccounts].isLocked = temp_isLocked;

        (*numAccounts)++;
    }
    fclose(file);
}
	
void writeAccountFile(Account *accounts, int numAccounts) {
    FILE *file = fopen("accounts.dat", "w");
    if (file == NULL) {
        printf("Cannot open file.\n");
        exit(1);
    }
    int i;
    for (i = 0; i < numAccounts; i++) {
    	deReplaceUnderscoresWithSpace(accounts[i].nameUser);
        fprintf(file, "%s %s %s %.2f %d\n", accounts[i].username, accounts[i].nameUser, accounts[i].password, accounts[i].balance, accounts[i].isLocked);
    }
    fclose(file);
}


void performTransaction(Account *accounts, Transaction *transactions,int *numTransactions,int numAccounts,char *username,char *password,int *reloadUser) {	

    int choice;
    do {
    	main_bgr();
		showMenu();	
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                withdraw(accounts, transactions, numTransactions, numAccounts, username, password, reloadUser);
                break;
            case 2:
                deposit(accounts, transactions, numTransactions, numAccounts, username, password, reloadUser);
                break;
            case 3:
                checkBalance(accounts, transactions, numTransactions, numAccounts, username, password, reloadUser);
                break;
            case 0:
                *reloadUser = 0;
                system("cls");
                return;
            default:
                printf("\n\033[0;31m                           Lua chon khong hop le, vui long thu lai.\n\n\033[0m");
                break;
        }
    } while (choice != 0 && *reloadUser);
    system("cls");
}

float getValidAmountForWithdraw(float maxAmount) {
    float amount;
    menuAmount();
    if(chooseAmount(&amount) == -1) return -1.0;
    while (amount >maxAmount) {
    	system("cls");
        printf("                     So tien ban nhap lon hon han muc cua ngan hang, vui long nhap lai: ");
        waitForClear();
        clearInputBuffer();
        menuAmount();
        chooseAmount(&amount); 
    }
    clearInputBuffer();
    return amount;   
}

float getValidAmountForDeposit() {
    float amount;
    menuAmount();
    if(chooseAmount(&amount) == -1) return -1.0;
    clearInputBuffer();
    return amount;
}

void clearInputBuffer() {
    while (getchar() != '\n');
}

void addTransaction(Transaction *transactions, int *numTransactions, char *username, char *type, float amount) {
    int capacity = *numTransactions + 1;
    transactions = realloc(transactions, capacity * sizeof(Transaction));
    if (transactions == NULL) {
        printf("\n\nMemory allocation failed. Exiting.\n");
        exit(EXIT_FAILURE);
    }
    strcpy((transactions)[*numTransactions].account_number, username);
    strcpy((transactions)[*numTransactions].type, type);
    (transactions)[*numTransactions].amount = amount;
    (*numTransactions)++;
}

void deposit(Account *accounts, Transaction *transactions, int *numTransactions, int numAccounts, char *username, char *password, int *reloadUser) {
    int index = is_trueUser(accounts, 0, numAccounts - 1, username);

    if (index >= 0 && strcmp(accounts[index].password, password) == 0) {
        if (accounts[index].isLocked) {
            printf("\n\n\n                         Tai khoan bi khoa, vui long lien he ho tro khach hang.\n");
            *reloadUser = 0;
            return;
        }
        printf("\n\n                         So du cua ban: %.2f\n", accounts[index].balance);
        
        float amount = getValidAmountForDeposit();
        
    	if(amount == -1.0) return;
        
        if (amount > 0) {
            accounts[index].balance += amount;
            printf("\n                           Nap tien thanh cong. So du moi: %.2f\n", accounts[index].balance);
            writeAccountFile(accounts, numAccounts);            
            waitForClear();
            // Record transaction
            addTransaction(transactions, numTransactions, accounts[index].username, "Deposit", amount);
            
            
        } else {
            printf("Invalid amount.\n");
            waitForClear();
        }         
    } else {
        printf("\n\nInvalid username or password.\n");
        waitForClear();
    }
    *reloadUser = isEndTransaction();
    system("cls");
}

void withdraw(Account *accounts, Transaction *transactions,int *numTransactions, int numAccounts,char *username,char* password,int *reloadUser){
	int index = is_trueUser(accounts, 0, numAccounts - 1, username);

    if (index >= 0 && strcmp(accounts[index].password, password) == 0) {
        if (accounts[index].isLocked) {
            printf("\n\n                              Tai khoan bi khoa, vui long lien he ho tro khach hang.\n");
            *reloadUser = 0;
            return;
        }
        printf("Your balance: %.2f\n", accounts[index].balance);
        
        float amount = getValidAmountForWithdraw(accounts[index].balance);
        
        if(amount == -1.0) return;
        
        if (amount > 0 && amount <= accounts[index].balance) {
            accounts[index].balance -= amount;
            printf("\n                           Rut tien thanh cong. So du moi: %.2f\n", accounts[index].balance);
            writeAccountFile(accounts, numAccounts);            
            waitForClear();
            
            // Record transaction
            addTransaction(transactions, numTransactions, accounts[index].username, "Withdrawal", amount);
            
            *reloadUser = isEndTransaction();
            clearInputBuffer();
        } else {
            printf("Invalid amount.\n");
            waitForClear();
        }         
    } else {
        printf("Invalid username or password.\n");
        waitForClear();
    }
    system("cls");
}

void checkBalance(Account *accounts, Transaction *transactions, int *numTransactions, int numAccounts, char *username, char *password, int *reloadUser) {
    int index = is_trueUser(accounts, 0, numAccounts - 1, username);
    
    if (index >= 0 && strcmp(username, accounts[index].username) == 0 && strcmp(password, accounts[index].password) == 0) {
        if (accounts[index].isLocked) {
             printf("\n\n                         Your account is locked. Please contact the bank.\n");
            *reloadUser = 0;
            return;
        }

        showBlance(accounts, index);
        
        // Record transaction
        addTransaction(transactions, numTransactions, accounts[index].username, "Check Balance", 0);  
        pressEnterForClear();
    } else {
        printf("Invalid username or password.\n");
        waitForClear();
    }
    system("cls");
    *reloadUser = isEndTransaction();
    system("cls");
}




void save_transactions(Transaction *transactions,int numTransactions){
	char filename[FILENAME_SIZE];
    char full_path[FILENAME_SIZE];
    const char *folder_path = "C:/Users/NGUYEN THI TRANG/OneDrive/Documents/PBL1/Transaction";


    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // YYYY-MM-DD.txt
    strftime(filename, sizeof(filename), "%Y-%m-%d_%H-%M-%S.txt", timeinfo);

    // link from folder_path,filename
	snprintf(full_path, sizeof(full_path), "%s/%s", folder_path, filename);
	
    // new file

    FILE *file = fopen(full_path, "w");
    if (file != NULL) {
        int i;
    	for (i = 0; i < numTransactions; i++) {
        	fprintf(file, "%s %.2f %s\n", transactions[i].account_number, transactions[i].amount,transactions[i].type);
    	}
        fclose(file);
        printf("END _%Y-%m-%d_\n", timeinfo);
    } else {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
}

void continue_gd(){
	printf("\033[1;33m");
	printf("                       _____________________    ________  .__________________ ___  .____  __._____.___.\n");
	printf("                      /  _  \\__    ___/     \\   \\______ \\ |   \\_   _____/    |   \\ |    |/ _|\\__  |   |\n");
	printf("                     /  /_\\  \\|    | /  \\ /  \\   |    |  \\|   ||    __)_|    |   / |      <   /   |   |\n");
	printf("                    /    |    \\    |/    Y    \\  |    `   \\   ||        \\    |  /  |    |  \\  \\____   |\n");
	printf("                    \\____|__  /____|\\____|__  / /_______  /___/_______  /______/   |____|__ \\ / ______|\n")  ;
	printf("                            \\/              \\/          \\/            \\/                   \\/ \\/       \n")  ;
	printf("\n\n");
	printf("\033[1;34m");
	printf("                    %c%c%c%c%c                                                                      %c%c%c%c%c                    \n",201,205,205,205,205,205,205,205,205,187);
	printf("                    %c                                                                              %c                    \n",186,186);
	printf("\n\n                                       BAN CO MUON THUC HIEN GIAO DICH MOI KHONG?   \n");
	printf("                                         Do you want to make a new transaction?     \n\n\n");
	printf("                    %c                                                                              %c                    \n",186,186);
	printf("                    %c%c%c%c%c                                                                      %c%c%c%c%c                    \n",200,205,205,205,205,205,205,205,205,188);
	printf("\033[0m");
	printf("\n\n");
	printf("                     Nhan 1 de tiep tuc thuc hien giao dich\n\n");
	printf("                     Hoac 0 de THOAT\n\n");
	printf("                     ");
}

void titleEnd(){
	printf("\n\n\n\n\n"); 
	printf("\033[1;33m");
	printf("                       _____________________    ________  .__________________ ___  .____  __._____.___.\n");
	printf("                      /  _  \\__    ___/     \\   \\______ \\ |   \\_   _____/    |   \\ |    |/ _|\\__  |   |\n");
	printf("                     /  /_\\  \\|    | /  \\ /  \\   |    |  \\|   ||    __)_|    |   / |      <   /   |   |\n");
	printf("                    /    |    \\    |/    Y    \\  |    `   \\   ||        \\    |  /  |    |  \\  \\____   |\n");
	printf("                    \\____|__  /____|\\____|__  / /_______  /___/_______  /______/   |____|__ \\ / ______|\n")  ;
	printf("                            \\/              \\/          \\/            \\/                   \\/ \\/       \n")  ;
	printf("\n\n");
	printf("\n\n");
	printf("                               CAM ON QUY KHACH DA SU DUNG DICH VU CUA NGAN HANG CHUNG TOI !!!\n");
	printf("                                        Thank you for using our bank's services !!!\n"); 
	printf("\033[1;34m");
	
	waitForClear();
	
}

void menuAmount(){
	system("cls");
	printf("\033[1;33m");
	printf("\n");
	printf("                                                                                                     \n");
	printf("                                           LUA CHON SO TIEN BAN MUON GIAO DICH                          \n");
    printf("                                ---             Select transaction amount            ---           \n");
	printf("                                                                                                     \n\n\n\n");
	printf("");
	printf("\033[0m");
    printf("                           1. 100000                                             2. 200000\n");
    printf("\n");
    printf("                           3. 500000                                             4. 1000000\n");
    printf("\n");
    printf("                           5. 2000000                                            6. 5000000\n");
    printf("\n");
    printf("                           7. Khac (Nhap so tien)                                0. Exit\n");
    printf("\n");
    printf("                           ");
}

int chooseAmount(float *amount) {
    int choice;
    do{
    	scanf("%d", &choice);
    	switch (choice) {
        case 1:
            *amount = 100000;
            return 1;
//            break;
        case 2:
            *amount = 200000;
            return 1;
//            break;
        case 3:
            *amount = 500000;
            return 1;
//            break;
        case 4:
            *amount = 1000000;
            return 1;
//            break;
        case 5:
            *amount = 2000000;
            return 1;
            break;
        case 6:
            *amount = 5000000;
            return 1;
//            break;
        case 7:
            getInputAmount(amount);
            if (*amount == -1) {
                // Exit if the user chooses to quit during the input
                printf("\n                           Tro ve man hinh chinh...\n");
                waitForClear();
                return -1;
            }
            return 1;
            break;
        case 0:
            system("cls");
            printf("\n                           Tro ve man hinh chinh...\n");
            waitForClear();
            return -1;
        default:
            printf("\n\033[0;31m                           Lua chon khong hop le, vui long thu lai.\n\n\033[0m");
            printf("                           ");
            break;
    	}
    } while(choice !=0);
	
    return 0; // Return 0 to indicate normal operation
}


void getInputAmount(float *amount) {
    char input[50];
    
    printf("\n                           Nhap vao so tien (hoac 'end' de thoat): ");
    scanf("%s", input);
    
    // Check if the input is 'q'
    if(!strcmp(input,"end")) {
        *amount = -1; // Indicate that the user wants to quit
        return;
    }

    // Convert the input to a float
    *amount = atof(input);
    
    if (*amount <= 0) {
        printf("\n\033[0;31m                           So tien khong hop le, vui long nhap so duong.\n\033[0m");
        getInputAmount(amount); // Recursive call to re-enter the amount
        return;
    }
    if (*amount > MAX_AMOUNTS) {
        printf("\n\033[0;31m                           So tien khong hop le, vui long nhap so nam trong han muc cua tai khoan.\n\033[0m");
        getInputAmount(amount); // Recursive call to re-enter the amount
        return;
    }
     if (fmod(*amount, MIN_AMOUNTS) != 0) {
        printf("\n\033[0;31m                           So tien khong hop le, vui long nhap so tien chia het cho 50000.\n\033[0m");
        getInputAmount(amount); // Recursive call to re-enter the amount
        return;
    }
}

void pressEnterForClear() {
    printf("\n\n                    Press Enter to continue...");
    while (getchar() != '\n'); // Wait for user to press Enter
    getchar(); // Clear the newline character from input buffer
}


//Thong bao khoa tai khoan
//Thong bao sai mat khau - tai khoan

void resetUserAccount(char *username, char* password){
	strcpy(username,"");
	strcpy(password,"");
} 

void replaceUnderscoresWithSpace(char *str) {
    while (*str != '\0') {
        if (*str == '_') {
            *str = ' '; // Thay th? d?u g?ch d�?i b?ng d?u c�ch
        }
        str++; // Di chuy?n �?n k? t? ti?p theo trong chu?i
    }
}

void deReplaceUnderscoresWithSpace(char *str) {
    while (*str != '\0') {
        if (*str == ' ') {
            *str = '_'; // Thay th? d?u g?ch d�?i b?ng d?u c�ch
        }
        str++; // Di chuy?n �?n k? t? ti?p theo trong chu?i
    }
}

void showBlance(Account *accounts,int i){
	system("cls");
	main_bgr();
	printf("\n\n\n");
	printf("\033[1;33m");
	printf("                                                    Thong tin tai khoan          \n");
    printf("                                      ----          Account information          ----\n\n\n\n");
    printf("\033[0;34m");
	printf("                       %c%c%c%c                                                                     %c%c%c%c\n",201,205,205,205,205,205,205,187);
	printf("                       %c                                                                           %c\n\n\n",186,186);
	printf("\033[1;34m") ;
	printf("                                           Account number:      %s \n\n",accounts[i].username);
	printf("                                            Customer name:      %s\n\n",accounts[0].nameUser);
	printf("                                          Current balance:      %0.2f\n",accounts[i].balance);
	printf("\033[0;34m");
	printf("\n\n                       %c                                                                           %c\n",186,186);
	printf("                       %c%c%c%c                                                                     %c%c%c%c\n",200,205,205,205,205,205,205,188);
	printf("\033[0m");
}














