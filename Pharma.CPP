/*
Pharmacy Management
*/
#include<iostream.h>
#include<fstream.h>
#include<conio.h>
#include<string.h>
#include<iomanip.h>
#include<stdio.h>
#include<process.h>
#include<ctype.h>

/*
user class
This Class uses users.dat to store info about users
*/
class user
{
  char uName[30], uPass[30];
  int isAdmin;//Admin = 1, User = 0

public:
  user()//Default Initilization
  {
    strcpy(uName, " \0");
    strcpy(uPass, " \0");
    isAdmin = 0;
  }
  char* get_uName()
  {
    return uName;
  }
  char* get_uPass()
  {
    return uPass;
  }
  int get_isAdmin()
  {
    return isAdmin;
  }
  int Edit(char N[], char P[], int ISADMIN);
  void view_all();
  int Reg(char N[], char P[], int ISADMIN);//Already exist = -1, Reg_Done = 0;
  int Login(char N[], char P[]);//if incorrect credentials -1, if admin = 1 if user = 0, nofile -2
};

int user::Login(char N[], char P[])
  {
    strcpy(uName, N);
    strcpy(uPass, P);

    user temp;
    ifstream u_iFile ;
    u_iFile.open("users.dat", ios::binary|ios::nocreate);
    if(u_iFile.good())
    {
      u_iFile.read((char*)&temp, sizeof(temp));
      while(! (u_iFile.eof()) )
      {
	if(! (strcmp(temp.uName, uName)) )
	{
	  if(! (strcmp(temp.uPass, uPass)) )
	    return temp.isAdmin;
	}
	u_iFile.read((char*)&temp, sizeof(temp));
      }
      return -1;
    }
    else
      return -2;
  }

int user::Reg(char N[], char P[], int ISADMIN)
  {
    strcpy(uName, N);
    strcpy(uPass, P);
    isAdmin = ISADMIN;

    user temp;

    fstream u_file;
    u_file.open("users.dat", ios::binary|ios::in|ios::out);

    u_file.read((char*)&temp, sizeof(temp));
    while(!u_file.fail()) //when fail() bit not set
    {
      if(!(strcmp(temp.uName, uName)))
      {
	return -1;
      }
      u_file.read((char*)&temp, sizeof(temp));
    }

    u_file.clear(); //clearing the fail bit
    u_file.seekp(u_file.tellg());

    u_file.write((char*)this, sizeof(*this));

    u_file.close();
    return 0;
  }


int user::Edit(char N[], char P[], int ISADMIN)
{
  user temp;
  fstream u_File ;
  u_File.open("users.dat", ios::binary|ios::in|ios::out|ios::nocreate);
  if(u_File.good())
  {
    u_File.read((char*)&temp, sizeof(temp));
    while(! (u_File.eof()) )
    {
      if(! (strcmp(temp.uName, uName)) )
      {
	u_File.seekp(u_File.tellg() - (sizeof(*this)));
	strcpy(uName, N);
	strcpy(uPass, P);
	isAdmin = ISADMIN;
	u_File.write((char*)this, sizeof(*this));
	u_File.close();
	return 1;
      }
    }
  }
  u_File.close();
  return 0;
}

void user::view_all()
{
  user T;
  cout.setf(ios::left);
  cout<<"\n\t"<<setw(30)<<"User Name"<<setw(30)<<"Password"<<setw(10)<<"Is Admin";
  ifstream file;
  file.open("users.dat", ios::binary|ios::nocreate);
  if(file.good())
  {
    file.read((char*)&T, sizeof(T));
    while(!file.eof())
    {
      cout<<"\n\t"<<setw(30)<<T.uName<<setw(30)<<T.uPass<<setw(10);
      if(T.isAdmin == 1)
	cout<<" YES";
      else
	cout<<" NO";
      file.read((char*)&T, sizeof(T));
    }
  }
}

class drug
{
  unsigned int dCode;
  char dName[50];

protected:
  int set_dCode(unsigned int dcode); // Found = 1, else = 0
public :
  int get_dCode()
  {
    return dCode;
  }
  char* get_dName()
  {
    return dName;
  }
  void drug_init();
  void view_all();
  int add(unsigned int, char[]);//success = 1, exist = -1
  int del(unsigned int);//success = 1, not found = -1

}drugs;

int drug::set_dCode(unsigned int dcode)
{
  dCode = dcode;
  drug temp;
  ifstream ifile;
  ifile.open("drug.dat", ios::binary|ios::nocreate);
  if(ifile.good())
  {
    ifile.read((char*)&temp, sizeof(temp));
    while(!ifile.eof())
    {
      if(temp.dCode == dCode)
      {
	strcpy(dName, temp.dName);
	return 1;
      }
      ifile.read((char*)&temp, sizeof(temp));
    }
  }
  return 0;
}

class stock
{
  drug Drug;
  unsigned int QTY;
public :
   void stock_init(drug T);
   void View();
   void Edit(unsigned int, int);
   int inStock(unsigned int);//Avail = 1;
}stocks;

