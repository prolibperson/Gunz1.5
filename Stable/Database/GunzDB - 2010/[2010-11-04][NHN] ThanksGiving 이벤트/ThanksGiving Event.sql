USE GunzDB
GO

--------------------------------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spEventThxGiving_GetTurkeyDinnerCount
--ALTER PROC dbo.spEventThxGiving_GetTurkeyDinnerCount
	@UserID			VARCHAR(24)
AS BEGIN

	SET NOCOUNT ON;
	
	DECLARE @AID INT;
	SELECT @AID = AID FROM Account WHERE UserID = @UserID;
	
	SELECT	COUNT(r.CID) AS Count
	FROM	(	SELECT c.CID 
				FROM Character c 
				WHERE c.AID = @AID ) r JOIN CharacterItem ci    
	ON		r.CID = ci.CID    
	AND		ci.ItemID = 400013 
	
END	
GO

--------------------------------------------------------------------------------------------------------------------------------

CREATE PROC dbo.spEventThxGiving_RedeemTurkeyDinner
-- ALTER PROC dbo.spEventThxGiving_RedeemTurkeyDinner
	@UserID			VARCHAR(24),
	@RedeemCount	INT
AS BEGIN

	SET NOCOUNT ON;

	DECLARE @AID INT;
	SELECT @AID = AID FROM Account WHERE UserID = @UserID;
	
	DECLARE @TotalCount INT;
	SELECT	@TotalCount = COUNT(r.CID)
	FROM	(	SELECT c.CID 
				FROM Character c 
				WHERE c.AID = @AID ) r JOIN CharacterItem ci    
	ON		r.CID = ci.CID    
	AND		ci.ItemID = 400013 
	
	IF( @TotalCount < @RedeemCount ) BEGIN
		PRINT 'Not Enough Tukey!';
	END
	ELSE BEGIN
		DECLARE @SQLQuery VARCHAR(MAX);
		
		SET @SQLQuery =				'UPDATE	ci ';
		SET @SQLQuery = @SQLQuery + 'SET CID = NULL ';
		SET @SQLQuery = @SQLQuery + 'FROM CharacterItem ci ';
		SET @SQLQuery = @SQLQuery + 'WHERE	ci.CIID IN ( SELECT	TOP ' + CAST(@RedeemCount AS VARCHAR(MAX)) + ' ci.CIID ';
		SET @SQLQuery = @SQLQuery + 'FROM ( SELECT c.CID FROM Character c WHERE c.AID = ' +  CAST(@AID AS VARCHAR(MAX)) + ' ) r JOIN CharacterItem ci '
		SET @SQLQuery = @SQLQuery + 'ON	r.CID = ci.CID AND ci.ItemID = 400013 )';
		
		PRINT 'Enough to remove Tukey!'
		--PRINT @SQLQuery;
		EXEC (@SQLQuery);
	END	
END
GO

--------------------------------------------------------------------------------------------------------------------------------

EXEC spEventThxGiving_GetTurkeyDinnerCount 'c1'
EXEC spEventThxGiving_RedeemTurkeyDinner 'c1', 20

/*
-- 사내 테스트 버젼
SELECT	* 
FROM	CharacterItem ci, (SELECT CID FROM Character WHERE AID = 1101) r
WHERE	ci.CID = r.CID
AND		ci.ItemID = 400013

INSERT CharacterItem(CID, ItemID, RegDate)
VALUES(1271, 400013, GETDATE())

INSERT CharacterItem(CID, ItemID, RegDate)
VALUES(1464, 400013, GETDATE())

*/