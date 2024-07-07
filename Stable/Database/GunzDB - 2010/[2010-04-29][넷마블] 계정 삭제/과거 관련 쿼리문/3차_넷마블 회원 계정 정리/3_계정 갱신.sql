UPDATE l SET l.Password = r.ValidUserCN
FROM dbo.Netma_Login l JOIN dbo.NetmarbleTmpForUpdate r ON r.AID = l.AID
WHERE r.UserID = l.UserID;