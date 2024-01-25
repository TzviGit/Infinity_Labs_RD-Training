/*****************************************************************************

Author:         Tzvi Moskowitz
Company:		Infinity
Reviewer:       Afik Grad
Date:           26.12.21
Group:          OL110
Description:    Compiling CPP source into C (including polymorphism, templates,
                inlining etc.).

*****************************************************************************/


#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc, free */
#include <stddef.h>

/*************************************************
********  MACRO TO IMITATE TEMPLATE <>  **********/

#define MAX_FUNC(t1, t2) (((t1) > (t2)) ? (t1) : (t2))


/****************************************************
********    SET TYPEDEFS   *************************/
typedef struct PT PublicTransport;
typedef struct MB Minibus;
typedef struct TX Taxi;
typedef struct specialTX SpecialTaxi;
typedef struct PublicConvoy PublicConvoy;
typedef void (*v_func)();
typedef struct VT v_table;


/***************************************************************
***** THESE MACROs IMITATE POLYMORPHISM  ***********************/

#define VTABLE_POINTER(object_pointer) (*(v_table **)(object_pointer))
#define POLYMORPHIC_DTOR(object_pointer) (VTABLE_POINTER((object_pointer))->vf_arr[0]((object_pointer)))
#define POLYMORPHIC_DISPLAY(object_pointer)  (VTABLE_POINTER((object_pointer))->vf_arr[1]((object_pointer)))
#define POLYMORPHIC_WASH(object_pointer, minutes) \
                    (VTABLE_POINTER((object_pointer))->vf_arr[2]((object_pointer), (minutes)))
#define POLYMORPHIC_DELETE(object_pointer) do {POLYMORPHIC_DTOR((object_pointer)); free((object_pointer));}while (0)

/****************************************************
*******  FUNC PROTOTYPES   *************************/
int Get_ID(PublicTransport *pt);
void CtorPT(PublicTransport *pt);
void CopyCtorPT(PublicTransport *this, PublicTransport *other);
void PTDisplay(PublicTransport *pt);
void DtorPT(PublicTransport *pt);

void CopyCtorMB(Minibus *this, Minibus *other);
void CtorMB(Minibus *mb);
void DtorMB(Minibus *mb);
void MBDisplay(Minibus *mb);
void Wash(Minibus *mb, int minutes);

void CopyCtorTX(Taxi *this, Taxi *other);
void CtorTX(Taxi *tx);
void DtorTX(Taxi *tx);
void TXDisplay(Taxi *tx);

void CtorSpecialTX(SpecialTaxi *this);
void CopyCtorSpecialTX(SpecialTaxi *this, SpecialTaxi *other);
void DtorSpecialTX(SpecialTaxi *this);
void SpecialTXDisplay(SpecialTaxi *this);

void CtorPublicConvoy(PublicConvoy *this);
void CopyCtorPublicConvoy(PublicConvoy *this, PublicConvoy *other);
void DtorPC(PublicConvoy *this);
void PCDisplay(PublicConvoy *this);

void InitPTVtable();
void InitMBVtable();
void InitTXVtable();
void InitSpecTX_vtable();

void print_count();

PublicTransport *NewPT();
Minibus *NewMinibus();
Taxi *NewTaxi();
SpecialTaxi *NewSpecialTaxi();

/****************************************************
******  VTABLE DECLARATION  ************************/

struct VT
{
    v_func vf_arr[10];

};

/***************************************************
********  GLOBAL VARS  ****************************/

int g_count = 0;

/* while were at it, heres the func which prints the g_count: */
void PrintCount()
{
    printf("s_count: %d\n", g_count);
}

v_table PT_vtable = {0};
v_table MB_vtable = {0};
v_table TX_vtable = {0};
v_table specTX_vtable = {0};
v_table PC_vtable = {0};

/********************************************************
***********  PUBLIC_TRANSPORT CLASS  ********************/
struct PT
{
    v_table *vptr;
    int m_license_plate;
};

void CtorPT(PublicTransport *pt)
{
    pt->m_license_plate = ++g_count;
   
    pt->vptr = &PT_vtable;

    printf("PublicTransport::Ctor() %d\n", pt->m_license_plate);
}

void CopyCtorPT(PublicTransport *this, PublicTransport *other)
{
    this->m_license_plate = ++g_count;
    this->vptr = &PT_vtable;

    printf("PublicTransport::CopyCtor() %d\n", this->m_license_plate);
}

void DtorPT(PublicTransport *pt)
{
    --g_count;
    pt->vptr = NULL;

    printf("PublicTransport::Dtor() %d\n", pt->m_license_plate);
}

int Get_ID(PublicTransport *pt)
{
    return pt->m_license_plate;
}

void PTDisplay(PublicTransport *pt)
{
    printf("PublicTransport::display(): %d\n", pt->m_license_plate);
}

void InitPTVtable()
{
    PT_vtable.vf_arr[0] = &DtorPT;
    PT_vtable.vf_arr[1] = &PTDisplay;
}


