CREATE PROCEDURE dbo.spAdmWebRestoreAccountInfo_Netmarble  
	@UserID VARCHAR(24)
AS BEGIN
	DECLARE @AID INT;  
	DECLARE @DeletedAID INT;  

	SELECT @AID = AID FROM Login l WHERE l.UserID = @UserID;
	SELECT @DeletedAID = AID FROM AccountDeleteLog WHERE UserID = @UserID;

	IF( EXISTS(SELECT * FROM Character WHERE AID = @AID AND DeleteFlag = 0) ) BEGIN  
		RETURN -1;  
	END 
	ELSE BEGIN
		UPDATE Character SET AID = @AID WHERE AID = @DeletedAID;  
		UPDATE AccountItem SET AID = @AID WHERE AID = @DeletedAID;
		RETURN 0;  
	END  
END