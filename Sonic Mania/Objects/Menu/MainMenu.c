#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectMainMenu *MainMenu;

void MainMenu_Update(void) {}

void MainMenu_LateUpdate(void) {}

void MainMenu_StaticUpdate(void)
{
    EntityUIControl *control = (EntityUIControl *)MainMenu->menuControlPtr;
    if (control && control->active) {
        EntityUIDiorama *diorama     = (EntityUIDiorama *)MainMenu->dioramaPtr;
        MainMenu->promptPtr->visible = (((EntityUIShifter *)control->shifter)->field_6C & 0xFFFF0000) > -0x700000;
        EntityUIButton *button       = control->entities[control->field_D8];
        if (button) {
            switch (button->frameID) {
                case 0: diorama->dioramaID = 0; break;
                case 1: diorama->dioramaID = 3; break;
                case 2: diorama->dioramaID = 4; break;
                case 3: diorama->dioramaID = 5; break;
                case 4: diorama->dioramaID = 6; break;
                case 5: diorama->dioramaID = 2; break;
                case 6: diorama->dioramaID = 1; break;
                case 7: diorama->dioramaID = 7; break;
                default: break;
            }
        }
        if (button->disabled)
           diorama->timer = 12;
    }
}

void MainMenu_Draw(void) {}

void MainMenu_Create(void *data) {}

void MainMenu_StageLoad(void) {}

void MainMenu_Initialize(void)
{
    TextInfo text;
    INIT_TEXTINFO(text);
    foreach_all(UIControl, control)
    {
        RSDK.PrependText(&text, "Main Menu");
        if (RSDK.StringCompare(&text, &control->tag, false)) {
            MainMenu->menuControlPtr = (Entity*)control;
            control->backPressCB     = MainMenu_ReturnToTitleOption;
        }
    }

    EntityUIControl *menuControl = (EntityUIControl *)MainMenu->menuControlPtr;

    foreach_all(UIButtonPrompt, prompt)
    {
        int x = menuControl->startPos.x - menuControl->cameraOffset.x;
        int y = menuControl->startPos.y - menuControl->cameraOffset.y;
        Hitbox hitbox;
        hitbox.right  = (menuControl->size.x >> 17);
        hitbox.left   = -(menuControl->size.x >> 17);
        hitbox.bottom = (menuControl->size.y >> 17);
        hitbox.top    = -(menuControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, x, y, &hitbox, prompt->position.x, prompt->position.y) /*&& !prompt->buttonID*/)
            MainMenu->promptPtr = (Entity*)prompt;
    }

    foreach_all(UIDiorama, diorama)
    {
        int x = menuControl->startPos.x - menuControl->cameraOffset.x;
        int y = menuControl->startPos.y - menuControl->cameraOffset.y;
        Hitbox hitbox;
        hitbox.right  = (menuControl->size.x >> 17);
        hitbox.left   = -(menuControl->size.x >> 17);
        hitbox.bottom = (menuControl->size.y >> 17);
        hitbox.top    = -(menuControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, x, y, &hitbox, diorama->position.x, diorama->position.y)) {
            MainMenu->dioramaPtr = (Entity *)diorama;
            diorama->parent = MainMenu->menuControlPtr;
        }
    }

#if RETRO_USE_PLUS
    int button1Frame         = 1;
    int button2Frame         = 2;
    int button3Frame         = 3;
    bool32 button3StopMus    = false;
    int button4Frame         = 4;
    int button5Frame         = 6;
    bool32 button5Transition = false;
    if (API.CheckDLC(DLC_PLUS)) {
        button1Frame      = 5;
        button2Frame      = 1;
        button3Frame      = 2;
        button4Frame      = 3;
        button5Frame      = 4;
        button3StopMus    = true;
        button5Transition = true;
    }

    EntityUIButton *button = menuControl->entities[0];
    button->frameID        = 0;
    button->transition     = true;
    button->stopMusic      = true;

    button             = menuControl->entities[1];
    button->frameID    = button1Frame;
    button->transition = true;
    button->stopMusic  = true;

    button             = menuControl->entities[2];
    button->frameID    = button2Frame;
    button->transition = true;
    button->stopMusic  = true;

    button             = menuControl->entities[3];
    button->frameID    = button3Frame;
    button->transition = true;
    button->stopMusic  = button3StopMus;

    button             = menuControl->entities[4];
    button->frameID    = button4Frame;
    button->transition = true;
    button->stopMusic  = false;

    button             = menuControl->entities[5];
    button->frameID    = button5Frame;
    button->transition = button5Transition;
    button->stopMusic  = false;

    button             = menuControl->entities[6];
    button->frameID    = 7;
    button->transition = false;
    button->stopMusic  = false;
#endif
}

bool32 MainMenu_ReturnToTitleOption(void)
{
    ManiaModeMenu_StartReturnToTitle();
    return true;
}

void MainMenu_ExitGame(void)
{
    API.ExitGame();
}

void MainMenu_ExitGameOption(void)
{
    TextInfo buffer;
    Localization_GetString(&buffer, STR_QUITWARNING);
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&buffer);
    if (dialog) {
        UIDialog_AddButton(1, dialog, 0, 1);
        UIDialog_AddButton(0, dialog, MainMenu_StartExitGame, 1);
        UIDialog_Setup(dialog);
    }
}

void MainMenu_StartExitGame(void)
{
    if (UIControl_GetUIControl())
        UIControl_GetUIControl()->state = StateMachine_None;
    Music_FadeOut(0.02);

    MenuSetup_StartTransition(MainMenu_ExitGame, 64);
}

