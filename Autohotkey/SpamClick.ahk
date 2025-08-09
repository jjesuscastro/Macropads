#Requires AutoHotkey v2.0

global spamClicking := false
global spamThread := false

; Toggle spam clicking with F24
F24:: {
    global spamClicking
    spamClicking := !spamClicking
    ToolTip "Spam Clicking: " (spamClicking ? "ON" : "OFF")
    SetTimer () => ToolTip(), -1000  ; Hide tooltip after 1 second
}

~LButton:: {
    global spamClicking, spamThread
    if !spamClicking || spamThread
        return

    spamThread := true
    while GetKeyState("LButton", "P") && spamClicking {
        Click
        Sleep(100) ; Adjust this delay to change click speed
    }
    spamThread := false
}
