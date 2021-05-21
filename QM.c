# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <windows.h>

//定义用户结构体(用户名wname，姓名rname，密码password，余额balance，手机号码tnumber)
struct User{
	char wname[20];
	float  balance;
	char tnumber[20];
	char rname[20];
	char password[20];
	int a[500];
};                                                   
//定义红包结构体(发送时间(tim),发送金钱总额(sum),拆分个数(split),用户(struct User user))
struct Rp{
	float sum;
	int split;
	char rpn[20];
	struct User user;
	time_t tm;
};                                                    //a.定义了一个struct Lnode的结构体(用户结构体user 指向下一个struct Lnode结构体的指针) 
                                                      //b.对  "struct Lnode {};" 起了个别名 ，称之为Lnode。即 Lnode == struct Lnode(...)
//定义用户链表节点和连线                              //c.并Lnode *Linklist 定义了一个名为Linklist 类型为Lnode的指针    所以 Linklist = Lnode*
typedef struct Lnode{
	struct User user;                                 //定义了一个struct User 结构体类型 名为user的变量
    struct Lnode *next;                               //指向下一个struct Lnode的结构体
}Lnode,*Linklist;                                     
//定空节点L                                           //定义了一个Linklist 类型的 L变量
Linklist L;//地址变量                                 //Linklist:指向的地址    *Linklist:指向地址的内容    &Linklist:Linklsit自己的地址
                                                      //   &L   :L自己的地址       L    :L的值
   
//定义红包链表节点和连线 
typedef struct Space
{
	struct Rp rp;
	struct Space *next;
}Space,*Linkage;
//定空节点N 
Linkage N;


//全局声明
int   login         (void);                            //登入微信 
int   givepwd       (void);                            //找回密码
int   findwname     (char name[]);                     //查找用户
int   findnbr       (char wname[],char tnumber[]);     //确认手机号是否正确 
int   findrname     (char wname[],char rname[]);       //确认密码是否正确
void  addRp         (struct Rp rp);                    //保存红包数据
void  addUser       (struct User user);                //保存用户数据
void  readFile      (void);                            //读取用户数据
void  readfile      (void);                            //读档红包数据
void  writeFile     (void);							   //存档用户数据
void  writefile     (void);							   //存档红包数据
void  createLinklist(void);                            //创造储存用户的空链表
void  createLinkage (void);                            //创造储存红包的空链表
void  myregister    (void);                            //注册一个新微信用户 
char *findpwd       (char wname[]);                    //根据用户查询密码 
void  deposit       (void);                            //充值 
float inquiry       (void);                            //查询余额 
void  drawmy        (void);                            //取钱 
void  tranmy        (void);                            //转账 
void  sendrp        (void);                            //发红包 
void  printrp       (void);                            //查看红包
void  grabrp        (void);                            //抢红包 
void  printe        (void);                            //查看红包详细信息 
void  detime        (void);                            //检测红包效期
                                                       //传入一整个红包，遍历检查，做出对应的修改，吴返回值 

float  balance;
char wname[20];
char rname[20];
char tnumber[20];
char password[20];
time_t tm;


//创造储存用户的空链表
void createLinklist()
{
	L = (Linklist)malloc(sizeof(Lnode));              //创造头结点
	L->next = NULL;                                   //连线，令头结点指向空
}

//存档用户数据
void writeFile()
{
	Linklist p = L->next;                             //定义了一个名为P的空节点 ，令p=L节点的下一个节点

	FILE *fp;
	fp = fopen("./data.dat","w+b");

	while (p)
	{
		fwrite(&p->user,sizeof(struct User),1,fp);
		p = p->next;
	}

	fclose(fp);
}

//读档用户数据
void readFile()
{
	int n;
	struct User user;

	FILE *fp;
	createLinklist();
	fp = fopen("./data.dat","r+b");

	if(fp == NULL)
	{
		return;
	}

	while(1)                                          //这个while是在fp!=NULL的时候执行的
	{
		n = fread(&user,sizeof(struct User),1,fp);
		if(n!=1)
		{
			break;
		}
		addUser(user);
	}
}

//保存用户数据
void addUser (struct User user)
{
	Linklist p = L;
	Linklist q;

	while (p->next)
	{
		p = p->next;
	}

	q = (Linklist)malloc(sizeof(Lnode));
	
	q->user = user;
	q->next = NULL;
	p->next = q;

	//writeFile();
}

