// INTEL CONFIDENTIAL - For Intel Internal Use Only
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif // HAVE_CONFIG_H
#include "gtCommon.h"

MethodCallLogEntry::MethodCallLogEntry(btcString method, Timer timestamp) :
   m_TimeStamp(timestamp)
{
   m_NVS.Add((btStringKey)"__func__", method);
}

btcString MethodCallLogEntry::MethodName() const
{
   btcString n = NULL;
   m_NVS.Get((btStringKey)"__func__", &n);
   return n;
}

void MethodCallLogEntry::AddParam(btcString name, btBool             value)
{ m_Order.push_back(TracksParamOrder(name, btBool_t)); m_NVS.Add(name, value); }
void MethodCallLogEntry::AddParam(btcString name, btByte             value)
{ m_Order.push_back(TracksParamOrder(name, btByte_t)); m_NVS.Add(name, value); }
void MethodCallLogEntry::AddParam(btcString name, bt32bitInt         value)
{ m_Order.push_back(TracksParamOrder(name, bt32bitInt_t)); m_NVS.Add(name, value); }
void MethodCallLogEntry::AddParam(btcString name, btUnsigned32bitInt value)
{ m_Order.push_back(TracksParamOrder(name, btUnsigned32bitInt_t)); m_NVS.Add(name, value); }
void MethodCallLogEntry::AddParam(btcString name, bt64bitInt         value)
{ m_Order.push_back(TracksParamOrder(name, bt64bitInt_t)); m_NVS.Add(name, value); }
void MethodCallLogEntry::AddParam(btcString name, btUnsigned64bitInt value)
{ m_Order.push_back(TracksParamOrder(name, btUnsigned64bitInt_t)); m_NVS.Add(name, value); }
void MethodCallLogEntry::AddParam(btcString name, btFloat            value)
{ m_Order.push_back(TracksParamOrder(name, btFloat_t)); m_NVS.Add(name, value); }
void MethodCallLogEntry::AddParam(btcString name, btcString          value)
{ m_Order.push_back(TracksParamOrder(name, btString_t)); m_NVS.Add(name, value); }
void MethodCallLogEntry::AddParam(btcString name, btObjectType       value)
{ m_Order.push_back(TracksParamOrder(name, btObjectType_t)); m_NVS.Add(name, value); }
void MethodCallLogEntry::AddParam(btcString name, INamedValueSet    *value)
{ m_Order.push_back(TracksParamOrder(name, btNamedValueSet_t)); m_NVS.Add(name, value); }

void MethodCallLogEntry::AddParam(btcString name, const TransactionID &value)
{
   // overload btUnknownType_t to mean const TransactionID & .
   m_Order.push_back(TracksParamOrder(name, btUnknownType_t));

   std::string var(name);
   std::string key;

   stTransactionID_t tidStruct = (stTransactionID_t)value;

   key = var + std::string(".m_Context");
   m_NVS.Add(key.c_str(), reinterpret_cast<btObjectType>(tidStruct.m_Context));

   key = var + std::string(".m_Handler");
   m_NVS.Add(key.c_str(), reinterpret_cast<btObjectType>(tidStruct.m_Handler));

   key = var + std::string(".m_IBase");
   m_NVS.Add(key.c_str(), reinterpret_cast<btObjectType>(tidStruct.m_IBase));

   key = var + std::string(".m_Filter");
   m_NVS.Add(key.c_str(), tidStruct.m_Filter);

   key = var + std::string(".m_intID");
   m_NVS.Add(key.c_str(), tidStruct.m_intID);
}

unsigned MethodCallLogEntry::Params() const
{
   return (btUnsignedInt)m_Order.size();
}

std::string MethodCallLogEntry::ParamName(unsigned i) const
{
   std::list<TracksParamOrder>::const_iterator iter = m_Order.begin();
   while ( i > 0 ) {
      ++iter;
      --i;
   }
   return (*iter).m_ParamName;
}

eBasicTypes MethodCallLogEntry::ParamType(unsigned i) const
{
   std::list<TracksParamOrder>::const_iterator iter = m_Order.begin();
   while ( i > 0 ) {
      ++iter;
      --i;
   }
   return (*iter).m_Type;
}

void MethodCallLogEntry::GetParam(btcString name, btBool                *pValue) const { m_NVS.Get(name, pValue); }
void MethodCallLogEntry::GetParam(btcString name, btByte                *pValue) const { m_NVS.Get(name, pValue); }
void MethodCallLogEntry::GetParam(btcString name, bt32bitInt            *pValue) const { m_NVS.Get(name, pValue); }
void MethodCallLogEntry::GetParam(btcString name, btUnsigned32bitInt    *pValue) const { m_NVS.Get(name, pValue); }
void MethodCallLogEntry::GetParam(btcString name, bt64bitInt            *pValue) const { m_NVS.Get(name, pValue); }
void MethodCallLogEntry::GetParam(btcString name, btUnsigned64bitInt    *pValue) const { m_NVS.Get(name, pValue); }
void MethodCallLogEntry::GetParam(btcString name, btFloat               *pValue) const { m_NVS.Get(name, pValue); }
void MethodCallLogEntry::GetParam(btcString name, btcString             *pValue) const { m_NVS.Get(name, pValue); }
void MethodCallLogEntry::GetParam(btcString name, btObjectType          *pValue) const { m_NVS.Get(name, pValue); }
void MethodCallLogEntry::GetParam(btcString name, INamedValueSet const **pValue) const { m_NVS.Get(name, pValue); }

void MethodCallLogEntry::GetParam(btcString name, TransactionID &tid) const
{
   std::string var(name);
   std::string key;

   stTransactionID_t tidStruct;

   key = var + std::string(".m_Context");
   m_NVS.Get(key.c_str(), reinterpret_cast<btObjectType *>(&tidStruct.m_Context));

   key = var + std::string(".m_Handler");
   m_NVS.Get(key.c_str(), reinterpret_cast<btObjectType *>(&tidStruct.m_Handler));

   key = var + std::string(".m_IBase");
   m_NVS.Get(key.c_str(), reinterpret_cast<btObjectType *>(&tidStruct.m_IBase));

   key = var + std::string(".m_Filter");
   m_NVS.Get(key.c_str(), &tidStruct.m_Filter);

   key = var + std::string(".m_intID");
   m_NVS.Get(key.c_str(), &tidStruct.m_intID);

   tid = tidStruct;
}

////////////////////////////////////////////////////////////////////////////////

MethodCallLogEntry * MethodCallLog::AddToLog(btcString method) const
{
   AutoLock(this);

   m_LogList.push_back(MethodCallLogEntry(method));

   iterator iter = m_LogList.end();
   --iter;
   return &(*iter);
}

