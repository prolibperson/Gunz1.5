------------------------------------------------------------------------------------------------------------------------
-- Step 1. 데이터 임시 테이블로 집계
------------------------------------------------------------------------------------------------------------------------
BEGIN TRAN --------

	CREATE TABLE Event_Alphabet_Temp
	(
		CID			INT,
		ItemID		INT,
		ItemCnt		INT
			
		CONSTRAINT Event_Alphabet_Temp_PK PRIMARY KEY CLUSTERED (CID, ItemID)
	)

	INSERT Event_Alphabet_Temp(CID, ItemID, ItemCnt)
		SELECT	ci.CID, ci.ItemID, COUNT(ci.ItemID) AS ItemCount
		FROM	CharacterItem ci(NOLOCK)
		WHERE	ci.ItemID BETWEEN 402000 AND 402025 -- 알파벳 아이템 ItemID 범위
		AND		CID IS NOT NULL
		GROUP BY ci.CID, ci.ItemID
	
COMMIT TRAN --------
GO




------------------------------------------------------------------------------------------------------------------------
-- Step 2. 데이터 가공
------------------------------------------------------------------------------------------------------------------------
BEGIN TRAN --------

	CREATE TABLE Event_Alphabet_NHN
	(
		CID INT,
		Spel_A SMALLINT, Spel_B SMALLINT, Spel_C SMALLINT, Spel_D SMALLINT,	Spel_E SMALLINT, Spel_F SMALLINT,
		Spel_G SMALLINT, Spel_H	SMALLINT, Spel_I SMALLINT, Spel_J SMALLINT, Spel_K SMALLINT, Spel_L	SMALLINT,
		Spel_M SMALLINT, Spel_N	SMALLINT, Spel_O SMALLINT, Spel_P SMALLINT,	Spel_Q SMALLINT, Spel_R SMALLINT,
		Spel_S SMALLINT, Spel_T SMALLINT, Spel_U SMALLINT, Spel_V SMALLINT, Spel_W SMALLINT, Spel_X SMALLINT,
		Spel_Y SMALLINT, Spel_Z SMALLINT
		
		CONSTRAINT Event_Alphabet_NHN_PK PRIMARY KEY CLUSTERED (CID)
	)
	
	INSERT Event_Alphabet_NHN(CID) SELECT CID FROM Event_Alphabet_Temp GROUP BY CID
	

	
	
	DECLARE @ItemID INT;
	DECLARE @strQuery NVARCHAR(1024);
	DECLARE @strColumn NVARCHAR(10);
	
	SET @ItemID = 402000;
	
	WHILE( @ItemID != 402025 ) BEGIN
		SET @strColumn = CASE	WHEN @ItemID = 402000 THEN 'Spel_A' WHEN @ItemID = 402001 THEN 'Spel_B' 
								WHEN @ItemID = 402002 THEN 'Spel_C' WHEN @ItemID = 402003 THEN 'Spel_D' 
								WHEN @ItemID = 402004 THEN 'Spel_E' WHEN @ItemID = 402005 THEN 'Spel_F' 
								WHEN @ItemID = 402006 THEN 'Spel_G' WHEN @ItemID = 402007 THEN 'Spel_H' 
								WHEN @ItemID = 402008 THEN 'Spel_I' WHEN @ItemID = 402009 THEN 'Spel_J' 
								WHEN @ItemID = 402010 THEN 'Spel_K' WHEN @ItemID = 402011 THEN 'Spel_L' 
								WHEN @ItemID = 402012 THEN 'Spel_M' WHEN @ItemID = 402013 THEN 'Spel_N' 
								WHEN @ItemID = 402014 THEN 'Spel_O' WHEN @ItemID = 402015 THEN 'Spel_P' 
								WHEN @ItemID = 402016 THEN 'Spel_Q' WHEN @ItemID = 402017 THEN 'Spel_R' 
								WHEN @ItemID = 402018 THEN 'Spel_S' WHEN @ItemID = 402019 THEN 'Spel_T' 
								WHEN @ItemID = 402020 THEN 'Spel_U' WHEN @ItemID = 402021 THEN 'Spel_V' 
								WHEN @ItemID = 402022 THEN 'Spel_W' WHEN @ItemID = 402023 THEN 'Spel_X' 
								WHEN @ItemID = 402024 THEN 'Spel_Y' WHEN @ItemID = 402025 THEN 'Spel_Z' END		
		
		SET @strQuery = N'UPDATE e SET e.' + @strColumn;
		SET @strQuery = @strQuery + N' = t.ItemCnt '
		SET @strQuery = @strQuery + N'FROM Event_Alphabet_NHN e, Event_Alphabet_Temp t '
		SET @strQuery = @strQuery + N'WHERE e.CID = t.CID AND ItemID = ' + CONVERT(NVARCHAR, @ItemID);
		
		EXEC sp_executesql @strQuery;
					
		SET @ItemID = @ItemID + 1;
	END
	
	DROP TABLE Event_Alphabet_Temp;
	
COMMIT TRAN --------
GO




------------------------------------------------------------------------------------------------------------------------
-- Step 3. 알파벳 아이템 삭제 쿼리
------------------------------------------------------------------------------------------------------------------------
BEGIN TRAN --------

	DELETE CharacterItem
	WHERE ItemID BETWEEN 402000 AND 402025
	
COMMIT TRAN --------
GO