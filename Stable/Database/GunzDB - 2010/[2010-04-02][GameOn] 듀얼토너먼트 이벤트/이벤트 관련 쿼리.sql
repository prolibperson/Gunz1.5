-- 1. DuelTournament 모드를 플레이한 캐릭터의 이름(특정 기간)
DECLARE @TimeStamp CHAR(8);
SET @TimeStamp = '특정기간입력';

SELECT ci.TimeStamp, c.Name, ci.Wins, ci.Loses
FROM DTCharacterInfo ci(NOLOCK) JOIN Character c(NOLOCK) ON ci.CID = c.CID
WHERE ci.TimeStamp = @TimeStamp AND (ci.Wins > 0 OR ci.Loses > 0)



-- 2. DuelTournament 모드로 우승한 횟수(특정 기간)
DECLARE @TimeStamp	CHAR(8);
DECLARE @Cnt		INT;

SET @TimeStamp  = '특정기간입력';
SET @Cnt		= '특정횟수입력';

SELECT ci.TimeStamp, c.Name, ci.FinalWins
FROM DTCharacterInfo ci(NOLOCK) JOIN Character c(NOLOCK) ON ci.CID = c.CID
WHERE ci.TimeStamp = @TimeStamp AND ci.FinalWins >= @Cnt
