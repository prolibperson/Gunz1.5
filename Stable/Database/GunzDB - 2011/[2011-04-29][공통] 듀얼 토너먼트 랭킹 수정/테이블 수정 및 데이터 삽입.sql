ALTER TABLE dbo.DTCharacterRankingHistory ADD Name VARCHAR(24);

UPDATE  DTH
SET     DTH.Name = CASE WHEN C.DeleteFlag = 0 THEN C.Name ELSE C.DeleteName END
FROM    dbo.DTCharacterRankingHistory DTH(NOLOCK)
        JOIN dbo.Character C(NOLOCK)
        ON DTH.CID = C.CID