unsigned MethodCallLog::LogEntries() const
{
   AutoLock(this);
   return (unsigned)m_LogList.size();
}

const MethodCallLogEntry & MethodCallLog::Entry(unsigned i) const
{
   AutoLock(this);

   const_iterator iter;
   for ( iter = m_LogList.begin() ; i-- ; ++iter ) { ; /* traverse */ }
   return *iter;
}

void MethodCallLog::ClearLog()
{
   m_LogList.clear();
}

GTCOMMON_API std::ostream & operator << (std::ostream &os, const MethodCallLog &l)
{
#define TYPE_CASE(x) case x##_t : os << #x << " "; break

   unsigned i;
   unsigned param;

   const unsigned E = l.LogEntries();
   for ( i = 0 ; i < E ; ++i ) {
      const MethodCallLogEntry &e(l.Entry(i));

      os << e.MethodName() << "(";

      const unsigned P = e.Params();
      for ( param = 0 ; param < P ; ++param ) {
         switch ( e.ParamType(param) ) {
            TYPE_CASE(btBool);
            TYPE_CASE(btByte);
            TYPE_CASE(bt32bitInt);
            TYPE_CASE(btInt);
            TYPE_CASE(btUnsigned32bitInt);
            TYPE_CASE(btUnsignedInt);
            TYPE_CASE(bt64bitInt);
            TYPE_CASE(btUnsigned64bitInt);
            TYPE_CASE(btFloat);
            TYPE_CASE(btString);
            case btNamedValueSet_t : os << "const NamedValueSet &"; break;
            TYPE_CASE(bt32bitIntArray);
            TYPE_CASE(btUnsigned32bitIntArray);
            TYPE_CASE(bt64bitIntArray);
            TYPE_CASE(btUnsigned64bitIntArray);
            TYPE_CASE(btObjectType);
            TYPE_CASE(btFloatArray);
            TYPE_CASE(btStringArray);
            TYPE_CASE(btObjectArray);
            TYPE_CASE(btByteArray);
            case btUnknownType_t : os << "const TransactionID &"; break;
         }
         os << e.ParamName(param);

         if ( P - 1 > param ) {
            os << ", ";
         }
      }

      os << ")" << std::endl;
   }

   return os;
#undef TYPE_CASE
}


////////////////////////////////////////////////////////////////////////////////
// IAALTransport

EmptyIAALTransport::EmptyIAALTransport() :
   m_connectremote_returns(true),
   m_waitforconnect_returns(true),
   m_disconnect_returns(true),
   m_getmsg_returns(""),
   m_putmsg_returns(0)
{}

btBool  EmptyIAALTransport::connectremote(NamedValueSet const & ) { return m_connectremote_returns;    }
btBool EmptyIAALTransport::waitforconnect(NamedValueSet const & ) { return m_waitforconnect_returns;   }
btBool     EmptyIAALTransport::disconnect()                       { return m_disconnect_returns;       }
btcString      EmptyIAALTransport::getmsg(btWSSize *pSz)          { *pSz = 0; return m_getmsg_returns; }
int            EmptyIAALTransport::putmsg(btcString , btWSSize )  { return m_putmsg_returns;           }

IMPLEMENT_RETVAL_ACCESSORS(EmptyIAALTransport, connectremote,  btBool,    m_connectremote_returns )
IMPLEMENT_RETVAL_ACCESSORS(EmptyIAALTransport, waitforconnect, btBool,    m_waitforconnect_returns)
IMPLEMENT_RETVAL_ACCESSORS(EmptyIAALTransport, disconnect,     btBool,    m_disconnect_returns    )
IMPLEMENT_RETVAL_ACCESSORS(EmptyIAALTransport, getmsg,         btcString, m_getmsg_returns        )
IMPLEMENT_RETVAL_ACCESSORS(EmptyIAALTransport, putmsg,         int,       m_putmsg_returns        )

////////////////////////////////////////////////////////////////////////////////
// IAALMarshaller

EmptyIAALMarshaller::EmptyIAALMarshaller() :
   m_pmsgp_returns(NULL)
{}

ENamedValues   EmptyIAALMarshaller::Empty()                                              { return m_NVS.Empty();               }
btBool           EmptyIAALMarshaller::Has(btStringKey Name)                        const { return m_NVS.Has(Name);             }
ENamedValues  EmptyIAALMarshaller::Delete(btStringKey Name)                              { return m_NVS.Delete(Name);          }
ENamedValues EmptyIAALMarshaller::GetSize(btStringKey Name,    btWSSize    *pSz)   const { return m_NVS.GetSize(Name, pSz);    }
ENamedValues    EmptyIAALMarshaller::Type(btStringKey Name,    eBasicTypes *pTy)   const { return m_NVS.Type(Name, pTy);       }
ENamedValues EmptyIAALMarshaller::GetName(btUnsignedInt index, btStringKey *pName) const { return m_NVS.GetName(index, pName); }

ENamedValues EmptyIAALMarshaller::Add(btNumberKey Name, btBool                Value) { return m_NVS.Add(Name, Value); }
ENamedValues EmptyIAALMarshaller::Add(btNumberKey Name, btByte                Value) { return m_NVS.Add(Name, Value); }
ENamedValues EmptyIAALMarshaller::Add(btNumberKey Name, bt32bitInt            Value) { return m_NVS.Add(Name, Value); }
ENamedValues EmptyIAALMarshaller::Add(btNumberKey Name, btUnsigned32bitInt    Value) { return m_NVS.Add(Name, Value); }
ENamedValues EmptyIAALMarshaller::Add(btNumberKey Name, bt64bitInt            Value) { return m_NVS.Add(Name, Value); }
ENamedValues EmptyIAALMarshaller::Add(btNumberKey Name, btUnsigned64bitInt    Value) { return m_NVS.Add(Name, Value); }
ENamedValues EmptyIAALMarshaller::Add(btNumberKey Name, btFloat               Value) { return m_NVS.Add(Name, Value); }
ENamedValues EmptyIAALMarshaller::Add(btNumberKey Name, btcString             Value) { return m_NVS.Add(Name, Value); }
ENamedValues EmptyIAALMarshaller::Add(btNumberKey Name, btObjectType          Value) { return m_NVS.Add(Name, Value); }
ENamedValues EmptyIAALMarshaller::Add(btNumberKey Name, const INamedValueSet *Value) { return m_NVS.Add(Name, Value); }