int stock::inStock(unsigned int DrC)
{
  stock temp;
  ifstream file;
  file.open("stocks.dat", ios::binary|ios::nocreate);
  if(file.good())
  {
    file.read((char*)&temp, sizeof(temp));
    while(!file.eof())
    {
      if(DrC == temp.Drug.get_dCode())
      {
	if(temp.QTY > 0)
	{
	  file.close();
	  return 1;
	}
      }
      file.read((char*)&temp, sizeof(temp));
    }
  }
  file.close();
  return 0;
}

void stock::stock_init(drug T)
{
  ofstream ofile;
  ofile.open("stocks.dat", ios::binary|ios::app);
  Drug = T;
  QTY = 0;
  ofile.write((char*)this, sizeof(*this));
  ofile.close();
}

void stock::View()
{
  stock temp;
  cout.setf(ios::left);
  cout<<"\t"<<setw(15)<<"D. Code"<<setw(40)<<"D. Name"<<setw(5)<<"QTY";
  ifstream file;
  file.open("stocks.dat", ios::binary|ios::nocreate);
  if(file.good())
  {
    file.read((char*)&temp, sizeof(temp));
    while(!file.eof())
    {
      cout<<"\n\t "<<setw(15)<<temp.Drug.get_dCode()<<setw(40)<<temp.Drug.get_dName()<<setw(5)<<temp.QTY;
      file.read((char*)&temp, sizeof(temp));
    }
  }
}
void stock::Edit(unsigned int dcode, int qty)
{
  stock temp;
  fstream file;
  file.open("stocks.dat", ios::binary|ios::in|ios::out|ios::nocreate);
  if(file.good())
  {
    file.read((char*)&temp, sizeof(temp));
    while(!file.eof())
    {
      if(dcode == temp.Drug.get_dCode())
      {
	temp.QTY += qty;
	file.seekp(file.tellg() - sizeof(temp));
	file.write((char*)&temp, sizeof(temp));
	break;
      }
      file.read((char*)&temp, sizeof(temp));
    }
  }
  file.close();
}


int drug::del(unsigned int dcode)
{
  int fl = -1;
  drug temp;
  ifstream ifile;
  ofstream ofile;
  ifile.open("drug.dat", ios::binary|ios::nocreate);
  ofile.open("newdrug.dat", ios::binary|ios::noreplace|ios::app);
  if(ifile.good())
  {
    ifile.read((char*)&temp, sizeof(temp));
    while(!ifile.eof())
    {
      if(temp.dCode != dcode)
      {
	ofile.write((char*)&temp, sizeof(temp));
      }
      else
	fl = 1;
      ifile.read((char*)&temp, sizeof(temp));
    }
  }
  ifile.close();
  ofile.close();
  remove("drug.dat");
  rename("newdrug.dat", "drug.dat");
  return fl;
}


int drug::add(unsigned int dcode, char dname[])
{
  int fl = -1;//flag
  drug temp;
  ifstream ifile;
  ifile.open("drug.dat", ios::binary|ios::nocreate);
  if(ifile.good())
  {
    ifile.read((char*)&temp, sizeof(temp));
    while(!ifile.eof())
    {
      if(dcode == temp.dCode)
      {
	fl = -1;
	break;
      }
      else
	fl = 0;
      ifile.read((char*)&temp, sizeof(temp));
    }
    if(fl)
    {
      ifile.close();
      return -1;
    }
    else
    {
      ifile.close();
      ofstream ofile;
      ofile.open("drug.dat", ios::binary|ios::nocreate|ios::app);

      temp.dCode = dcode;
      for(int w = 0; w<strlen(dname); w++)
	dname[w] = toupper(dname[w]);
      strcpy(temp.dName, dname);
      ofile.write((char*)&temp, sizeof(temp));
      stocks.stock_init(temp);
      ofile.close();
      return 0;
    }
  }
  return -1;
}
void drug::view_all()
{
  int rec = 0;
  drug temp;
  ifstream file;
  file.open("drug.dat", ios::binary|ios::nocreate);
  if(file.good())
  {
    cout.setf(ios::left);
    cout<<"\n\t"<<setw(20)<<"Drug Code"<<setw(50)<<"Drug Name";
    cout<<"\n\n";
    file.read((char*)&temp, sizeof(temp));
    while(!file.eof())
    {
      cout<<"\t"<<setw(20)<<temp.dCode<<setw(50)<<temp.dName<<"\n";
      ++rec;
      if(rec == 12 || ((rec%20) == 0))
      {
	cout<<"\nPress any key to view More....";
	getch();
	clrscr();
      }
      file.read((char*)&temp, sizeof(temp));
    }
  }
  file.close();
}

