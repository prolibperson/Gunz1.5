ALTER PROCEDURE dbo.spDTFetchDTRanking	
AS BEGIN  

	SET NOCOUNT ON;

    ------------------------------------------------------------------------------------------------------------- 

	DECLARE @CurTimeStamp CHAR(8);  
	DECLARE @CurTimeStampIndex INT;
	
	DECLARE @PreTimeStamp CHAR(8);  
	DECLARE @PreTimeStampIndex INT;
			
	SELECT TOP 1 @CurTimeStamp = ts.TimeStamp, @CurTimeStampIndex = ts.ID  
	FROM DTTimeStamp ts WITH (NOLOCK)
	ORDER BY ts.TimeStamp DESC;
	
	SELECT @PreTimeStamp = TimeStamp 
	FROM DTTimeStamp 
	WHERE ID = @CurTimeStampIndex - 1;
	
	DECLARE @Closed TINYINT;
	SELECT @Closed = Closed FROM DTTimeStamp WHERE TimeStamp = @PreTimeStamp;

    ------------------------------------------------------------------------------------------------------------

	--과거의 TimeStamp의 랭킹 계산도 해주자!
	IF( @Closed = 0 ) BEGIN	

		UPDATE DTTimeStamp SET Closed = Closed + 1 WHERE TimeStamp = @PreTimeStamp;
		
		-- 일단 랭킹 계산 테이블의 모든 데이터를 지우고...  
		TRUNCATE TABLE DTCharacterRanking;		

        INSERT DTCharacterRanking(CID, TimeStamp, TP, FinalWins, Wins, Loses, PreGrade)  
	        SELECT  CI.CID, CI.TimeStamp, CI.TP, CI.FinalWins, CI.Wins, CI.Loses, CI.PreGrade  
	        FROM    DTCharacterInfo CI(NOLOCK) 
                    JOIN Character C(NOLOCK) 
                    ON C.CID = CI.CID
	        WHERE   CI.TimeStamp = @PreTimeStamp    
            AND     C.DeleteFlag = 0
	        ORDER BY CI.TP DESC, CI.FinalWins DESC, CI.SFinalWins DESC, CI.QFinalWins DESC
		

		-- 듀얼토너먼트 케릭터 정보 업데이트 해주고..
		UPDATE  ci  
		SET     ci.RankingIncrease = CASE WHEN ci.Ranking = -1 THEN -1 ELSE ci.Ranking - cr.Rank END  
			    , ci.Ranking  = cr.Rank
		FROM    DTCharacterInfo ci 
                JOIN DTCharacterRanking cr 
                ON ci.CID = cr.CID  
		WHERE   ci.CID = cr.CID 
        AND     ci.TimeStamp = @PreTimeStamp;  

		-- 랭킹 계산 테이블에 랭킹 변동치를 다시 업데이트 해준다.  
		UPDATE  cr  
		SET     cr.RankingIncrease = ci.RankingIncrease  
		FROM    DTCharacterRanking cr 
                JOIN DTCharacterInfo ci 
                ON ci.CID = cr.CID  
		WHERE   ci.CID = cr.CID 
        AND     ci.TimeStamp = @PreTimeStamp; 
		
		-- RankingHistory 테이블에 데이터 삽입!
		DELETE DTCharacterRankingHistory WHERE TimeStamp = @PreTimeStamp;
		
		DECLARE @UserCount INT;
		SELECT @UserCount = COUNT(*) FROM DTCharacterInfo WHERE TimeStamp = @PreTimeStamp;
	
		UPDATE  ci
		SET     ci.PreGrade = CASE  WHEN (cr.TP <= 1000) THEN 10
					WHEN ((cr.Rank * 100) / @UserCount <= 4 )   THEN 1
					WHEN ((cr.Rank * 100) / @UserCount <= 11 )  THEN 2
					WHEN ((cr.Rank * 100) / @UserCount <= 23 )  THEN 3
					WHEN ((cr.Rank * 100) / @UserCount <= 40 )  THEN 4
					WHEN ((cr.Rank * 100) / @UserCount <= 60 )  THEN 5
					WHEN ((cr.Rank * 100) / @UserCount <= 77 )  THEN 6
					WHEN ((cr.Rank * 100) / @UserCount <= 89 )  THEN 7
					WHEN ((cr.Rank * 100) / @UserCount <= 96 )  THEN 8
					WHEN ((cr.Rank * 100) / @UserCount <= 100 ) THEN 9
					ELSE 10 END
		FROM    DTCharacterInfo ci, DTCharacterRanking cr
		WHERE   ci.TimeStamp = @CurTimeStamp 
        AND     cr.TimeStamp = @PreTimeStamp
        AND     ci.CID = cr.CID;
		
		INSERT DTCharacterRankingHistory(TimeStamp, Rank, CID, Name, TP, FinalWins, Wins, Loses, Grade)
			SELECT TOP 100 cr.TimeStamp
						, cr.Rank
						, cr.CID
                        , CASE  WHEN c.DeleteFlag = 0 THEN c.Name ELSE c.DeleteName END AS Name
						, cr.TP
						, cr.FinalWins
						, cr.Wins
						, cr.Loses
						, CASE  WHEN (cr.TP <= 1000) THEN 10	
								WHEN (((cr.Rank * 100) / @UserCount) <= 4 )    THEN 1 
								WHEN (((cr.Rank * 100) / @UserCount) <= 11 )   THEN 2
								WHEN (((cr.Rank * 100) / @UserCount) <= 23 )   THEN 3 
								WHEN (((cr.Rank * 100) / @UserCount) <= 40 )   THEN 4 
								WHEN (((cr.Rank * 100) / @UserCount) <= 60 )   THEN 5 
								WHEN (((cr.Rank * 100) / @UserCount) <= 77 )   THEN 6 
								WHEN (((cr.Rank * 100) / @UserCount) <= 89 )   THEN 7 
								WHEN (((cr.Rank * 100) / @UserCount) <= 96 )   THEN 8 
								WHEN (((cr.Rank * 100) / @UserCount) <= 100 )  THEN 9 								
								ELSE 10 END		
			FROM    DTCharacterRanking cr(NOLOCK)
                    JOIN Character C(NOLOCK)
                    ON cr.CID = C.CID
			WHERE   cr.TimeStamp = @PreTimeStamp
			ORDER BY cr.Rank;
	END
	   