void MainMenu_ChangeMenu(void)
{
    EntityUIButton *button = (EntityUIButton *)RSDK_sceneInfo->entity;
    switch (button->frameID) {
        case 0:
            if (checkNoSave) {
                UIControl_MatchMenuTag("No Save Mode");
            }
            else {
                EntityUIControl *saveSelect = (EntityUIControl *)ManiaModeMenu->saveSelectMenu;
                saveSelect->activeEntityID  = 7;
#if RETRO_USE_PLUS
                saveSelect->dwordCC     = 0;
                ManiaModeMenu->field_28 = -1;
                for (int i = 0; i < saveSelect->unknownCount1; ++i) {
                    Entity *store          = RSDK_sceneInfo->entity;
                    RSDK_sceneInfo->entity = (Entity *)saveSelect->entities[i];
                    UISaveSlot_Unknown21();
                    RSDK_sceneInfo->entity = store;
                }
#endif
                UIControl_MatchMenuTag("Save Select");
            }
            break;
        case 1:
#if RETRO_USE_PLUS
            if (API.CheckDLC(DLC_PLUS)) {
                // EntityUIControl *control = (EntityUIControl *)TimeAttackMenu->timeAttackControl;
                // control->activeEntityID  = 0;
                // control->dwordCC         = 0;
                UIControl_MatchMenuTag("Time Attack");
            }
            else {
                // EntityUIControl *control = (EntityUIControl *)TimeAttackMenu->timeAttackControl_Legacy;
                // control->activeEntityID  = 0;
                // control->dwordCC         = 0;
                UIControl_MatchMenuTag("Time Attack Legacy");
            }
#else
            UIControl_MatchMenuTag("Time Attack");
#endif
            break;
        case 2:
#if RETRO_USE_PLUS
            if (API.CheckDLC(DLC_PLUS))
                UIControl_MatchMenuTag("Competition");
            else
                UIControl_MatchMenuTag("Competition Legacy");
#else
            UIControl_MatchMenuTag("Competition");
#endif
            break;
        case 3: UIControl_MatchMenuTag("Options"); break;
#if RETRO_USE_PLUS
        case 4: UIControl_MatchMenuTag("Extras"); break;
        case 5:
            if (API.GetUserStorageNoSave()) {
                UIControl_MatchMenuTag("No Save Encore");
            }
            else {
                EntityUIControl *encoreSaveSel = (EntityUIControl *)ManiaModeMenu->encoreSaveSelect;
                encoreSaveSel->activeEntityID  = 1;
                encoreSaveSel->dwordCC         = 0;
                for (int i = 0; i < encoreSaveSel->unknownCount1; ++i) {
                    Entity *store          = RSDK_sceneInfo->entity;
                    RSDK_sceneInfo->entity = (Entity *)encoreSaveSel->entities[i];
                    UISaveSlot_Unknown21();
                    RSDK_sceneInfo->entity = store;
                }
                UIControl_MatchMenuTag("Encore Mode");
            }
            break;
        case 6: API.ShowExtensionOverlay(0);
#if RETRO_USE_EGS
            if (!API.CheckDLC(DLC_PLUS)) {
                if (API.EGS_Checkout(0)) {
                    API.ShowEncorePage(0);
                }
                else {
                    TextInfo buffer;
                    INIT_TEXTINFO(buffer);
                    Localization_GetString(&buffer, STR_CONNECTINGTOEGS);
                    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(buffer);
                    if (dialog) {
                        UIDialog_AddButton(2, dialog, MainMenu_BuyPlusDialogCB, true);
                        UIDialog_AddButton(3, dialog, 0, true);
                        UIDialog_Setup(dialog);
                    }
                }
            }
#endif
            break;
#endif
        default: break;
    }
}

#if RETRO_USE_EGS
int MainMenu_BuyPlusDialogCB(void)
{
    API.EGS_Unknown4(0);
    return 1;
}
#endif

void MainMenu_Unknown2(void)
{
    EntityUIControl *control = (EntityUIControl *)MainMenu->menuControlPtr;
    EntityUIButton *taButton = control->entities[1];
    if (API.CheckDLC(DLC_PLUS))
        taButton = control->entities[2];
    taButton->disabled = !SaveGame_CheckUnlock(0);

    EntityUIButton *compButton = control->entities[2];
    if (API.CheckDLC(DLC_PLUS))
        compButton = control->entities[3];
    compButton->disabled = !SaveGame_CheckUnlock(1);
}

void MainMenu_Unknown3(void)
{
    foreach_all(UIButton, button)
    {
        if (button->listID == 1) {
            if (button->frameID == 7) {
                if (RSDK_sku->platform && RSDK_sku->platform != PLATFORM_DEV) {
                    EntityUIControl *mainMenu = (EntityUIControl *)MainMenu->menuControlPtr;
                    RSDK.ResetEntityPtr(button, TYPE_BLANK, NULL);
                    --mainMenu->unknownCount1;
                    --mainMenu->rowCount;
                    mainMenu->entities[6] = NULL;
                }
                else {
                    button->options2 = MainMenu_ExitGameOption;
                }
            }
            else {
                button->options2 = MainMenu_ChangeMenu;
            }
        }
    }
    ((EntityUIControl *)MainMenu->menuControlPtr)->unknownCallback3 = MainMenu_Unknown4;
}

void MainMenu_Unknown4()
{
    ((EntityUIDiorama *)MainMenu->dioramaPtr)->dioramaSubID = -1;
}

void MainMenu_EditorDraw(void) {}

void MainMenu_EditorLoad(void) {}

void MainMenu_Serialize(void) {}
#endif
