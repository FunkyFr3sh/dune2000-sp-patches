%include "macros/patch.inc"
%include "macros/datatypes.inc"
%include "dune2000.inc"
%include "patch.inc"

; ### TODO ###
;fix sp anim

gstring uibb_r16FileName, "UIBB.R16", 100
gstring uibb_r8FileName, "UIBB.R8", 100

@SET 0x00460D23, dd uibb_r16FileName
@SET 0x00460D37, dd uibb_r16FileName
@SET 0x00460E3D, dd uibb_r16FileName
@SET 0x00460D3E, dd uibb_r8FileName
@SET 0x00460D52, dd uibb_r8FileName
@SET 0x00460E4B, dd uibb_r8FileName


;map too small crash fixes
@CLEAR_INT 0x0044B6BC, 0x0044B6D8 ; __Do_Tooltips() - TOOLTIPS: Map item out of range
@LJMP 0x0044B6BC, 0x0044BAD7 ; __Do_Tooltips() - TOOLTIPS: Map item out of range
@CLEAR_INT 0x004434A4, 0x004434B6 ; Model__HandleGameLoopEvents() - Invalid index to gGameMap.map (1)
@LJMP 0x004434A4, 0x00443C7F ; Model__HandleGameLoopEvents() - Invalid index to gGameMap.map (1)
@CLEAR_INT 0x0045946C, 0x0045947E ; Generate_Unit_Move_Order(int_int_int) - Invalid targetX
@LJMP 0x0045946C, 0x00459531 ; Generate_Unit_Move_Order(int_int_int) - Invalid targetX
@CLEAR_INT 0x00459496, 0x004594A8 ; Generate_Unit_Move_Order(int_int_int) - Invalid targetY
@LJMP 0x00459496, 0x00459531 ; Generate_Unit_Move_Order(int_int_int) - Invalid targetY

 
hack 0x00470EE8, 0x00470EF2 ; Menu fade effect
    push dword[GameHeight]
    push dword[GameWidth]
    jmp hackend

    
hack 0x0046CFF1, 0x0046CFF7 ; Airstrike OOS - GetCenterOfScreen? Building Exists - Use original battlefield width to prevent OOS online
    cmp dword[esp+0x14], BattleFieldPosX ; this will be true when the H key was pressed to center the screen over the cy
    jz .out
    mov ebp, 480 ; Use original battlefield width 
    jmp hackend
    
.out:
    mov ebp, dword[BattleFieldWidth]
    jmp hackend
    

hack 0x0046D00A, 0x0046D010 ; Airstrike OOS - GetCenterOfScreen? Building Exists - Use original battlefield height to prevent OOS online
    cmp dword[esp+0x14], BattleFieldPosX ; this will be true when the H key was pressed to center the screen over the cy
    jz .out
    mov edi, 380 ; Use original battlefield height
    jmp hackend
    
.out:
    mov edi, dword[BattleFieldHeight]
    jmp hackend
    
    
hack 0x0046D09D, 0x0046D0A3 ; Airstrike OOS - GetCenterOfScreen? Building not Exists - Use original battlefield width to prevent OOS online
    cmp dword[esp+0x14], BattleFieldPosX ; this will be true when the H key was pressed to center the screen over the mcv
    jz .out
    mov ebp, 480 ; Use original battlefield width 
    jmp hackend
    
.out:
    mov ebp, dword[BattleFieldWidth]
    jmp hackend
    

hack 0x0046D0D5, 0x0046D0DB ; Airstrike OOS - GetCenterOfScreen? Building not Exists - Use original battlefield height to prevent OOS online
    cmp dword[esp+0x14], BattleFieldPosX ; this will be true when the H key was pressed to center the screen over the mcv
    jz .out
    mov ecx, 380 ; Use original battlefield height
    jmp hackend
    
.out:
    mov ecx, dword[BattleFieldHeight]
    jmp hackend

    
hack 0x0044C784 ; LoadMapData() - map too small crash fix, adjust BattleField size
    mov dword[gGameMapHeight], eax
    
    pushad
    call SetBattleFieldSize
    popad
    jmp hackend
    
    
hack 0x00444678, 0x0044467E ; map too small - Do not draw selection rectangle if cursor out of battlefield
    mov edx, dword[OptionsBarHeight]
    push ecx
    mov ecx, edx
    add ecx, dword[BattleFieldHeight]
    cmp eax, ecx ; eax = MousePositionY
    pop ecx
    jge 0x004446A9
    jmp hackend
    
    
hack 0x004455BE ; map too small - Ignore order if cursor out of battlefield
    mov eax, dword[OptionsBarHeight]
    push edx
    mov edx, eax
    add edx, dword[BattleFieldHeight]
    cmp dword[MousePositionY], edx
    pop edx
    jge 0x00445D99
    jmp hackend

    
