DECLARE @TestCase	INT;
DECLARE @Count		INT;
DECLARE @TimeStamp	CHAR(8);
DECLARE @TP			INT;

SET @TestCase = 10000;
SET @Count = 0;
SET @TimeStamp = dbo.fnGetDTTimeStamp(GETDATE());

WHILE(@Count != @TestCase) BEGIN
	SET @Count = @Count + 1;
	SET @TP = 1000000 % (@Count * 12);
	
	INSERT DTCharacterInfo(TimeStamp, CID, TP, Wins, Loses, Ranking, RankingIncrease, FinalGames, FinalWins, SFinalGames, SFinalWins, QFinalGames, QFinalWins)
	VALUES(@TimeStamp, @Count, @TP, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);	
END

SELECT * FROM DTCharacterInfo
TRUNCATE TABLE DTCharacterInfo