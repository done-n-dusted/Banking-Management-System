# Banking Management System

## Author : Anurag Pendyala

### Files Description
1. `server.c` 		-> code for server
2. `client.c`		-> code for client
3. `initialize.c` 	-> code for creating one joint user and one normal user to get started with
4. `data/NU.data` 	-> data about normal users
5. `data/JU.data`	-> data about joint users

### Types of Users

* Administrator
	- Can add, delete, modify existing data.
	- Search for users in the database.

* Normal Users
	- Have fields that are unique to one Normal user.
* Joint Users
	- Have fields that are unique to Joint user account. The main difference between Normal and Joint user account is that the Joint User account has an addition partner name included.

### Properties of fields for accounts

* Account Number (Starts from 0)
* Name (Only for Normal User Accounts)
* Name1 & Name2 (Only for Joint User Accounts)
* Password (A 10 character long unique password)
* Balance
* Status (Whether the account is ACTIVE or CLOSED)

### Properties for Administrator

* There can be only one administrator.
* Password: `1234567890`
	
### Running the code

#### In the cloned repository,

Run the following block for running the server. Server should be running before the client can start working.
```
$cc server.c -o server && ./server
```

Run the following block for running the client.
```
$cc client.c -o client && ./client
```

Run the following block to initialize some data in the datafiles.
```
$cc initalize.c -o initialize && ./initialize
```

### Features available for the users

* Deposit
* Withdraw
* Balance Enquiry
* Change Password

### Features for the Administrator

* Add account
* Delete account
* Modify account
* Search for account

### Things to be noted

* The server can have multiple client connections simultaneously. This was achieved using fork() function.
* The connection between client and server is established using socket programming at IP address `127.0.0.1` and port `8000`.
* The server implements appropriate file locks wherever necessary.
* The program stores the data structure as a whole in the files and not the fields separately. 

### Extension

The server could be running in a remote server in the cloud with the data files. In that case, with a little tweaking of IP address and Port number, a remote connection can be maintained. Running the initialize command in the server will set up the data to begin with.