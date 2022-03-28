#pragma once

#define DataOK   0
#define DataErr -1

class TDataCom {
protected:
    int RetCode;

    int SetRetCode(int ret) { return RetCode = ret; }
public:
    TDataCom() : RetCode(DataOK) {}
    virtual ~TDataCom() = 0 {}
    int GetRetCode() {
        int temp = RetCode;
        RetCode = DataOK;
        return temp;
    }
};