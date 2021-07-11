CREATE PROCEDURE dbo.spWebDTGetDTTimeStamp
AS BEGIN
	SELECT dt.TimeStamp
	FROM DTTimeStamp dt(NOLOCK)
	WHERE dt.Closed = 1
	ORDER BY dt.TimeStamp
END