//开设微信账户
void myregister()
{
	int  n;
	int  i,j; 
	int  s,m;
	char psd[20];
	char pwd[20];
	char name[20];
	struct User user;

	while (1)
	{
		printf ("\t请设置您心仪的用户名:\n");
		scanf  ("%s",name);
		
		if(findwname(name) == 1)
		{
			printf ("\t\t它已有所属\n");
			printf ("\t人长这么大 总得懂得学会放手与选择\n");
			while(1)
			{
				printf ("\t1重新设置用户名          2退出\n");
				scanf  ("%d",&n);
	
				if (n == 1)
				{
					break;
				}
				else if (n == 2)
				{
					return;
				}
				else
				{
					printf ("\t404-----wrong\n");
					continue;
				}
			}
		}
		else 
		{
			break;
		}
	}
	
	while(1)
	{
		printf ("\t请输入您的手机号：\n");
		scanf  ("%s",tnumber);
		
		if(strlen(tnumber)==11)
		{
			while (1)
			{ 
				srand(time(NULL));
				s = rand()%9000+1000;
				
				printf ("\t请将短信验证码填入相应位置\n"); 
				printf ("\n");
				printf ("\t\t\t\t您的短信验证码为：%d\n",s);
				printf ("\n");
				printf ("\t短信验证码：");
				scanf  ("%d",&m);
				printf ("\n");
				
				if (m == s)
				{
					goto l; 					
				}
				else
				{
					printf ("\t抄都能抄错？小脑脑残？\n");
					while(1)
					{
						printf ("\t1重新获取   2退出\n");
						scanf  ("%d",&n);
						
						if (n == 1)
						{
							printf ("\t您将重新获取一个验证码：\n");	
							break;
						}
						else if (n == 2)
						{
							return;
						}
						else
						{
							printf ("\t404-----wrong\n");
							continue;
						}
					}
				}
			}
		}
		else
		{
			printf ("\t手机号有误：\n");
			while(1)
			{
				printf ("\t1重新输入          2退出\n");
				scanf  ("%d",&n);
	
				if (n == 1)
				{
					break;
				}
				else if (n == 2)
				{
					return;
				} 
				else
				{
					printf ("\t404-----wrong\n");
					continue;
				} 
			}
		}	 
	}
		
		
l:	for(i=0;i<3;i++)
	{
		printf ("\t您现在有%d次设置密码的机会！\n",3-i); 
		printf ("\t请设置一个新密码：\n");
		scanf  ("%s",psd);
		printf ("\t请确认您的新密码：\n");
		scanf  ("%s",pwd); 
		
		if(strcmp(psd,pwd) != 0)
		{
			printf ("\t两次密码输入不一致，请动一下你的脑子:\n");
			continue;
		}
		else
		{
			printf ("\t请输入您的真实姓名:\n");
			scanf  ("%s",rname);
			
			user.balance = 0;
			strcpy (user.wname,name);
			strcpy (user.rname,rname);
			strcpy (user.password,psd);
			strcpy (user.tnumber,tnumber);
			
			for (j=0;j<501;j++)
			{
				user.a[j] = 0;
			}

			addUser(user);
			writeFile ();
				
			printf ("\t注册成功!\n");
			return;
		}
	}
	
	printf ("\t三次还不对？\n");
    return;
}

//登入
int login()
{
	int  i;
	int  n;
	char pwd[20];
	
	while(1)
	{
l:
		printf ("\t请输入您正确的用户名:\n");
		scanf  ("%s",wname);
		
		if(findwname(wname)==-1)
		{
			printf ("\t查无此人\n");
			while(1)
			{
			    printf ("\t1重新登入   2退出\n");
				scanf  ("%d",&n);
	
				if (n == 1)
				{
					goto l;
				}
				else if (n == 2)
				{
					return -1;
				}
				else
				{
					printf ("\t404-----wrong\n");
					continue;
				}
			}
		}
		
		for(i=0;i<3;i++)
		{
			printf ("\t您现在有%d次输入密码的机会！\n",3-i); 
			printf ("\n");
			printf ("\t请输入密码：\n");
			scanf  ("\t%s",pwd);
			
			if(strcmp(findpwd(wname),pwd)==0)
			{
				printf ("\t登入成功\n");
				return 1;
			}
			else
			{
				printf ("\t好好回想以下密码，别浪费你我时间\n");
				continue;
			}				
		}
		
		printf ("\t不对 事不过三 你耗尽了我所有的耐心！\n"); 
		return -1; 
	}
} 