/********************************************************
***********  MINIBUS CLASS  **************************/
struct MB
{
    PublicTransport public_transport;
    int m_num_seats;
};

void CtorMB(Minibus *mb)
{
    CtorPT(&(mb->public_transport));

    mb->public_transport.vptr = &MB_vtable;
    mb->m_num_seats = 20;

    printf("Minibus::Ctor() \n");
}

void CopyCtorMB(Minibus *this, Minibus *other)
{
    CopyCtorPT(&this->public_transport, &other->public_transport);
    this->public_transport.vptr = &MB_vtable;
    this->m_num_seats = 20;

    printf("Minibus::CopyCtor()\n");
}


void DtorMB(Minibus *mb)
{
    printf("Minibus::Dtor()\n");
    mb->public_transport.vptr = &PT_vtable;
    
    POLYMORPHIC_DTOR(&mb->public_transport);
}

void MBDisplay(Minibus *mb)
{
    printf("Minibus::display() ID: %d ", Get_ID(&(mb->public_transport)));
    printf("num seats: %d \n", mb->m_num_seats);
}

void Wash(Minibus *mb, int minutes)
{
    printf("Minibus::wash( %d )-- ID: %d \n", minutes, Get_ID(&(mb->public_transport)));
}

void InitMBVtable()
{
    MB_vtable.vf_arr[0] = &DtorMB;
    MB_vtable.vf_arr[1] = &MBDisplay;
    MB_vtable.vf_arr[2] = &Wash;
}

/********************************************************
***********  TAXI CLASS  *******************************/

struct TX
{
    PublicTransport public_transport;
};

void CtorTX(Taxi *tx)
{
    CtorPT(&(tx->public_transport));

    tx->public_transport.vptr = &TX_vtable;

    printf("Taxi::Ctor() \n");
}

void CopyCtorTX(Taxi *this, Taxi *other)
{
    CopyCtorPT(&this->public_transport, &other->public_transport);
    this->public_transport.vptr = &TX_vtable;
    
    printf("Taxi::CopyCtor()\n");
}

void DtorTX(Taxi *this)
{
    printf("Taxi::Dtor()\n");
    this->public_transport.vptr = &PT_vtable;

    POLYMORPHIC_DTOR(&this->public_transport);
}

void TXDisplay(Taxi *tx)
{
    printf("Taxi::display() ID: %d \n", Get_ID(&(tx->public_transport)));
}

void InitTXVtable()
{
    TX_vtable.vf_arr[0] = &DtorTX;
    TX_vtable.vf_arr[1] = &TXDisplay;
}

/********************************************************
***********  SPECIAL_TAXI CLASS  *******************************/

struct specialTX
{
    Taxi taxi;
};

void CtorSpecialTX(SpecialTaxi *this)
{
    CtorTX(&(this->taxi));

    this->taxi.public_transport.vptr = &specTX_vtable;

    printf("SpecialTaxi::Ctor() \n");
}

void CopyCtorSpecialTX(SpecialTaxi *this, SpecialTaxi *other)
{
    CopyCtorTX(&this->taxi, &other->taxi);
    this->taxi.public_transport.vptr = &TX_vtable;
    
    printf("SpecialTaxi::CopyCtor()\n");
}

void DtorSpecialTX(SpecialTaxi *this)
{
    printf("SpecialTaxi::Dtor()\n");
    this->taxi.public_transport.vptr = &TX_vtable;

    POLYMORPHIC_DTOR(&this->taxi);
}

void SpecialTXDisplay(SpecialTaxi *this)
{
    printf("SpecialTaxi::display() ID: %d \n", Get_ID(&(this->taxi.public_transport)));
}

void InitSpecTX_vtable()
{
    specTX_vtable.vf_arr[0] = &DtorSpecialTX;
    specTX_vtable.vf_arr[1] = &SpecialTXDisplay;
}

/******************************************************
********  PUBLIC_CONVOY CLASS  ***********************/

struct PublicConvoy
{
    v_table *vptr;
    Minibus m_mb;
    Taxi m_tx;
    PublicTransport *pt1;
    PublicTransport *pt2;
};

void CtorPublicConvoy(PublicConvoy *this)
{
    CtorMB(&this->m_mb);
    CtorTX(&this->m_tx);
    this->pt1 = (PublicTransport *)NewMinibus();
    this->pt2 = (PublicTransport *)NewTaxi();

    this->vptr = &PC_vtable;
}

void CopyCtorPublicConvoy(PublicConvoy *this, PublicConvoy *other)
{
    this->vptr = other->vptr;
    CopyCtorMB(&this->m_mb, &other->m_mb);
    CopyCtorTX(&this->m_tx, &other->m_tx);

    this->pt1 = (PublicTransport *)NewMinibus();
    this->pt2 = (PublicTransport *)NewTaxi();
}

void DtorPC(PublicConvoy *this)
{
    POLYMORPHIC_DELETE(this->pt2);
    POLYMORPHIC_DELETE(this->pt1);

    POLYMORPHIC_DTOR(&this->m_tx);
    POLYMORPHIC_DTOR(&this->m_mb);
}

