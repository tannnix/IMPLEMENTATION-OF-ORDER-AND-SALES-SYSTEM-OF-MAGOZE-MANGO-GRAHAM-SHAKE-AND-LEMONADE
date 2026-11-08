#include <iostream>
#include <cstdlib>

using namespace std;


//Limit muna natin sa 8 sikat na drinks mula sa menu para di sumabog
const int MAX_MENU=8;


//Bawat drink may presyo for Medium (M) at Large (L) galing sa tarpaulin!
struct MenuItem
{
    char name[35];
    int priceMedium;
    int priceLarge;
};


//Kumpletong info ng order kasama na kung anong size pinili
struct Order
{
    int orderID;
    int menuIndex;
    char size;
    int quantity;
    int total;
    int payment;
    int change;
};


//Record ng tapos na order para sa sales
struct SalesRecord
{
    int orderID;
    int menuIndex;
    char size;
    int quantity;
    int total;
};


// Bawat waiting order ay may sariling node sa Queue
struct QueueNode
{
    Order data;
    QueueNode* next;
};


// Bawat completed sale ay may sariling node sa Linked List
struct SalesNode
{
    SalesRecord data;
    SalesNode* next;
};


//ARRAY: Menu items diretso mula sa Mangooze menu!
MenuItem menuItems[MAX_MENU]=
{
    {"Mango Graham",79,99},
    {"Cheezy Mango Graham",95,115},
    {"Fresh Lemonade",59,75},
    {"Fresh Lemon Yakult",75,89},
    {"Mango Oreo Shake",79,99},
    {"Avocado Milkshake",85,105},
    {"Lychee Freeze",55,65},
    {"Mango Overload",110,129}
};


// QUEUE: FRONT ang susunod na ipo-process, REAR ang dulo ng pila
QueueNode* front=NULL;
QueueNode* rear=NULL;


// LINKED LIST: simula ito ng completed sales history
SalesNode* salesHead=NULL;


// Para unique ang Order ID ng bawat bagong order
int nextOrderID=1;


// Number muna tayo nang maayos—huwag papasok ang maling input!
int readInteger(const char prompt[])
{
    int value;

    while(true)
    {
        cout<<prompt;
        cin>>value;

        // Okay ang input? Gamitin na natin.
        if(cin.fail()==false)
        {
            return value;
        }

        // Nagkamali ng input? Linisin muna para makapag-try ulit.
        cin.clear();
        cin.ignore(10000,'\n');

        cout<<"Invalid input. Enter a whole number."<<endl;
    }
}


// Basahin ang isang character, tulad ng M o L
char readCharacter(const char prompt[])
{
    char value;

    while(true)
    {
        cout<<prompt;
        cin>>value;

        // Nakuha ang character? Ibalik na natin.
        if(cin.fail()==false)
        {
            return value;
        }

        // Linisin ang maling input bago magtanong ulit
        cin.clear();
        cin.ignore(10000,'\n');

        cout<<"Invalid input. Try again."<<endl;
    }
}


// Tingnan kung wala nang waiting orders sa pila
bool isQueueEmpty()
{
    return front==NULL;
}


// Ipakita ang buong menu para makapili ang customer
void displayMenu()
{
    cout<<endl;
    cout<<"================ MANGOOZE MENU ================"<<endl;
    cout<<"No.  Drink Name\t\t\tMedium\tLarge"<<endl;
    cout<<"------------------------------------------------"<<endl;

    // Ikutin ang menu array at ipakita ang bawat drink
    for(int i=0;i<MAX_MENU;i++)
    {
        cout<<i+1<<".   "
            <<menuItems[i].name<<"\t\tP"
            <<menuItems[i].priceMedium<<"\tP"
            <<menuItems[i].priceLarge<<endl;
    }

    cout<<"================================================"<<endl;
}


// ENQUEUE: idagdag ang bagong order sa dulo ng pila
void enqueue(Order newOrder)
{
    // Gumawa ng bagong node para sa order
    QueueNode* newNode=new QueueNode;

    newNode->data=newOrder;
    newNode->next=NULL;

    // Kung bakante ang pila, ang bagong order ang FRONT at REAR
    if(isQueueEmpty()==true)
    {
        front=newNode;
        rear=newNode;
    }
    else
    {
        // May laman na ang pila—idugtong sa dulo
        rear->next=newNode;
        rear=newNode;
    }
}


