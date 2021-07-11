--USE GunzdB
USE TGunzDB
GO

BEGIN TRAN -------------
	EXEC sp_helpindex Character
	
	DROP INDEX dbo.Character.IX_Character_Name
	DROP INDEX dbo.Character.IX_Character_DeleteFlag	
	DROP INDEX dbo.Character.IX_Character_AID_CharNum
	DROP INDEX dbo.Character.IX_Character_AID_DeleteFlag
			
		 
	UPDATE	c 
	SET		c.DeleteName = c.Name, c.Name = '', c.CharNum = -1, c.DeleteFlag = 1
	FROM	dbo.Character c 
			, dbo.CharacterDeleteTarget t	 	
	WHERE	c.CID = t.CID
	AND		c.DeleteFlag = 0;
	
		
	CREATE INDEX IX_Character_Name				ON dbo.Character(Name)
	CREATE INDEX IX_Character_DeleteFlag		ON dbo.Character(DeleteFlag)
	CREATE INDEX IX_Character_AID_CharNum		ON dbo.Character(AID, CharNum)
	CREATE INDEX IX_Character_AID_DeleteFlag	ON dbo.Character(AID, DeleteFlag)	
COMMIT TRAN ------------



----------------------------------------------------------------------------------------------
-- Step 1. 삭제된 케릭터들과 관련된 아이템 관리
----------------------------------------------------------------------------------------------
BEGIN TRAN ------------	
	INSERT INTO dbo.AccountItem(AID, ItemID, RentDate, RentHourPeriod)
		SELECT	t.AID, ci.ItemID, ci.RentDate, ci.RentHourPeriod
		FROM	dbo.CharacterDeleteTarget t
				, dbo.CharacterItem ci
		WHERE	ci.CID = t.CID 	
		AND		ci.ItemID >= 500000
		
		
	DELETE	ci
	FROM	dbo.CharacterDeleteTarget t
			, dbo.CharacterItem ci
	WHERE	ci.CID = t.CID 	
	AND		ci.ItemID >= 500000
COMMIT TRAN ------------



----------------------------------------------------------------------------------------------
-- Step 2. 삭제된 케릭터들과 관련된 클랜 관리
----------------------------------------------------------------------------------------------
BEGIN TRAN ------------	
	EXEC sp_helpindex Clan
	
	DROP INDEX dbo.Clan.IX_Clan_DeleteFlag
	DROP INDEX dbo.Clan.IX_Clan_Name
	
	-- 마스터인 케릭터가 삭제되면 클랜을 폐쇄	
	UPDATE	c
	SET		c.DeleteName = c.Name, c.Name = NULL, c.DeleteFlag = 1, c.DeleteDate = GETDATE()
	FROM	dbo.CharacterDeleteTarget t
			, dbo.Clan c 
	WHERE	c.MasterCID = t.CID	
	
	
	CREATE INDEX IX_Clan_Name		ON dbo.Clan(Name)
	CREATE INDEX IX_Clan_DeleteFlag	ON dbo.Clan(DeleteFlag)
				

	-- 삭제된 케릭터들 중에, 클랜에 속해 있는 사람은 모두 탈퇴시킨다.
	DELETE	c
	FROM	dbo.CharacterDeleteTarget t
		  , dbo.ClanMember c 
	WHERE	c.CID = t.CID
	
		
	-- 아무도 없는 클랜이 있을 경우, 클랜을 폐쇄
	UPDATE Clan
	SET	   DeleteName = Name, Name = NULL, DeleteFlag = 1, DeleteDate = GETDATE()
	WHERE  DeleteFlag = 0 
	AND	   NOT EXISTS(SELECT 1 FROM ClanMember cm WHERE cm.CLID = Clan.CLID);	
	
	
	
COMMIT TRAN ------------



----------------------------------------------------------------------------------------------
-- Step 3. 삭제된 케릭터들과 관련된 친구 관리
----------------------------------------------------------------------------------------------
BEGIN TRAN ------------
	EXEC sp_helpindex Friend
	
	DELETE	f
	FROM	dbo.CharacterDeleteTarget t
			, dbo.Friend f
	WHERE	t.CID = f.FriendCID	
	OR		t.CID = f.CID
	
COMMIT TRAN ------------


----------------------------------------------------------------------------------------------
-- Step 4. 삭제된 케릭터들과 관련된 케릭터 생성 로그 관리
----------------------------------------------------------------------------------------------
BEGIN TRAN ------------

	INSERT INTO GunzDB.dbo.CharacterMakingLog(AID, CharName, Type, Date)
		SELECT	t.AID, t.Name, '삭제', GETDATE()
		FROM	dbo.CharacterDeleteTarget t(NOLOCK)	
		
COMMIT TRAN ------------