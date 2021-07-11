BEGIN TRAN -------------

	ALTER TABLE dbo.SurvivalCharacterInfo ADD DeleteFlag TINYINT


	UPDATE	SurvivalCharacterInfo 
	SET		DeleteFlag = 0
	
	
	UPDATE	ci
	SET		ci.DeleteFlag = 1
	FROM	SurvivalCharacterInfo ci
			, Character c
	WHERE	c.CID = ci.CID
	AND		c.DeleteFlag = 1

COMMIT TRAN ------------