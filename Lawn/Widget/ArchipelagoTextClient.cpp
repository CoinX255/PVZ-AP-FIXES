#include "ArchipelagoTextClient.h"

#include <algorithm>
#include <sstream>

#include "../../LawnApp.h"
#include "../../SexyAppFramework/WidgetManager.h"
#include "../../Sexy.TodLib/TodCommon.h"
#include "../../Resources.h"
#include "../../SexyAppFramework/APWrapper.h"
#include "../../SexyAppFramework/Font.h"

const int CHAT_LINE_HEIGHT = 10;
const int CHAT_WIDTH = 600;

APTextEditWidget::APTextEditWidget(ArchipelagoTextClient* parent, int theId, EditListener* theListener,
    Dialog* theDialog) : LawnEditWidget(theId, theListener, theDialog)
{
    mParent = parent;    
}

APTextEditWidget::~APTextEditWidget()
{
}

void APTextEditWidget::MouseWheel(int theDelta)
{
    LawnEditWidget::MouseWheel(theDelta);
    
    mParent->MouseWheel(theDelta);
}

ArchipelagoTextClient::ArchipelagoTextClient(LawnApp* theApp)
{
    mApp = theApp;
    mFirstCharTyped = true;
    mScroll = 0;
    mCurrentHistoryItem = -1;
    
    int gLawnEditWidgetColors[][4] = {
        { 0,   0,   0,   0 },
        { 0,   0,   0,   0 },
        { 240, 240, 255, 255 },
        { 255, 255, 255, 255 },
        { 0,   0,   0,   255 },
    };
    
    mMessageEditWidget = new APTextEditWidget(this, 0, this, nullptr);
    mMessageEditWidget->SetFont(Sexy::FONT_BRIANNETOD16);
    mMessageEditWidget->SetColors(gLawnEditWidgetColors, EditWidget::NUM_COLORS);
    mMessageEditWidget->mBlinkDelay = 14;
    mMessageEditWidget->DisableAutocap();
    mMessageEditWidget->SetFont(FONT_PICO129);
    mMessageEditWidget->Resize(0, BOARD_HEIGHT - FONT_PICO129->GetHeight() - 10, CHAT_WIDTH, FONT_PICO129->GetHeight());
    
    Widget::Resize(0, 0, BOARD_WIDTH, BOARD_HEIGHT);
    
    mAnyChatHandler = mApp->mAP->AddAnyChatMessageListener([this](const std::string&)
    {
        UpdateLines();
    });
    UpdateLines();
}

ArchipelagoTextClient::~ArchipelagoTextClient()
{
    delete mMessageEditWidget;
    delete mAnyChatHandler;
}

void ArchipelagoTextClient::Draw(Sexy::Graphics* g)
{
    Widget::Draw(g);
    
    g->PushState();
    
    auto font_height = FONT_PICO129->GetHeight();
    
    g->SetColor({0, 0, 0, 150});
    g->FillRect(0, BOARD_HEIGHT - 40 - 10 - font_height * CHAT_LINE_HEIGHT, CHAT_WIDTH, font_height * CHAT_LINE_HEIGHT + 10);
    g->FillRect(0, BOARD_HEIGHT - 15 - font_height, CHAT_WIDTH, font_height + 10);
    
    // Get the last 10 lines of text
    Sexy::Color aTextColor(255, 255, 255);
    auto i = 0;
    for (auto message = mLines.rbegin(); message != mLines.rend(); ++message)
    {
        TodDrawString(g, *message, 0, BOARD_HEIGHT - 45 - font_height * i, FONT_PICO129, aTextColor, DS_ALIGN_LEFT);
        i++;
    }
    
    g->PopState();
}

void ArchipelagoTextClient::AddedToManager(Sexy::WidgetManager* theWidgetManager)
{
    Widget::AddedToManager(theWidgetManager);
    AddWidget(mMessageEditWidget);
    theWidgetManager->SetFocus(mMessageEditWidget);
}

void ArchipelagoTextClient::RemovedFromManager(Sexy::WidgetManager* theWidgetManager)
{
    Widget::RemovedFromManager(theWidgetManager);
    RemoveWidget(mMessageEditWidget);
}

