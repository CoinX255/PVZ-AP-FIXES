#pragma once
#include "LawnDialog.h"

class ArchipelagoConnectingDialog : public LawnDialog
{
public:
    
    LawnApp*			mApp;
    int count;

public:
    ArchipelagoConnectingDialog(LawnApp* theApp);
    virtual ~ArchipelagoConnectingDialog();
    void Update() override;
};
