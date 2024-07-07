USE GunzDB
GO

------------------------------------------------------------------------------------------------------------------------
-- Step 1. 데이터 임시 테이블로 집계
------------------------------------------------------------------------------------------------------------------------
BEGIN TRAN --------

	CREATE TABLE Event_Word_Temp
	(
		CID			INT,
		ItemID		INT,
		ItemCnt		INT
			
		CONSTRAINT Event_Word_Temp_PK PRIMARY KEY CLUSTERED (CID, ItemID)
	)

	INSERT Event_Word_Temp(CID, ItemID, ItemCnt)
		SELECT	ci.CID, ci.ItemID, COUNT(ci.ItemID) AS ItemCount
		FROM	CharacterItem ci(NOLOCK)
		WHERE	ci.ItemID BETWEEN 403001 AND 403011 -- 단어 아이템 ItemID 범위
		AND		CID IS NOT NULL
		GROUP BY ci.CID, ci.ItemID
	
COMMIT TRAN --------
GO


------------------------------------------------------------------------------------------------------------------------
-- Step 2. 데이터 가공
------------------------------------------------------------------------------------------------------------------------
BEGIN TRAN --------

	CREATE TABLE Event_Word_NHN
	(
		CID INT,
		[Have A]	SMALLINT, 
		[Happy]		SMALLINT, 
		[Halloween]	SMALLINT, 
		[From]		SMALLINT,	
		[IJJI]		SMALLINT, 
		[AND]		SMALLINT,
		[MAIET!]	SMALLINT,
		[Pumpkin]	SMALLINT, 
		[Ghost]		SMALLINT, 
		[Trick Or]	SMALLINT, 
		[Treat!!]	SMALLINT
		
		CONSTRAINT Event_Word_NHN_PK PRIMARY KEY CLUSTERED (CID)
	)
	GO
	
	INSERT Event_Word_NHN(CID) 
		SELECT CID FROM Event_Word_Temp GROUP BY CID
	
	
	DECLARE @ItemID INT;
	DECLARE @strQuery NVARCHAR(MAX);
	DECLARE @strColumn NVARCHAR(MAX);
	
	SET @ItemID = 403001;
	
	WHILE( @ItemID < 403011 ) BEGIN
		SET @strColumn = CASE	WHEN @ItemID = 403001 THEN '[Have A]' 
								WHEN @ItemID = 403002 THEN '[Happy]' 
								WHEN @ItemID = 403003 THEN '[Halloween]' 
								WHEN @ItemID = 403004 THEN '[From]' 
								WHEN @ItemID = 403005 THEN '[IJJI]' 
								WHEN @ItemID = 403006 THEN '[AND]' 
								WHEN @ItemID = 403007 THEN '[MAIET!]' 
								WHEN @ItemID = 403008 THEN '[Pumpkin]' 
								WHEN @ItemID = 403009 THEN '[Ghost]' 
								WHEN @ItemID = 403010 THEN '[Trick Or]' 
								WHEN @ItemID = 403011 THEN '[Treat!!]' END
		
		SET @strQuery = N'UPDATE e SET e.' + @strColumn;
		SET @strQuery = @strQuery + N' = t.ItemCnt '
		SET @strQuery = @strQuery + N'FROM Event_Word_NHN e, Event_Word_Temp t '
		SET @strQuery = @strQuery + N'WHERE e.CID = t.CID AND ItemID = ' + CONVERT(NVARCHAR, @ItemID);
		
		PRINT @strQuery;		
		EXEC (@strQuery);
					
		SET @ItemID = @ItemID + 1;
	END
	
	DROP TABLE Event_Word_Temp;

COMMIT TRAN --------
GO




------------------------------------------------------------------------------------------------------------------------
-- Step 3. 알파벳 아이템 삭제 쿼리
------------------------------------------------------------------------------------------------------------------------
BEGIN TRAN --------

	DELETE CharacterItem
	WHERE ItemID BETWEEN 403001 AND 403011
	
COMMIT TRAN --------
GO