hack 0x0043D2D5, 0x0043D2DB ; align credits screen text
    mov ecx, dword[GameWidth]
    mov dword[CreditsScreenWidth], ecx
    mov ecx, 50
    add ecx, dword[HighResUIAlignY]
    mov dword[CreditsScreenTop], ecx
    mov ecx, 350
    add ecx, dword[HighResUIAlignY]
    mov dword[CreditsScreenBottom], ecx

    mov ecx, dword[0x4DEDA8]
    jmp hackend
    
    
hack 0x0042CD30, 0x0042CD37 ; Temporary Disable SP region animation since it crashes the game
    cmp byte[HighResPatchEnabled], 1
    jnz .out
    retn
    
.out:
    sub esp, 0x18
    mov ecx, dword[esp+0x24]
    jmp 0x0042CD37
    

hack 0x0043F7E0, 0x0043F7FB ; align score screen anim (my) 1
    mov ecx, 0x109 ; x
    add ecx, dword[HighResUIAlignX]
    mov dword[esp+0x20], ecx
    lea eax, [eax*2+eax]
    mov ecx, 0x28 ; y
    add ecx, dword[HighResUIAlignY]
    mov dword[esp+0x24], ecx
    mov ecx, 0x38 ; y
    add ecx, dword[HighResUIAlignY]
    mov dword[esp+0x2C], ecx
    jmp hackend
    
    
hack 0x0043F81F, 0x0043F82B ; align score screen anim (my) 1
    mov edx, 0x28 ; y
    add edx, dword[HighResUIAlignY]
    push edx
    mov edx, 0x109 ; x
    add edx, dword[HighResUIAlignX]
    push edx
    mov edx, 0x109
    add edx, dword[HighResUIAlignX]
    add eax, edx
    jmp hackend


hack 0x0043F2A0, 0x0043F2BB ; align score screen anim (my) 2
    mov ecx, 0x109 ; x
    add ecx, dword[HighResUIAlignX]
    mov dword[esp+0x20], ecx
    lea eax, [eax*2+eax]
    mov ecx, 0x0A0 ; y
    add ecx, dword[HighResUIAlignY]
    mov dword[esp+0x24], ecx
    mov ecx, 0x0B0
    add ecx, dword[HighResUIAlignY]
    mov dword[esp+0x2C], ecx
    jmp hackend
    

hack 0x0043F2DD, 0x0043F2EC ; align score screen anim (my) 2
    mov edx, 0x0A0 ; y
    add edx, dword[HighResUIAlignY]
    push edx
    mov edx, 0x109 ; x
    add edx, dword[HighResUIAlignX]
    push edx
    mov edx, 0x109
    add edx, dword[HighResUIAlignX]
    add eax, edx
    jmp hackend
    
    
hack 0x0043F528, 0x0043F543 ; align score screen anim (my) 3
    mov ecx, 0x109 ; x
    add ecx, dword[HighResUIAlignX]
    mov dword[esp+0x20], ecx
    lea eax, [eax*2+eax]
    mov ecx, 0x64 ; y
    add ecx, dword[HighResUIAlignY]
    mov dword[esp+0x24], ecx
    mov ecx, 0x74
    add ecx, dword[HighResUIAlignY]
    mov dword[esp+0x2C], ecx
    jmp hackend
    

hack 0x0043F567, 0x0043F573 ; align score screen anim (my) 3
    mov edx, 0x64 ; y
    add edx, dword[HighResUIAlignY]
    push edx
    mov edx, 0x109 ; x
    add edx, dword[HighResUIAlignX]
    push edx
    mov edx, 0x109
    add edx, dword[HighResUIAlignX]
    add eax, edx
    jmp hackend
    

hack 0x0043F65D, 0x0043F686 ; align score screen anim (enemy) 4
    mov ecx, 0x78 ; y
    add ecx, dword[HighResUIAlignY]
    push ecx
    lea edx, [esp+0x28]
    mov ecx, 0x109 ; x
    add ecx, dword[HighResUIAlignX]
    push ecx
    push edx
    mov ecx, 0x109
    add ecx, dword[HighResUIAlignX]
    mov dword[esp+0x30], ecx
    mov ecx, 0x78 ; y
    add ecx, dword[HighResUIAlignY]
    mov dword[esp+0x34], ecx
    mov ecx, 0x88 ; y
    add ecx, dword[HighResUIAlignY]
    mov dword[esp+0x3C], ecx
    mov ecx, 0x109
    add ecx, dword[HighResUIAlignX]
    add eax, ecx
    jmp hackend

    