// Kunin ang order ng customer at ipasok sa pila
void addOrder()
{
    int menuChoice;
    int quantity;
    int payment;
    char sizeChoice;

    // Ipakita muna ang menu para may mapagpilian
    displayMenu();

    menuChoice=readInteger("Choose the Number of the Drink: ");

    // Check kung may ganitong number sa menu
    if(menuChoice<1||menuChoice>MAX_MENU)
    {
        cout<<"Invalid Order, Please Input a Valid Menu Option."<<endl;
        return;
    }

    // Alamin kung Medium o Large ang gusto
    sizeChoice=readCharacter("Input Cup Size? [M]edium o [L]arge: ");

    // Tanggapin pati lowercase m at l
    if (sizeChoice == 'm') sizeChoice = 'M';
    if (sizeChoice == 'l') sizeChoice = 'L';

    // Hindi M o L? Hindi muna natin tatanggapin
    if(sizeChoice!='M'&&sizeChoice!='L')
    {
        cout<<"Only Medium(M) and Large(L) is Available."<<endl;
        return;
    }

    // Quantity dapat at least isa
    quantity=readInteger("Quantity: ");

    if(quantity<=0)
    {
        cout<<"Quantity must be greater than zero."<<endl;
        return;
    }

    // Sa array nagsisimula ang index sa zero
    int menuIndex=menuChoice-1;
    int unitPrice;

    // Piliin ang tamang presyo base sa size
    if(sizeChoice=='M')
    {
        unitPrice=menuItems[menuIndex].priceMedium;
    }
    else
    {
        unitPrice=menuItems[menuIndex].priceLarge;
    }

    // Presyo bawat isa times quantity—ayan ang total
    int total=unitPrice*quantity;

    cout<<"Total Amount: P"<<total<<endl;
    payment=readInteger("Input Payment: P");

    // Kulang ang bayad? Hindi muna idadagdag ang order
    if(payment<total)
    {
        cout<<"Your Payment is Short, Please Input a Valid Ammount."<<endl;
        return;
    }

    // Ihanda lahat ng detalye bago ipasok sa Que2ue
    Order newOrder;

    newOrder.orderID=nextOrderID;
    newOrder.menuIndex=menuIndex;
    newOrder.size=sizeChoice;
    newOrder.quantity=quantity;
    newOrder.total=total;
    newOrder.payment=payment;
    newOrder.change=payment-total;

    // Pasok na sa pila!
    enqueue(newOrder);

    cout<<"\n============================================="<<endl;
    cout<<"Order Received! Please go in Line."<<endl;
    cout<<"Order ID: "<<nextOrderID<<endl;
    cout<<"Change: P"<<newOrder.change<<endl;
    cout<<"============================================="<<endl;

    // Ready na ang susunod na unique Order ID
    nextOrderID++;
}


// Ipakita ang pila mula FRONT hanggang REAR
void displayQueue()
{
    // Walang pila? Walang orders na ipapakita
    if(isQueueEmpty()==true)
    {
        cout<<"No Orders in Queue."<<endl;
        return;
    }

    cout<<endl;
    cout<<"--- IN QUEUE (Front to Rear) ---"<<endl;

    // Simulan sa FRONT at sundan ang bawat next pointer
    QueueNode* current=front;

    while(current!=NULL)
    {
        cout<<"ID: "<<current->data.orderID
            <<" | "<<menuItems[current->data.menuIndex].name
            <<" ("<<current->data.size <<")"
            <<" x"<<current->data.quantity
            <<" | Total: P"<<current->data.total
            <<endl;

        // Sunod naman!
        current=current->next;
    }
}


// Ilipat ang natapos na order sa sales Linked List
void addSale(Order completedOrder)
{
    // Gumawa ng node para sa bagong sales record
    SalesNode* newNode=new SalesNode;

    newNode->data.orderID=completedOrder.orderID;
    newNode->data.menuIndex=completedOrder.menuIndex;
    newNode->data.size=completedOrder.size;
    newNode->data.quantity=completedOrder.quantity;
    newNode->data.total=completedOrder.total;
    newNode->next=NULL;

    // Kung wala pang sales, ito ang magiging unang record
    if(salesHead==NULL)
    {
        salesHead=newNode;
    }
    else
    {
        // Hanapin ang pinakahuling node
        SalesNode* current=salesHead;

        while(current->next!=NULL)
        {
            current=current->next;
        }

        // Idagdag ang bagong sale sa dulo ng history
        current->next=newNode;
    }
}


// PROCESS: kunin at alisin ang order sa FRONT ng Queue
void processNextOrder()
{
    if(isQueueEmpty()==true)
    {
        cout<<"There are no orders to process."<<endl;
        return;
    }

    Order completedOrder=front->data;

    addSale(completedOrder);

    QueueNode* temp=front;
    front=front->next;

    if(front==NULL)
    {
        rear=NULL;
    }

    delete temp;

    cout<<endl;
    cout<<">>> PREPARING YOUR ORDER <<<"<<endl;
    cout<<"Order ID: "<<completedOrder.orderID<<endl;
    cout<<"Drink: "<<menuItems[completedOrder.menuIndex].name<<endl;

    if(completedOrder.size=='M')
    {
        cout<<"Size: Medium"<<endl;
    }
    else
    {
        cout<<"Size: Large"<<endl;
    }

    cout<<"Quantity: "<<completedOrder.quantity<<endl;
    cout<<"Total: P"<<completedOrder.total<<endl;
    cout<<"Order is done and served!"<<endl;
}


