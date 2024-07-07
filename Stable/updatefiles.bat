@rem -----------------------------
@rem 소스 받아오기
@rem -----------------------------
@CALL Utils\bin\setenv

svn update 

@rem -----------------------------
@rem 리소스 가져오기
@rem -----------------------------

SET SSDIR=\\Builder\ArtWorks

@cd "Gunz\XML"
%SourceSafe% CP $/XML
%SourceSafe% GET * -R -I- 
@cd "..\.."

@cd "Gunz\Runtime"
%SourceSafe% CP $/Runtime
%SourceSafe% GET * -R -I-

@cd "..\.."
