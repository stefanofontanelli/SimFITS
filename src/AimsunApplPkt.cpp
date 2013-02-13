//
// Generated file, do not edit! Created by opp_msgc 4.1 from AimsunApplPkt.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "AimsunApplPkt.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(AimsunApplPkt);

AimsunApplPkt::AimsunApplPkt(const char *name, int kind) : cPacket(name,kind)
{
    this->destAddr_var = -1;
    this->srcAddr_var = -1;
    this->aimsunId_var = -1;
}

AimsunApplPkt::AimsunApplPkt(const AimsunApplPkt& other) : cPacket()
{
    setName(other.getName());
    operator=(other);
}

AimsunApplPkt::~AimsunApplPkt()
{
}

AimsunApplPkt& AimsunApplPkt::operator=(const AimsunApplPkt& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    this->destAddr_var = other.destAddr_var;
    this->srcAddr_var = other.srcAddr_var;
    this->aimsunId_var = other.aimsunId_var;
    return *this;
}

void AimsunApplPkt::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->destAddr_var);
    doPacking(b,this->srcAddr_var);
    doPacking(b,this->aimsunId_var);
}

void AimsunApplPkt::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->destAddr_var);
    doUnpacking(b,this->srcAddr_var);
    doUnpacking(b,this->aimsunId_var);
}

int AimsunApplPkt::getDestAddr() const
{
    return destAddr_var;
}

void AimsunApplPkt::setDestAddr(int destAddr_var)
{
    this->destAddr_var = destAddr_var;
}

int AimsunApplPkt::getSrcAddr() const
{
    return srcAddr_var;
}

void AimsunApplPkt::setSrcAddr(int srcAddr_var)
{
    this->srcAddr_var = srcAddr_var;
}

int AimsunApplPkt::getAimsunId() const
{
    return aimsunId_var;
}

void AimsunApplPkt::setAimsunId(int aimsunId_var)
{
    this->aimsunId_var = aimsunId_var;
}

class AimsunApplPktDescriptor : public cClassDescriptor
{
  public:
    AimsunApplPktDescriptor();
    virtual ~AimsunApplPktDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(AimsunApplPktDescriptor);

AimsunApplPktDescriptor::AimsunApplPktDescriptor() : cClassDescriptor("AimsunApplPkt", "cPacket")
{
}

AimsunApplPktDescriptor::~AimsunApplPktDescriptor()
{
}

bool AimsunApplPktDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<AimsunApplPkt *>(obj)!=NULL;
}

const char *AimsunApplPktDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int AimsunApplPktDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int AimsunApplPktDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *AimsunApplPktDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "destAddr",
        "srcAddr",
        "aimsunId",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int AimsunApplPktDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destAddr")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcAddr")==0) return base+1;
    if (fieldName[0]=='a' && strcmp(fieldName, "aimsunId")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *AimsunApplPktDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *AimsunApplPktDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int AimsunApplPktDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    AimsunApplPkt *pp = (AimsunApplPkt *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string AimsunApplPktDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    AimsunApplPkt *pp = (AimsunApplPkt *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getDestAddr());
        case 1: return long2string(pp->getSrcAddr());
        case 2: return long2string(pp->getAimsunId());
        default: return "";
    }
}

bool AimsunApplPktDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    AimsunApplPkt *pp = (AimsunApplPkt *)object; (void)pp;
    switch (field) {
        case 0: pp->setDestAddr(string2long(value)); return true;
        case 1: pp->setSrcAddr(string2long(value)); return true;
        case 2: pp->setAimsunId(string2long(value)); return true;
        default: return false;
    }
}

const char *AimsunApplPktDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<3) ? fieldStructNames[field] : NULL;
}

void *AimsunApplPktDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    AimsunApplPkt *pp = (AimsunApplPkt *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


