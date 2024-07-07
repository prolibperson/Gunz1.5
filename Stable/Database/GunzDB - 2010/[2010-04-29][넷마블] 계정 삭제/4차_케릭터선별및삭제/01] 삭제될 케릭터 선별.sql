--USE GunzDB
USE TGunzDB
GO


CREATE TABLE dbo.CharacterDeleteTarget
(
	AID			INT				NOT NULL,
	CID			INT				NOT NULL,
	Name		VARCHAR(24) 	NOT NULL
)
GO



BEGIN TRAN -----------
	-- ¥Î√Ê 2327894(1415656)
	INSERT INTO CharacterDeleteTarget(AID, CID, Name)
		SELECT	c.AID, c.CID, c.Name
		FROM	AccountDeleteTarget at(NOLOCK)
				, Character c(NOLOCK)
		WHERE	c.AID = at.AID
		AND		c.DeleteFlag = 0
		

	ALTER TABLE CharacterDeleteTarget ADD CONSTRAINT CharacterDeleteTarget_PK PRIMARY KEY CLUSTERED(CID);
	CREATE INDEX IX_CharacterDeleteTarget_AID ON dbo.CharacterDeleteTarget(AID);
COMMIT TRAN -----------
GO