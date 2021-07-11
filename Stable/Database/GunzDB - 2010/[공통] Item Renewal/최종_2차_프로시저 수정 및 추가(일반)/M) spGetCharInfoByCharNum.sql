USE GunzDB
GO

----------------------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®
EXEC sp_rename 'spGetCharInfoByCharNum', 'BackUp_spGetCharInfoByCharNum'
GO

CREATE PROC dbo.spGetCharInfoByCharNum
-- ALTER PROC dbo.spGetCharInfoByCharNum
	@AID		INT,
	@CharNum	SMALLINT
AS BEGIN        
	SET NOCOUNT ON;
	
	DECLARE @CID INT;	      
	SELECT @CID = CID FROM Character(NOLOCK) WHERE AID=@AID AND CharNum=@CharNum        
      
	SELECT	c.CID, c.AID, c.Name, c.Level, c.Sex, c.CharNum, c.Hair, c.Face      
			, c.XP, c.BP, c.HP, c.AP, c.FR, c.CR, c.ER, c.WR, c.GameCount, c.KillCount, c.DeathCount, c.PlayTime
			, cm.CLID, cl.Name AS 'ClanName', cm.Grade AS 'ClanGrade', cm.ContPoint AS ClanContPoint       
			, ISNULL(tr.Rank, 0) as 'rank'    
	FROM	( Character c(NOLOCK) LEFT OUTER JOIN TotalRanking tr(NOLOCK) ON c.name = tr.name )
			LEFT OUTER JOIN (Clan cl(NOLOCK) JOIN ClanMember cm(NOLOCK) ON cl.CLID = cm.CLID) ON c.CID = cm.CID      
	WHERE c.CID = @CID      
END 


----------------------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ Äõ¸®
/*
DROP PROC spGetCharInfoByCharNum
EXEC sp_rename 'BackUp_spGetCharInfoByCharNum', 'spGetCharInfoByCharNum'
*/
