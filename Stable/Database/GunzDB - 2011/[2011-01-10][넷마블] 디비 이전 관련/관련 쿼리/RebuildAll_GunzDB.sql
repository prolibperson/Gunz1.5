SELECT	name AS TBLName, 0 AS Duration
INTO	#TempTableList
FROM	sysobjects 
WHERE	Type = 'U'
ORDER BY name

----------------------------------------------------------------------------

DECLARE @SQLString VARCHAR(MAX), @TBLName VARCHAR(MAX);
DECLARE @StartTime DATETIME;

DECLARE CUR_TBLList CURSOR
FOR
	SELECT	TBLName
	FROM	#TempTableList
	
OPEN CUR_TBLList;
FETCH CUR_TBLList INTO @TBLName;

WHILE( @@FETCH_STATUS = 0 ) 
BEGIN
	SET @StartTime = GETDATE();
		
	SET @SQLString = 'DBCC DBREINDEX (' + @TBLName + ')';
	EXEC (@SQLString);
	
	UPDATE	#TempTableList
	SET		Duration = DATEDIFF(ms, @StartTime, GETDATE())
	WHERE	TBLName = @TBLName;		
	
	FETCH CUR_TBLList INTO @TBLName;
END

CLOSE CUR_TBLList;
DEALLOCATE CUR_TBLList;

----------------------------------------------------------------------------