//找回密码
int givepwd()                                          //用 1 -1 判断是否找到   若找到直接printf出密码 
{
	int  i,n,m,s;
	char pwd[20];
	 
	while(1)
	{
		printf ("\t请输入正确的微信账户：\n");
		scanf  ("%s",wname);
		
		if(findwname(wname)==-1)                      //这里需要findwname 
		{
			printf ("\t查无此人\n");
			while(1)
			{
			    printf ("\t1重新输入   2退出\n");
				scanf  ("%d",&n);
	
				if (n == 1)
				{
					break;
				}
				else if (n == 2)
				{
					return -1;
				}
				else
				{
					printf ("\t404-----wrong\n");
					continue;
				}
			}
		}
		else
		{
			break; 
		}
	}
	
	while(1)
	{
		printf ("\t请输入真实姓名：\n");
		scanf  ("%s",rname);
		
		if(findrname(wname,rname)==-1)                //这里需要一个findrname 
		{
			printf ("\t自己名字都能记错？是猪头？\n");
			while(1)
			{
			    printf ("\t1重新输入   2退出\n");
				scanf  ("%d",&n);
	
				if (n == 1)
				{
					break;
				}
				else if (n == 2)
				{
					return -1;
				}
				else
				{
					printf ("\t404-----wrong\n");
					continue;
				}
			}
		}
		else
		{
			break; 
		}
	}
	
	while(1)
	{
		printf ("\t请输入当前账户绑定的手机号\n");     
		scanf  ("%s",tnumber);
		
		if(findnbr(wname,tnumber)==-1)                //这里需要一个findbr 
		{
			printf ("\t号码有误\n");
			while(1)
			{
			    printf ("\t1重新输入   2退出\n");
				scanf  ("%d",&n);
	
				if (n == 1)
				{
					break;
				}
				else if (n == 2)
				{
					return -1;
				}
				else
				{
					printf ("\t404-----wrong\n");
					continue;
				}
			}
		}
		else
		{
			while (1)
			{ 
				srand(time(NULL));
				s = rand()%9000+1000;
				
				printf ("\t请将短信验证码填入相应位置\n"); 
				printf ("\n");
				printf ("\t\t\t\t您的短信验证码为：%d\n",s);
				printf ("\n");
				printf ("\t短信验证码：");
				scanf  ("%d",&m);
				printf ("\n");
				
				if (m == s)
				{
					strcpy (pwd,findpwd(wname));
					printf ("\t您的密码为：%s\n",pwd);
					return 1; 					
				}
				else
				{
					printf ("\t抄都能抄错？小脑脑残？\n");
					while(1)
					{
						printf ("\t1重新获取   2退出\n");
						scanf  ("%d",&n);
						
						if (n == 1)
						{
							printf ("\t您将重新获取一个验证码：\n");	
							break;
						}
						else if (n == 2)
						{
							return -1;
						}
						else
						{
							printf ("\t404-----wrong\n");
							continue;
						}
					}
				}
			}
		}
	} 
} 

//充值
void deposit()
{
	float temp;                                          //日后补充绑定银行卡功能
	Linklist p = L->next;
	printf ("\t请输入需要充值的金额:\n");
	scanf  ("%f",&temp);
	printf ("\t你确定你有那么多钱？\n\n");
	
	while(p)
	{
		if(strcmp(p->user.wname,wname)==0)
		{
			p->user.balance = p->user.balance + temp;
			printf ("\t行吧，充值成功\n\n");
			writeFile();
			return;
		}
		p = p->next;
	}
	printf ("\t网络超时\n");
	return;                                  
} 
//提现 
void drawmy()
{
	float temp;
	Linklist p = L->next;
	
	printf ("\t您需要提现多少钱呢？\n");
	scanf  ("%f",&temp);
	
	while(p)
	{
		if(strcmp(p->user.wname,wname)==0)
		{
			if(p->user.balance >= temp)
			{
				p->user.balance = p->user.balance - temp;
				writeFile();
				printf ("\t提现成功\n");
				return;				
			}
			else
			{
				printf ("\t很尴尬！您的账户余额不足诶~好丢脸\n");
				return;
				 
			}
		}
		p = p->next;
	}
	printf ("\t网络超时！\n");
	return;
}

