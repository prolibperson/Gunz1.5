ALTER PROCEDURE dbo.spDTInsertDTTimeStamp
	@StampType TINYINT
AS BEGIN	
	SET NOCOUNT ON;
	
	DECLARE @TimeStamp CHAR(8);
	DECLARE @CurTimeStamp CHAR(8);
	DECLARE @TotalUser INT;
	DECLARE @RowID INT;

	-- Stamp Type에 따라 Stamp값을 구한다.
	IF( @StampType = 0 ) BEGIN		-- Daily
		SELECT @CurTimeStamp = CONVERT( CHAR(8), GETDATE(), 112);
	END ELSE IF( @StampType = 1 ) BEGIN -- Weekly
		SELECT @CurTimeStamp = CONVERT( CHAR(8), DATEADD(dd, DATEPART(WEEKDAY, CONVERT(DATETIME, GETDATE()))*(-1)+2, CONVERT(DATETIME, GETDATE())), 112);
	END ELSE BEGIN
		RETURN;
	END
	
	-- 가장 최근의 Stamp값을 구하고..
	SELECT TOP 1 @TimeStamp = TimeStamp, @RowID = ID + 1
	FROM DTTimeStamp(NOLOCK)
	ORDER BY TimeStamp DESC;
	
	IF( @TimeStamp IS NULL ) BEGIN
		INSERT INTO DTTimeStamp(ID, TimeStamp, Closed) VALUES(1, @CurTimeStamp, 0);
		RETURN;
	END
	
	-- 비교해서 최근의 Stamp값과 계산한 Stamp값을 비교!
	IF( @TimeStamp != @CurTimeStamp ) BEGIN
		SELECT @TotalUser = COUNT(ci.CID) 
		FROM DTCharacterInfo ci(NOLOCK) 
		WHERE ci.TimeStamp = @TimeStamp AND ci.TP > 1000;
		
		UPDATE DTTimeStamp
		SET TotalUser = @TotalUser
		WHERE TimeStamp = @TimeStamp;
		
		INSERT INTO DTTimeStamp(ID, TimeStamp, Closed) VALUES(@RowID, @CurTimeStamp, 0)
	END	
END

EXEC spDTInsertDTTimeStamp 1