void drug::drug_init()
{
  drug temp;
  ofstream file;
  file.open("drug.dat", ios::binary);
  temp.dCode = 1;
  strcpy(temp.dName, "FLEXON");
  file.write((char*)&temp, sizeof(temp));
  stocks.stock_init(temp);

  temp.dCode = 2;
  strcpy(temp.dName, "IBUACT");
  file.write((char*)&temp, sizeof(temp));
  stocks.stock_init(temp);

  temp.dCode = 3;
  strcpy(temp.dName, "NORFLOX - 400");
  file.write((char*)&temp, sizeof(temp));
  stocks.stock_init(temp);

  temp.dCode = 4;
  strcpy(temp.dName, "PANADOL");
  file.write((char*)&temp, sizeof(temp));
  stocks.stock_init(temp);

  temp.dCode = 5;
  strcpy(temp.dName, "GABANTIN PLUS");
  file.write((char*)&temp, sizeof(temp));
  stocks.stock_init(temp);

  file.close();
}

class inv_drug : protected drug
{
  unsigned int sno, qty;
  float MRP;
public:
  int set(unsigned int SNO, unsigned int dcode, int QTY, float mrp)// dcode !found = 0, found = 1
  {
    if(!set_dCode(dcode))
      return 0;
    sno = SNO;
    qty = QTY;
    MRP = mrp;
    return 1;
  }
  unsigned int get_dCode()
  {
    return (drug::get_dCode());
  }
  char* get_dName()
  {
    return (drug::get_dName());
  }
  unsigned int get_sno()
  {
    return sno;
  }
  unsigned int get_qty()
  {
    return qty;
  }
  float get_MRP()
  {
    return MRP;
  }
};

class invoice
{
  unsigned int Items, BNo;
  char Cust[40], Doc[40];
  inv_drug Inv[10];
protected :
  unsigned int get_BNo()
  {
    return BNo;
  }
  char* get_Cust()
  {
    return Cust;
  }
  char* get_Doc()
  {
    return Doc;
  }
  inv_drug* get_Inv()
  {
    return Inv;
  }
public :
  void show(unsigned int bno);
  void save(unsigned int bno, char cust[], char doc[], inv_drug inv[], unsigned int items);
  int get_new_bno();
  //void print();

}INV;

void invoice::show(unsigned int bno)
{
  invoice temp;
  ifstream file;
  file.open("invoice.dat", ios::binary|ios::nocreate);
  file.read((char*)&temp, sizeof(temp));
  int fn = 0;
  if(file.good())
  {
    while(!file.eof())
    {
      if(bno == temp.get_BNo())
      {
	fn = 1;
	break;
      }
      file.read((char*)&temp, sizeof(temp));
    }
  }

  if(fn)
  {
    float Total = 0;
    cout<<"\tBill No. : "<<temp.get_BNo();
    cout<<"\n\tCustomer Name : "<<temp.get_Cust();
    cout<<"\n\tDoctor Name : "<<temp.get_Doc();
    cout.setf(ios::left);
    cout<<"\n\t"<<setw(5)<<"SN"<<setw(30)<<"Particulars"<<setw(6)<<"QTY"<<setw(4)<<"MRP";
    inv_drug* pt = temp.get_Inv();
    for(int k = 0; k<temp.Items; ++k, ++pt)
    {
      cout.setf(ios::left);
      cout<<"\n\t"<<setw(5)<<pt->get_sno()<<setw(30)<<pt->get_dName()<<setw(6)<<pt->get_qty();
      cout.setf(ios::right);
      cout<<setw(4)<<pt->get_MRP();
      Total += pt->get_MRP();
    }
    cout<<"\n\n\tTotal : "<<Total;
  }
  else
    cout<<"\tNOT FOUND!!!!";
}
int invoice::get_new_bno()
{
  invoice temp;
  ifstream file;
  file.open("invoice.dat", ios::binary|ios::nocreate);
  file.read((char*)&temp, sizeof(temp));
  if(file.good())
  {
    while(!file.eof())
    {
    file.read((char*)&temp, sizeof(temp));
    }
    return(temp.BNo + 1);
  }
  else
    return 1;
}
void invoice::save(unsigned int bno, char cust[], char doc[], inv_drug inv[], unsigned int items)
{
  BNo = bno;
  Items = items;
  strcpy(Cust, cust);
  strcpy(Doc, doc);
  for(int i = 0; i<Items; i++)
  {
    Inv[i] = inv[i];
    stocks.Edit(Inv[i].get_dCode(), ((-1)*Inv[i].get_qty()));
  }
  ofstream ofs;
  ofs.open("invoice.dat", ios::binary|ios::app);
  ofs.write((char*)this, sizeof(*this));
  ofs.close();
}

class store
{
  char StName[30], RgNo[20], Owner[30], Add[50];
  int good;
public:
  store()
  {
    fstream file;
    file.clear();
    file.open("store.dat", ios::binary|ios::in|ios::nocreate);
    if(file.good())
    {
      file.read((char*)this, sizeof(*this));
      good = 1;
      file.close();
    }
    else
    {
      good = 0;
      file.close();
    }
  }

  int is_good()
  {
    return (good);
  }

  char* get_StName()
  {
    return StName;
  }

  void show();
  void set_store();
}StInfo;