// Ipakita lahat ng completed sales
void displaySales()
{
    if(salesHead==NULL)
    {
        cout<<"\nNo Sales Yet!"<<endl;
        return;
    }

    cout<<endl;
    cout<<"=== SALES HISTORY ==="<<endl;

    SalesNode* current=salesHead;
    int totalSales=0;

    while(current!=NULL)
    {
        cout<<"Order ID: "<<current->data.orderID
            <<" | "<<menuItems[current->data.menuIndex].name
            <<" ("<<current->data.size <<")"
            <<" x"<<current->data.quantity
            <<" | P"<<current->data.total
            <<endl;

        // I-add ang sale na ito sa overall total
        totalSales=totalSales+current->data.total;

        // Silipin ang susunod na sale
        current=current->next;
    }

    cout<<"-----------------------------------------------"<<endl;
    cout<<"Total sales: P"<<totalSales<<endl;
}


// LINEAR SEARCH: hanapin ang completed sale gamit ang Order ID
void searchSale()
{
    // Walang records? Wala pang mahahanap
    if(salesHead==NULL)
    {
        cout<<"No Data to Search."<<endl;
        return;
    }

    
    int targetID=readInteger("Input Order ID to search: ");


    // Magsimula sa unang sale at mag-check isa-isa
    SalesNode* current=salesHead;

    while(current!=NULL)
    {
        if(current->data.orderID==targetID)
        {
            cout<<"\n >>> Searching for Receipt! <<<"<<endl;
            cout<<"Order ID: "<<current->data.orderID;
            cout<<" | "<<menuItems[current->data.menuIndex].name;

            if(current->data.size=='M')
            {
                cout<<" (M)";
            }
            else
            {
                cout<<" (L)";
            }

            cout<<" x"<<current->data.quantity;
            cout<<" | Total: P"<<current->data.total<<endl;
            return;
        }

        current=current->next;
    }

    cout<<"Order ID was not found in completed sales."<<endl;
}


// BUBBLE SORT: ilagay sa unahan ang mas malaking total
void bubbleSortSales()
{
    if(salesHead==NULL||salesHead->next==NULL)
    {
        cout<<"At least two completed sales are needed to sort."
            <<endl;
        return;
    }

    bool swapped;

    do
    {
        swapped=false;
        SalesNode* current=salesHead;

        while(current->next!=NULL)
        {
            // Mas maliit ang nasa unahan? Pagpalitin para mas malaki ang mauna
            if(current->data.total<current->next->data.total)
            {
                SalesRecord temp=current->data;
                current->data=current->next->data;
                current->next->data=temp;

                swapped=true;
            }

            current=current->next;
        }

    }while(swapped==true);

    cout<<"\nThe Options have been sorted from the Expensive to Cheap:"<<endl;
    displaySales();
}


// Linisin ang natitirang waiting-order nodes bago mag-exit
void clearQueue()
{
    while(front!=NULL)
    {
        QueueNode* temp=front;
        front=front->next;
        delete temp;
    }

    front=NULL;
    rear=NULL;
}


// Linisin ang completed-sales nodes bago mag-exit
void clearSales()
{
    while(salesHead!=NULL)
    {
        SalesNode* temp=salesHead;
        salesHead=salesHead->next;
        delete temp;
    }

    salesHead=NULL;
}


//Main loop: Dito umiikot ang tindahan (favrite part)
int main()
{
    int choice;

    do
    {
        cout<<endl;
        cout<<"===== MANGOOZE ORDER AND SALES SYSTEM ====="<<endl;
        cout<<"1. Display Menu"<<endl;
        cout<<"2. Add Customer Order"<<endl;
        cout<<"3. Display Waiting Orders"<<endl;
        cout<<"4. Process Next Order"<<endl;
        cout<<"5. Display Completed Sales"<<endl;
        cout<<"6. Search Sale by Order ID"<<endl;
        cout<<"7. Sort Sales by Total"<<endl;
        cout<<"8. Exit"<<endl;

        cout<<endl;
        choice=readInteger("Choose an option (1-8): ");

        if(choice==1)
        {
            displayMenu();
        }
        else if(choice==2)
        {
            addOrder();
        }
        else if(choice==3)
        {
            displayQueue();
        }
        else if(choice==4)
        {
            processNextOrder();
        }
        else if(choice==5)
        {
            displaySales();
        }
        else if(choice==6)
        {
            searchSale();
        }
        else if(choice==7)
        {
            bubbleSortSales();
        }
        else if(choice==8)
        {
            cout<<"Thank you for stopping by at Mangooze! Come Again"<<endl;
        }
        else
        {
            cout<<"Invalid choice"<<endl;
        }

    }while(choice!=8);


    // I-free ang dynamic nodes bago tuluyang matapos ang program
    clearQueue();
    clearSales();

    return EXIT_SUCCESS;
}