//转账
void tranmy()
{
	int n;
	float temp;
	char dname[20];
	Linklist p = L->next;
	Linklist q = L->next;
	
	while(1)
	{
		printf ("\t请输入对方账户：\n");
		scanf  ("%s",dname);
		
		if(findwname(dname)==-1)
		{
			printf ("\t在耍我？根本没这个人！\n");

l:			
			while(1)
			{
				printf ("\t1 重新输入   2 退出\n");
				scanf  ("%d",&n);
				
				if(n==1)
				{
					break;
				}		
				else if(n==2)
				{
					return;
				}		
				else
				{
					printf ("\t404-----wrong\n");
					continue;
				}
			}
		}
		else
		{
			if (strcmp(dname,wname)==0)
			{
				printf ("\t自己转自己？有病否？\n");
				
				goto l;
			}
			
			break;
		} 
	}

	while(p)
	{
		if(strcmp(p->user.wname,wname)==0)
		{
			while(1)
			{
				printf ("\t请输入需要转账的金额:\n");
				scanf  ("%f",&temp);
				
				if(temp > p->user.balance)
				{
					printf ("\t穷鬼！没钱还想装大佬！\n");
					while(1)
					{
						printf ("\t1 少转一点  2 羞涩离开\n");
						scanf  ("%d",&n);
						
						if(n==1)
						{
							break;
						} 
						else if(n==2)
						{
							return;
						}
						else
						{
							printf ("\t404-----wrong\n");
							continue;
						}
					}
				}
				else
				{
					p->user.balance = p->user.balance - temp;
					writeFile();
					while(q)
					{
						if(strcmp(q->user.wname,dname)==0)
						{
							q->user.balance = q->user.balance + temp;
							writeFile();
							printf ("\t转账成功\n");
							return;
						}
						q = q->next;
					}
					printf ("\t网络超时\n"); 
					return; 
				}
			}
		}
		p = p->next;
	}
	printf ("\t网络超时\n"); 
	return; 
} 

//查看零钱 
float inquiry()
{
	Linklist p = L->next;
	
	while(p)
	{
		if(strcmp(p->user.wname,wname)==0)
		{
			return p->user.balance;
		}
		p = p->next;
	}
	printf ("\t网络超时\n");
	return -1; 
} 

//查找用户是否已存在
int findwname(char name[]) 
{
	Linklist p = L->next;
	
	while (p)
	{
		if (strcmp(p->user.wname,name) == 0)
		{
			return 1;
		}
		p = p->next;
	}
	
	return -1;
}

//查找姓名是否正确 
int findrname(char wname[],char rname[]) 
{
	Linklist p = L->next;
	
	while (p)
	{
		if (strcmp(p->user.wname,wname) == 0)
		{
			if(strcmp(p->user.rname,rname)==0)
			{
				return 1;
			}
			else
			{
				return -1;
			}
		}
		p = p->next;
	}
	return -1;
}

//查找手机号是否正确
int findnbr(char wname[],char tnumber[20])
{
	Linklist p = L->next;
	
	while (p)
	{
		if (strcmp(p->user.wname,wname) == 0)
		{
			if(strcmp(p->user.tnumber,tnumber)==0)
			{
				return 1;
			}
			else
			{
				return -1;
			}
		}
		p = p->next;
	}
	return -1;
}


//查找用户密码
char* findpwd(char wname[])
{
	Linklist p = L->next;
	
	while(p)
	{
		if(strcmp(p->user.wname,wname)==0)
		{
			return p->user.password;
		}
		p = p->next;
	}
	
	return NULL;
}

//创造储存红包的空链表
void createLinkage ()
{
	N = (Linkage)malloc(sizeof(Space));
	N->next = NULL;
}

//存档红包数据
void writefile()
{
	Linkage q = N->next;

	FILE *fp;
	fp = fopen ("./datam.dat","w+b");

	while (q)
	{
		fwrite (&q->rp,sizeof(struct Rp),1,fp);
		q = q->next;
	}
	fclose (fp);
}

//读档红包数据
void readfile()
{
	int n;
	struct Rp rp;

	FILE *fp;
	createLinkage();
	fp = fopen("./datam.dat","r+b");

	if(fp == NULL)
	{
		return;
	}

	while(1)
	{
		n = fread(&rp,sizeof(struct Rp),1,fp);
		if(n!=1)
		{
			break;
		}
		addRp(rp);
	}
}

