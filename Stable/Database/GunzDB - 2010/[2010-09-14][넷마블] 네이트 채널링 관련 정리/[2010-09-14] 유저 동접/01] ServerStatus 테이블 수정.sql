USE GunzDB
GO

--ALTER TABLE dbo.ServerStatus ADD NetmarblePlayer INT
ALTER TABLE dbo.ServerStatus ADD NatePlayer INT DEFAULT(0)
GO

ALTER TABLE dbo.ServerStatus ADD CONSTRAINT DF_ServerStatus_NatePlayer DEFAULT (0) FOR NatePlayer
GO
