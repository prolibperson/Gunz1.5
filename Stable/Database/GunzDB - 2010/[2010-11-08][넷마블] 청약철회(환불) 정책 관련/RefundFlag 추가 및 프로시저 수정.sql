ALTER TABLE CashShop ADD RefundFlag INT DEFAULT(1);
ALTER TABLE CashSetShop ADD RefundFlag INT DEFAULT(1);
GO

-------------------------------------------------------------------------------------------------------------------------------------------------

UPDATE CashShop SET RefundFlag = 1
UPDATE CashSetShop SET RefundFlag = 1

UPDATE	CashShop
SET		RefundFlag = 0
WHERE	ItemID = 5000001
OR		ItemID = 5000002
OR		ItemID = 5000003

-------------------------------------------------------------------------------------------------------------------------------------------------

ALTER PROC dbo.spGetCashItemInfo2
	@CSID  INT
AS BEGIN
	SELECT	cs.csid AS CSID, i.name AS Name, i.Slot AS Slot,     
			cs.CashPrice AS Cash, cs.WebImgName As WebImgName, cs.WebBigImgName As WebBigImgName,    
			i.ResSex AS ResSex, i.ResLevel AS ResLevel, i.Weight AS Weight,    
			i.Damage AS Damage, i.Delay AS Delay, i.Controllability AS Controllability,    
			i.Magazine AS Magazine, i.MaxBullet AS MaxBullet, i.ReloadTime AS ReloadTime,     
			i.HP AS HP, i.AP AS AP, i.MAXWT AS MaxWeight, i.LimitSpeed AS LimitSpeed,    
			i.FR AS FR, i.CR AS CR, i.PR AS PR, i.LR AS LR,    
			i.Description AS Description, cs.NewItemOrder AS IsNewItem,    
			cs.RentType AS RentType, RefundFlag AS RefundFlag
	FROM	CashShop cs, Item i    
	WHERE	i.ItemID = cs.ItemID 
	AND		cs.csid = @CSID 
	AND		cs.opened = 1  
END
GO
  
-------------------------------------------------------------------------------------------------------------------------------------------------

ALTER PROC dbo.spGetCashSetItemInfo2  
	@CSSID INT
AS BEGIN
	SELECT	CSSID AS CSSID, Name AS Name, CashPrice AS Cash, WebImgName AS WebImgName,   
			WebBigImgName AS WebBigImgName, ResSex AS ResSex, ResLevel AS ResLevel, Weight AS Weight,  
			Description AS Description, NewItemOrder As IsNewItem, RentType AS RentType, RefundFlag AS RefundFlag
	FROM	CashSetShop css  
	WHERE	CSSID = @CSSID  
END
GO

-------------------------------------------------------------------------------------------------------------------------------------------------