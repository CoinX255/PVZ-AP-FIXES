#ifndef ARCHIPELAGOTEXTCLIENT_H
#define ARCHIPELAGOTEXTCLIENT_H
#include "../LawnCommon.h"
#include "../../SexyAppFramework/EditListener.h"
#include "../../SexyAppFramework/Widget.h"

class ListenerHandle;
class LawnEditWidget;
class LawnApp;

class ArchipelagoTextClient;

class APTextEditWidget : public LawnEditWidget
{
public:
    APTextEditWidget(ArchipelagoTextClient* parent, int theId, EditListener* theListener, Dialog* theDialog);
    ~APTextEditWidget();
    void MouseWheel(int theDelta) override;

private:
    ArchipelagoTextClient* mParent;
};

class ArchipelagoTextClient : public Sexy::Widget, public Sexy::EditListener
{
public:
    ArchipelagoTextClient(LawnApp* theApp);
    ~ArchipelagoTextClient();
    
    LawnApp*                    mApp;
    APTextEditWidget*		    mMessageEditWidget;
    bool                        mFirstCharTyped;
    int64_t                     mScroll;
    std::list<std::string>      mLines;
    ListenerHandle*             mAnyChatHandler;
    int                         mCurrentHistoryItem;
    
public:
    void Draw(Sexy::Graphics* g) override;
    void AddedToManager(Sexy::WidgetManager* theWidgetManager) override;
    void RemovedFromManager(Sexy::WidgetManager* theWidgetManager) override;
    void EditWidgetText(int theId, const SexyString& theString) override;
    bool AllowChar(int theId, SexyChar theChar) override;
    void MouseWheel(int theDelta) override;
    void Up() override;
    void Down() override;
    void PgUp() override;
    void PgDown() override;
    
    void UpdateLines();
};

#endif