ENamedValues EmptyIAALMarshaller::Add(btNumberKey Name,
                                      btByteArray             Value, btUnsigned32bitInt NumElements)
{ return m_NVS.Add(Name, Value, NumElements); }
ENamedValues EmptyIAALMarshaller::Add(btNumberKey Name,
                                      bt32bitIntArray         Value, btUnsigned32bitInt NumElements)
{ return m_NVS.Add(Name, Value, NumElements); }
ENamedValues EmptyIAALMarshaller::Add(btNumberKey Name,
                                      btUnsigned32bitIntArray Value, btUnsigned32bitInt NumElements)
{ return m_NVS.Add(Name, Value, NumElements); }
ENamedValues EmptyIAALMarshaller::Add(btNumberKey Name,
                                      bt64bitIntArray         Value, btUnsigned32bitInt NumElements)
{ return m_NVS.Add(Name, Value, NumElements); }
ENamedValues EmptyIAALMarshaller::Add(btNumberKey Name,
                                      btUnsigned64bitIntArray Value, btUnsigned32bitInt NumElements)
{ return m_NVS.Add(Name, Value, NumElements); }
ENamedValues EmptyIAALMarshaller::Add(btNumberKey Name,
                                      btFloatArray            Value, btUnsigned32bitInt NumElements)
{ return m_NVS.Add(Name, Value, NumElements); }
ENamedValues EmptyIAALMarshaller::Add(btNumberKey Name,
                                      btStringArray           Value, btUnsigned32bitInt NumElements)
{ return m_NVS.Add(Name, Value, NumElements); }
ENamedValues EmptyIAALMarshaller::Add(btNumberKey Name,
                                      btObjectArray           Value, btUnsigned32bitInt NumElements)
{ return m_NVS.Add(Name, Value, NumElements); }

ENamedValues EmptyIAALMarshaller::Add(btStringKey Name, btBool                Value) { return m_NVS.Add(Name, Value); }
ENamedValues EmptyIAALMarshaller::Add(btStringKey Name, btByte                Value) { return m_NVS.Add(Name, Value); }
ENamedValues EmptyIAALMarshaller::Add(btStringKey Name, bt32bitInt            Value) { return m_NVS.Add(Name, Value); }
ENamedValues EmptyIAALMarshaller::Add(btStringKey Name, btUnsigned32bitInt    Value) { return m_NVS.Add(Name, Value); }
ENamedValues EmptyIAALMarshaller::Add(btStringKey Name, bt64bitInt            Value) { return m_NVS.Add(Name, Value); }
ENamedValues EmptyIAALMarshaller::Add(btStringKey Name, btUnsigned64bitInt    Value) { return m_NVS.Add(Name, Value); }
ENamedValues EmptyIAALMarshaller::Add(btStringKey Name, btFloat               Value) { return m_NVS.Add(Name, Value); }
ENamedValues EmptyIAALMarshaller::Add(btStringKey Name, btcString             Value) { return m_NVS.Add(Name, Value); }
ENamedValues EmptyIAALMarshaller::Add(btStringKey Name, btObjectType          Value) { return m_NVS.Add(Name, Value); }
ENamedValues EmptyIAALMarshaller::Add(btStringKey Name, const INamedValueSet *Value) { return m_NVS.Add(Name, Value); }

ENamedValues EmptyIAALMarshaller::Add(btStringKey Name,
                                      btByteArray             Value, btUnsigned32bitInt NumElements)
{ return m_NVS.Add(Name, Value, NumElements); }
ENamedValues EmptyIAALMarshaller::Add(btStringKey Name,
                                      bt32bitIntArray         Value, btUnsigned32bitInt NumElements)
{ return m_NVS.Add(Name, Value, NumElements); }
ENamedValues EmptyIAALMarshaller::Add(btStringKey Name,
                                      btUnsigned32bitIntArray Value, btUnsigned32bitInt NumElements)
{ return m_NVS.Add(Name, Value, NumElements); }
ENamedValues EmptyIAALMarshaller::Add(btStringKey Name,
                                      bt64bitIntArray         Value, btUnsigned32bitInt NumElements)
{ return m_NVS.Add(Name, Value, NumElements); }
ENamedValues EmptyIAALMarshaller::Add(btStringKey Name,
                                      btUnsigned64bitIntArray Value, btUnsigned32bitInt NumElements)
{ return m_NVS.Add(Name, Value, NumElements); }
ENamedValues EmptyIAALMarshaller::Add(btStringKey Name,
                                      btFloatArray            Value, btUnsigned32bitInt NumElements)
{ return m_NVS.Add(Name, Value, NumElements); }
ENamedValues EmptyIAALMarshaller::Add(btStringKey Name,
                                      btStringArray           Value, btUnsigned32bitInt NumElements)
{ return m_NVS.Add(Name, Value, NumElements); }
ENamedValues EmptyIAALMarshaller::Add(btStringKey Name,
                                      btObjectArray           Value, btUnsigned32bitInt NumElements)
{ return m_NVS.Add(Name, Value, NumElements); }

char const * EmptyIAALMarshaller::pmsgp(btWSSize *len) { return m_pmsgp_returns; }

IMPLEMENT_RETVAL_ACCESSORS(EmptyIAALMarshaller, pmsgp, char const *, m_pmsgp_returns)

////////////////////////////////////////////////////////////////////////////////
// IAALUnMarshaller

EmptyIAALUnMarshaller::EmptyIAALUnMarshaller() {}

ENamedValues   EmptyIAALUnMarshaller::Empty()                                              { return m_NVS.Empty();               }
btBool           EmptyIAALUnMarshaller::Has(btStringKey Name)                        const { return m_NVS.Has(Name);             }
ENamedValues  EmptyIAALUnMarshaller::Delete(btStringKey Name)                              { return m_NVS.Delete(Name);          }
ENamedValues EmptyIAALUnMarshaller::GetSize(btStringKey Name,    btWSSize    *pSz)   const { return m_NVS.GetSize(Name, pSz);    }
ENamedValues    EmptyIAALUnMarshaller::Type(btStringKey Name,    eBasicTypes *pTy)   const { return m_NVS.Type(Name, pTy);       }
ENamedValues EmptyIAALUnMarshaller::GetName(btUnsignedInt index, btStringKey *pName) const { return m_NVS.GetName(index, pName); }

ENamedValues EmptyIAALUnMarshaller::Get(btNumberKey Name, btBool                  *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btNumberKey Name, btByte                  *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btNumberKey Name, bt32bitInt              *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btNumberKey Name, btUnsigned32bitInt      *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btNumberKey Name, bt64bitInt              *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btNumberKey Name, btUnsigned64bitInt      *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btNumberKey Name, btFloat                 *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btNumberKey Name, btcString               *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btNumberKey Name, btObjectType            *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btNumberKey Name, INamedValueSet const   **pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btNumberKey Name, btByteArray             *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btNumberKey Name, bt32bitIntArray         *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btNumberKey Name, btUnsigned32bitIntArray *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btNumberKey Name, bt64bitIntArray         *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btNumberKey Name, btUnsigned64bitIntArray *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btNumberKey Name, btFloatArray            *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btNumberKey Name, btStringArray           *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btNumberKey Name, btObjectArray           *pValue) const { return m_NVS.Get(Name, pValue); }

