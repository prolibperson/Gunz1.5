USE GunzDB
GO

----------------------------------------------------------------------------------------------------------------------------------------
-- ÀÛ¾÷ÇÒ Äõ¸®
EXEC sp_rename 'spGetAccountCharInfo', 'BackUp_spGetAccountCharInfo'
GO

CREATE PROC dbo.spGetAccountCharInfo
-- ALTER PROC dbo.spGetAccountCharInfo
	@AID		INT, 
	@CharNum	SMALLINT
AS BEGIN

	DECLARE @CID			INT
	DECLARE @CLID			INT
	DECLARE @ClanName		VARCHAR(24)  
	DECLARE @ClanGrade		INT
	DECLARE @ClanContPoint	INT
  
	SELECT @CID = CID FROM Character(NOLOCK) WHERE AID=@AID AND CharNum=@CharNum  
  
	SELECT @CID AS CID, c.Name AS Name, c.CharNum AS CharNum, c.Level AS Level
			, c.Sex AS Sex, c.Hair AS Hair, c.Face AS Face, c.XP AS XP, c.BP AS BP
			, (	SELECT	cl.Name 
				FROM	Clan cl(NOLOCK), ClanMember cm(NOLOCK) 
				WHERE	cm.cid = @CID 
				AND		cm.CLID=cl.CLID ) AS ClanName
	FROM Character c(NOLOCK)  
	WHERE c.CID = @CID
END

----------------------------------------------------------------------------------------------------------------------------------------
-- º¹±¸ Äõ¸®
/*
DROP PROC spGetAccountCharInfo
EXEC sp_rename 'BackUp_spGetAccountCharInfo', 'spGetAccountCharInfo'
*/