hack 0x0043F3D8, 0x0043F404 ; align score screen anim (enemy) 5
    mov ecx, 0x0B4 ; y
    add ecx, dword[HighResUIAlignY]
    push ecx
    lea edx, [esp+0x28]
    mov ecx, 0x109 ; x
    add ecx, dword[HighResUIAlignX]
    push ecx
    push edx
    mov ecx, 0x109
    add ecx, dword[HighResUIAlignX]
    mov dword[esp+0x30], ecx
    mov ecx, 0x0B4 ; y
    add ecx, dword[HighResUIAlignY]
    mov dword[esp+0x34], ecx
    mov ecx, 0x0C4 ; y
    add ecx, dword[HighResUIAlignY]
    mov dword[esp+0x3C], ecx
    mov ecx, 0x109
    add ecx, dword[HighResUIAlignX]
    add eax, ecx
    jmp hackend

    
hack 0x0043F915, 0x0043F93E ; align score screen anim (enemy) 6
    mov ecx, 0x3C ; y
    add ecx, dword[HighResUIAlignY]
    push ecx
    lea edx, [esp+0x28]
    mov ecx, 0x109 ; x
    add ecx, dword[HighResUIAlignX]
    push ecx
    push edx
    mov ecx, 0x109
    add ecx, dword[HighResUIAlignX]
    mov dword[esp+0x30], ecx
    mov ecx, 0x3C ; y
    add ecx, dword[HighResUIAlignY]
    mov dword[esp+0x34], ecx
    mov ecx, 0x4C ; y
    add ecx, dword[HighResUIAlignY]
    mov dword[esp+0x3C], ecx
    mov ecx, 0x109
    add ecx, dword[HighResUIAlignX]
    add eax, ecx
    jmp hackend


hack 0x00437AB9, 0x00437AC4 ; CampaignSelectMouseClickCheckXY - fake the mouse position
    mov edx, dword[MousePositionY]
    mov eax, dword[MousePositionX]
    sub edx, dword[HighResUIAlignY]
    sub eax, dword[HighResUIAlignX]
    jmp hackend


hack 0x0042C400, 0x0042C40B ; CampaignSelectChangeCursorIcon - fake the mouse position
    mov eax, dword[MousePositionY]
    mov ecx, dword[MousePositionX]
    sub eax, dword[HighResUIAlignY]
    sub ecx, dword[HighResUIAlignX]
    jmp hackend
    
    
hack 0x0043C550, 0x0043C556 ; center background images
    lea edx, [esp+0x20]
    add eax, dword[HighResUIAlignY]
    add ecx, dword[HighResUIAlignX]
    push eax
    push ecx
    jmp hackend


hack 0x0042565A ; AlignMissionTimer
    cmp byte[HighResPatchEnabled], 1
    jnz .out
    mov eax, dword[HighResAddedWidth]
    add eax, 0x1A4
    push eax
    xor eax, eax
    jmp 0x0042565F
    
.out:
    push 0x1A4
    jmp 0x0042565F


hack 0x0044916C, 0x00449172 ; SideBarStarportDrawingGlitchFix
    pushad
    push dword[0x6D60D0]
    push dword[0x5178B4]
    call DrawEmptySideBarIcons
    add esp, 8
    popad
    
    mov ecx, dword[0x5178B4]
    jmp hackend


hack 0x00449145, 0x0044914B ; SideBarUpgradeDrawingGlitchFix
    pushad
    push dword[0x6D60D0]
    push dword[0x5178B4]
    call DrawEmptySideBarIcons
    add esp, 8
    popad
    
    mov edx, dword[0x5178B4]
    jmp hackend


hack 0x00472142, 0x000472147 ; MenuToolTipFixWidth
    cmp byte[HighResPatchEnabled], 1
    jnz .out
    cmp eax, dword[GameWidth]
    jmp 0x00472147
    
.out:
    cmp eax, 0x280
    jmp 0x00472147


hack 0x00472153, 0x00472159 ; MenuToolTipFixHeight
    cmp byte[HighResPatchEnabled], 1
    jnz .out
    cmp ecx, dword[GameHeight]
    jmp 0x00472159
    
.out:
    cmp ecx, 0x190
    jmp 0x00472159


hack 0x00436056, 0x0043605E ; UilCenterExcept
    pushad
    push eax
    call UilCenterExceptCheck
    add esp, 4
    popad
    
    push eax
    mov ecx, ebp
    call 0x004390E0
    jmp hackend


hack 0x00436036, 0x0043603C ; UilCenterExcept2
    pushad
    push eax
    call UilCenterExceptCheck
    add esp, 4
    popad

    push eax
    call 0x004390E0
    jmp hackend


hack 0x004391D7, 0x004391DE ; ControlCenterOnlyOnce
    pushad
    push edx
    call ControlCenterExceptCheck
    add esp, 4
    popad
    
    push 0x0A
    push 0x004DF3C4
    jmp hackend