ENamedValues EmptyIAALUnMarshaller::Get(btStringKey Name, btBool                  *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btStringKey Name, btByte                  *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btStringKey Name, bt32bitInt              *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btStringKey Name, btUnsigned32bitInt      *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btStringKey Name, bt64bitInt              *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btStringKey Name, btUnsigned64bitInt      *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btStringKey Name, btFloat                 *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btStringKey Name, btcString               *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btStringKey Name, btObjectType            *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btStringKey Name, INamedValueSet const   **pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btStringKey Name, btByteArray             *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btStringKey Name, bt32bitIntArray         *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btStringKey Name, btUnsigned32bitIntArray *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btStringKey Name, bt64bitIntArray         *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btStringKey Name, btUnsigned64bitIntArray *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btStringKey Name, btFloatArray            *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btStringKey Name, btStringArray           *pValue) const { return m_NVS.Get(Name, pValue); }
ENamedValues EmptyIAALUnMarshaller::Get(btStringKey Name, btObjectArray           *pValue) const { return m_NVS.Get(Name, pValue); }

void EmptyIAALUnMarshaller::importmsg(char const *pmsg, btWSSize len) {}

////////////////////////////////////////////////////////////////////////////////
// IServiceClient

EmptyIServiceClient::EmptyIServiceClient()
{
   if ( EObjOK != SetInterface(iidServiceClient, dynamic_cast<IServiceClient *>(this)) ) {
      m_bIsOK = false;
   }
}

CallTrackingIServiceClient::CallTrackingIServiceClient()
{}

void CallTrackingIServiceClient::serviceAllocated(IBase               *pBase,
                                                  TransactionID const &tid)
{
   MethodCallLogEntry *l = AddToLog("IServiceClient::serviceAllocated");
   l->AddParam("pBase", reinterpret_cast<btObjectType>(pBase));
   l->AddParam("tid",   tid);
}

void CallTrackingIServiceClient::serviceAllocateFailed(const IEvent &e)
{
   MethodCallLogEntry *l = AddToLog("IServiceClient::serviceAllocateFailed");
   l->AddParam("e", reinterpret_cast<btObjectType>( & const_cast<IEvent &>(e) ));
}

void CallTrackingIServiceClient::serviceReleased(TransactionID const &tid)
{
   MethodCallLogEntry *l = AddToLog("IServiceClient::serviceReleased");
   l->AddParam("tid", tid);
}

void CallTrackingIServiceClient::serviceReleaseFailed(const IEvent &e)
{
   MethodCallLogEntry *l = AddToLog("IServiceClient::serviceReleaseFailed");
   l->AddParam("e", reinterpret_cast<btObjectType>( & const_cast<IEvent &>(e) ));
}

void CallTrackingIServiceClient::serviceEvent(const IEvent &e)
{
   MethodCallLogEntry *l = AddToLog("IServiceClient::serviceEvent");
   l->AddParam("e", reinterpret_cast<btObjectType>( & const_cast<IEvent &>(e) ));
}

SynchronizingIServiceClient::SynchronizingIServiceClient()
{
   m_Bar.Create(1, false);
}

void SynchronizingIServiceClient::serviceAllocated(IBase               *pBase,
                                                   TransactionID const &tid)
{
   CallTrackingIServiceClient::serviceAllocated(pBase, tid);
   Post();
}

void SynchronizingIServiceClient::serviceAllocateFailed(const IEvent &e)
{
   CallTrackingIServiceClient::serviceAllocateFailed(e);
   Post();
}

void SynchronizingIServiceClient::serviceReleased(TransactionID const &tid)
{
   CallTrackingIServiceClient::serviceReleased(tid);
   Post();
}

void SynchronizingIServiceClient::serviceReleaseFailed(const IEvent &e)
{
   CallTrackingIServiceClient::serviceReleaseFailed(e);
   Post();
}

void SynchronizingIServiceClient::serviceEvent(const IEvent &e)
{
   CallTrackingIServiceClient::serviceEvent(e);
   Post();
}

btBool SynchronizingIServiceClient::Wait(btTime Timeout)
{
   btBool resWait  = m_Bar.Wait(Timeout);
   btBool resReset = m_Bar.Reset();
   return resWait && resReset;
}

btBool SynchronizingIServiceClient::Post(btUnsignedInt Count) { return m_Bar.Post(Count); }

////////////////////////////////////////////////////////////////////////////////
// IRuntimeClient

EmptyIRuntimeClient::EmptyIRuntimeClient()
{
   if ( EObjOK != SetInterface(iidRuntimeClient, dynamic_cast<IRuntimeClient *>(this)) ) {
      m_bIsOK = false;
   }
}

CallTrackingIRuntimeClient::CallTrackingIRuntimeClient()
{}

void CallTrackingIRuntimeClient::runtimeCreateOrGetProxyFailed(IEvent const &e)
{
   MethodCallLogEntry *l = AddToLog("IRuntimeClient::runtimeCreateOrGetProxyFailed");
   l->AddParam("e", reinterpret_cast<btObjectType>( & const_cast<IEvent &>(e) ));
}

void CallTrackingIRuntimeClient::runtimeStarted(IRuntime            *pRuntime,
                                                const NamedValueSet &nvs)
{
   MethodCallLogEntry *l = AddToLog("IRuntimeClient::runtimeStarted");
   l->AddParam("pRuntime", reinterpret_cast<btObjectType>(pRuntime));
   l->AddParam("nvs",      dynamic_cast<INamedValueSet *>( & const_cast<NamedValueSet &>(nvs)));
}

void CallTrackingIRuntimeClient::runtimeStopped(IRuntime *pRuntime)
{
   MethodCallLogEntry *l = AddToLog("IRuntimeClient::runtimeStopped");
   l->AddParam("pRuntime", reinterpret_cast<btObjectType>(pRuntime));
}

void CallTrackingIRuntimeClient::runtimeStartFailed(const IEvent &e)
{
   MethodCallLogEntry *l = AddToLog("IRuntimeClient::runtimeStartFailed");
   l->AddParam("e", reinterpret_cast<btObjectType>( & const_cast<IEvent &>(e) ));
}

void CallTrackingIRuntimeClient::runtimeStopFailed(const IEvent &e)
{
   MethodCallLogEntry *l = AddToLog("IRuntimeClient::runtimeStopFailed");
   l->AddParam("e", reinterpret_cast<btObjectType>( & const_cast<IEvent &>(e) ));
}

