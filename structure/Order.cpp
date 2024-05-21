#include "Order.hpp"

bool OrderDate::buy_ticket(Username &u,TrainID &i,Time &leave,Time &arrive, Station &f, Station &t,int &cost,int &num, Status stat,int &seq)
{
    OrderVal val;
    val.trainID=i;
    val.arriving_time=arrive;
    val.leaving_time=leave;
    val.cost=cost;
    val.from=f;
    val.to=t;
    val.seq=seq;
    val.status=stat;
    val.ticketNum=num;
    database.Insert(u,val);
    return true;
}
OrderVal val[1001];
bool OrderDate::query_order(Username &u)
{
    int size=0;
    // std::<<u<<'\n';
    database.Find(u,val,size);
    if(size==0)
    {
        std::cout<<"0\n";
        return false;
    }
    std::cout<<size<<"\n";
    string stat;
    for(int i=size-1;i>=0;--i)
    {
        if(val[i].status==pending)
        {
            stat="pending";
        }
        else if(val[i].status==success)
        {
            stat="success";
        }
        else{
            stat="refunded";
        }
        std::cout<<"["<<stat<<"]"<<' '<<val[i].trainID<<' '<<val[i].from<<' '<<val[i].leaving_time<<" -> "<<val[i].to<<' '<<val[i].arriving_time<<" "<<val[i].cost<<' '<<val[i].ticketNum<<'\n';
        // std::<<val[i].seq<<'\n';
    }
    return true;
}

bool OrderDate::refund_ticket(Username &u, int &n,OrderVal &odval)
{
    int size=0;
    database.Find(u,val,size);
    if(size<n)
    {
        return false;
    }
    if(size>1001)
    {
        throw "refund_ticket out of bound";
    }
    odval=val[size-n];
    if(odval.status==refunded)return false;
    odval.status=refunded;
    database.Delete(u,val[size-n]);
    database.Insert(u,odval);
    // database.Modify(u, odval);
    odval.status=val[size-n].status;
    return true;
}

bool OrderDate::pending_success(Username &u, int &seq)
{
    // OrderVal val[1001];
    int size=0;
    database.Find(u,val,size);
    if(size>1001)
    {
        throw "refund_ticket out of bound";
    }
    int pos=0;
    for(pos=0;pos<size;++pos)
    {
        if(val[pos].seq==seq)
        {
            break;
        }
    }
    // std::<<seq<<" "<<pos<<"\n";
    assert(pos<size && val[pos].seq==seq);
    if(val[pos].status==refunded)
    {
        std::cout<<val[pos].seq<<" "<<val[pos].trainID<<" "<<val[pos].from<<'\n';
        return false;
    }
    else if(val[pos].status==success)
    {
        std::cout<<val[pos].seq<<"what can I say!\n";
        return false;
    }
    assert(val[pos].status==pending);
    database.Delete(u,val[pos]);
    val[pos].status=success;
    database.Insert(u,val[pos]);
    // database.Modify(u,val[pos]);
    return true;
}

void OrderDate::clean()
{
    database.clear();
}