ALTER PROC spGetCashSetItemInfo2  
	@CSSID INT
AS BEGIN
	SET NOCOUNT ON;

	SELECT	CSSID AS CSSID, Name AS Name, CashPrice AS Cash, WebImgName AS WebImgName,   
			WebBigImgName AS WebBigImgName, ResSex AS ResSex, ResLevel AS ResLevel, Weight AS Weight,  
			Description AS Description, NewItemOrder As IsNewItem, RentType AS RentType  
	FROM	CashSetShop css  
	WHERE	css.CSSID = @CSSID
	AND		css.Opened = 1
END