void CallTrackingIRuntimeClient::runtimeAllocateServiceFailed(IEvent const &e)
{
   MethodCallLogEntry *l = AddToLog("IRuntimeClient::runtimeAllocateServiceFailed");
   l->AddParam("e", reinterpret_cast<btObjectType>( & const_cast<IEvent &>(e) ));
}

void CallTrackingIRuntimeClient::runtimeAllocateServiceSucceeded(IBase               *pServiceBase,
                                                                 TransactionID const &tid)
{
   MethodCallLogEntry *l = AddToLog("IRuntimeClient::runtimeAllocateServiceSucceeded");
   l->AddParam("pServiceBase", reinterpret_cast<btObjectType>(pServiceBase));
   l->AddParam("tid",          tid);
}

void CallTrackingIRuntimeClient::runtimeEvent(const IEvent &e)
{
   MethodCallLogEntry *l = AddToLog("IRuntimeClient::runtimeEvent");
   l->AddParam("e", reinterpret_cast<btObjectType>( & const_cast<IEvent &>(e) ));
}

SynchronizingIRuntimeClient::SynchronizingIRuntimeClient()
{
   m_Bar.Create(1, false);
}

void SynchronizingIRuntimeClient::runtimeCreateOrGetProxyFailed(IEvent const &e)
{
   CallTrackingIRuntimeClient::runtimeCreateOrGetProxyFailed(e);
   Post();
}

void SynchronizingIRuntimeClient::runtimeStarted(IRuntime            *pRuntime,
                                                 const NamedValueSet &nvs)
{
   CallTrackingIRuntimeClient::runtimeStarted(pRuntime, nvs);
   Post();
}

void SynchronizingIRuntimeClient::runtimeStopped(IRuntime *pRuntime)
{
   CallTrackingIRuntimeClient::runtimeStopped(pRuntime);
   Post();
}

void SynchronizingIRuntimeClient::runtimeStartFailed(const IEvent &e)
{
   CallTrackingIRuntimeClient::runtimeStartFailed(e);
   Post();
}

void SynchronizingIRuntimeClient::runtimeStopFailed(const IEvent &e)
{
   CallTrackingIRuntimeClient::runtimeStopFailed(e);
   Post();
}

void SynchronizingIRuntimeClient::runtimeAllocateServiceFailed(IEvent const &e)
{
   CallTrackingIRuntimeClient::runtimeAllocateServiceFailed(e);
   Post();
}

void SynchronizingIRuntimeClient::runtimeAllocateServiceSucceeded(IBase               *pServiceBase,
                                                                  TransactionID const &tid)
{
   CallTrackingIRuntimeClient::runtimeAllocateServiceSucceeded(pServiceBase, tid);
   Post();
}

void SynchronizingIRuntimeClient::runtimeEvent(const IEvent &e)
{
   CallTrackingIRuntimeClient::runtimeEvent(e);
   Post();
}

btBool SynchronizingIRuntimeClient::Wait(btTime Timeout)
{
   btBool resWait  = m_Bar.Wait(Timeout);
   btBool resReset = m_Bar.Reset();
   return resWait && resReset;
}

btBool SynchronizingIRuntimeClient::Post(btUnsignedInt Count) { return m_Bar.Post(Count); }

////////////////////////////////////////////////////////////////////////////////
// ISvcsFact

EmptyISvcsFact::EmptyISvcsFact() :
   m_CreateServiceObject_returns(NULL),
   m_InitializeService_returns(true)
{}

IBase * EmptyISvcsFact::CreateServiceObject(AALServiceModule * ,
                                            IRuntime         * ) { return m_CreateServiceObject_returns; }

btBool EmptyISvcsFact::InitializeService(IBase               * ,
                                         IBase               * ,
                                         TransactionID const & ,
                                         NamedValueSet const & ) { return m_InitializeService_returns; }

IMPLEMENT_RETVAL_ACCESSORS(EmptyISvcsFact, CreateServiceObject, IBase * , m_CreateServiceObject_returns)
IMPLEMENT_RETVAL_ACCESSORS(EmptyISvcsFact, InitializeService,   btBool ,  m_InitializeService_returns)

IBase * CallTrackingISvcsFact::CreateServiceObject(AALServiceModule *container,
                                                   IRuntime         *pRuntime)
{
   MethodCallLogEntry *l = AddToLog("ISvcsFact::CreateServiceObject");
   l->AddParam("container", reinterpret_cast<btObjectType>(container));
   l->AddParam("pRuntime",  reinterpret_cast<btObjectType>(pRuntime));
   return EmptyISvcsFact::CreateServiceObject(container, pRuntime);
}

void CallTrackingISvcsFact::DestroyServiceObject(IBase *pServiceBase)
{
   MethodCallLogEntry *l = AddToLog("ISvcsFact::DestroyServiceObject");
   l->AddParam("pServiceBase", reinterpret_cast<btObjectType>(pServiceBase));
}

btBool CallTrackingISvcsFact::InitializeService(IBase               *newService,
                                                IBase               *Client,
                                                TransactionID const &rtid,
                                                NamedValueSet const &optArgs)
{
   MethodCallLogEntry *l = AddToLog("ISvcsFact::InitializeService");
   l->AddParam("newService", reinterpret_cast<btObjectType>(newService));
   l->AddParam("Client",     reinterpret_cast<btObjectType>(Client));
   l->AddParam("rtid",       rtid);
   l->AddParam("optArgs",    dynamic_cast<INamedValueSet *>( & const_cast<NamedValueSet &>(optArgs) ));
   return EmptyISvcsFact::InitializeService(newService, Client, rtid, optArgs);
}

////////////////////////////////////////////////////////////////////////////////
// IRuntime

EmptyIRuntime::EmptyIRuntime() :
   m_start_returns(true),
   m_schedDispatchable_returns(true),
   m_getRuntimeProxy_returns(NULL),
   m_releaseRuntimeProxy_returns(true),
   m_getRuntimeClient_returns(NULL),
   m_IsOK_returns(true)
{
   if ( EObjOK != SetInterface(iidRuntime, dynamic_cast<IRuntime *>(this)) ) {
      m_bIsOK = false;
   }
}

btBool                      EmptyIRuntime::start(const NamedValueSet & ) { return m_start_returns;               }
btBool          EmptyIRuntime::schedDispatchable(IDispatchable * )       { return m_schedDispatchable_returns;   }
IRuntime *        EmptyIRuntime::getRuntimeProxy(IRuntimeClient * )      { return m_getRuntimeProxy_returns;     }
btBool        EmptyIRuntime::releaseRuntimeProxy()                       { return m_releaseRuntimeProxy_returns; }
IRuntimeClient * EmptyIRuntime::getRuntimeClient()                       { return m_getRuntimeClient_returns;    }
btBool                       EmptyIRuntime::IsOK()                       { return m_IsOK_returns;                }

