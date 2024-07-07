========================================================================
    SVNRevisionTracker 프로젝트 개요
    
		2007-03-22 RaonHaje 프로젝트 시작
========================================================================

Subversion의 Revision을 얻어와 cpp 파일로 만들어준다.

> svn info --xml > revision.xml		// 서브버전에서의 리비전정보 획득
> SVNRevisionTracker revision.xml	// 리비전 정보 파싱 & cpp 생성
