local UITest = {}
_G["UITest"] = UITest
UITest.LuaClass = UILuaGetObject("UITest.LuaClass")

local mainwnd = nil

function Filter(wnd, msg, wparam, lparam)
	UILog("Filter", wnd:GetID(), msg, wparam, lparam)
end

function OnShowWindow(self, visible)
	--UIMessageBox(tostring(visible))
end

function bbbb()
	local windowFactory = UILuaGetObject("UI.WindowFactory")
	mainwnd = windowFactory:Create("mainwnd", "mainwnd.instance")
	mainwnd:AttachListener("OnShowWindow", OnShowWindow, false)
	mainwnd:Show(4)
	--mainwnd:AddInputFilter(Filter)
	--[[
	local tree = mainwnd:GetTreeContainer()
	if tree then
		local bkg = tree:GetUIObject("image.bkg")
		UIMessageBox(bkg:GetID())
		local ownertree = bkg:GetOwnerTree()
		local wnd = ownertree:GetOwnerWnd()
		UIMessageBox(wnd:GetID())
	end
	]]
	--[[
	local resFactory = UILuaGetObject("UI.ResFactory")
	local bitmap = resFactory:GetBitmap("bitmap.mainwnd.background.png")
	UIMessageBox(bitmap:GetID())
	]]
	--local mainwnd = windowFactory:Create("mainwnd")
end

bbbb()

function OnTimer(self, id)
	--UILog(tostring(self), tostring(id))
	--[[
	if mainwnd:GetVisible() then
		mainwnd:SetVisible(false)
	else
		mainwnd:SetVisible(true)
	end
	]]
	UIMessageBox(id)
end

--UILuaGetObject("UI.TimerManager"):SetOnceTimer(OnTimer, 1000)
--SetOnceTimer(OnTimer, 1000)