IMPLEMENT_RETVAL_ACCESSORS(EmptyIRuntime, start,               btBool,            m_start_returns              )
IMPLEMENT_RETVAL_ACCESSORS(EmptyIRuntime, schedDispatchable,   btBool,            m_schedDispatchable_returns  )
IMPLEMENT_RETVAL_ACCESSORS(EmptyIRuntime, getRuntimeProxy,     IRuntime * ,       m_getRuntimeProxy_returns    )
IMPLEMENT_RETVAL_ACCESSORS(EmptyIRuntime, releaseRuntimeProxy, btBool,            m_releaseRuntimeProxy_returns)
IMPLEMENT_RETVAL_ACCESSORS(EmptyIRuntime, getRuntimeClient,    IRuntimeClient * , m_getRuntimeClient_returns   )
IMPLEMENT_RETVAL_ACCESSORS(EmptyIRuntime, IsOK,                btBool,            m_IsOK_returns               )

CallTrackingIRuntime::CallTrackingIRuntime()
{}

btBool CallTrackingIRuntime::start(const NamedValueSet &rconfigParms)
{
   MethodCallLogEntry *l = AddToLog("IRuntime::start");
   l->AddParam("rconfigParms", dynamic_cast<INamedValueSet *>( & const_cast<NamedValueSet &>(rconfigParms) ) );
   return EmptyIRuntime::start(rconfigParms);
}

void CallTrackingIRuntime::stop()
{
   AddToLog("IRuntime::stop");
}

void CallTrackingIRuntime::allocService(IBase               *pClient,
                                        NamedValueSet const &rManifest,
                                        TransactionID const &rTranID)
{
   MethodCallLogEntry *l = AddToLog("IRuntime::allocService");
   l->AddParam("pClient",   reinterpret_cast<btObjectType>(pClient));
   l->AddParam("rManifest", dynamic_cast<INamedValueSet *>( & const_cast<NamedValueSet &>(rManifest) ) );
   l->AddParam("rTranID",   rTranID);
}

btBool CallTrackingIRuntime::schedDispatchable(IDispatchable *pDisp)
{
   MethodCallLogEntry *l = AddToLog("IRuntime::schedDispatchable");
   l->AddParam("pDisp", reinterpret_cast<btObjectType>(pDisp));
   return EmptyIRuntime::schedDispatchable(pDisp);
}

IRuntime * CallTrackingIRuntime::getRuntimeProxy(IRuntimeClient *pClient)
{
   MethodCallLogEntry *l = AddToLog("IRuntime::getRuntimeProxy");
   l->AddParam("pClient", reinterpret_cast<btObjectType>(pClient));
   return EmptyIRuntime::getRuntimeProxy(pClient);
}

btBool CallTrackingIRuntime::releaseRuntimeProxy()
{
   AddToLog("IRuntime::releaseRuntimeProxy");
   return EmptyIRuntime::releaseRuntimeProxy();
}

IRuntimeClient * CallTrackingIRuntime::getRuntimeClient()
{
   AddToLog("IRuntime::getRuntimeClient");
   return EmptyIRuntime::getRuntimeClient();
}

btBool CallTrackingIRuntime::IsOK()
{
   AddToLog("IRuntime::IsOK");
   return EmptyIRuntime::IsOK();
}

////////////////////////////////////////////////////////////////////////////////
// IServiceBase

EmptyIServiceBase::EmptyIServiceBase() :
   m_initComplete_returns(false),
   m_initFailed_returns(false),
   m_ReleaseComplete_returns(false),
   m_getServiceClient_returns(NULL),
   m_getServiceClientBase_returns(NULL),
   m_getRuntime_returns(NULL),
   m_getRuntimeClient_returns(NULL),
   m_getAALServiceModule_returns(NULL)
{
   if ( EObjOK != SetInterface(iidServiceBase, this) ) {
      m_bIsOK = false;
   }
}

btBool                    EmptyIServiceBase::initComplete(TransactionID const & ) { return m_initComplete_returns;         }
btBool                      EmptyIServiceBase::initFailed(IEvent const * )        { return m_initFailed_returns;           }
btBool                 EmptyIServiceBase::ReleaseComplete()                       { return m_ReleaseComplete_returns;      }
NamedValueSet const &          EmptyIServiceBase::OptArgs()                 const { return m_OptArgs_returns;              }
IServiceClient *      EmptyIServiceBase::getServiceClient()                 const { return m_getServiceClient_returns;     }
IBase *           EmptyIServiceBase::getServiceClientBase()                 const { return m_getServiceClientBase_returns; }
IRuntime *                  EmptyIServiceBase::getRuntime()                 const { return m_getRuntime_returns;           }
IRuntimeClient *      EmptyIServiceBase::getRuntimeClient()                 const { return m_getRuntimeClient_returns;     }
AALServiceModule * EmptyIServiceBase::getAALServiceModule()                 const { return m_getAALServiceModule_returns;  }

IMPLEMENT_RETVAL_ACCESSORS(EmptyIServiceBase, initComplete,         btBool,                m_initComplete_returns)
IMPLEMENT_RETVAL_ACCESSORS(EmptyIServiceBase, initFailed,           btBool,                m_initFailed_returns)
IMPLEMENT_RETVAL_ACCESSORS(EmptyIServiceBase, ReleaseComplete,      btBool,                m_ReleaseComplete_returns)
IMPLEMENT_RETVAL_ACCESSORS(EmptyIServiceBase, OptArgs,              NamedValueSet const &, m_OptArgs_returns)
IMPLEMENT_RETVAL_ACCESSORS(EmptyIServiceBase, getServiceClient,     IServiceClient *,      m_getServiceClient_returns)
IMPLEMENT_RETVAL_ACCESSORS(EmptyIServiceBase, getServiceClientBase, IBase *,               m_getServiceClientBase_returns)
IMPLEMENT_RETVAL_ACCESSORS(EmptyIServiceBase, getRuntime,           IRuntime *,            m_getRuntime_returns)
IMPLEMENT_RETVAL_ACCESSORS(EmptyIServiceBase, getRuntimeClient,     IRuntimeClient *,      m_getRuntimeClient_returns)
IMPLEMENT_RETVAL_ACCESSORS(EmptyIServiceBase, getAALServiceModule,  AALServiceModule *,    m_getAALServiceModule_returns)

CallTrackingIServiceBase::CallTrackingIServiceBase()
{}

btBool CallTrackingIServiceBase::initComplete(TransactionID const &rtid)
{
   MethodCallLogEntry *l = AddToLog("IServiceBase::initComplete");
   l->AddParam("rtid", rtid);
   return EmptyIServiceBase::initComplete(rtid);
}

