#include "Object.hpp"


CObject::CObject() : ValidityToken(std::make_shared<bool>(true)) {}

CObject::~CObject() {}

bool CClass::IsChildOf(const CClass* Other) const {
    const CClass* Current = this;
    while (Current) {
        if (Current == Other) return true;
        Current = Current->SuperClass;
    }
    return false;
}

CClass* CObject::StaticClass() {
    static CClass ClassInfo = {"CObject", []() { return new CObject(); }, nullptr};
    return &ClassInfo;
}