void PCDisplay(PublicConvoy *this)
{
    POLYMORPHIC_DISPLAY(&this->m_mb);
    POLYMORPHIC_DISPLAY(&this->m_tx);
    POLYMORPHIC_DISPLAY(this->pt1);
    POLYMORPHIC_DISPLAY(this->pt2);   
}

void InitPC_vtable()
{
    PC_vtable.vf_arr[0] = &DtorPC;
    PC_vtable.vf_arr[1] = &PCDisplay;
}


/*******************************************************
***********  THE "NEW" OPERATOR FUNCTIONS  ************/

PublicTransport *NewPT()
{
    PublicTransport *pt = (PublicTransport *)malloc(sizeof(*pt));
    if(NULL != pt)
    {
        CtorPT(pt);
    }
    return pt;
}

Minibus *NewMinibus()
{
    Minibus *minibus = (Minibus *)malloc(sizeof(*minibus));
    if (NULL != minibus) 
    {CtorMB(minibus);}
    return minibus;
}

Taxi *NewTaxi()
{
    Taxi *taxi = (Taxi *)malloc(sizeof(*taxi));
    if (NULL != taxi)
    {CtorTX(taxi);}
    return taxi;
}

SpecialTaxi *NewSpecialTaxi()
{
    SpecialTaxi *special_taxi = (SpecialTaxi *)malloc(sizeof(*special_taxi));
    if (NULL != special_taxi)
    {CtorSpecialTX(special_taxi);}
    return special_taxi;
}

PublicConvoy *NewPublicConvoy()
{
    PublicConvoy *pc = (PublicConvoy *)malloc(sizeof(*pc));
    if (NULL != pc)
    {
        CtorPublicConvoy(pc);
    }
    return pc;
}


/*********************************************************
*******  SOME EXTRAENIOUS FUNCTIONS   *******************/

PublicTransport RET_PTPrintInfo(int i)
{
    Minibus ret;
    PublicTransport pt;
    PublicTransport *ptr = *(PublicTransport **)&ret;
    CtorMB(&ret);
    printf("print_info(int i)\n");
    POLYMORPHIC_DISPLAY(&ret);
    (void)i;

    CopyCtorPT(&pt, ptr);
    DtorMB(&ret);
    return pt;
}

void PTPrintInfo(PublicTransport *a)
{
    POLYMORPHIC_DISPLAY(a);
}

void BLANKPrintInfo()
{
    PrintCount();
}

void MBPrintInfo(Minibus *m)
{
    POLYMORPHIC_WASH(m, 3);
}


void TaxiDisplay(Taxi s)
{
    CtorTX(&s);
    POLYMORPHIC_DISPLAY(&s);

    DtorTX(&s);
}

#define SHOW_LINE printf("line is now - %d\n", __LINE__)
/************************************************************
*******  THE TESTER --> MAIN  ******************************/

int main()
{
    int i = 0;
    Minibus m;
    PublicTransport pub_trans;
    PublicTransport *pt_array[3] = {0};
    
    /* vars for part #2 of main */

    Minibus mb2;
    Taxi taxi;
    PublicTransport pt_array2[3] = {0};
    SpecialTaxi sp_tx1;


    InitPTVtable();
    InitMBVtable();
    InitTXVtable();
    InitSpecTX_vtable();
    InitPC_vtable();

    CtorMB(&m);
    MBPrintInfo(&m);

    pub_trans = RET_PTPrintInfo(3);
    
    POLYMORPHIC_DISPLAY(&pub_trans);
    DtorPT(&pub_trans);

    pt_array[0] = (PublicTransport *)NewMinibus();
    pt_array[1] = (PublicTransport *)NewTaxi();
    pt_array[2] = (PublicTransport *)NewMinibus(); 
    
    for (i = 0; i < 3; ++i)
    {
        POLYMORPHIC_DISPLAY(pt_array[i]);
      
    }
   
    for (i = 0; i < 3; ++i)
    {
        POLYMORPHIC_DELETE(pt_array[i]);
    }
    printf("*****   PART #2  *****  \n\n");

    CtorMB(&mb2);
    CopyCtorPT(&pt_array2[0], (PublicTransport *)&mb2);
    DtorMB(&mb2);

    CtorTX(&taxi);
    CopyCtorPT(&pt_array2[1], (PublicTransport *)&taxi);
    DtorTX(&taxi);

    CtorPT(&pt_array2[2]);

    for (i = 0; i < 3; ++i)
    {
        POLYMORPHIC_DISPLAY(&pt_array2[i]);
    }

    PTPrintInfo(&pt_array2[0]);

     

     PrintCount();
     CtorMB(&mb2);
     PrintCount();

    
    /* TODO: GO BACK TO SOMETING!!! */

    printf("%d\n", MAX_FUNC(1, 2));
    printf("%d\n", MAX_FUNC(1, (int)2.0f));

    CtorSpecialTX(&sp_tx1);
    TaxiDisplay(sp_tx1.taxi);
    printf("\nHERE __ HERE _____ \n\n");
    return 0;
}