btBool CallTrackingIServiceBase::initFailed(IEvent const *ptheEvent)
{
   MethodCallLogEntry *l = AddToLog("IServiceBase::initFailed");
   l->AddParam("ptheEvent", reinterpret_cast<btObjectType>( const_cast<IEvent *>(ptheEvent) ));
   return EmptyIServiceBase::initFailed(ptheEvent);
}

btBool CallTrackingIServiceBase::ReleaseComplete()
{
   AddToLog("IServiceBase::ReleaseComplete");
   return EmptyIServiceBase::ReleaseComplete();
}

NamedValueSet const & CallTrackingIServiceBase::OptArgs() const
{
   AddToLog("IServiceBase::OptArgs");
   return EmptyIServiceBase::OptArgs();
}

IServiceClient * CallTrackingIServiceBase::getServiceClient() const
{
   AddToLog("IServiceBase::getServiceClient");
   return EmptyIServiceBase::getServiceClient();
}

IBase * CallTrackingIServiceBase::getServiceClientBase() const
{
   AddToLog("IServiceBase::getServiceClientBase");
   return EmptyIServiceBase::getServiceClientBase();
}

IRuntime * CallTrackingIServiceBase::getRuntime() const
{
   AddToLog("IServiceBase::getRuntime");
   return EmptyIServiceBase::getRuntime();
}

IRuntimeClient * CallTrackingIServiceBase::getRuntimeClient() const
{
   AddToLog("IServiceBase::getRuntimeClient");
   return EmptyIServiceBase::getRuntimeClient();
}

AALServiceModule * CallTrackingIServiceBase::getAALServiceModule() const
{
   AddToLog("IServiceBase::getAALServiceModule");
   return EmptyIServiceBase::getAALServiceModule();
}

////////////////////////////////////////////////////////////////////////////////
// ServiceBase

EmptyServiceBase::EmptyServiceBase(AALServiceModule *container,
                                   IRuntime         *pAALRUNTIME,
                                   IAALTransport    *ptransport,
                                   IAALMarshaller   *marshaller,
                                   IAALUnMarshaller *unmarshaller) :
   ServiceBase(container, pAALRUNTIME, ptransport, marshaller, unmarshaller),
   m_init_returns(false)
{}

btBool EmptyServiceBase::init(IBase * ,
                              NamedValueSet const & ,
                              TransactionID const & ) { return m_init_returns; }

IMPLEMENT_RETVAL_ACCESSORS(EmptyServiceBase, init, btBool, m_init_returns)

void EmptyServiceBase::ServiceClient(IServiceClient *pSvcClient)
{
   m_pclient = pSvcClient;
}

void EmptyServiceBase::ServiceClientBase(IBase *pBase)
{
   m_pclientbase = pBase;
}

void EmptyServiceBase::RT(IRuntime *pRT)
{
   m_Runtime = pRT;
}

CallTrackingServiceBase::CallTrackingServiceBase(AALServiceModule *container,
                                                 IRuntime         *pAALRUNTIME,
                                                 IAALTransport    *ptransport,
                                                 IAALMarshaller   *marshaller,
                                                 IAALUnMarshaller *unmarshaller) :
   EmptyServiceBase(container, pAALRUNTIME, ptransport, marshaller, unmarshaller)
{}

btBool CallTrackingServiceBase::init(IBase               *pclientBase,
                                     NamedValueSet const &optArgs,
                                     TransactionID const &rtid)
{
   MethodCallLogEntry *l = AddToLog("ServiceBase::init");
   l->AddParam("pclientBase", reinterpret_cast<btObjectType>(pclientBase));
   l->AddParam("optArgs",     dynamic_cast<INamedValueSet *>( & const_cast<NamedValueSet &>(optArgs)) );
   l->AddParam("rtid",        rtid);
   return EmptyServiceBase::init(pclientBase, optArgs, rtid);
}

btBool CallTrackingServiceBase::_init(IBase               *pclientBase,
                                      TransactionID const &rtid,
                                      NamedValueSet const &optArgs,
                                      CAALEvent           *pcmpltEvent)
{
   MethodCallLogEntry *l = AddToLog("ServiceBase::_init");
   l->AddParam("pclientBase", reinterpret_cast<btObjectType>(pclientBase));
   l->AddParam("rtid",        rtid);
   l->AddParam("optArgs",     dynamic_cast<INamedValueSet *>( & const_cast<NamedValueSet &>(optArgs)) );
   l->AddParam("pcmpltEvent", reinterpret_cast<btObjectType>(pcmpltEvent));
   return EmptyServiceBase::_init(pclientBase, rtid, optArgs, pcmpltEvent);
}

btBool CallTrackingServiceBase::Release(TransactionID const &rTranID, btTime timeout)
{
   MethodCallLogEntry *l = AddToLog("ServiceBase::Release");
   l->AddParam("rTranID", rTranID);
   l->AddParam("timeout", timeout);
   return EmptyServiceBase::Release(rTranID, timeout);
}

btBool CallTrackingServiceBase::initComplete(TransactionID const &rtid)
{
   MethodCallLogEntry *l = AddToLog("ServiceBase::initComplete");
   l->AddParam("rtid", rtid);
   return EmptyServiceBase::initComplete(rtid);
}

btBool CallTrackingServiceBase::initFailed(IEvent const *ptheEvent)
{
   MethodCallLogEntry *l = AddToLog("ServiceBase::initFailed");
   l->AddParam("ptheEvent", reinterpret_cast<btObjectType>( const_cast<IEvent *>(ptheEvent) ));
   return EmptyServiceBase::initFailed(ptheEvent);
}

btBool CallTrackingServiceBase::ReleaseComplete()
{
   AddToLog("ServiceBase::ReleaseComplete");
   return EmptyServiceBase::ReleaseComplete();
}

NamedValueSet const & CallTrackingServiceBase::OptArgs() const
{
   AddToLog("ServiceBase::OptArgs");
   return EmptyServiceBase::OptArgs();
}

IServiceClient * CallTrackingServiceBase::getServiceClient() const
{
   AddToLog("ServiceBase::getServiceClient");
   return EmptyServiceBase::getServiceClient();
}

IBase * CallTrackingServiceBase::getServiceClientBase() const
{
   AddToLog("ServiceBase::getServiceClientBase");
   return EmptyServiceBase::getServiceClientBase();
}

IRuntime * CallTrackingServiceBase::getRuntime() const
{
   AddToLog("ServiceBase::getRuntime");
   return EmptyServiceBase::getRuntime();
}

IRuntimeClient * CallTrackingServiceBase::getRuntimeClient() const
{
   AddToLog("ServiceBase::getRuntimeClient");
   return EmptyServiceBase::getRuntimeClient();
}