//保存红包数据
void addRp (struct Rp rp)
{
	Linkage p = N;
	Linkage q;

	while (p->next)
	{
		p = p->next;
	}

	q = (Linkage)malloc(sizeof(Space));
	q->rp = rp;
	q->next = NULL;
	p->next = q;

	writefile();
}

//发红包
void sendrp()
{
	int n;
	float temp;
	int split;
	char rpn[20];
	struct Rp rp;
	Linklist p = L->next;
	
	while(p)
	{
		if(strcmp(p->user.wname,wname)==0)
		{
			while(1)
			{
				printf ("\t敢问大佬想发多少钱呢：\n");
		        scanf  ("%f",&temp);
		        
		        if(temp > p->user.balance)
				{
					printf ("\t穷鬼！没钱还想装大佬！\n");
					while(1)
					{
						printf ("\t1 少发一点  2 羞涩离开\n");
						scanf  ("%d",&n);
						
						if(n==1)
						{
							break;
						} 
						else if(n==2)
						{
							return;
						}
						else
						{
							printf ("\t404-----wrong\n");
							continue;
						}
					}
				}
				else
				{
					while(1)
					{
						printf ("\t大佬想拆分成几个呢？\n");
						scanf  ("%d",&split);
						
						if(temp/split>=0.01)
						{
							printf ("\t请给这个红包小可爱起个名字吧\n");
							scanf  ("%s",rpn);
							
							p->user.balance =p->user.balance - temp;
							writeFile();
							rp.sum = temp;
							rp.split = split;
							strcpy(rp.rpn,rpn);
							rp.user = p->user;
							
							tm = time(NULL);
							rp.tm = tm; 
							
							writefile();
							addRp(rp);
							printf ("\t发送成功\n");
							return;							
						}
						else
						{
							printf ("\t按你这么分，能抢到个屁?\n");
							
							while(1)
							{
								printf ("\t1 重设个数  2 羞愧离开\n");
								scanf  ("%d",&n);
								
								if (n==1)
								{
									break;
								} 
								else if(n==2)
								{
									return;
								}
								else
								{
									continue;
								}
							}
							
						}
					}
				}
			}
		}
		p = p->next;
	}
	printf ("\t网络超时\n");
	return;
} 

//查看当前红包
void printrp()
{
	int i=1;
	Linkage q = N->next;
	
	detime();
	
	while(q)
	{
		if(q->rp.sum !=0 )
		{
			printf("\t%d %s大佬的%s红包\n",i,q->rp.user.wname,q->rp.rpn);
		}
		else
		{
			printf("\t%d %s大佬的%s红包\t已被抢完\n",i,q->rp.user.wname,q->rp.rpn); 
		}
		i++;
		q = q->next;
	}

	printf ("\n");
	printf ("\n");
	return;
} 

//查看红包详细信息
void printe()
{
	
	int i = 1;
	Linkage q = N->next;
	
	detime();
	
	while(q)
	{
		printf ("\t%d %s 的 %s红包 余额为%0.2f 剩余个数为%d\n",i,q->rp.user.wname,q->rp.rpn,q->rp.sum,q->rp.split);
		q = q->next;
	}
	printf ("\n");
	printf ("\n");
	return;
} 

//查看时间效期
void detime()
{
	float temp;
	time_t tmn;
	Linklist p = L->next;
	Linkage q = N->next;
	
	while(q)
	{
		tmn = time(NULL);
		
		if (tmn>=q->rp.tm+30)
		{
			//进行红包金额退回原用户的行为
			q->rp.split = 0;
			temp = q->rp.sum;
			q->rp.sum = 0;
			writefile();
			while(p)//要是有人销户了怎么办 
			{
				if(strcmp(q->rp.user.wname,p->user.wname)==0)
				{
					p->user.balance = p->user.balance+temp;
					writeFile();
					break;
				}
				p = p->next;
			}
		}
		
		q = q->next;
	}
} 