void store::set_store()
{
  fstream st;
  st.open("store.dat", ios::binary|ios::out|ios::trunc);
  cout<<"\t\tEnter Store Name : ";
  cin.getline(StName, 30);
  cout<<"\t\tEnter Reg. No. : ";
  cin.getline(RgNo, 20);
  cout<<"\t\tEnter Name of Owner : ";
  cin.getline(Owner, 30);
  cout<<"\t\tEnter Address : ";
  cin.getline(Add, 50);
  good = 1;
  st.write((char*)this, sizeof(*this));
  st.close();
}

void store::show()
{
  cout<<"\n\t\tStore Name : "<<StName;
  cout<<"\n\t\tReg. No. : "<<RgNo;
  cout<<"\n\t\tOwner : "<<Owner;
  cout<<"\n\t\tAddress : "<<Add;
}


void wel_msg()
{
  clrscr();
  cout<<"\n\n\n\n\n\n\n";
  cout<<"\t__           __ ______   _       _____   ____    __  __  ______ \n";
  cout<<"\t\\ \\         / /|  ____| | |     / ____/ / __ \\  |  \\/  | | ____|\n";
  cout<<"\t \\ \\      / /  | |__    | |    | |     | |  | | | \\  / | | |__  \n";
  cout<<"\t  \\ \\/  \\/ /   |  __|   | |    | |     | |  | | | |\\/| | |  __| \n";
  cout<<"\t   \\  /\\  /    | |____  | |___ | |___  | |__| | | |  | | | |___ \n";
  cout<<"\t    \\/  \\/     |______| |_____| \\_____\\ \\____/  |_|  |_| |_____|\n";
}

void pass(char p[], int max)
{
  char ch;
  int i = 0;
  do
  {
    ch = getch();
    if(ch == 13)
      break;
    cout<<"*";
    p[i] = ch;
    ++i;
  }while(i < (max-1));
  p[i] = '\0';
}

void header(char acc[])
{
  clrscr();
  cout<<"\n\n|------------------------------------------------------------------------------|";
  int space = ((80 - strlen(StInfo.get_StName()))/2);
  for(int i = 1; i<=space; i++)
   cout<<' ';
  cout<<StInfo.get_StName();
  cout<<"\n|------------------------------------------------------------------------------|";
  cout.setf(ios::right);
  cout<<setw(65)<<"Current User : "<<acc;
  cout<<"\n|------------------------------------------------------------------------------|\n";
}

void line()
{
  cout<<"\n\n|------------------------------------------------------------------------------|\n";
}

void cin_flush()
{
  char a;
  do
  {
    cin.get(a);
  }while(a != '\n');
}


void main_admin(user &admin);
void main_user(user &User);

int main_init()
{
  if(!StInfo.is_good())
  {
    drugs.drug_init();
    user admin;
    wel_msg();
    cout<<"\n\n\n\n\n\nPress any key to continue......";
    getch();
    clrscr();
    cout<<"\n\n\t\tIt Seems you are starting this app for First time\n";
    cout<<"\t\tPlease do submit Store Info First.\n\n";
    StInfo.set_store();
    clrscr();
    cout<<"\n\n\n\n\n\n\t\tThis software needs an Administrator Account\n";
    cout<<"\t\tPress any key to create one.......";
    getch();
    clrscr();
    cout<<"\n\n\n\n\n\n\t\tHey There,\n";
    char uname[30], upass[30], p1[30], p2[30];
    cout<<"\t\tEnter User Name : ";
    cin.getline(uname, 30);
    while(1)
    {
      cout<<"\t\tEnter Password : ";
      pass(p1, 30);
      cout<<"\n\t\tConfirm Password : ";
      pass(p2, 30);
      if(!(strcmp(p1, p2)))
      {
	strcpy(upass, p1);
	if(!(admin.Reg(uname, upass, 1)))
	{
	  cout<<"\n\n\n\t\tRegistered successfully";
	  cout<<"\n\n\n\n\n\t\tPress any key to Login";
	  getch();
	  main_admin(admin);
	  return 1;
	}
      }
      else
      {
	cout<<"\n\n\n\n\n\n\t\tPassword NOT MATCHED. Press any key to Try Again.....";
	getch();
	clrscr();
	cout<<"\n\n\n\n\n\n";
      }
    }
  }
  return 0;
}

int main_login()
{
  user usr;
  char un[30], up[30];
  wel_msg();
  cout<<"\n\n\n\n                                       to\n";
  for(int I=0; I<((80 - strlen(StInfo.get_StName()))/2); I++)
      cout<<' ';
  cout<<StInfo.get_StName();
  cout<<"\n\n\n\n\n\nPress any key to continue......";
  getch();
  clrscr();
  cout<<"\n\n\n\n\n\n";
  cout<<"\t\t\t|-------------------------|\n";
  cout<<"\t\t\t           LOGIN\n";
  cout<<"\t\t\t|-------------------------|\n\n";
  cout<<"\t\t\t  User Name : ";
  cin.getline(un, 30);
  cout<<"\t\t\t  Password  : ";
  pass(up, 30);
  cout<<"\n\n\t\t\t|-------------------------|\n";
  int uType = usr.Login(un, up);
  if(uType == -1)
  {
    cout<<"\n\n\n\n\nUser Name OR Password is incorrect\n";
    cout<<"\nPress any key to try again.........";
    getch();
    main_login();
  }
  else if(uType == 1)
  {
    cout<<"\n\n\n\n\n\n\nLogin Successful!!\nPress any key to continue.....";
    getch();
    clrscr();
    main_admin(usr);
  }
  else if(uType == 0)
  {
    cout<<"\n\n\n\n\n\n\nLogin Successful!!\nPress any key to continue.....";
    getch();
    clrscr();
    main_user(usr);
  }
  return 0;
}

