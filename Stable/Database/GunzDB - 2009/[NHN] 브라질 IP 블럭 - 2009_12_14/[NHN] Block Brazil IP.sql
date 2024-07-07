-- IPtoCountry의 Table 내의 데이터를 모두 삭제한다.
DELETE IPtoCountry


-- 다음의 쿼리를 이용하여 데이터를 모두 삽입한다.
-- 주의점 1. SQL Server가 설치된 서버에서 실행해야 됩니다.
-- 주의점 2. 경로명 및 파일명이 SQL Server에 있어야 합니다.
-- 주의점 3. 경로명 및 파일명이 정확해야 합니다.
BULK INSERT dbo.IPtoCountry FROM '경로명\파일명'		-- ex) 'c:\ip-to-country.csv'
WITH (
	DATAFILETYPE = 'char',
	FIELDTERMINATOR = ',',
	ROWTERMINATOR = '\n'
)