hack 0x004827D8, 0x004827E6 ; CenterUI
    mov ecx, dword[esp+0x1C]
    cmp byte[HighResPatchEnabled], 1
    jnz .out

    cmp byte[HighResUilCenterExcept], 1
    jz .out
    cmp byte[HighResControlCenterExcept], 1
    jnz .align
    cmp byte[HighResControlFirstTimeCenter], 1
    jnz .out
    
.align:
    add edx, dword[HighResUIAlignY]
    add ecx, dword[HighResUIAlignX]
    mov byte[HighResControlFirstTimeCenter], 0
    
.out:
    mov dword[ebp+0x0C], edx
    mov edx, dword[esp+0x2C]
    mov dword[ebp+0x8], ecx
    jmp 0x004827E6


hack 0x00460FD3 ; AdjustGUI X - UI_LANG_r file
    cmp byte[HighResPatchEnabled], 1
    jnz .out
    cmp dword[eax], 320
    jbe .out
    mov ecx, dword[HighResAddedWidth]
    add dword[eax], ecx
    
.out:
    mov ecx, dword[eax]
    add eax,4
    jmp 0x00460FD8


hack 0x00460FDF ; AdjustGUI Y - UI_LANG_r file
    cmp byte[HighResPatchEnabled], 1
    jnz .out
    cmp dword[eax], 200
    jbe .out
    mov edx, dword[HighResAddedHeight]
    add dword[eax], edx
    
.out:
    mov edx, dword[eax]
    add eax,4
    jmp 0x00460FE4


hack 0x0044423A ; patch1
    cmp byte[HighResPatchEnabled], 1
    jz .patch
    push 0x004DFDD4
    jmp 0x0044423F
    
.patch:
    pop edi
    pop esi
    pop ebp
    pop ebx
    add esp,0x0CC
    retn
    

hack 0x004442F7 ; patch2
    cmp byte[HighResPatchEnabled], 1
    jz .patch
    push 0x004DFDD4
    jmp 0x004442FC
    
.patch:
    pop edi
    pop esi
    pop ebp
    pop ebx
    add esp,0x0CC
    retn


hack 0x00444D79 ; patch3
    cmp byte[HighResPatchEnabled], 1
    jz .patch
    mov eax,dword[0x4E4208]
    jmp 0x00444D7E
    
.patch:
    mov eax,0x0B8
    cmp ecx,eax
    jle 0x00445010
    mov ebx,0x2F
    mov edx,ebx
    imul edx, dword[SideBarIconCount]
    add edx,eax
    cmp ecx,edx
    jge 0x0044500B
    mov eax,dword[0x516430]
    cmp eax,0x3
    jnz 0x00444E16
    xchg eax,ecx
    mov ecx,0x0B8
    sub eax,ecx
    cdq
    div ebx
    add edi, dword[SideBarPanelLeftUIPosX]
    lea ecx, [edi]
    sub edi, dword[SideBarPanelLeftUIPosX]
    cmp eax,0x3
    jg 0x00445010
    jmp 0x00444DBF


hack  0x0044BB50, 0x0044BB58 ; patch4
    cmp byte[HighResPatchEnabled], 1
    jz .patch
    mov eax,ecx
    mov ecx,dword[0x4E41FC]
    jmp 0x0044BB58
    
.patch:
    xchg eax,ecx
    mov ecx,dword[0x4E41FC]
    shr eax,0x1
    add ecx,edx
    xor edx,edx
    add eax,ecx
    mov CX,0x0B8
    mov dword[esp+0x14],eax
    mov eax,dword[0x4EB04C]
    sub eax,ecx
    add edx,0x2F
    div dl
    cbw
    mov ecx,edx
    shr edx,0x1
    imul ecx,eax
    mov dword[esp+0x10],eax
    mov eax,dword[0x4E4200]
    add eax,ecx
    add edx,eax
    mov eax,dword[0x516430]
    mov ecx,dword[esp+0x10]
    xor ebx,ebx
    inc ebx
    jmp 0x0044BB96


hack  0x0044BE0B ; patch5
    cmp byte[HighResPatchEnabled], 1
    jz .patch
    cmp eax,1
    jbe 0x0044BE15
    jmp 0x0044BE10
    
.patch:
    cmp eax,ebx
    jbe .loc1
    xchg eax,ebx
    
.loc1:
    cmp ecx,0x3
    jg 0x0044C10F
    jmp 0x0044BE19


hack 0x004A3E9D, 0x004A3EA4 ; patch6 ;align top left
    cmp byte[HighResPatchEnabled], 1
    jz .patch
    sar eax,1
    mov dword[0x7984D8],eax
    jmp 0x004A3EA4
    
.patch:
    xor eax,eax
    mov dword[0x7984D8],eax
    jmp 0x004A3EA4
