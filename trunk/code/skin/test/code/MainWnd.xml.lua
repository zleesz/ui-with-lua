
function OnCreate(self)
	-- SetOnceTimer(function()
		-- local tree = self:GetTreeContainer()
		-- local webbrowser = tree:GetUIObject("webbrowser")
		-- webbrowser:Navigate("http://aplayer.open.xunlei.com/")
		-- webbrowser:SetVisible(true)
	-- end, 2000)
end

function OnClose(self)
	self:Destroy()
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

function OnSize(self, _type, width, height)
	if _type == 2 then -- max
		local tree = self:GetTreeContainer()
		tree:GetUIObject("button.max"):SetVisible(false)
		tree:GetUIObject("button.restore"):SetVisible(true)
	elseif _type == 0 then -- restore
		local tree = self:GetTreeContainer()
		local btnMax = tree:GetUIObject("button.max")
		btnMax:SetVisible(true)
		tree:GetUIObject("button.restore"):SetVisible(false)
	end
end

function OnBkgLButtonDown(self)
	UIMessageBox("OnBkgLButtonDown")
end

function OnMinClick(self)
	local hostwnd = self:GetOwnerTree():GetOwnerWnd()
	hostwnd:Min()
end

function OnMaxClick(self)
	local hostwnd = self:GetOwnerTree():GetOwnerWnd()
	hostwnd:Max()
end

function OnRestoreClick(self)
	local hostwnd = self:GetOwnerTree():GetOwnerWnd()
	hostwnd:Restore()
end

function OnCloseClick(self)
	local windowFactory = UILuaGetObject("UI.WindowFactory")
	local hostwnd = self:GetOwnerTree():GetOwnerWnd()
	windowFactory:DestroyWindow(hostwnd)
end

function OnDestroy(self)
	UITest.LuaClass:Exit()
end

--[[
SetOnceTimer(function()
	local windowFactory = UILuaGetObject("UI.WindowFactory")
	local mainwnd = windowFactory:GetWindow("mainwnd")
	local mainwndHandle = mainwnd:GetHWND()
	--UIMessageBox(tostring(mainwndHandle))
	--childwnd:SetParent(mainwndHandle)
	local childwnd = windowFactory:Create("childwnd", mainwndHandle)
	local maintree = mainwnd:GetTreeContainer()
	--local title = maintree:GetUIObject("text.title")
	--title:SetObjPos(50, 30, 200, 24)
end, 3000)
]]
