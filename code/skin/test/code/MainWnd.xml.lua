
function OnCreate(self)
end

function OnClose(self)
	--UIMessageBox(self:GetID())
	UITest.LuaClass:Exit()
end

function OnShowWindow(self, visible)
	--UIMessageBox(self:GetID())
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

function OnCloseClick(self)
	UITest.LuaClass:Exit()
end

AsynCall(function()
	--local windowFactory = UILuaGetObject("UI.WindowFactory")
	--local mainwnd = windowFactory:GetWindow("mainwnd")
	--UIMessageBox(tostring(mainwnd:GetVisible()))
end)