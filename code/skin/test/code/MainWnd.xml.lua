
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
	UITest.LuaClass:Exit()
end

AsynCall(function()
	--local windowFactory = UILuaGetObject("UI.WindowFactory")
	--local mainwnd = windowFactory:GetWindow("mainwnd")
	--UIMessageBox(tostring(mainwnd:GetVisible()))
end)
