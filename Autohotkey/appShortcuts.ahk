#Requires AutoHotkey v2.0

global overlayVisible := false

; #region arrays
websiteNameArray := [
    "Gmail", 
    "GDrive", 
    "Netflix", 
    "Crunchyroll", 
    "YouTube", 
    "YouTube Music", 
    "Twitch", 
    "Facebook", 
    "Twitter", 
    "Telegram"]

websiteLinkArray := [
    "https://www.gmail.com", 
    "https://www.gmail.com", 
    "https://www.netflix.com", 
    "https://www.crunchyroll.com", 
    "https://www.youtube.com", "https://music.youtube.com/", 
    "https://www.twitch.tv", 
    "https://www.facebook.com", 
    "https://www.x.com", 
    "https://web.telegram.org"]

programNameArray := [
    "Unity Hub",
    "Rider",
    "Fork",
    "PowerShell",
    "VSCode",
    "Cancer"
]

programLocationArray := [
    "C:\Program Files\Unity Hub\Unity Hub.exe",
    "C:\Program Files (x86)\JetBrains\JetBrains Rider 2025.1.4\bin\rider64.exe",
    "C:\Users\Jesus\AppData\Local\Fork\current\Fork.exe",
    "powershell",
    "C:\Users\Jesus\AppData\Local\Programs\Microsoft VS Code\Code.exe",
    "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Riot Games\VALORANT.lnk"
]
; #endregion

F23::ShowOverlay(programNameArray)
F23 Up::HideOverlay()

F24::ShowOverlay(websiteNameArray)
F24 Up::HideOverlay()

m := Max(programNameArray.Length, programLocationArray.Length)
Loop m {
    idx := A_Index
    name := programNameArray.Has(idx) ? programNameArray[idx] : ""
    path := programLocationArray.Has(idx) ? programLocationArray[idx] : ""

    if (!name || !path)
        continue

    fKeyNumber := 12 + idx  ; F13 = 12 + 1
    h := "^F" . fKeyNumber

    bindProgramHotkey(h, name, path)
}

m := Max(websiteNameArray.Length, websiteLinkArray.Length)
Loop m {
    idx := A_Index
    name := websiteNameArray.Has(idx) ? websiteNameArray[idx] : ""
    link := websiteLinkArray.Has(idx) ? websiteLinkArray[idx] : ""

    if (!name || !link)
        continue

    fKeyNumber := 12 + idx  ; F13 = 12 + 1
    h := "!F" . fKeyNumber

    bindWebsiteHotkey(h, name, link)
}

; #region functions
bindProgramHotkey(key, name, path) {
    Hotkey key, (*) => openApplication(name, path)
}

bindWebsiteHotkey(key, name, link) {
    Hotkey key, (*) => openWebsite(name, link)
}

openApplication(name, path) {
    ToolTip "Opening " name
    SetTimer () => ToolTip(), -1000  ; Hide tooltip after 1 second
    Run path
}

openWebsite(name, url) {
    edgePath := "C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe"

    ToolTip "Opening " name
    SetTimer () => ToolTip(), -1000  ; Hide tooltip after 1 second
    ; Check if Edge is running
    if WinExist("ahk_exe msedge.exe") {
        ; Try to activate Edge's window
        WinActivate("ahk_exe msedge.exe")
        Sleep(300)  ; Give it time to come forward

        Run(Format('"{}" --new-tab "{}"', edgePath, url))
    } else {
        Run(Format('"{}" "{}"', edgePath, url))
    }
}

ShowOverlay(arr) {
    global layerGui, overlayVisible

    if overlayVisible
        return  ; Don't redraw if already visible

    layerGui := Gui("+AlwaysOnTop -Caption +ToolWindow")
    layerGui.BackColor := "Black"
    layerGui.SetFont("s12", "Consolas")

    y := 10  ; Starting Y position
    xKey := 10
    xName := 50

    Loop 10 {
        i := A_Index
        if arr.Has(i) {
            fKey := Format("F{}:", i)  ; Show F1 to F10 (for display only)
            name := arr[i]

            ; Add key label in lime
            layerGui.SetFont("cLime")
            layerGui.AddText("x" xKey " y" y, fKey)

            ; Add name in white
            layerGui.SetFont("cWhite")
            layerGui.AddText("x" xName " y" y, name)

            y += 25  ; Move to next line
        }
    }

    layerGui.Show("x50 y50 NoActivate")
    overlayVisible := true
}

HideOverlay() {
    global layerGui, overlayVisible

    if overlayVisible {
        layerGui.Hide()
        overlayVisible := false
    }
}
; #endregion