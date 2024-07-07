USE GunzDB
GO

/*
    *******************************************************************************
        Notice 
    *******************************************************************************

     1. SleepCharacterNHN 테이블에 AID에만 Index가 걸려있다면 지워주세요.
       예전에 잘못 구성된 것 같습니다.
       ( DROP INDEX IX_SleepCharacterNHN_AID )

     2. SleepCharacterNHN 테이블에 CID에만 Index가 없다면 생성해주세요.
       ( CREATE INDEX IX_SleepCharacterNHN_CID ON dbo.SleepCharacterNHN(CID); )


*/


CREATE PROC dbo.spWebGetSleepCharacter_NHN
	@CID		INT
AS BEGIN
	SET NOCOUNT ON;	
	
	SELECT	t.AID
		  , t.CID
		  , t.Name
		  , -1 AS CharNum
		  , CASE WHEN EXISTS (SELECT Name FROM Character(NOLOCK) WHERE Name = t.Name) 
				 THEN 0 
				 ELSE 1 
			END AS NameUsed
	FROM	SleepCharacterNHN t(NOLOCK)
	WHERE	t.CID = @CID
END



