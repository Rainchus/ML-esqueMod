void btl_set_state(s32 battleState) {
    s32 flags = gBattleStatus.flags2;
    gBattleState = battleState;
    D_800DC4E0 = 1;
    gBattleState2 = BATTLE_STATES2_UNK_0;

    flags &= BS_FLAGS2_40;
    if (flags) {
        switch (battleState) {
            case 14:
                battleState = 16;
                break;
            case 13:
                battleState = 15;
                break;
            case 7:
                battleState = 8;
                break;
            case 8:
                battleState = 7;
                break;
        }
        gBattleState = battleState;
    }
}

void btl_update(void) {
    BattleStatus* battleStatus = &gBattleStatus;
    PlayerData* playerData = &gPlayerData;
    Actor* partner = battleStatus->partnerActor;
    f32 dpadAngle;
    f32 dpadMagnitude;
    s32 cond;

    if (battleStatus->inputBitmask != -1) {
        if (battleStatus->flags1 & 0x80000 && gGameStatusPtr->unk_81 != 0) {
            s32 inputBitmask = battleStatus->inputBitmask;

            battleStatus->currentButtonsDown = gGameStatusPtr->altCurrentButtons & inputBitmask;
            battleStatus->currentButtonsPressed = gGameStatusPtr->altPressedButtons & inputBitmask;
            battleStatus->currentButtonsHeld = gGameStatusPtr->altHeldButtons & inputBitmask;
            battleStatus->stickX = gGameStatusPtr->altStickX;
            battleStatus->stickY = gGameStatusPtr->altStickY;
        } else {
            s32 inputBitmask2 = battleStatus->inputBitmask;

            battleStatus->currentButtonsDown = gGameStatusPtr->currentButtons & inputBitmask2;
            battleStatus->currentButtonsPressed = gGameStatusPtr->pressedButtons & inputBitmask2;
            battleStatus->currentButtonsHeld = gGameStatusPtr->heldButtons & inputBitmask2;
            battleStatus->stickX = gGameStatusPtr->stickX;
            battleStatus->stickY = gGameStatusPtr->stickY;
        }
    }

    get_dpad_input_radial(&dpadAngle, &dpadMagnitude);
    battleStatus->dpadX = dpadAngle;
    battleStatus->dpadY = dpadMagnitude;
    battleStatus->pushInputBuffer[battleStatus->inputBufferPos] = battleStatus->currentButtonsPressed;
    battleStatus->holdInputBuffer[battleStatus->inputBufferPos] = battleStatus->currentButtonsDown;

    battleStatus->inputBufferPos++;
    if (battleStatus->inputBufferPos >= ARRAY_COUNT(battleStatus->pushInputBuffer)) {
        battleStatus->inputBufferPos = 0;
    }

    if (battleStatus->preUpdateCallback != NULL) {
        battleStatus->preUpdateCallback();
    }

    cond = TRUE;
    if (battleStatus->unk_95 == 0 || battleStatus->unk_95 != gBattleState) {
        switch (gBattleState) {
            case -1:
            case 0:
                return;
            case 1:
                btl_state_update_normal_start();
                cond = FALSE;
                break;
            case 7:
                btl_state_update_begin_player_turn();
                break;
            case 8:
                btl_state_update_begin_partner_turn();
                break;
            case 9:
                func_80242FE0();
                break;
            case 5:
                btl_state_update_begin_turn();
                break;
            case 6:
                btl_state_update_end_turn();
                break;
            case 10:
                btl_state_update_switch_to_player();
                break;
            case 11:
                btl_state_update_switch_to_partner();
                break;
            case 12:
                btl_state_update_prepare_menu();
                break;
            case 13:
                btl_state_update_player_menu();
                break;
            case 14:
                btl_state_update_partner_menu();
                break;
            case 16:
                btl_state_update_twink_menu();
                break;
            case 15:
                btl_state_update_peach_menu();
                break;
            case 17:
                btl_state_update_select_target();
                break;
            case 18:
                btl_state_update_player_move();
                break;
            case 2:
                btl_state_update_first_strike();
                break;
            case 22:
                btl_state_update_end_player_turn();
                break;
            case 23:
                btl_state_update_end_partner_turn();
                break;
            case 21:
                btl_state_update_enemy_move();
                break;
            case 3:
                btl_state_update_partner_striking_first();
                break;
            case 20:
                btl_state_update_next_enemy();
                break;
            case 19:
                btl_state_update_partner_move();
                break;
            case 26:
                btl_state_update_victory();
                break;
            case 32:
                btl_state_update_end_battle();
                break;
            case 29:
                btl_state_update_change_partner();
                break;
            case 24:
                btl_state_update_run_away();
                break;
            case 25:
                btl_state_update_defend();
                break;
            case 27:
                btl_state_update_defeat();
                break;
            case 28:
                btl_state_update_1C();
                break;
            case 30:
                btl_state_update_end_training_battle();
                break;
            case 4:
                btl_state_update_enemy_striking_first();
                break;
            case 34:
                btl_state_update_22();
                break;
            case 33:
                btl_state_update_celebration();
                break;
            case 35:
                btl_state_update_end_demo_battle();
                cond = FALSE;
                break;
        }
    }

    battleStatus->unk_90++;
    if (battleStatus->unk_90 == 40) {
        func_8024F7C8();
    }

    func_80266684();
    func_80266978();
    func_80266B14();
    func_8024EE48();
    update_actor_shadows();

    if (battleStatus->unk_432 != -2) {
        u8 paramType;
        f32 paramAmount;

        get_screen_overlay_params(1, &paramType, &paramAmount);

        if (battleStatus->unk_432 > 0) {
            set_screen_overlay_color(1, 0, 0, 0);
            if (partner == NULL) {
                set_screen_overlay_params_back(0, 215.0f);
            } else if (playerData->currentPartner == 6) {
                paramAmount -= 10.0f;
                if (paramAmount < 0.0f) {
                    paramAmount = 0.0f;
                }
                set_screen_overlay_params_back(0, paramAmount);
            } else {
                paramAmount += 10.0f;
                if (paramAmount > 215.0f) {
                    paramAmount = 215.0f;
                }
                set_screen_overlay_params_back(0, paramAmount);
            }
        } else if (battleStatus->unk_432 < 0) {
            paramAmount -= 10.0f;
            if (paramAmount < 0.0f) {
                paramAmount = 0.0f;
                set_screen_overlay_params_back(0xFF, -1.0f);
                battleStatus->unk_432 = 0;
            } else {
                set_screen_overlay_params_back(0, paramAmount);
            }
        }

        if (cond || D_802809F6 != -1) {
            if (D_802809F6 == -1) {
                if (gGameStatusPtr->demoState == 2) {
                    u8 paramType;
                    f32 paramAmount;

                    get_screen_overlay_params(0, &paramType, &paramAmount);
                    if (paramType == 0xFF) {
                        D_802809F6 = 0;
                        set_screen_overlay_params_front(0, 0.0f);
                    }
                }
            } else if (D_802809F6 == 0xFF) {
                if (gBattleState != 0x23) {
                    btl_set_state(0x23);
                }
            } else {
                D_802809F6 += 10;
                if (D_802809F6 > 0xFF) {
                    D_802809F6 = 0xFF;
                }

                set_screen_overlay_params_front(0, D_802809F6);
                set_screen_overlay_color(0, 208, 208, 208);
                intro_logos_set_fade_alpha(255);
                intro_logos_set_fade_color(224);
            }
        }
    }
}