AALServiceModule * CallTrackingServiceBase::getAALServiceModule() const
{
   AddToLog("ServiceBase::getAALServiceModule");
   return EmptyServiceBase::getAALServiceModule();
}

////////////////////////////////////////////////////////////////////////////////
// IServiceModule / IServiceModuleCallback

EmptyServiceModule::EmptyServiceModule() :
   m_Construct_returns(NULL),
   m_ServiceInitialized_returns(false),
   m_ServiceInitFailed_returns(false)
{}

btBool EmptyServiceModule::Construct(IRuntime            *pAALRUNTIME,
                                     IBase               *Client,
                                     TransactionID const &tid,
                                     NamedValueSet const &optArgs) { return m_Construct_returns; }
void          EmptyServiceModule::Destroy()         {}
void  EmptyServiceModule::ServiceReleased(IBase * ) {}
btBool EmptyServiceModule::ServiceInitialized(IBase * , TransactionID const & )
{ return m_ServiceInitialized_returns; }
btBool EmptyServiceModule::ServiceInitFailed(IBase * , IEvent const * )
{ return m_ServiceInitFailed_returns; }

IMPLEMENT_RETVAL_ACCESSORS(EmptyServiceModule, Construct,          btBool, m_Construct_returns         )
IMPLEMENT_RETVAL_ACCESSORS(EmptyServiceModule, ServiceInitialized, btBool, m_ServiceInitialized_returns)
IMPLEMENT_RETVAL_ACCESSORS(EmptyServiceModule, ServiceInitFailed,  btBool, m_ServiceInitFailed_returns )

CallTrackingServiceModule::CallTrackingServiceModule() {}

btBool CallTrackingServiceModule::Construct(IRuntime            *pAALRUNTIME,
                                            IBase               *Client,
                                            TransactionID const &tid,
                                            NamedValueSet const &optArgs)
{
   MethodCallLogEntry *l = AddToLog("IServiceModule::Construct");
   l->AddParam("pAALRUNTIME", reinterpret_cast<btObjectType>(pAALRUNTIME));
   l->AddParam("Client", reinterpret_cast<btObjectType>(Client));
   l->AddParam("tid", tid);
   l->AddParam("optArgs", dynamic_cast<INamedValueSet *>( & const_cast<NamedValueSet &>(optArgs)) );

   return EmptyServiceModule::Construct(pAALRUNTIME, Client, tid, optArgs);
}

void CallTrackingServiceModule::Destroy()
{
   AddToLog("IServiceModule::Destroy");
}

void CallTrackingServiceModule::ServiceReleased(IBase *pService)
{
   MethodCallLogEntry *l = AddToLog("IServiceModuleCallback::ServiceReleased");
   l->AddParam("pService", reinterpret_cast<btObjectType>(pService));
}

btBool CallTrackingServiceModule::ServiceInitialized(IBase               *pService,
                                                     TransactionID const &rtid)
{
   MethodCallLogEntry *l = AddToLog("IServiceModuleCallback::ServiceInitialized");
   l->AddParam("pService", reinterpret_cast<btObjectType>(pService));
   l->AddParam("rtid",     rtid);
   return EmptyServiceModule::ServiceInitialized(pService, rtid);
}

btBool CallTrackingServiceModule::ServiceInitFailed(IBase        *pService,
                                                    IEvent const *pEvent)
{
   MethodCallLogEntry *l = AddToLog("IServiceModuleCallback::ServiceInitFailed");
   l->AddParam("pService", reinterpret_cast<btObjectType>(pService));
   l->AddParam("pEvent",   reinterpret_cast<btObjectType>( const_cast<IEvent *>(pEvent) ));
   return EmptyServiceModule::ServiceInitFailed(pService, pEvent);
}

////////////////////////////////////////////////////////////////////////////////

GTCOMMON_API void AllocSwvalMod(AAL::IRuntime            *pRuntime,
                                AAL::IBase               *pClientBase,
                                const AAL::TransactionID &tid)
{
   NamedValueSet configrec;

   configrec.Add(AAL_FACTORY_CREATE_CONFIGRECORD_FULL_SERVICE_NAME, "libswvalmod");
   configrec.Add(AAL_FACTORY_CREATE_SOFTWARE_SERVICE, true);

   NamedValueSet manifest;

   manifest.Add(AAL_FACTORY_CREATE_CONFIGRECORD_INCLUDED, &configrec);
   manifest.Add(AAL_FACTORY_CREATE_SERVICENAME, "swval module");

   pRuntime->allocService(pClientBase, manifest, tid);
}

GTCOMMON_API void AllocSwvalSvcMod(AAL::IRuntime            *pRuntime,
                                   AAL::IBase               *pClientBase,
                                   const AAL::TransactionID &tid)
{
   NamedValueSet configrec;

   configrec.Add(AAL_FACTORY_CREATE_CONFIGRECORD_FULL_SERVICE_NAME, "libswvalsvcmod");
   configrec.Add(AAL_FACTORY_CREATE_SOFTWARE_SERVICE, true);

   NamedValueSet manifest;

   manifest.Add(AAL_FACTORY_CREATE_CONFIGRECORD_INCLUDED, &configrec);
   manifest.Add(AAL_FACTORY_CREATE_SERVICENAME, "swval service module");

   pRuntime->allocService(pClientBase, manifest, tid);
}


EmptySwvalSvcClient::EmptySwvalSvcClient()
{
   if ( EObjOK != SetInterface(iidSwvalSvcClient, dynamic_cast<ISwvalSvcClient *>(this)) ) {
      m_bIsOK = false;
   }
}

void EmptySwvalSvcClient::DidSomething(const AAL::TransactionID & , int ) {}


CallTrackingSwvalSvcClient::CallTrackingSwvalSvcClient()
{
   if ( EObjOK != SetInterface(iidSwvalSvcClient, dynamic_cast<ISwvalSvcClient *>(this)) ) {
      m_bIsOK = false;
   }
}

void CallTrackingSwvalSvcClient::DidSomething(const AAL::TransactionID &tid, int val)
{
   MethodCallLogEntry *l = AddToLog("ISwvalSvcClient::DidSomething");
   l->AddParam("tid", tid);
   l->AddParam("val", val);
}


SynchronizingSwvalSvcClient::SynchronizingSwvalSvcClient()
{
   if ( EObjOK != SetInterface(iidSwvalSvcClient, dynamic_cast<ISwvalSvcClient *>(this)) ) {
      m_bIsOK = false;
   }
}

void SynchronizingSwvalSvcClient::DidSomething(const AAL::TransactionID &tid, int val)
{
   MethodCallLogEntry *l = AddToLog("ISwvalSvcClient::DidSomething");
   l->AddParam("tid", tid);
   l->AddParam("val", val);

   Post();
}

