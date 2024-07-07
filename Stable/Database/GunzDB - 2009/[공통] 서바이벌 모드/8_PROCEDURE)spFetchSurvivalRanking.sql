USE GunzDB
GO

ALTER PROCEDURE dbo.spFetchSurvivalRanking
AS
BEGIN
	SET NOCOUNT ON;
	
	BEGIN TRAN;
	
	------------------------------------------------------------------------------------------------------
	-- 먼저 SurvivalCharacterInfo 테이블의 tmpRP를 RP로 업데이트한다
	-- 랭킹을 계산할 당시의 RP를 잠시 저장한다.
	UPDATE	dbo.SurvivalCharacterInfo
	SET		tmpRP = RP;
	
	IF (0 <> @@ERROR)
	BEGIN
		ROLLBACK TRAN;
		RETURN;
	END
	
	-- 임시로 테이블을 만든다(추후 랭킹 테이블이 될 테이블이다)
	CREATE TABLE dbo.tmpRanking(SID INT NOT NULL, RP INT NOT NULL, Ranking INT);	
	
	------------------------------------------------------------------------------------------------------
	
	DECLARE @nRank		INT
	DECLARE @nPreSID	INT
	DECLARE @nSID		INT
	DECLARE @nRP		INT
	DECLARE @nCount		INT
	
	-- Cursor를 설정한다. 
	-- Cursor는 SurvivalCharacterInfo 테이블을 SID, tmpRP 순으로 정렬하여,
	-- SID, RP, 중복된 RP들의 수로 만든다.
	DECLARE Curs CURSOR FAST_FORWARD FOR 
	SELECT t.SID, t.RP, t.Cnt 
	FROM	
	(
		SELECT	SID, tmpRP AS RP, COUNT(tmpRP) AS Cnt	
		FROM	dbo.SurvivalCharacterInfo WITH (NOLOCK)
		WHERE	RP_LatestTime < CONVERT(CHAR, DATEADD(dd, 0, GETDATE()), 112)
		AND		DeleteFlag != 1
		GROUP BY SID, tmpRP
	) AS t
	ORDER BY t.SID ASC, t.RP DESC;
	
	-- 커서를 열고.. Fetch 시작
	OPEN Curs
	FETCH NEXT FROM Curs INTO @nSID, @nRP, @nCount;

	SET @nRank = 1
	SET @nPreSID = @nSID	
				
	WHILE( @@FETCH_STATUS = 0 )
	BEGIN
		-- 임시 테이블에 모두 집어넣는다.
		INSERT INTO tmpRanking(SID, RP, Ranking) VALUES (@nSID, @nRP, @nRank);
		IF (0 = @@ROWCOUNT OR 0 <> @@ERROR)
		BEGIN
			ROLLBACK TRAN;
			CLOSE Curs;
			DEALLOCATE Curs;
			RETURN;
		END
		
		SET @nRank = @nRank + @nCount 		
		FETCH NEXT FROM Curs INTO @nSID, @nRP, @nCount
		
		-- SID 값이 바뀌면(증가할 경우), 랭킹을 다시 1부터 계산한다.
		-- 시나리오별로 랭킹을 계산해야 된다.
		IF( @nPreSID < @nSID )
		BEGIN
			SET @nRank = 1
			SET @nPreSID = @nSID			
		END
	END
	
	CLOSE Curs
	DEALLOCATE Curs
	
	------------------------------------------------------------------------------------------------------
	
	-- Primary Key를 현재의 시간을 이용하여, 만든다.
	-- 아래와 같이 만드는 이유는 테이블을 Switch할 때, Key가 겹치면 안되기 때문이다.
	DECLARE @Date NVARCHAR(15);
	DECLARE @PKSQL NVARCHAR(512);
	DECLARE @IXSQL NVARCHAR(512);
	
	SET @Date = CAST(DATEDIFF(ss, '2009-06-09T00:00:00', GETDATE()) AS NVARCHAR(15));
	SET @PKSQL = N'ALTER TABLE tmpRanking ADD CONSTRAINT PK_SurvivalRanking_SID_RP_' + @Date + N' PRIMARY KEY (SID ASC, RP DESC);'	
	EXEC sp_executesql @PKSQL;
		
	-- 기존의 Index의 이름을 바꾼다. 이유는 tmpRP가 이제 RankRP가 되기 때문이다.
	EXEC sp_rename 'SurvivalCharacterInfo.IX_SurvivalCharacterInfo_RankRP', 'IX_SurvivalCharacterInfo_tmpRP', 'INDEX';	
	
	-- 기존의 Index 이름을 바꾼 후, 다시 Index를 만든다.
	CREATE NONCLUSTERED INDEX IX_SurvivalCharacterInfo_RankRP ON SurvivalCharacterInfo(tmpRP);
	
	-- Column을 Rename한다.
	EXEC sp_rename 'SurvivalCharacterInfo.RankRP', 'tmpRP2', 'COLUMN';
	EXEC sp_rename 'SurvivalCharacterInfo.tmpRP', 'RankRP', 'COLUMN';
	EXEC sp_rename 'SurvivalCharacterInfo.tmpRP2', 'tmpRP', 'COLUMN';
	
	-- 기존의 Index를 삭제한다.
	DROP INDEX dbo.SurvivalCharacterInfo.IX_SurvivalCharacterInfo_tmpRP;
		
	-- 기존의 Ranking 테이블을 삭제하고 임시 테이블의 이름을 바꾼다.
	DROP TABLE dbo.SurvivalRanking;
	EXEC sp_rename 'tmpRanking', 'SurvivalRanking';
	
	
	------------------------------------------------------------------------------------------------------		
	TRUNCATE TABLE dbo.SurvivalCharacterInfoWeb;	
	INSERT INTO dbo.SurvivalCharacterInfoWeb(SID, CID)
		SELECT	SID, CID 
		FROM	dbo.SurvivalCharacterInfo WITH (NOLOCK)
		WHERE	RP_LatestTime < CONVERT(CHAR, DATEADD(dd, 0, GETDATE()), 112)
		AND		DeleteFlag != 1
		ORDER BY SID ASC, RankRP DESC
	------------------------------------------------------------------------------------------------------			
	
	COMMIT TRAN;
END
GO