//抢
void grabrp()
{
	int m,s;
	int n,j;
	float r;
	int i=1;
	Linklist p = L->next;
	Linkage q = N->next;
	
	detime();
	
	if(!q)
	{
		printf ("\t去去去去发红包去  没红包可抢\n");
		return;
	}

	while(1)
	{
		printf ("\t你想抢哪个红包呢？\n");
		scanf  ("%d",&n);
		
		j = n;

		while(q->next)                                //这里的while循环用来查询当前红包数                         
		{
			q = q->next;
			i++;
		}
		
		q = N->next;
		
		if(n>i)
		{
			printf ("\t总共就那么几个红包，你还要我怎样？？？？？\n");
			printf ("\t爱抢抢 不抢拉倒！！！\n");
			return;			
		}
		else
		{
			//查询是否抢过
			i=0;
			while(p)
			{
				if(strcmp(p->user.wname,wname)==0)
				{
					while(i<501)
					{
						if(p->user.a[i] == j)
						{
							printf ("\t为什么要反复蹂躏同一个红包呢？\n");
							return;
						}
						i++;
					}
					break;
				}
				p = p->next;
			}

			p = L->next;

			while(n-1)
			{
				q = q->next;
				n--;
			}
			//退出while说明找到了要抢的红包此时进行拆分
			//0 检测q->rp.spilt ?= 0 1 生成一个介于0~sum之间的数  2 q->rp.sum - rand    p->user.balance + rand    p->rp.spilt-1 
			if(q->rp.split>1)
			{
				m = q->rp.sum;
				
				srand(time(NULL));
				s = rand()%m+0.000000;
				
				r = s*0.9;
				
				q->rp.sum = q->rp.sum - r;
				q->rp.split = q->rp.split - 1;
				writefile();
				
				while(p)
				{
					if(strcmp(p->user.wname,wname)==0)
					{
						p->user.balance = p->user.balance + r;
						p->user.a[j] = j;
						writeFile();
						printf ("\t恭喜您抽取到%0.2f元的红包\n",r);
						return;
					}
					p =p->next;
				}
				return;
			}
			else if(q->rp.split == 1)
			{
				p->user.balance = p->user.balance+q->rp.sum;
				writeFile();
				printf ("\t恭喜您抽取到%0.2f元的红包\n",q->rp.sum);
				q->rp.split = 0;
				q->rp.sum = 0;
				writefile();
				return;
			}
			else
			{
				printf ("\t晚起的鸟儿没虫吃\n");
				return;
			}
		}
	}
} 



//主函数
void main()
{
	int n;
	readFile();
	readfile();
	
	while(1)
	{
		printf ("\n");
		printf ("\n");
		printf ("\t1 注册\n");
		printf ("\t2 登入\n");
		printf ("\t3 找回密码\n");
		printf ("\t4 查找用户\n");
		scanf  ("%d",&n);
		
		if(n==1)
		{
			myregister();
			continue;
		}
		else if(n==2)
		{
			n = login();
			if(n==-1)
			{
				continue;
			}
			else
			{
				while(1)
				{
					printf ("\n");
					printf ("\n");
					printf ("\t1 零钱\t5 发红包\n");
					printf ("\t2 存钱\t6 抢红包\n");
					printf ("\t3 提现\t7 显示当前所有红包\n");
					printf ("\t4 转账\n");
					printf ("\t10086 调试员功能\n"); 
					printf ("\t     按任意键退出\n");
					scanf  ("%d",&n);
					
					if(n==1)
					{
						printf ("\t您的余额为：%0.2f\n",inquiry());
						continue;
					}
					else if(n==2)
					{
						deposit();
						continue;
					}
					else if(n==3)
					{
						drawmy();
						continue;
					}
					else if(n==4)
					{
						tranmy();
						continue;
					}
					else if(n==5)
					{
						sendrp();
						continue;
					}
					else if(n==6)
					{
						printrp();
						
						grabrp();
						continue;
					}
					else if(n==7)
					{
						printrp();
						continue;
					}
					else if(n==10086)
					{
						printe();
						continue;
					}
					else
					{
						break;
					}
				}
			}
			
		}
		else if(n==3)
		{
			if(givepwd() ==-1)
			{
				continue;
			}
			else
			{
				continue;
			}
		}
		else if(n==4)
		{
			printf ("\t请输入需要查询是否存在的用户名:\n");
			scanf  ("%s",wname);
			
			if(findwname(wname)==1)
			{
				printf ("\t此人尚在\n");
				printf ("\n");
				continue;
			}
			else
			{
				printf ("\t查无此人\n");
				printf ("\n");
				continue;
			}
		}
		else
		{
			printf ("\t您的输入有误,请重新输入\n");
			continue; 
		}
	}
}


















