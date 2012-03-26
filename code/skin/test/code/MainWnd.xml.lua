
function OnCreate(self)
end

function OnClose(self)
	UITest.LuaClass:Exit()
end

function OnShowWindow(self, visible)
	UIMessageBox(tostring(visible))
end

function OnMove(self, x, y)
	UILog("OnMove", x, y)
end

function OnMoving(self, left, top, right, bottom)
	UILog("OnMoving", left, top, right, bottom)
end

function OnFocusChange(self, focus, lastwnd)
	UILog("OnFocusChange", focus, lastwnd)
end

function OnBkgLButtonDown(self)
	UIMessageBox("OnBkgLButtonDown")
end

function OnOpenClick(self)
	UIMessageBox("OnOpenClick")
end

AsynCall(function()
	--local windowFactory = UILuaGetObject("UI.WindowFactory")
	--local mainwnd = windowFactory:GetWindow("mainwnd")
	--UIMessageBox(tostring(mainwnd:GetVisible()))
end)