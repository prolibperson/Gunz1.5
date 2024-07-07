ALTER PROCEDURE dbo.spDTInsertDTCharacterInfo  
	@CID INT  
AS BEGIN  
	SET NOCOUNT ON;
	
	DECLARE @BasicTP INT;		
	DECLARE @TimeStampIndex INT;
	DECLARE @TimeStamp CHAR(8);	
	
	SET @BasicTP = 1000;
	
	SELECT TOP 1 @TimeStampIndex = ts.ID, @TimeStamp = ts.TimeStamp 
	FROM DTTimeStamp ts WITH (NOLOCK) 
	ORDER BY TimeStamp DESC;
	
	----------------------------------------------------------------------------------
	-- 전주 랭킹에 대한 Grade 구하기	
	--  1등급 상위 4%,  2등급 상위 11%, 3등급 상위 23%, 4등급 상위 40%,  5등급 상위 60% 
	--	6등급 상위 77%,	7등급 상위 89%,	8등급 상위 96%,	9등급 상위 100%, 10등급 상위 천점이하
	
	DECLARE @Grade INT;	
	DECLARE @PreTP INT;
	DECLARE @PreRank INT;
	DECLARE @TotalUser INT;
	DECLARE @PreTimeStamp CHAR(8);	
	
	SELECT @PreTimeStamp = ts.TimeStamp, @TotalUser = ts.TotalUser 
	FROM DTTimeStamp ts WITH (NOLOCK) 
	WHERE ts.ID = @TimeStampIndex - 1 AND ts.TimeStamp < @TimeStamp;
	
	SELECT @PreRank = ci.Ranking, @PreTP = ci.TP
	FROM DTCharacterInfo ci WITH (NOLOCK) 
	WHERE ci.CID = @CID AND ci.TimeStamp = @PreTimeStamp;

	IF( @PreRank IS NOT NULL AND @TotalUser IS NOT NULL) BEGIN
		IF( @PreTP <= 1000 ) SET @Grade = 10;
		ELSE BEGIN
			SET @Grade = (@PreRank * 100) / @TotalUser;
			IF( @Grade <= 4 )			SET @Grade = 1;
			ELSE IF( @Grade <= 11 )		SET @Grade = 2;
			ELSE IF( @Grade <= 23 )		SET @Grade = 3;
			ELSE IF( @Grade <= 40 )		SET @Grade = 4;
			ELSE IF( @Grade <= 60 )		SET @Grade = 5;
			ELSE IF( @Grade <= 77 )		SET @Grade = 6;
			ELSE IF( @Grade <= 89 )		SET @Grade = 7;
			ELSE IF( @Grade <= 96 )		SET @Grade = 8;
			ELSE IF( @Grade <= 100 )	SET @Grade = 9;			
		END
	END	
	ELSE SET @Grade = 10;
----------------------------------------------------------------------------------

	INSERT INTO DTCharacterInfo   
	VALUES(@TimeStamp, @CID, @BasicTP, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, @Grade);	
END