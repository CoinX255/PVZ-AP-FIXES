#include "ArchipelagoConnectingDialog.h"

ArchipelagoConnectingDialog::ArchipelagoConnectingDialog(LawnApp* theApp) : LawnDialog(
    theApp, Dialogs::DIALOG_ARCHIPELAGO_CONNECTING, true, "Connecting to Archipelago...",
    "Please wait for the connection to be established...", "Cancel", Dialog::BUTTONS_FOOTER)
{
    count = 0;
    
    this->mReanimation->AddReanimation(120, 42.0f, ReanimationType::REANIM_ZOMBIE_FOOTBALL);
    this->mSpaceAfterHeader = 155;
    
    CalcSize(0, 10);
}

ArchipelagoConnectingDialog::~ArchipelagoConnectingDialog()
{
}

void ArchipelagoConnectingDialog::Update()
{
    LawnDialog::Update();
    
    count++;
    
    if (count > 1000)
    {
        mDialogLines = "The connection is still in progress. Ensure that the connection details are correct.\n\nStill trying to establish a connection...";
        CalcSize(0, 10);
    }
}