void ArchipelagoTextClient::EditWidgetText(int theId, const SexyString& theString)
{
    EditListener::EditWidgetText(theId, theString);
    mApp->mAP->SendAPMessage(theString);
    mApp->mAP->PushMessageHistory(theString);
    mCurrentHistoryItem = -1;
    mMessageEditWidget->SetText("");
}

bool ArchipelagoTextClient::AllowChar(int theId, SexyChar theChar)
{
    // Avoid t appearing in the chat when first opening it
    if (!mFirstCharTyped)
    {
        mFirstCharTyped = true;
        return false;
    }
    return EditListener::AllowChar(theId, theChar);
}

void ArchipelagoTextClient::MouseWheel(int theDelta)
{
    Widget::MouseWheel(theDelta);
    
    mScroll += theDelta;
    mScroll = std::max<int64_t>(mScroll, 0);
    
    UpdateLines();
}

void ArchipelagoTextClient::Up()
{
    if (mCurrentHistoryItem == -1)
    {
        mCurrentHistoryItem = mApp->mAP->HistoryLength() - 1;
    } else
    {
        mCurrentHistoryItem -= 1;
    }
    
    if (mCurrentHistoryItem == -1)
    {
        mMessageEditWidget->SetText("");
    }
    else
    {
        mMessageEditWidget->SetText(mApp->mAP->HistoryItem(mCurrentHistoryItem));
    }
}

void ArchipelagoTextClient::Down()
{
    if (mCurrentHistoryItem == mApp->mAP->HistoryLength() - 1)
    {
        mCurrentHistoryItem = -1;
    } else
    {
        mCurrentHistoryItem += 1;
    }
    
    if (mCurrentHistoryItem == -1)
    {
        mMessageEditWidget->SetText("");
    }
    else
    {
        mMessageEditWidget->SetText(mApp->mAP->HistoryItem(mCurrentHistoryItem));
    }
}

void ArchipelagoTextClient::PgUp()
{
    mScroll += 10;
    mScroll = std::max<int64_t>(mScroll, 0);
    
    UpdateLines();
}

void ArchipelagoTextClient::PgDown()
{
    mScroll += -10;
    mScroll = std::max<int64_t>(mScroll, 0);
    
    UpdateLines();
}

void ArchipelagoTextClient::UpdateLines()
{
    mLines.clear();
    
    // Get the last 10 lines of text
    auto chat_messages = mApp->mAP->ChatMessages();
    auto i = 0;
    for (auto message = chat_messages.rbegin(); message != chat_messages.rend(); ++message)
    {
        std::vector<std::string> lines;
        std::string line;
        std::stringstream ss(*message);

        // Split by newline
        while (std::getline(ss, line)) {
            lines.push_back(line);
        }
        
        for (auto line = lines.rbegin(); line != lines.rend(); ++line)
        {
            std::vector<std::string> wraps;
            
            // Split the line as necessary
            std::string rest = *line;
            
            while (!rest.empty())
            {
                // If the string fits, we don't need to wrap
                if (FONT_PICO129->StringWidth(rest) <= CHAT_WIDTH + 10)
                {
                    wraps.push_back(rest);
                    break;
                }
                
                // Find the longest substring that fits
                auto break_max = rest.length();
                while (break_max > 0 && FONT_PICO129->StringWidth(rest.substr(0, break_max)) > CHAT_WIDTH + 10)
                {
                    break_max--;
                }

                auto break_at = rest.find_last_of(" \t", break_max);
                if (break_at != std::string::npos && break_at > 0)
                {
                    // Break at the whitespace
                    wraps.push_back(rest.substr(0, break_at));
                    
                    // Ignore the whitespace
                    rest = rest.substr(break_at + 1);
                }
                else
                {
                    // Force a brake because there is no whitespace
                    wraps.push_back(rest.substr(0, break_max));
                    rest = rest.substr(break_max);
                }
            }
            
            for (auto wrap = wraps.rbegin(); wrap != wraps.rend(); ++wrap)
            {
                if (i < mScroll)
                {
                    i++;
                    continue;
                }
                
                mLines.push_front(" " + *wrap);
                i++;
                if (i == CHAT_LINE_HEIGHT + mScroll) return;
            }
        }
    }
}
