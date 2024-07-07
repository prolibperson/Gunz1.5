============================================================================================

DROP INDEX dbo.Netma_Account.Netma_Account_UserID_IX;


UPDATE a SET a.UserID = r.DEL_ID
FROM dbo.Netma_Account a JOIN dbo.NetmarbleTmpForRemove r ON r.AID = a.AID
WHERE r.UserID = a.UserID;

CREATE NONCLUSTERED INDEX Netma_Account_UserID_IX ON Netma_Account(UserID);

============================================================================================
UPDATE l SET l.UserID = r.DEL_ID
FROM dbo.Netma_Login l JOIN dbo.NetmarbleTmpForRemove r ON l.AID = r.AID
WHERE r.UserID = l.UserID
-- 3Ка 32УЪ
============================================================================================

UPDATE r SET r.AccountUpdated = 1
FROM dbo.Netma_Account a, dbo.NetmarbleTmpForRemove r WHERE a.AID = r.AID;

UPDATE r SET r.LoginUpdated = 1
FROM dbo.Netma_Login l, dbo.NetmarbleTmpForRemove r WHERE l.AID = r.AID;

INSERT INTO dbo.AccountDeleteLog 
	SELECT r.AID, r.UserID, l.Password, r.DEL_ID, GETDATE()
	FROM NetmarbleTmpForRemove r JOIN Netma_Login l ON l.AID = r.AID
	WHERE r.AccountUpdated = 1 AND r.LoginUpdated = 1;