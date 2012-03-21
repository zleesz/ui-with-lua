local UITest = {}
_G["UITest"] = UITest
UITest.LuaClass = UILuaGetObject("UITest.LuaClass")

function bbbb()
	local windowFactory = UILuaGetObject("UI.WindowFactory")
	local mainwnd = windowFactory:Create("mainwnd")
	local tree = mainwnd:GetTreeContainer()
	UIMessageBox(tostring(tree))
	if tree then
		local bkg = tree:GetUIObject("image.bkg")
		UIMessageBox(bkg:GetID())
	end
	--[[
	local resFactory = UILuaGetObject("UI.ResFactory")
	local bitmap = resFactory:GetBitmap("bitmap.mainwnd.background.png")
	UIMessageBox(bitmap:GetID())
	]]
	--local mainwnd = windowFactory:Create("mainwnd")
end

bbbb()