void main_admin(user &admin)
{
  int ch;
  do
  {
    header(admin.get_uName());
    cout<<"\t1. Sales\n\t2. Inventory\n\t3. Drug Management\n\t4. Accounts\n\t5. Store Info\n\t6. Logout\n\t7. EXIT\n\t8. Factory Reset";
    line();
    cout<<"Enter Your Choice : ";
    cin>>ch;
    cin_flush();
    switch(ch)
    {
      case 1 :
	int C;
	do
	{
	  header(admin.get_uName());
	  cout<<"\t1. New Sale\n\t2. View old Bill\n\t3. Back";
	  line();
	  cout<<"Enter Your Choice : ";
	  cin>>C;
	  cin_flush();
	  switch(C)
	  {
	    case 1 :
	      header(admin.get_uName());
	      inv_drug inv[10];
	      float Total = 0;
	      int i = 0;
	      unsigned int DCODE, QTY, MRP, BNO = INV.get_new_bno();
	      char CUST[40], DOC[40], D[50];
	      cout<<"\tEnter Customer Name : ";
	      cin.getline(CUST, 40);
	      cout<<"\tEnter Doctor Name : ";
	      cin.getline(DOC, 40);
	      int wh = 1;
	      do
	      {
		stock temp;
		cout<<"\tEnter Drug Code : ";
		cin>>DCODE;
		cin_flush();
		if(temp.inStock(DCODE))
		{
		  cout<<"\tEnter Quantity : ";
		  cin>>QTY;
		  cin_flush();
		  cout<<"\tEnter MRP : ";
		  cin>>MRP;
		  cin_flush();
		  if(inv[i].set((i+1), DCODE, QTY, MRP))
		  {
		    Total += inv[i].get_MRP();
		    do
		    {
		      cout<<"\n\tDo You Want to add more? Y/N";
		      char co;
		      co = getch();
		      if(co == 'y' || co == 'Y')
		      {
			++i;
			clrscr();
			break;
		      }
		      else if(co == 'n' || co == 'N')
		      {
			wh = 0;
			break;
		      }
		      else
		      {
			cout<<"\n Wrong KEY. Press any key to try again";
			clrscr();
		      }
		    }while(1);
		  }
		}
	      }while(wh == 1 && i<10);
	      header(admin.get_uName());
	      cout<<"\tBill No. : "<<BNO;
	      cout<<"\n\tCustomer Name : "<<CUST;
	      cout<<"\n\tDoctor Name : "<<DOC;
	      cout.setf(ios::left);
	      cout<<"\n\t"<<setw(5)<<"SN"<<setw(30)<<"Particulars"<<setw(6)<<"QTY"<<setw(4)<<"MRP";
	      for(int k = 0; k<(i+1); k++)
	      {
		cout.setf(ios::left);
		cout<<"\n\t"<<setw(5)<<inv[k].get_sno()<<setw(30)<<inv[k].get_dName()<<setw(6)<<inv[k].get_qty();
		cout.setf(ios::right);
		cout<<setw(4)<<inv[k].get_MRP();
	      }
	      cout<<"\n\n\tTotal : "<<Total;
	      line();
	      do
	      {
		cout<<"\nAmout Paid? Y/N";
		char co;
		co = getch();
		if(co == 'y' || co == 'Y')
		{
		  INV.save(BNO, CUST, DOC, inv, (i+1));
		  break;
		}
		else if(co == 'n' || co == 'N')
		{
		  wh = 0;
		  break;
		}
		else
		{
		  cout<<"\n Wrong KEY. Press any key to try again";
		  clrscr();
		}
	      }while(1);
	      break;
	    case 2 :
	      header(admin.get_uName());
	      unsigned int bno;
	      invoice temp;
	      cout<<"\tEnter Bill No. : ";
	      cin>>bno;
	      cin_flush();
	      header(admin.get_uName());
	      temp.show(bno);
	      line();
	      cout<<"\nPress any key to Go Back.....";
	      getch();
	      break;
	    case 3 :
	      break;
	    default :
	      cout<<"\nWrong Choice. Press any key to try again";
	      getch();
	      break;
	  }
	}while(C != 3);
	break;

      case 2 :
	int c;
	do
	{
	  header(admin.get_uName());
	  cout<<"\t1. View Stocks\n\t2. Add Stocks\n\t3. Remove Stocks\n\t4. Back";
	  line();
	  cout<<"Enter Your Choice : ";
	  cin>>c;
	  cin_flush();
	  switch(c)
	  {
	    case 1 :
	      header(admin.get_uName());
	      stocks.View();
	      line();
	      cout<<"Press any key to Go Back.....";
	      getch();
	      break;
	    case 2 :
	      header(admin.get_uName());
	      cout<<"\tEnter Drug Code : ";
	      unsigned int DC;
	      int QTY;
	      cin>>DC;
	      cout<<"\tEnter Qty to ADD : ";
	      cin>>QTY;
	      cin_flush();
	      stocks.Edit(DC, QTY);
	      line();
	      cout<<"Done Press any key to Go Back.....";
	      break;
	    case 3 :
	      header(admin.get_uName());
	      cout<<"\tEnter Drug Code : ";
	      unsigned int DCo;
	      int QTy;
	      cin>>DCo;
	      cout<<"\tEnter Qty to Remove : ";
	      cin>>QTy;
	      cin_flush();
	      QTy *= -1;
	      char fs;
	      cin.get(fs);
	      stocks.Edit(DCo, QTy);
	      line();
	      cout<<"Done Press any key to Go Back.....";
	      break;
	    case 4 :
	      break;
	    default :
	      cout<<"\nWrong Choice. Press any key to try again";
	      getch();
	      break;
	  }
	}while(c != 4);
	break;
      case 3 :
	int P;
	do
	{
	  header(admin.get_uName());
	  cout<<"\t1. View All Drugs\n\t2. Add a Drug\n\t3. Remove a Drug\n\t4. Back";
	  line();
	  cout<<"Enter Your Choice : ";
	  cin>>P;
	  cin_flush();
	  switch(P)
	  {
	    case 1 :
	      header(admin.get_uName());
	      drugs.view_all();
	      line();
	      cout<<"Press any key to Go Back.....";
	      getch();
	      break;
	    case 2 :
	      header(admin.get_uName());
	      unsigned int dco;
	      char dna[50];
	      cout<<"\n\tEnter New Drug Code : ";
	      cin>>dco;
	      cin_flush();
	      cout<<"\tEnter New Drug Name : ";
	      cin.getline(dna, 50);
	      line();
	      if(!drugs.add(dco, dna))
		cout<<"Success!!\nPress any key to go back....";
	      getch();
	      break;
	    case 3 :
	      header(admin.get_uName());
	      unsigned int Dco;
	      cout<<"Enter Drug Code to Delete : ";
	      cin>>Dco;
	      cin_flush();
	      line();
	      if(drugs.del(Dco))
		cout<<"Success!!\nPress any key to go back....";
	      else
		cout<<"NOT FOUND!!\nPress any key to go back....";
	      getch();
	      break;
	    case 4 :
	      break;
	    default :
	      cout<<"\nWrong Choice. Press any key to try again";
	      getch();
	      break;
	  }
	}while(P != 4);
	break;
      case 4 :
	int x;
	do
	{
	  header(admin.get_uName());
	  cout<<"\t1. Manage My Account\n\t2. Manage other account\n\t3. Back\n";
	  line();
	  cout<<"Enter Your Choice : ";
	  cin>>x;
	  cin_flush();
	  switch(x)
	  {
	    case 1 :
	      int l;
	      do
	      {
		header(admin.get_uName());
		cout<<"\t1. View My Account\n\t2. Change My Account\n\t3. Back\n";
		line();
		cout<<"Enter Your Choice : ";
		cin>>l;
		cin_flush();
		switch(l)
		{
		  case 1 :
		    header(admin.get_uName());
		    cout<<"\n\t\tMy User Name : "<<admin.get_uName();
		    cout<<"\n\t\tMy Password : "<<admin.get_uPass();
		    cout<<"\n\t\tAccount Type : ";
		    if(admin.get_isAdmin())
		      cout<<"Admin";
		    else
		      cout<<"Non - Admin";
		    line();
		    cout<<"Press any key to Go Back.....";
		    getch();
		    break;
		  case 2 :
		    char us[30], np1[30], np2[30];
		    header(admin.get_uName());
		    cout<<"\n\t\tEnter New User Name : ";
		    cin.getline(us, 30);
		    do
		    {
		      cout<<"\t\tEnter New Password : ";
		      pass(np1, 30);
		      cout<<"\n\t\tConfirm Password : ";
		      pass(np2, 30);
		      if(!strcmp(np1, np2))
		      {
			if(admin.Edit(us, np1, 1))
			  cout<<"SUCCESS!!!";
			else
			  cout<<"Something went wrong!!";
			break;
		      }
		      else
		      {
			cout<<"Press any key to try again";
			getch();
			clrscr();
		      }
		    }while(1);
		    break;
		  case 3 :
		    break;
		  default :
		    cout<<"\nWrong Choice. Press any key to try again";
		    getch();
		    break;
		}
	      }while(l != 3);

	      break;
	    case 2 :
	      int v;
	      do
	      {
		header(admin.get_uName());
		cout<<"\n\t1. Create New Account\n\t2. View All Account\n\t3. Back\n";
		line();
		cout<<"Enter Your Choice : ";
		cin>>v;
		cin_flush();
		switch(v)
		{
		  case 1 :
		    char UN[30], UP1[30], UP2[30];
		    header(admin.get_uName());
		    cout<<"\n\t\tEnter User Name : ";
		    cin.getline(UN, 30);
		    while(1)
		    {
		      cout<<"\t\tEnter Password : ";
		      pass(UP1, 30);
		      cout<<"\n\t\tConfirm Password : ";
		      pass(UP2, 30);
		      if(!strcmp(UP1, UP2))
		      {
			user temp;
			while(1)
			{
			  cout<<"\n\n\t\tIs An ADMIN? Y/N";
			  char g;
			  g = getche();
			  if(g == 'y' || g == 'Y')
			  {
			    if(!temp.Reg(UN, UP1, 1))
			    {
			      line();
			      cout<<"\nSuccess!!\nPress any key to Go Back...";
			      getch();
			    }
			    break;
			  }
			  else if(g == 'n' || g == 'N')
			  {
			    if(!temp.Reg(UN, UP1, 0))
			    {
			      line();
			      cout<<"\nSuccess!!\nPress any key to Go Back...";
			      getch();
			    }
			    break;
			  }
			  else
			  {
			    cout<<"Wrong choice. Try Again";
			    getch();
			  }
			}
			break;
		      }
		      else
		      {
			cout<<"Try Again";
			getch();
			clrscr();
		      }
		    }
		    break;
		  case 2 :
		    header(admin.get_uName());
		    admin.view_all();
		    line();
		    cout<<"Press any key to Go Back";
		    getch();
		    break;
		  case 3 :
		    break;

		}
	      }while(v != 3);
	      break;
	    case 3 :
	      break;
	    default :
	      cout<<"\nWrong Choice. Press any key to try again";
	      getch();
	      break;
	  }
	}while(x != 3);
	break;
      case 5 :
	int y;
	do
	{
	  header(admin.get_uName());
	  cout<<"\t1. View Store Info\n\t2. Change Store Info\n\t3. Back";
	  line();
	  cout<<"Enter Your Choice : ";
	  cin>>y;
	  cin_flush();
	  switch(y)
	  {
	    case 1 :
	      header(admin.get_uName());
	      StInfo.show();
	      line();
	      cout<<"Press any key to Go Back.....";
	      getch();
	      break;
	    case 2 :
	      header(admin.get_uName());
	      StInfo.set_store();
	      line();
	      cout<<"New Data Saved..";
	      cout<<"\nPress any key to Go Back.....";
	      getch();
	      break;
	    case 3 :
	      break;
	    default :
	      cout<<"\nWrong Choice. Press any key to try again";
	      getch();
	      break;
	  }
	}while(y != 3);
	break;
      case 6 :
	main_login();
	break;
      case 7 :
	exit(0);
	break;
      case 8 :
	int cont;
	do
	{
	  header(admin.get_uName());
	  cout<<"\n\nThis will Factory Reset the Application And Delete all the data stored";
	  cout<<"\nDo you Want to continue?";
	  cout<<"\n\t1. Yes\n\t2. No\n";
	  cout<<"\n\n|------------------------------------------------------------------------------|\n";
	  cout<<"Enter Your Choice : ";
	  cin>>cont;
	  cin_flush();
	  switch(cont)
	  {
	    case 1 :
	      remove("users.dat");
	      remove("store.dat");
	      remove("drugs.dat");
	      remove("stocks.dat");
	      remove("invoice.dat");
	      exit(0);
	      break;
	    case 2 :
	      break;
	    default :
	      cout<<"\nWrong Choice. Press any key to try again";
	      getch();
	      break;
	  }
	}while(cont != 2);
	break;
    }
  }while(ch != 7);
}