------------------------------------------------------------------------------------------------------------
	-- 일단 랭킹 계산 테이블의 모든 데이터를 지우고...  
	TRUNCATE TABLE DTCharacterRanking;
	   
	-- 랭킹 계산 테이블에 모두 집어넣고..  
	INSERT DTCharacterRanking(CID, TimeStamp, TP, FinalWins, Wins, Loses, PreGrade)  
		SELECT  CI.CID, CI.TimeStamp, CI.TP, CI.FinalWins, CI.Wins, CI.Loses, CI.PreGrade  
		FROM    DTCharacterInfo CI(NOLOCK) 
                JOIN Character C(NOLOCK) 
                ON C.CID = CI.CID
		WHERE   CI.TimeStamp = @CurTimeStamp    
        AND     C.DeleteFlag = 0
		ORDER BY CI.TP DESC, CI.FinalWins DESC, CI.SFinalWins DESC, CI.QFinalWins DESC  
	    
	-- 듀얼토너먼트 케릭터 정보 업데이트 해주고..  
	UPDATE  ci  
	SET     ci.RankingIncrease = CASE WHEN ci.Ranking = -1 THEN 1000000000 ELSE ci.Ranking - cr.Rank END  
		    , ci.Ranking = cr.Rank  
	FROM    DTCharacterInfo ci 
            JOIN DTCharacterRanking cr 
            ON ci.CID = cr.CID  
	WHERE   ci.CID = cr.CID 
    AND     ci.TimeStamp = @CurTimeStamp;  

	-- 랭킹 계산 테이블에 랭킹 변동치를 다시 업데이트 해준다.  
	UPDATE  cr  
	SET     cr.RankingIncrease = ci.RankingIncrease  
	FROM    DTCharacterRanking cr 
            JOIN DTCharacterInfo ci 
            ON  ci.CID = cr.CID  
	WHERE   ci.CID = cr.CID 
    AND     ci.TimeStamp = @CurTimeStamp;  
------------------------------------------------------------------------------------------------------------
END
