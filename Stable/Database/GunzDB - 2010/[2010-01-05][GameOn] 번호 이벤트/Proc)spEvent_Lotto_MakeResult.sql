ALTER PROCEDURE dbo.spEvent_Lotto_MakeResult
AS
BEGIN
	SET NOCOUNT ON;
		
	DECLARE @SDate DATETIME;
	DECLARE @SIndex INT;
	
	SET @SIndex = 21716055;
	SET @SDate = '2009-12-24 00:00:00.000';
	
	DECLARE @ItemID INT;
	DECLARE @MaxItemID INT;
	
	SET @ItemID = 401000;	
	SET @MaxItemID = 401009;
	
	DELETE Event_Lotto_GameOn;
	
	INSERT INTO dbo.Event_Lotto_GameOn
		SELECT ci.CID, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		FROM dbo.CharacterItem ci(NOLOCK)
		WHERE ci.CIID > @SIndex
			AND ci.RegDate > @SDate
			AND ci.CID IS NOT NULL
		GROUP BY ci.CID
		
	WHILE( @ItemID <= @MaxItemID ) BEGIN
		IF( @ItemID = 401000 ) BEGIN
			UPDATE r SET r.Num0 = t.Count
			FROM 
			(
				SELECT ci.CID, COUNT(ci.CID) AS Count
				FROM dbo.CharacterItem ci(NOLOCK)
				WHERE ci.CIID > @SIndex	AND ci.RegDate > @SDate	AND ci.CID IS NOT NULL AND ci.ItemID = @ItemID
				GROUP BY ci.CID
			) t, dbo.Event_Lotto_GameOn r
			WHERE t.CID = r.CID
		END ELSE IF( @ItemID = 401001 ) BEGIN
			UPDATE r SET r.Num1 = t.Count
			FROM 
			(
				SELECT ci.CID, COUNT(ci.CID) AS Count
				FROM dbo.CharacterItem ci(NOLOCK)
				WHERE ci.CIID > @SIndex	AND ci.RegDate > @SDate	AND ci.CID IS NOT NULL AND ci.ItemID = @ItemID
				GROUP BY ci.CID
			) t, dbo.Event_Lotto_GameOn r
			WHERE t.CID = r.CID
		END ELSE IF( @ItemID = 401002 ) BEGIN
			UPDATE r SET r.Num2 = t.Count
			FROM 
			(
				SELECT ci.CID, COUNT(ci.CID) AS Count
				FROM dbo.CharacterItem ci(NOLOCK)
				WHERE ci.CIID > @SIndex	AND ci.RegDate > @SDate	AND ci.CID IS NOT NULL AND ci.ItemID = @ItemID
				GROUP BY ci.CID
			) t, dbo.Event_Lotto_GameOn r
			WHERE t.CID = r.CID
		END ELSE IF( @ItemID = 401003 ) BEGIN
			UPDATE r SET r.Num3 = t.Count
			FROM 
			(
				SELECT ci.CID, COUNT(ci.CID) AS Count
				FROM dbo.CharacterItem ci(NOLOCK)
				WHERE ci.CIID > @SIndex	AND ci.RegDate > @SDate	AND ci.CID IS NOT NULL AND ci.ItemID = @ItemID
				GROUP BY ci.CID
			) t, dbo.Event_Lotto_GameOn r
			WHERE t.CID = r.CID
		END ELSE IF( @ItemID = 401004 ) BEGIN
			UPDATE r 
			SET r.Num4 = t.Count
			FROM 
			(
				SELECT ci.CID, COUNT(ci.CID) AS Count
				FROM dbo.CharacterItem ci(NOLOCK)
				WHERE ci.CIID > @SIndex	AND ci.RegDate > @SDate	AND ci.CID IS NOT NULL AND ci.ItemID = @ItemID
				GROUP BY ci.CID
			) t, dbo.Event_Lotto_GameOn r
			WHERE t.CID = r.CID
		END ELSE IF( @ItemID = 401005 ) BEGIN
			UPDATE r SET r.Num5 = t.Count
			FROM 
			(
				SELECT ci.CID, COUNT(ci.CID) AS Count
				FROM dbo.CharacterItem ci(NOLOCK)
				WHERE ci.CIID > @SIndex	AND ci.RegDate > @SDate	AND ci.CID IS NOT NULL AND ci.ItemID = @ItemID
				GROUP BY ci.CID
			) t, dbo.Event_Lotto_GameOn r
			WHERE t.CID = r.CID
		END ELSE IF( @ItemID = 401006 ) BEGIN
			UPDATE r SET r.Num6 = t.Count
			FROM 
			(
				SELECT ci.CID, COUNT(ci.CID) AS Count
				FROM dbo.CharacterItem ci(NOLOCK)
				WHERE ci.CIID > @SIndex	AND ci.RegDate > @SDate	AND ci.CID IS NOT NULL AND ci.ItemID = @ItemID
				GROUP BY ci.CID
			) t, dbo.Event_Lotto_GameOn r
			WHERE t.CID = r.CID
		END ELSE IF( @ItemID = 401007 ) BEGIN
			UPDATE r SET r.Num7 = t.Count
			FROM 
			(
				SELECT ci.CID, COUNT(ci.CID) AS Count
				FROM dbo.CharacterItem ci(NOLOCK)
				WHERE ci.CIID > @SIndex	AND ci.RegDate > @SDate	AND ci.CID IS NOT NULL AND ci.ItemID = @ItemID
				GROUP BY ci.CID
			) t, dbo.Event_Lotto_GameOn r
			WHERE t.CID = r.CID
		END ELSE IF( @ItemID = 401008 ) BEGIN
			UPDATE r SET r.Num8 = t.Count
			FROM 
			(
				SELECT ci.CID, COUNT(ci.CID) AS Count
				FROM dbo.CharacterItem ci(NOLOCK)
				WHERE ci.CIID > @SIndex	AND ci.RegDate > @SDate	AND ci.CID IS NOT NULL AND ci.ItemID = @ItemID
				GROUP BY ci.CID
			) t, dbo.Event_Lotto_GameOn r
			WHERE t.CID = r.CID
		END ELSE IF( @ItemID = 401009 ) BEGIN
			UPDATE r SET r.Num9 = t.Count
			FROM 
			(
				SELECT ci.CID, COUNT(ci.CID) AS Count
				FROM dbo.CharacterItem ci(NOLOCK)
				WHERE ci.CIID > @SIndex	AND ci.RegDate > @SDate	AND ci.CID IS NOT NULL AND ci.ItemID = @ItemID
				GROUP BY ci.CID
			) t, dbo.Event_Lotto_GameOn r
			WHERE t.CID = r.CID
		END
		
		SET @ItemID = @ItemID + 1
	END		
END