void main_user(user &User)
{
  int ch;
  do
  {
    header(User.get_uName());
    cout<<"\t1. Sales\n\t2. View Stocks\n\t3. My Account\n\t4. View Store Info\n\t5. Logout\n\t6. EXIT";
    line();
    cout<<"Enter Your Choice : ";
    cin>>ch;
    cin_flush();
    switch(ch)
    {
      case 1 :
	int C;
	do
	{
	  header(User.get_uName());
	  cout<<"\t1. New Sale\n\t2. View old Bill\n\t3. Back";
	  line();
	  cout<<"Enter Your Choice : ";
	  cin>>C;
	  cin_flush();
	  switch(C)
	  {
	    case 1 :
	      header(User.get_uName());
	      inv_drug inv[10];
	      float Total = 0;
	      int i = 0;
	      unsigned int DCODE, QTY, MRP, BNO = INV.get_new_bno();
	      char CUST[40], DOC[40], D[50];
	      cout<<"\tEnter Customer Name : ";
	      cin.getline(CUST, 40);
	      cout<<"\tEnter Doctor Name : ";
	      cin.getline(DOC, 40);
	      int wh = 1;
	      do
	      {
		stock temp;
		cout<<"\tEnter Drug Code : ";
		cin>>DCODE;
		cin_flush();
		if(temp.inStock(DCODE))
		{
		  cout<<"\tEnter Quantity : ";
		  cin>>QTY;
		  cin_flush();
		  cout<<"\tEnter MRP : ";
		  cin>>MRP;
		  cin_flush();
		  if(inv[i].set((i+1), DCODE, QTY, MRP))
		  {
		    Total += inv[i].get_MRP();
		    do
		    {
		      cout<<"\n\tDo You Want to add more? Y/N";
		      char co;
		      co = getch();
		      if(co == 'y' || co == 'Y')
		      {
			++i;
			clrscr();
			break;
		      }
		      else if(co == 'n' || co == 'N')
		      {
			wh = 0;
			break;
		      }
		      else
		      {
			cout<<"\n Wrong KEY. Press any key to try again";
			clrscr();
		      }
		    }while(1);
		  }
		}
	      }while(wh == 1 && i<10);
	      header(User.get_uName());
	      cout<<"\tBill No. : "<<BNO;
	      cout<<"\n\tCustomer Name : "<<CUST;
	      cout<<"\n\tDoctor Name : "<<DOC;
	      cout.setf(ios::left);
	      cout<<"\n\t"<<setw(5)<<"SN"<<setw(30)<<"Particulars"<<setw(6)<<"QTY"<<setw(4)<<"MRP";
	      for(int k = 0; k<(i+1); k++)
	      {
		cout.setf(ios::left);
		cout<<"\n\t"<<setw(5)<<inv[k].get_sno()<<setw(30)<<inv[k].get_dName()<<setw(6)<<inv[k].get_qty();
		cout.setf(ios::right);
		cout<<setw(4)<<inv[k].get_MRP();
	      }
	      cout<<"\n\n\tTotal : "<<Total;
	      line();
	      do
	      {
		cout<<"\nAmout Paid? Y/N";
		char co;
		co = getch();
		if(co == 'y' || co == 'Y')
		{
		  INV.save(BNO, CUST, DOC, inv, (i+1));
		  break;
		}
		else if(co == 'n' || co == 'N')
		{
		  wh = 0;
		  break;
		}
		else
		{
		  cout<<"\n Wrong KEY. Press any key to try again";
		  clrscr();
		}
	      }while(1);
	      break;
	    case 2 :
	      header(User.get_uName());
	      unsigned int bno;
	      invoice temp;
	      cout<<"\tEnter Bill No. : ";
	      cin>>bno;
	      cin_flush();
	      header(User.get_uName());
	      temp.show(bno);
	      line();
	      cout<<"\nPress any key to Go Back.....";
	      getch();
	      break;
	    case 3 :
	      break;
	    default :
	      cout<<"\nWrong Choice. Press any key to try again";
	      getch();
	      break;
	  }
	}while(C != 3);
	break;

      case 2 :
	header(User.get_uName());
	stocks.View();
	line();
	cout<<"Press any key to Go Back.....";
	getch();
	break;
      case 3 :
	int l;
	do
	{
	  header(User.get_uName());
	  cout<<"\t1. View My Account\n\t2. Change My Account\n\t3. Back\n";
	  line();
	  cout<<"Enter Your Choice : ";
	  cin>>l;
	  cin_flush();
	  switch(l)
	  {
	    case 1 :
	      header(User.get_uName());
	      cout<<"\n\t\tMy User Name : "<<User.get_uName();
	      cout<<"\n\t\tMy Password : "<<User.get_uPass();
	      cout<<"\n\t\tAccount Type : ";
	      if(User.get_isAdmin())
		cout<<"Admin";
	      else
		cout<<"Non - Admin";
	      line();
	      cout<<"Press any key to Go Back.....";
	      getch();
	      break;
	    case 2 :
	      char us[30], np1[30], np2[30];
	      header(User.get_uName());
	      cout<<"\n\t\tEnter New User Name : ";
	      cin.getline(us, 30);
	      do
	      {
		cout<<"\t\tEnter New Password : ";
		pass(np1, 30);
		cout<<"\n\t\tConfirm Password : ";
		pass(np2, 30);
		if(!strcmp(np1, np2))
		{
		  if(User.Edit(us, np1, 1))
		    cout<<"SUCCESS!!!";
		  else
		    cout<<"Something went wrong!!";
		  break;
		}
		else
		{
		  cout<<"Press any key to try again";
		  getch();
		  clrscr();
		}
	      }while(1);
	      break;
	    case 3 :
	      break;
	    default :
	      cout<<"\nWrong Choice. Press any key to try again";
	      getch();
	      break;
	  }
	}while(l != 3);
	break;
      case 4 :
	header(User.get_uName());
	StInfo.show();
	line();
	cout<<"Press any key to Go Back.....";
	getch();
	break;
      case 5 :
	main_login();
	break;
      case 6 :
	exit(0);
	break;
    }
  }while(ch != 6);
}

int main()
{
  clrscr();
  if(!main_init())
    main_login();

  return 0;
}