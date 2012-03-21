
function OnCreate(self)
	UIMessageBox("OnCreate")
end

function OnClose(self)
	UITest.LuaClass:Exit()
end

function OnBkgLButtonDown(self)
	UIMessageBox("OnBkgLButtonDown")
end

function OnOpenClick(self)
	UIMessageBox("OnOpenClick")
end