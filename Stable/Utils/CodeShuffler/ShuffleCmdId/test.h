#ifndef MSHAREDCOMMANDTABLE_H
#define MSHAREDCOMMANDTABLE_H

class MCommandManager;


#define MCOMMAND_VERSION	58



#define MSCT_MASTER			1
#define MSCT_CLIENT			2
#define MSCT_MATCHSERVER	4
#define MSCT_AGENT			8
#define MSCT_ALL			(MSCT_MASTER+MSCT_CLIENT+MSCT_AGENT)




/// 모든 커뮤니케이터가 공유하는 커맨드
void MAddSharedCommandTable(MCommandManager* pCommandManager, int nSharedType);


/// 서버 클라이언트 공유 상수
#define MATCH_CYCLE_CHECK_SPEEDHACK	20000


// 기본 커맨드 리스트
#define MC_HELP						0
#define MC_VERSION					1

#define MC_DEBUG_TEST				100

#define MC_LOCAL_INFO				201		///< Local 정보를 얻는다.
#define MC_LOCAL_ECHO				202		///< Local Echo 테스트
#define MC_LOCAL_LOGIN				203		///< Login 처리 Local Command



#define MC_NET_ENUM					301		///< 연결할 수 있는 커뮤니케이터 Enum
#define MC_NET_CONNECT				302		///< 커뮤니케이터와 연결
#define MC_NET_DISCONNECT			303		///< 커뮤니케이터와 연결 해제
#define MC_NET_CLEAR				304		///< 커뮤니케이터와 연결 해제 후 관련자원 처리
#define MC_NET_ONCONNECT			311		///< 커뮤니케이터와 연결되었을 때
#define MC_NET_ONDISCONNECT			312		///< 커뮤니케이터와 연결이 끊겼을 때
#define MC_NET_ONERROR				313		///< 커뮤니케이터와 연결 에러
#define MC_NET_CHECKPING			321		///< 커뮤니케이터 반응속도검사
#define MC_NET_PING					322		///< 커뮤니케이터 반응속도검사 요청
#define MC_NET_PONG					323		///< 커뮤니케이터 반응속도검사 응답

#define MC_HSHIELD_PING				324		///< 핵실드 메세지 요청(매치서버 -> 클라이언트)
#define MC_HSHIELD_PONG				325		///< 핵실드 메세지 응답(클라이언트 -> 매치서버)

#define MC_NET_BANPLAYER_FLOODING	326		///< CommandBuilder에서 Flooding 검출시 만드는 Command

#define MC_NET_CONNECTTOZONESERVER	331		///< 127.0.0.1:6000 연결

#define MC_NET_REQUEST_INFO			341		///< Net 정보를 요청한다.
#define MC_NET_RESPONSE_INFO		342		///< Net 정보를 얻는다.
#define MC_NET_REQUEST_UID			343		///< MUID를 요청
#define MC_NET_RESPONSE_UID			344		///< MUID를 확보해서 돌려준다.
#define MC_NET_ECHO					351		///< Net Echo 테스트

#define MC_CLOCK_SYNCHRONIZE		361		///< 클럭 싱크




// 미국 핵 제작을 방해하기 위해 아래 SHUFFLE BEGIN ~ END 사이의 블럭의 커맨드ID를 별도의 프로그램으로 섞는다
//[[SHUFFLE_BEGIN]] 				<< 이 주석문은 ID 섞어주는 프로그램에서 인식하는 마크이다
//[[SHUFFLE_RANGE]](70000,79999)

// 매치서버 관련 명령어
#define MC_MATCH_NOTIFY						70231		///< 알림 메시지
#define MC_MATCH_ANNOUNCE					70253		///< 공지
#define MC_MATCH_RESPONSE_RESULT			70033		///< request에 대한 결과를 알림(S -> C)

// 관리자 전용 명령어
#define MC_ADMIN_ANNOUNCE					70021		///< 전체 접속자에게 방송
#define MC_ADMIN_REQUEST_SERVER_INFO		70076		///< 서버 정보 요청
#define MC_ADMIN_RESPONSE_SERVER_INFO		70336		///< 서버 정보 응답
#define MC_ADMIN_SERVER_HALT				70199		///< 서버를 종료 한다
#define MC_ADMIN_TERMINAL					70049		///< 터미날
#define MC_ADMIN_REQUEST_UPDATE_ACCOUNT_UGRADE	70241	///< 계정 권한 변경 요청
#define MC_ADMIN_RESPONSE_UPDATE_ACCOUNT_UGRADE	70348	///< 계정 권한 변경 응답
#define MC_ADMIN_REQUEST_BAN_PLAYER			70096		///< 타겟 접속 끊기 요청
#define MC_ADMIN_RESPONSE_BAN_PLAYER		70326		///< 타겟 접속 끊기 응답
#define MC_ADMIN_PING_TO_ALL				70157		///< Garbage Connection 청소 확인을 위한 수동명령
#define MC_ADMIN_REQUEST_SWITCH_LADDER_GAME	70354		///< 클랜전 신청 가능여부 설정
#define MC_ADMIN_HIDE						70184		///< 운영자 안보이기
#define MC_ADMIN_RELOAD_CLIENT_HASH			70171		///< reload XTrap Hashmap
#define MC_ADMIN_RESET_ALL_HACKING_BLOCK	70180		///< 모든 해킹 블럭을 취소한다.
#define MC_ADMIN_RELOAD_GAMBLEITEM			70322		///< 겜블 아이템 정보 다시 읽기.
#define MC_ADMIN_DUMP_GAMBLEITEM_LOG		70264		///< 현제 겜블 아이템 정보를 Log파일에 출력한다.
#define MC_ADMIN_ASSASIN					70053		///< GM이 커맨더가 된다.

// 이벤트 마스터 명령어
#define MC_EVENT_CHANGE_MASTER				70052		///< 방장권한을 뺏어온다
#define MC_EVENT_CHANGE_PASSWORD			70303		///< 방의 비밀번호를 바꾼다
#define MC_EVENT_REQUEST_JJANG				70014		///< 짱마크 부여
#define MC_EVENT_REMOVE_JJANG				70250		///< 짱마크 회수
#define MC_EVENT_UPDATE_JJANG				70191		///< 짱마크 알림

// 매치서버-클라이언트
#define MC_MATCH_LOGIN							70287	///< 로그인
#define MC_MATCH_RESPONSE_LOGIN					70073	///< 로그인 응답
#define MC_MATCH_LOGIN_NETMARBLE				70127	///< 넷마블에서 로그인
#define MC_MATCH_LOGIN_NETMARBLE_JP				70175	///< 일본넷마블에서 로그인
#define MC_MATCH_LOGIN_FROM_DBAGENT				70213	///< DBAgent로부터의 로그인 응답(일본넷마블 전용)
#define MC_MATCH_LOGIN_FROM_DBAGENT_FAILED		70304	///< DBAgent로부터의 로그인 실패(일본넷마블 전용)
#define MC_MATCH_FIND_HACKING					70056	///< 해킹 검출
#define MC_MATCH_DISCONNMSG						70100	///< Discconst메시지.
#define MC_MATCH_LOGIN_NHNUSA					70290	///< 미국 NHN에서 로그인.
#define MC_MATCH_LOGIN_GAMEON_JP				70261	///< 일본 gameon에서 로그인

#define MC_MATCH_BRIDGEPEER						70310	///< MatchServer에 Peer정보 알림
#define MC_MATCH_BRIDGEPEER_ACK					70307	///< Peer정보 수신확인

#define MC_MATCH_OBJECT_CACHE					70247	///< 오브젝트 캐시

#define MC_MATCH_REQUEST_RECOMMANDED_CHANNEL	70024	///< 최초 참가할 채널에대해 추천을 요청한다.
#define MC_MATCH_RESPONSE_RECOMMANDED_CHANNEL	70329	///< 최초 참가할 채널에대해 추천한다.
#define MC_MATCH_CHANNEL_REQUEST_JOIN			70208	///< 채널에 참가(C -> S)
#define MC_MATCH_CHANNEL_REQUEST_JOIN_FROM_NAME	70097	///< 채널이름으로 채널 조인 요청
#define MC_MATCH_CHANNEL_RESPONSE_JOIN			70243	///< 채널 조인 응답(S -> C)
#define MC_MATCH_CHANNEL_LEAVE					70006	///< 채널 탈퇴
#define MC_MATCH_CHANNEL_LIST_START				70128	///< 채널 목록 전송시작 요청
#define MC_MATCH_CHANNEL_LIST_STOP				70151	///< 채널 목록 전송중지 요청
#define MC_MATCH_CHANNEL_LIST					70121	///< 채널 목록
#define MC_MATCH_CHANNEL_REQUEST_PLAYER_LIST	70300	///< 플레이어 리스트 요청
#define MC_MATCH_CHANNEL_RESPONSE_PLAYER_LIST	70160	///< 플레이어 리스트 응답
#define MC_MATCH_CHANNEL_REQUEST_CHAT			70344	///< 채널에 대화 요청
#define MC_MATCH_CHANNEL_CHAT					70205	///< 채널에서 대화 응답
#define MC_MATCH_CHANNEL_DUMB_CHAT				70176	///< 채널에서 대화 차단, 벙어리 기능 - 홍기주(2009.08.05)

#define MC_MATCH_CHANNEL_REQUEST_RULE			70227	///< 채널규칙 요청
#define MC_MATCH_CHANNEL_RESPONSE_RULE			70313	///< 채널규칙 응답
#define MC_MATCH_CHANNEL_REQUEST_ALL_PLAYER_LIST		70362	///< 채널의 로비에 있는 모든 플레이어 리스트 요청
#define MC_MATCH_CHANNEL_RESPONSE_ALL_PLAYER_LIST		70150	///< 채널의 로비에 있는 플레이어 리스트 요청


#define MC_MATCH_STAGE_CREATE					70083	///< 스테이지 생성
#define MC_MATCH_RESPONSE_STAGE_CREATE			70043	///< 스테이지 생성 응답
#define MC_MATCH_STAGE_JOIN						70339	///< 스테이지 참가
#define MC_MATCH_REQUEST_STAGE_JOIN				70086	///< 스테이지 참가 요청
#define MC_MATCH_REQUEST_PRIVATE_STAGE_JOIN		70332	///< 비밀 스테이지 참가 요청
#define MC_MATCH_RESPONSE_STAGE_JOIN			70221	///< 스테이지에 조인여부를 알림
#define MC_MATCH_STAGE_LEAVE					70000	///< 스테이지 탈퇴
#define MC_MATCH_STAGE_REQUEST_PLAYERLIST		70011	///< 스테이지의 유저목록 요청
#define MC_MATCH_STAGE_FOLLOW					70113	///< 지정 유저를 따라서 스테이지 참가
#define MC_MATCH_RESPONSE_STAGE_FOLLOW			70146	///< 지정 유저를 따라서 스테이지 참가 응답
#define MC_MATCH_REQUEST_STAGE_LIST				70206	///< 클라이언트가 스테이지 리스트 요청
#define MC_MATCH_STAGE_LIST_START				70222	///< 스테이지 목록 전송시작 요청
#define MC_MATCH_STAGE_LIST_STOP				70147	///< 스테이지 목록 전송중지 요청
#define MC_MATCH_STAGE_LIST						70103	///< 스테이지 목록
#define MC_MATCH_STAGE_CHAT						70259	///< 스테이지 대화
#define MC_MATCH_STAGE_REQUEST_QUICKJOIN		70345	///< 퀵조인 요청
#define MC_MATCH_STAGE_RESPONSE_QUICKJOIN		70081	///< 퀵조인 응답
#define MC_MATCH_STAGE_GO						70260	///< 스테이지 번호로 참가
#define MC_MATCH_STAGE_REQUIRE_PASSWORD			70325	///< 비밀방이라 패스워드가 필요하다.(S -> C)


#define MC_MATCH_STAGE_REQUEST_ENTERBATTLE		70163	///< 스테이지 전투 참가한다고 요청
#define MC_MATCH_STAGE_ENTERBATTLE				70055	///< 스테이지 전투참가
#define MC_MATCH_STAGE_LEAVEBATTLE_TO_SERVER	70108	///< 스테이지 전투탈퇴(C -> S)
#define MC_MATCH_STAGE_LEAVEBATTLE_TO_CLIENT	70072	///< 스테이지 전투탈퇴(S -> C)

#define MC_MATCH_REQUEST_STAGESETTING			70360	///< 스테이지의 설정을 요청
#define MC_MATCH_RESPONSE_STAGESETTING			70135	///< 스테이지의 설정을 알림
#define MC_MATCH_STAGESETTING					70274	///< 스테이지 설정
#define MC_MATCH_STAGE_MAP						70185	///< 스테이지 맵 설정
#define MC_MATCH_STAGE_REQUEST_FORCED_ENTRY		70305	///< 스테이지에서 게임에 난입 요청
#define MC_MATCH_STAGE_RESPONSE_FORCED_ENTRY	70045	///< 스테이지 게임 난입 응답
#define MC_MATCH_STAGE_RELAY_MAP_ELEMENT_UPDATE	70257	///< 스테이지 릴레이맵 요소(타입, 회차) 업데이트
#define MC_MATCH_STAGE_RELAY_MAP_INFO_UPDATE	70079	///< 스테이지 릴레이맵 리스트 업데이트

#define MC_MATCH_STAGE_MASTER					70039	///< 스테이지 방장 알림
#define MC_MATCH_STAGE_PLAYER_STATE				70023	///< 스테이지 레디 등의 플레이어 상태 정보
#define MC_MATCH_STAGE_TEAM						70050	///< 스테이지 팀
#define MC_MATCH_STAGE_START					70068	///< 스테이지 시작(카운트다운)
#define MC_MATCH_STAGE_LAUNCH					70204	///< 스테이지 실행
#define MC_MATCH_STAGE_RELAY_LAUNCH				70217	///< 릴레이된 스테이지 실행

#define MC_MATCH_LOADING_COMPLETE				70004	///< 로딩이 끝났음
#define MC_MATCH_STAGE_FINISH_GAME				70152	///< 스테이지 게임 종료

#define MC_MATCH_REQUEST_GAME_INFO				70018	///< 게임안 정보를 달라 요청
#define MC_MATCH_RESPONSE_GAME_INFO				70186	///< 게임안 정보를 알려준다

#define MC_MATCH_REQUEST_PEERLIST				70101	///< 참여중인 유저들의 Peer정보를 요청
#define MC_MATCH_RESPONSE_PEERLIST				70131	///< 참여중인 유저들의 Peer정보를 알림

#define MC_MATCH_REQUEST_PEER_RELAY				70252	///< 메치서버에 해당Peer에 대한 중계요청
#define MC_MATCH_RESPONSE_PEER_RELAY			70142	///< 해당Peer에 대한 중계허가 통보

// 게임 관련
#define MC_MATCH_GAME_ROUNDSTATE				70158	///< 라운드의 상태를 동기화
#define MC_MATCH_ROUND_FINISHINFO				70125	///< 라운드 종료시 정보 업데이트 (C <- S)
#define MC_MATCH_GAME_KILL						70020	///< 캐릭터 사망 (C -> S)
#define MC_MATCH_GAME_DEAD						70123	///< 캐릭터 죽었다 (S -> C)
#define MC_MATCH_GAME_LEVEL_UP					70092	///< 게임중 레벨업
#define MC_MATCH_GAME_LEVEL_DOWN				70266	///< 게임중 레벨다운
#define MC_MATCH_GAME_REQUEST_SPAWN				70140	///< 캐릭터 부활요청 (C -> S)
#define MC_MATCH_GAME_RESPONSE_SPAWN			70207	///< 캐릭터 부활요청 (C -> S)
#define MC_MATCH_GAME_TEAMBONUS					70267	///< 팀 보너스(S -> C)

#define MC_MATCH_GAME_REQUEST_TIMESYNC			70109	///< Match서버에 시간동기화 요청
#define MC_MATCH_GAME_RESPONSE_TIMESYNC			70238	///< 시간동기화
#define MC_MATCH_GAME_REPORT_TIMESYNC			70077	///< 시간동기 검사

#define MC_MATCH_REQUEST_SUICIDE				70155	///< 자살 요청
#define MC_MATCH_RESPONSE_SUICIDE				70027	///< 자살 응답
#define MC_MATCH_RESPONSE_SUICIDE_RESERVE		70088	///< 자살 예약.
#define MC_MATCH_REQUEST_OBTAIN_WORLDITEM		70114	///< 아이템 먹도록 요청
#define MC_MATCH_OBTAIN_WORLDITEM				70353	///< 아이템 먹음 브로드캐스트(S -> C)
#define MC_MATCH_SPAWN_WORLDITEM				70177	///< 아이템이 스폰됨
#define MC_MATCH_REQUEST_SPAWN_WORLDITEM		70359	///< 아이템을 떨구도록 요청(C -> S)
#define MC_MATCH_REMOVE_WORLDITEM				70104	///< 아이템 없어짐(S -> C)
#define MC_MATCH_ASSIGN_COMMANDER				70156	///< 암살전 대장으로 임명
#define MC_MATCH_RESET_TEAM_MEMBERS				70299	///< 팀멤버 다시 세팅
#define MC_MATCH_SET_OBSERVER					70041	///< 강제로 옵저버모드로 전환


// 동의관련
#define MC_MATCH_REQUEST_PROPOSAL				70182	///< 다른 사람의 동의 요청
#define MC_MATCH_RESPONSE_PROPOSAL				70192	///< 동의 요청 응답
#define MC_MATCH_ASK_AGREEMENT					70060	///< 다른 사람에게 동의 질의(S -> C)
#define MC_MATCH_REPLY_AGREEMENT				70223	///< 동의 응답(C -> S)

// 래더 관련
#define MC_MATCH_LADDER_REQUEST_CHALLENGE		70037	///< 래더 도전 요청(C -> S)
#define MC_MATCH_LADDER_RESPONSE_CHALLENGE		70029	///< 래더 도전 응답
#define MC_MATCH_LADDER_SEARCH_RIVAL			70330	///< 래더 검색중		
#define MC_MATCH_LADDER_REQUEST_CANCEL_CHALLENGE	70129	///< 래더 도전 취소요청
#define MC_MATCH_LADDER_CANCEL_CHALLENGE		70136	///< 래더 도전 취소
#define MC_MATCH_LADDER_PREPARE					70196	///< 래더 시작준비
#define MC_MATCH_LADDER_LAUNCH					70272	///< 래더 런치 (S -> C)

// 의사소통 관련
#define MC_MATCH_USER_WHISPER					70134	///< 귓속말
#define MC_MATCH_USER_WHERE						70078	///< 위치확인
#define MC_MATCH_USER_OPTION					70087	///< 각종옵션(귓말거부,초대거부,친구거부 등등)
#define MC_MATCH_CHATROOM_CREATE				70262	///< 채팅룸 개설
#define MC_MATCH_CHATROOM_JOIN					70019	///< 채팅룸 참가
#define MC_MATCH_CHATROOM_LEAVE					70280	///< 채팅룸 탈퇴
#define MC_MATCH_CHATROOM_INVITE				70251	///< 채팅룸 초대
#define MC_MATCH_CHATROOM_CHAT					70013	///< 채팅
#define MC_MATCH_CHATROOM_SELECT_WRITE			70242	///< 채팅룸 선택

// 캐릭터 관련
#define MC_MATCH_REQUEST_ACCOUNT_CHARLIST		70116	///< 계정의 전체 캐릭터 리스트를 요청
#define MC_MATCH_RESPONSE_ACCOUNT_CHARLIST		70311	///< 계정의 전체 캐릭터 리스트를 응답
#define MC_MATCH_REQUEST_SELECT_CHAR			70340	///< 캐릭터 선택 요청
#define MC_MATCH_RESPONSE_SELECT_CHAR			70138	///< 캐릭터 선택 응답
#define MC_MATCH_REQUEST_MYCHARINFO				70017	///< 내 캐릭터 정보 요청
#define MC_MATCH_RESPONSE_MYCHARINFO			70105	///< 내 캐릭터 정보 응답
#define MC_MATCH_REQUEST_CREATE_CHAR			70296	///< 내 캐릭터 생성 요청
#define MC_MATCH_RESPONSE_CREATE_CHAR			70293	///< 내 캐릭터 생성 응답
#define MC_MATCH_REQUEST_DELETE_CHAR			70308	///< 내 캐릭터 삭제 요청
#define MC_MATCH_RESPONSE_DELETE_CHAR			70126	///< 내 캐릭터 삭제 응답
#define MC_MATCH_REQUEST_COPY_TO_TESTSERVER		70130	///< 캐릭터 테스트서버로 전송 요청 - 미구현
#define MC_MATCH_RESPONSE_COPY_TO_TESTSERVER	70093	///< 캐릭터 테스트서버로 전송 응답 - 미구현
#define MC_MATCH_REQUEST_CHARINFO_DETAIL		70346	///< 다른 플레이어 정보보기 등의 자세한 플레이어 정보 요청
#define MC_MATCH_RESPONSE_CHARINFO_DETAIL		70040	///< 다른 플레이어 정보보기 등의 자세한 플레이어 정보 응답
#define MC_MATCH_REQUEST_ACCOUNT_CHARINFO		70095	///< 계정의 한 캐릭터 정보 요청
#define MC_MATCH_RESPONSE_ACCOUNT_CHARINFO		70228	///< 계정의 한 캐릭터 정보 응답

// 아이템 관련
#define MC_MATCH_REQUEST_SIMPLE_CHARINFO		70154	///< 캐릭터의 간단한 정보 요청	
#define MC_MATCH_RESPONSE_SIMPLE_CHARINFO		70082	///< 캐릭터의 간단한 정보 응답
#define MC_MATCH_REQUEST_MY_SIMPLE_CHARINFO		70166	///< 내 캐릭터 XP, BP등 정보 요청
#define MC_MATCH_RESPONSE_MY_SIMPLE_CHARINFO	70281	///< 내 캐릭터 XP, BP등 정보 응답

#define MC_MATCH_REQUEST_BUY_ITEM				70232	///< 아이템 구매 요청
#define MC_MATCH_RESPONSE_BUY_ITEM				70189	///< 아이템 구매 응답
#define MC_MATCH_REQUEST_SELL_ITEM				70118	///< 아이템 판매 요청
#define MC_MATCH_RESPONSE_SELL_ITEM				70225	///< 아이템 판매 응답
#define MC_MATCH_REQUEST_SHOP_ITEMLIST			70315	///< 샵에서 판매하는 아이템 리스트 요청
#define MC_MATCH_RESPONSE_SHOP_ITEMLIST			70337	///< 샵에서 판매하는 아이템 리스트 응답

#define MC_MATCH_REQUEST_CHARACTER_ITEMLIST		70164	///< 내 아이템 리스트 주시오
#define MC_MATCH_RESPONSE_CHARACTER_ITEMLIST	70167	///< 아이템 리스트 여깄오
#define MC_MATCH_REQUEST_EQUIP_ITEM				70314	///< 아이템 장비 요청
#define MC_MATCH_RESPONSE_EQUIP_ITEM			70069	///< 아이템 장비 응답
#define MC_MATCH_REQUEST_TAKEOFF_ITEM			70255	///< 아이템 해제 요청
#define MC_MATCH_RESPONSE_TAKEOFF_ITEM			70351	///< 아이템 해제 응답
#define MC_RESPONSE_GAMBLEITEMLIST				70120	///< DB의 겜블 아이템 정보 전송.
#define MC_MATCH_ROUTE_UPDATE_STAGE_EQUIP_LOOK	70028	///< 캐릭터의 Look만들 업데이트할때 사용한다.
#define MC_MATCH_REQUEST_CHARACTER_ITEMLIST_FORCE	70297	///< 내 아이템 리스트 주시오 : 중복 요청도 무시하지 않는 버전(주의해서 사용할것)

#define MC_MATCH_REQUEST_ACCOUNT_ITEMLIST		70022	///< 내 창고 아이템 리스트 주시오
#define MC_MATCH_RESPONSE_ACCOUNT_ITEMLIST		70239	///< 창고 아이템 리스트 여깄오
#define MC_MATCH_REQUEST_BRING_ACCOUNTITEM		70327	///< 창고 아이템을 내 캐릭터로 가져오기
#define MC_MATCH_RESPONSE_BRING_ACCOUNTITEM		70188	///< 창고 아이템 가져오기 응답
#define MC_MATCH_REQUEST_BRING_BACK_ACCOUNTITEM	70246	///< 내 캐쉬아이템 창고로 옮기기 요청
#define MC_MATCH_RESPONSE_BRING_BACK_ACCOUNTITEM	70144	///< 내 캐쉬아이템 창고로 옮기기 응답
#define MC_MATCH_EXPIRED_RENT_ITEM				70112	///< 아이템이 기간 만료되었습니다.(S -> C)

#define MC_MATCH_REQUEST_GAMBLE					70233	///< 뽑기 (C -> S)
#define MC_MATCH_RESPONSE_GAMBLE				70115	///< 뽑기 결과 (S -> C)

// 친구 관련
#define MC_MATCH_FRIEND_ADD						70211	///< 친구 추가
#define MC_MATCH_FRIEND_REMOVE					70363	///< 친구 삭제
#define MC_MATCH_FRIEND_LIST					70058	///< 친구 목록 요청
#define MC_MATCH_RESPONSE_FRIENDLIST			70226	///< 친구 목록 전송
#define MC_MATCH_FRIEND_MSG						70107	///< 친구 채팅


// 클랜관련
#define MC_MATCH_CLAN_REQUEST_CREATE_CLAN			70145	///< 클랜 생성 요청(C -> S)
#define MC_MATCH_CLAN_RESPONSE_CREATE_CLAN			70032	///< 클랜 생성 응답(S -> C)
#define MC_MATCH_CLAN_ASK_SPONSOR_AGREEMENT			70236	///< 발기인에게 클랜 생성 동의 질의(S -> C)
#define MC_MATCH_CLAN_ANSWER_SPONSOR_AGREEMENT		70178	///< 발기인이 동의를 응답(C -> S)
#define MC_MATCH_CLAN_REQUEST_AGREED_CREATE_CLAN	70284	///< 발기인이 동의한 클랜 생성 요청(C -> S)
#define MC_MATCH_CLAN_RESPONSE_AGREED_CREATE_CLAN	70057	///< 발기인이 동의한 클랜 생성 응답(S -> C)
#define MC_MATCH_CLAN_REQUEST_CLOSE_CLAN			70289	///< 마스터가 클랜 폐쇄 요청(C -> S)
#define MC_MATCH_CLAN_RESPONSE_CLOSE_CLAN			70323	///< 마스터의 클랜 폐쇄 응답(S -> C)
#define MC_MATCH_CLAN_REQUEST_JOIN_CLAN				70153	///< 운영자가 가입자의 가입을 요청(C -> S)
#define MC_MATCH_CLAN_RESPONSE_JOIN_CLAN			70265	///< 운영자의 가입자 가입의 응답(S -> C)
#define MC_MATCH_CLAN_ASK_JOIN_AGREEMENT			70237	///< 가입자에게 가입 동의 질의(S -> C)
#define MC_MATCH_CLAN_ANSWER_JOIN_AGREEMENT			70031	///< 가입자의 가입 동의 응답(C -> S)
#define MC_MATCH_CLAN_REQUEST_AGREED_JOIN_CLAN		70203	///< 가입자가 동의한 클랜 가입 요청(C -> S)
#define MC_MATCH_CLAN_RESPONSE_AGREED_JOIN_CLAN		70071	///< 가입자가 동의한 클랜 가입 응답(S -> C)
#define MC_MATCH_CLAN_REQUEST_LEAVE_CLAN			70352	///< 클랜원이 탈퇴를 요청(C -> S)
#define MC_MATCH_CLAN_RESPONSE_LEAVE_CLAN			70016	///< 클랜원의 탈퇴의 응답(S -> C)
#define MC_MATCH_CLAN_UPDATE_CHAR_CLANINFO			70210	///< 캐릭터의 클랜정보가 업데이트됨(S -> C)
#define MC_MATCH_CLAN_MASTER_REQUEST_CHANGE_GRADE	70256	///< 멤버의 권한 변경 요청(C -> S)
#define MC_MATCH_CLAN_MASTER_RESPONSE_CHANGE_GRADE	70170	///< 멤버의 권한 변경 응답(S -> C)
#define MC_MATCH_CLAN_ADMIN_REQUEST_EXPEL_MEMBER	70324	///< 멤버를 탈퇴 요청(C -> S)
#define MC_MATCH_CLAN_ADMIN_RESPONSE_EXPEL_MEMBER	70047	///< 멤버를 탈퇴 응답(S -> C)
#define MC_MATCH_CLAN_REQUEST_MSG					70268	///< 클랜 채팅 요청(C -> S)
#define MC_MATCH_CLAN_MSG							70119	///< 클랜 채팅(S -> C)
#define MC_MATCH_CLAN_REQUEST_MEMBER_LIST			70122	///< 클랜 멤버 리스트 요청(C -> S)
#define MC_MATCH_CLAN_RESPONSE_MEMBER_LIST			70283	///< 클랜 멤버 리스트 응답(S -> C)
#define MC_MATCH_CLAN_REQUEST_CLAN_INFO				70320	///< 클랜 정보 요청(C -> S)
#define MC_MATCH_CLAN_RESPONSE_CLAN_INFO			70044	///< 클랜 정보 응답(S -> C)
#define MC_MATCH_CLAN_STANDBY_CLAN_LIST				70298	///< 클랜전 대기중인 클랜 리스트 응답(S -> C)
#define MC_MATCH_CLAN_MEMBER_CONNECTED				70194	///< 클랜원이 접속함(S -> C)
#define MC_MATCH_CLAN_REQUEST_EMBLEMURL				70312	///< 클랜마크 URL 요청(C -> S)
#define MC_MATCH_CLAN_RESPONSE_EMBLEMURL			70098	///< 클랜마크 URL 응답(S -> C)
#define MC_MATCH_CLAN_LOCAL_EMBLEMREADY				70286	///< 클랜마크 다운로드 완료(C -> C)
#define MC_MATCH_CLAN_ACCOUNCE_DELETE				70002	///< 클랜 폐쇄 요청 접수 정보 공지.(S -> C)


// 투표관련
#define MC_MATCH_CALLVOTE							70317	///< 투표를 제안한다. (C -> S)
#define MC_MATCH_NOTIFY_CALLVOTE					70214	///< 투표제안을 알린다. (S -> C)
#define MC_MATCH_NOTIFY_VOTERESULT					70279	///< 투표결과를 알린다. (S -> C)
#define MC_MATCH_VOTE_YES							70038	///< 투표 (C -> S)
#define MC_MATCH_VOTE_NO							70347	///< 투표 (C -> S)
#define MC_MATCH_VOTE_RESPONSE						70240	///< 투표 예외.(S -> C)
#define MC_MATCH_VOTE_STOP							70269	///< 투표 취소. (S -> C)

// 방송관련
#define MC_MATCH_BROADCAST_CLAN_RENEW_VICTORIES			70292	///< 클랜이 계속 연승중입니다.(S -> C)
#define MC_MATCH_BROADCAST_CLAN_INTERRUPT_VICTORIES		70319	///< 클랜의 연승을 저지하였습니다.(S -> C)
#define MC_MATCH_BROADCAST_DUEL_RENEW_VICTORIES			70159	///< xx님이 xx채널의 xx방에서 x연승중입니다.(S -> C)
#define MC_MATCH_BROADCAST_DUEL_INTERRUPT_VICTORIES		70143	///< xx님이 xx님의 xx연승을 저지했습니다. (S -> C)


// Duel Tournament 관련.. Added by 홍기주
#define MC_MATCH_DUELTOURNAMENT_REQUEST_JOINGAME				70334	///< 듀얼 토너먼트의 참가 신청(C -> S)
#define MC_MATCH_DUELTOURNAMENT_RESPONSE_JOINGAME				70220	///< 듀얼 토너먼트의 참가 신청에 대한 응답(S -> C)
#define MC_MATCH_DUELTOURNAMENT_REQUEST_CANCELGAME				70224	///< 듀얼 토너먼트의 참가 신청 취소(C -> S)
#define MC_MATCH_DUELTOURNAMENT_CHAR_INFO						70235	///< 듀얼 토너먼트의 케릭터 정보(S -> C)
#define MC_MATCH_DUELTOURNAMENT_PREPARE_MATCH					70200	///< 듀얼 토너먼트 시작 전
#define MC_MATCH_DUELTOURNAMENT_LAUNCH_MATCH					70277	///< 듀얼 토너먼트 시작! (S - > C)
#define MC_MATCH_DUELTOURNAMENT_CANCEL_MATCH					70046	///< 듀얼 토너먼트 취소! (S - > C)
#define MC_MATCH_DUELTOURNAMENT_CHAR_INFO_PREVIOUS				70106	///< 듀얼 토너먼트 캐릭터 지난주 정보(S -> C)
#define MC_MATCH_DUELTOURNAMENT_NOT_SERVICE_TIME				70074	///< 현재 듀얼토너먼트 서비스 시간이 아닙니다.(오픈시간 %s%d시00분 ~ %s%d시59분) (S - > C)

#define MC_MATCH_DUELTOURNAMENT_REQUEST_SIDERANKING_INFO		70229	///< 케릭터 랭킹의 근처 랭킹 정보 요청(C -> S)
#define MC_MATCH_DUELTOURNAMENT_RESPONSE_SIDERANKING_INFO		70187	///< 케릭터 랭킹의 근처 랭킹 정보 응답(S -> C)
#define MC_MATCH_DUELTOURNAMENT_GROUPRANKING_INFO				70276	///< 듀얼 토너먼트의 그룹 랭킹 정보(S -> C)

#define MC_MATCH_DUELTOURNAMENT_GAME_INFO						70270	///< 듀얼 토너먼트 게임 내에서의 정보
#define MC_MATCH_DUELTOURNAMENT_GAME_ROUND_RESULT_INFO			70183	///< 듀얼 토너먼트 게임 내에서의 라운드 승패 결과 정보
#define MC_MATCH_DUELTOURNAMENT_GAME_MATCH_RESULT_INFO			70212	///< 듀얼 토너먼트 게임 내에서의 매치 승패 결과 정보
#define MC_MATCH_DUELTOURNAMENT_GAME_PLAYER_STATUS				70051	///< 듀얼 토너먼트 게임 내에서의 라운드가 종료될 때, 해당 게이머의 상태를 서버로 보냄(C -> S)
#define MC_MATCH_DUELTOURNAMENT_GAME_NEXT_MATCH_PLYAERINFO		70181	///< 듀얼 토너먼트 게임 내에서의 다음 매치업의 플레이어 정보

// 게임 룰 관련
// 버서커 모드
#define MC_MATCH_ASSIGN_BERSERKER					70306	///< 버서커 임명(S -> C)
#define MC_MATCH_DUEL_QUEUEINFO						70179	///< 대기열 순번 알림(S -> C)

// 퀘스트 관련 명령어(서바이벌 모드, 퀘스트 모드 공통)
#define MC_QUEST_NPC_SPAWN					70190	///< NPC 스폰(S -> C)
#define MC_QUEST_ENTRUST_NPC_CONTROL		70165	///< NPC의 조종을 피어에게 위임(S -> C)
#define MC_QUEST_CHECKSUM_NPCINFO			70026	///< NPC정보들의 체크섬을 서버에게 보낸다(C -> S)
#define MC_QUEST_REQUEST_NPC_DEAD			70067	///< NPC 죽음 요청(C -> S)
#define MC_QUEST_NPC_DEAD					70193	///< NPC 죽음(S -> C)
#define MC_QUEST_REFRESH_PLAYER_STATUS		70066	///< 죽었으면 다시 태어나고, 모든 상태를 최상으로 복귀(S -> C)
#define MC_QUEST_NPC_ALL_CLEAR				70148	///< NPC 모두 삭제(S -> C)
#define MC_MATCH_QUEST_REQUEST_DEAD			70219	///< 플레이어 죽었다고 요청(C -> S) 퀘스트모드에서는 MC_MATCH_GAME_KILL대신 보낸다.
#define MC_MATCH_QUEST_PLAYER_DEAD			70036	///< 플레이어 죽음(S -> C)
#define MC_QUEST_OBTAIN_QUESTITEM			70318	///< 퀘스트 아이템 획득(S -> C)
#define MC_QUEST_STAGE_MAPSET				70216	///< 스테이지 맵셋 변경(S <-> C)
#define MC_QUEST_OBTAIN_ZITEM				70282	///< 일반 아이템 획득(S -> C)
#define MC_QUEST_PING						70273	///< 서버->클라 핑 (S -> C)
#define MC_QUEST_PONG						70012	///< 클라->서버 핑 (C -> S)
#define MC_QUEST_NPCLIST					70349	///< 현제 퀘스트에서 사용할 NPC의 정보.(S->C) 
///<  클라는 서버의 정보를 가지고 NPC를 초기화 한다.


#define MC_QUEST_PEER_NPC_BASICINFO			70133	///< 정기적으로 업데이트되는 NPC 기본정보
#define MC_QUEST_PEER_NPC_HPINFO			70275	///< 정기적으로 업데이트되는 NPC 체력정보
#define MC_QUEST_PEER_NPC_ATTACK_MELEE		70149	///< 근접 공격
#define MC_QUEST_PEER_NPC_ATTACK_RANGE		70358	///< 원거리 공격
#define MC_QUEST_PEER_NPC_SKILL_START		70062	///< 스킬 캐스팅 시작
#define MC_QUEST_PEER_NPC_SKILL_EXECUTE		70084	///< 스킬 캐스팅성공 효과 시작
#define MC_QUEST_PEER_NPC_DEAD				70328	///< 죽은 그 시점에 피어들에게 보낸다. 
#define MC_QUEST_PEER_NPC_BOSS_HPAP			70295	///< 정기적으로 업데이트되는 NPC 보스 체력 아머

// 퀘스트 게임 진행 관련(퀘스트 모드)
#define MC_QUEST_GAME_INFO					70059	///< 게임 정보. 로딩할 리소스 등을 알림(S -> C)
#define MC_QUEST_COMBAT_STATE				70075	///< 퀘스트 게임내 상태 정보(S -> C)
#define MC_QUEST_SECTOR_START				70034	///< 맵섹터 게임 시작(S -> C)
#define MC_QUEST_COMPLETED					70263	///< 해당 퀘스트 클리어(S -> C)
#define MC_QUEST_FAILED						70302	///< 퀘스트 실패(S -> C)

#define MC_QUEST_REQUEST_MOVETO_PORTAL		70285	///< 포탈로 이동함(C -> S)
#define MC_QUEST_MOVETO_PORTAL				70309	///< 포탈로 이동(S -> C)
#define MC_QUEST_READYTO_NEWSECTOR			70162	///< 새로운 섹터 준비 완료(S <-> C)

#define MC_GAME_START_FAIL					70168	///< 퀘스트 시작을 실패.(S -> C)
#define MC_QUEST_STAGE_GAME_INFO			70198	///< 대기중 스테이지 정보 알림(QL, 시나리오ID). (S -> C)
#define MC_QUEST_SECTOR_BONUS				70161	///< 섹터 보너스(S -> C)

// 퀘스트 게임 진행 관련(서바이벌 모드)
#define MC_QUEST_ROUND_START				70091	///< 새로운 라운드 시작(S -> C)
#define MC_QUEST_REQUEST_QL					70248	///< 현재 퀘스트 스테이지의 QL정보를 요청.(C -> S)
#define MC_QUEST_RESPONSE_QL				70174	///< 현재 퀘스트 스테이지의 QL정보를 알림.(S -> C)
#define MC_QUEST_SURVIVAL_RESULT			70070	///< 서바이벌 결과 통보(S -> C)
#define MC_SURVIVAL_RANKINGLIST				70291	///< 서바이벌 상위 랭킹 목록 전송(S -> C)
#define MC_SURVIVAL_PRIVATERANKING			70341	///< 유저의 서바이벌 랭킹 정보 전송(S -> C)


// 퀘스트 테스트용
#define MC_QUEST_TEST_REQUEST_NPC_SPAWN		70350	///< NPC 스폰 부탁(C -> S)
#define MC_QUEST_TEST_REQUEST_CLEAR_NPC		70195	///< NPC Clear 부탁(C -> S)
#define MC_QUEST_TEST_REQUEST_SECTOR_CLEAR	70357	///< 섹터 Clear(C -> S)	
#define MC_QUEST_TEST_REQUEST_FINISH		70278	///< 퀘스트 Complete(C -> S)


// MatchServer Schedule관련.
#define MC_MATCH_SCHEDULE_ANNOUNCE_MAKE				70085	///< 스케쥴 공지 생성.
#define MC_MATCH_SCHEDULE_ANNOUNCE_SEND				70090	///< 스케쥴 공지 전송.
#define MC_MATCH_SCHEDULE_CLAN_SERVER_SWITCH_DOWN	70008	///< 클랜서버 클랜전 비활성화.
#define MC_MATCH_SCHEDULE_CLAN_SERVER_SWITCH_ON		70202	///< 클랜서버 클랜전 활성화.
#define MC_MATCH_SCHEDULE_STOP_SERVER				70063	///< 서버 종료.


// x-trap
#define MC_REQUEST_XTRAP_HASHVALUE					70030	///< 이전과의 호환으로 존재만(사용하지 않는다.)
#define MC_RESPONSE_XTRAP_HASHVALUE					70137	///< 이전과의 호환으로 존재만(사용하지 않는다.)
#define MC_REQUEST_XTRAP_SEEDKEY					70111	///< 암호키 요청(S -> C)
#define MC_RESPONSE_XTRAP_SEEDKEY					70102	///< 암호키 응답(C -> S)
#define MC_REQUEST_XTRAP_DETECTCRACK				70042	///< 이전과의 호환으로 존재만(사용하지 않는다.)

// gameguard
#define MC_REQUEST_GAMEGUARD_AUTH					70258
#define MC_RESPONSE_GAMEGUARD_AUTH					70338
#define MC_REQUEST_FIRST_GAMEGUARD_AUTH				70249
#define MC_RESPONSE_FIRST_GAMEGUARD_AUTH			70244

// 자수.
#define MC_REQUEST_GIVE_ONESELF_UP					70342	///< 자수하다.

//// 피어투피어 관련 명령어 /////////////////////////////////////////////////////////////
#define MC_PEER_PING				70173	///< peer들 사이의 network latency(ping)을 측정하기위한 command
#define MC_PEER_PONG				70124	///< ping에 대한 응답
#define MC_PEER_UDPTEST				70230	///< Peer to Peer UDP 전송이 가능한지 검사한다.
#define MC_PEER_UDPTEST_REPLY		70099	///< Peer to Peer UDP 전송이 가능한지 검사한다.

#define MC_UDP_PING					70117	///< Client와 Agent들 사이의 ping을 측정하기위한 command
#define MC_UDP_PONG					70080	///< ping에 대한 응답

#define MC_PEER_OPENED				70169	///< 개별 Peer 통신 개통알림
#define MC_PEER_BASICINFO			70054	///< 정기적으로 업데이트되는 캐릭터 기본정보 투표판정의 근거가 된다
#define MC_PEER_HPINFO				70025	///< 정기적으로 업데이트되는 캐릭터 체력정보 투표 판정의 근거가 된다
#define MC_PEER_HPAPINFO			70215	///< 정기적으로 업데이트되는 캐릭터 체력/아머 정보
#define MC_PEER_DUELTOURNAMENT_HPAPINFO		70065	///< 정기적으로 업데이트되는 듀얼토너먼트 전용 캐릭터 체력/아머 정보

#define MC_PEER_CHANGECHARACTER		70209
#define MC_PEER_MOVE				70035   ///< 에 나중에 MC_OBJECT_MOVE로 바뀌어야할듯..
#define MC_PEER_CHANGE_WEAPON		70271   
#define MC_PEER_CHANGE_PARTS		70061
#define MC_PEER_ATTACK				70331
#define MC_PEER_DAMAGE				70321
#define MC_PEER_RELOAD				70234
#define MC_PEER_SHOT				70333
#define MC_PEER_SHOT_SP				70005
#define MC_PEER_SKILL				70048
#define MC_PEER_SHOT_MELEE			70316	///< 근접공격
#define MC_PEER_DIE					70294
#define MC_PEER_SPAWN				70110
#define MC_PEER_DASH				70007
#define MC_PEER_SPMOTION			70094
#define MC_PEER_CHAT				70132
#define MC_PEER_CHAT_ICON			70356	///< 채팅 시작/끝. 머리에 아이콘을 보여주거나 감춘다.
#define MC_PEER_REACTION			70201	///< 이펙트를 위해 알려준다 
#define MC_PEER_ENCHANT_DAMAGE		70009	///< 인챈트 데미지를 맞았다

// 퀘스트 아이템 관련 부분.
#define MC_MATCH_REQUEST_CHAR_QUEST_ITEM_LIST		70355	///< 나의 퀘스트 아이템 목록을 요청.(C -> S)
#define MC_MATCH_RESPONSE_CHAR_QUEST_ITEM_LIST		70015	///< 나의 퀘스트 아이템 목록을 전송.(S -> C)
#define MC_MATCH_REQUEST_BUY_QUEST_ITEM				70139	///< 퀘스트 아이템 구입 요청.(C -> S)
#define MC_MATCH_RESPONSE_BUY_QUEST_ITEM			70218	///< 퀘스트 아이템 구입 완료.(S -> C)
#define MC_MATCH_REQUEST_SELL_QUEST_ITEM			70064	///< 나의 퀘스트 아이템 판매 요청.(C -> S)
#define MC_MATCH_RESPONSE_SELL_QUEST_ITEM			70361	///< 나의 퀘스트 아이템 판매 완료.(S -> C)
#define MC_MATCH_USER_REWARD_QUEST					70245	///< 퀘스트가 완료된후 보상된 퀘스트 아이템 정보.
#define MC_MATCH_REQUEST_DROP_SACRIFICE_ITEM		70141	///< 희생 아이템을 슬롯에 놓기 요청.(C -> S)
#define MC_MATCH_RESPONSE_DROP_SACRIFICE_ITEM		70172	///< 희생 아이템을 슬롯에 놓기 응담.(S -> C)
#define MC_MATCH_REQUEST_CALLBACK_SACRIFICE_ITEM	70089	///< 슬롯에 올려놓은 희생 아이템을 회수 요청.(C -> S)
#define MC_MATCH_RESPONSE_CALLBACK_SACRIFICE_ITEM	70335	///< 스롯에 올려놓은 희생 아이템을 휘수 응답.(S -> C)
#define MC_MATCH_REQUEST_SLOT_INFO					70003	///< 희생 아이템 슬롯 정보를 요청.(C -> S)
#define MC_MATCH_RESPONSE_SLOT_INFO					70288	///< 희생 아이템 슬롯 정보를 알림.(S -> C)
#define MC_MATCH_NEW_MONSTER_INFO					70010	///< 몬스터 도감에 추가될 새로운 몬스터 정보.
#define MC_MATCH_REQUEST_MONSTER_BIBLE_INFO			70254	///< 몬스터 도감의 내용 요청.(C -> S)
#define MC_MATCH_RESPONSE_MONSTER_BIBLE_INFO		70001	///< 몬스터 도감의 내용 알림.(S -> C)

//[[SHUFFLE_END]]		<< 이 주석문은 ID 섞어주는 프로그램에서 인식하는 마크이다






// 매치 에이젼트 관련 명령어
#define MC_MATCH_REGISTERAGENT				5001	///< 에이젼트 등록
#define MC_MATCH_UNREGISTERAGENT			5002	///< 에이젼트 해지
#define MC_MATCH_AGENT_REQUEST_LIVECHECK	5011	///< 에이젼트 확인요청
#define MC_MATCH_AGENT_RESPONSE_LIVECHECK	5012	///< 에이젼트 확인응답
#define MC_AGENT_ERROR						5013	///< 에이전트 관련 에러

#define MC_AGENT_CONNECT					5021	///< 매치서버에 에이젼트 서버 등록
#define MC_AGENT_DISCONNECT					5022	///< 매치서버에 에이젼트 서버 등록해지
#define MC_AGENT_LOCAL_LOGIN				5023	///< 클라이언트 접속처리
#define MC_AGENT_RESPONSE_LOGIN				5024	///< 클라이언트에 로그인 통지
//#define MC_AGENT_MATCH_VALIDATE				5031	///< 매치서버와의 접속확인
#define MC_AGENT_STAGE_RESERVE				5051	///< 에이전트에 스테이지 배정
#define MC_AGENT_STAGE_RELEASE				5052	///< 에이전트에 스테이지 해지
#define MC_AGENT_STAGE_READY				5053	///< 에이전트에 스테이지 준비완료
#define MC_AGENT_LOCATETO_CLIENT			5061	///< 클라이언트에게 Agent 알림
#define MC_AGENT_RELAY_PEER					5062	///< 에이전트에 중계지시
#define MC_AGENT_PEER_READY					5063	///< 메치서버에 피어준비 알림
#define MC_AGENT_PEER_BINDTCP				5071	///< 클라이언트 Bind TCP
#define MC_AGENT_PEER_BINDUDP				5072	///< 클라이언트 Bind UDP
#define MC_AGENT_PEER_UNBIND				5073	///< 클라이언트 Unbind

#define MC_AGENT_TUNNELING_TCP				5081	///< TCP 터널링
#define MC_AGENT_TUNNELING_UDP				5082	///< UDP 터널링
#define MC_AGENT_ALLOW_TUNNELING_TCP		5083	///< TCP 터널링 허용
#define MC_AGENT_ALLOW_TUNNELING_UDP		5084	///< UDP 터널링 허용
#define MC_AGENT_DEBUGPING					5101	///< 디버깅용
#define MC_AGENT_DEBUGTEST					5102	///< 디버깅용


#define MC_REQUEST_RESOURCE_CRC32				11001
#define MC_RESPONSE_RESOURCE_CRC32				12002

/// Keeper와 Keeper Manager와 통신하는데 필요한 명령. 2005.06.03 커맨드 정리 필요.
#define MC_KEEPER_MANAGER_CONNECT				31001	///< keeper Manager에서 keeper로 접속 요청.
#define MC_CHECK_KEEPER_MANAGER_PING			31002	///< Keeper와 Keeper Manager가 정상적으로 연결되어 있는지 검사.
#define MC_REQUEST_MATCHSERVER_STATUS			31004	///< MatchServer의 정보를 요청.(Keeper -> MatchServer)
#define MC_RESPONSE_MATCHSERVER_STATUS			31005	///< Keeper로 MatchServer의 정보를 전송해 줌.(MatchServer -> Keeper)
// 2005.06.01 ~
#define MC_RESPONSE_KEEPER_MANAGER_CONNECT		31006	///< Keeper manager의 접속이 정상적으로 이루어 짐을 알림.( Keeper -> Keeper Manager )
#define MC_REQUEST_CONNECTION_STATE				31007	///< 현제 서버의 상태.(Keeper Manager -> Keeper)
#define MC_RESPONSE_CONNECTION_STATE			31008	///< 서버상태 응답.(Keeper -> Keeper Manager)
#define MC_REQUEST_SERVER_HEARBEAT				31009	///< 서버가 동작하고 있는지 응답 요청.(Keeper -> MatchServer)
#define MC_RESPONSE_SERVER_HEARHEAT				31010	///< 서버가 동작하고 있을을 응답.(MatchServer -> Keeper)
#define MC_REQUEST_KEEPER_CONNECT_MATCHSERVER	31011	///< Keeper가 MatchServer로 접속하도록 요청함(Keeper Manager -> Keeper -> MatchServer )
#define MC_RESPONSE_KEEPER_CONNECT_MATCHSERVER	31012	///< Keeper가 MatchServer에 접속했다는 응답.(MatchServer -> Keeper -> Keeper Manager )
#define MC_REQUEST_REFRESH_SERVER				31013	///< Keeper Manager의 서버 리스트 수동 갱신을 위해서 Keeper로 MatchServer의 Heartbaet채크 요청.(Keeper Manager -> Keeepr)
#define MC_REQUEST_LAST_JOB_STATE				31014	///< 마지막 실행된 일의 결과를 요청.(Keeper -> Keeper Manager )
#define MC_RESPONSE_LAST_JOB_STATE				31015	///< 작업 결과 요청 응답.(Keeper -> Keeper Manager)
#define MC_REQUEST_CONFIG_STATE					31016	///< Keeper에 저장된 설정 정보 요청.(Keeper Manager -> Keeper)
#define MC_RESPONSE_CONFIG_STATE				31017	///< Keeper에 저장된 설정 정보 요청 응답.(Keeper -> Keeper Manager)
#define MC_REQUEST_SET_ONE_CONFIG				31018	///< Config하나 설정 요청.(Keeper Manager -> Keeper)
#define MC_RESPONSE_SET_ONE_CONFIG				31019	///< Config하나 설정 요청 응답.(Keeper -> Keeper Manager)
#define MC_REQUEST_KEEPERMGR_ANNOUNCE			31020	///< Keeper Manager에서 각 서버로 공지를 보내도록 Keeper에 요청함.(Keeper Manager -> Keeper)
#define MC_REQUEST_KEEPER_ANNOUNCE				31021	///< Keeper에서 MatchServer가 공지사항을 알리도록 요청함.(Keeper -> MatchServer)
#define MC_REQUEST_RESET_PATCH					31022	///< 현제+까지 진행한 패치 작업상태를 초기화함.
#define MC_REQUEST_DISCONNECT_SERVER			31023	///< Keeper와 MatchServer와의 연결 종료.
#define MC_REQUEST_REBOOT_WINDOWS				31024	///< 윈도우 재부팅.(Keeper Manager -> Keeper)
#define MC_REQUEST_ANNOUNCE_STOP_SERVER			31025	///< 공지사항과 함게 서버를 종료 시킴 요청.
#define MC_RESPONSE_ANNOUNCE_STOP_SERVER		31026	///< 공지사항과 함게 서버를 종료 시킴 요청 응답.
#define MC_REQUEST_SERVER_AGENT_STATE			31027	///< 현제 서버와 에이전트의 실행 상태 요청.
#define MC_RESPONSE_SERVER_AGENT_STATE			31028	///< 현제 서버와 에이전트의 실행 상태 요청 응답.
#define MC_REQUEST_WRITE_CLIENT_CRC				31029	///< 클라이언트의 CRC CheckSum을 서버 ini파일에 저장 요청.(KeeperManager -> Keeper)
#define MC_RESPONSE_WRITE_CLIENT_CRC			31030	///< 클라이언트의 CRC CheckSum을 서버 ini파일에 저장 요청 응답.(Keeper -> Keeper Manager)
#define MC_REQUEST_KEEPER_RELOAD_SERVER_CONFIG	31031	///< Keeper와 연결되어있는 서버가 설정 파일을 다시 로드하도록 요청함.(KeeperManager -> Keeper)
#define MC_REQUEST_RELOAD_CONFIG				31032	///< 서버의 설정 파일을 다시 로다하도록 요청.(Keeper -> server)
#define MC_REQUEST_KEEPER_ADD_HASHMAP			31033	
#define MC_RESPONSE_KEEPER_ADD_HASHMAP			31034
#define MC_REQUEST_ADD_HASHMAP					31035
#define MC_RESPONSE_ADD_HASHMAP					31036

// MatchServer 패치 관련.
#define MC_REQUEST_DOWNLOAD_SERVER_PATCH_FILE	32024	///< Keeper가 FTP서버에서 Patch파일을 다운로드 시킴.(Keeper Manager -> Keeper )
#define MC_REQUEST_START_SERVER					32026	///< MatchServer실행 요청.(Keeper Manager -> Keeper)
#define MC_REQUEST_STOP_SERVER					32028	///< 서버 정지 요청.(Keeper -> Keeper Manager)
#define MC_REQUEST_PREPARE_SERVER_PATCH			32030	///< 패치 준비 작업.(압축 풀기, 임시 파일 제거.)
#define MC_REQUEST_SERVER_PATCH					32032	///< 패치 파일을 복사함.(Keeper Manager -> Keeper )
// MatchAgent 패치 관련.
#define MC_REQUEST_STOP_AGENT_SERVER			33034	///< Agent server정지 요청.(Keeper Manager -> Keeper )
#define MC_REQUEST_START_AGENT_SERVER			33036	///< Agent server시작 요청.(Keeper Manager -> Keeper)
#define MC_REQUEST_DOWNLOAD_AGENT_PATCH_FILE	33038	///< request agent patch file download.(Keeper Manager -> Keeper)
#define MC_REQUEST_PREPARE_AGENT_PATCH			33040	///< reqeust prepare agent patch.(Keeper Manager -> Keeper)
#define MC_REQUEST_AGENT_PATCH					33042	///< request agent patch.(Keeper Manager -> Keeper)
#define MC_REQUEST_SERVER_STATUS				33043	///< server와 agent의 정보를 Keeper로 요청함.(Keeper Manager -> keeper)
#define MC_RESPONSE_SERVER_STATUS				33044	///< server와 agetn의 정보 요청 응답.(Keeper -> Keeper Manager)
// Keeper manager schedule.
#define MC_REQUEST_KEEPER_MANAGER_SCHEDULE		34001	///< request keeper manager schedule.(Keeper Manager -> Keeper -> MatchServer)
#define MC_RESPONSE_KEEPER_MANAGER_SCHEDULE		34002	///< Keeper Manager의 스케쥴등록 요청 응답.
#define MC_REQUEST_START_SERVER_SCHEDULE		34003	///< 다시시작 스케줄에서 서버를 시작하는 시케줄. 시작할때까지 스케줄이 제거되지 않음.
///


/// Locator관련.
#define MC_REQUEST_SERVER_LIST_INFO				40001	/// 접속가능한 서버의 리스트 정보 요청.(Client -> Locator)
#define MC_RESPONSE_SERVER_LIST_INFO			40002	/// 접속가능한 서버 리스트 정보 응답.(Locator -> Client)
#define MC_RESPONSE_BLOCK_COUNTRY_CODE_IP		40003	/// 접속불가 국가 코드의 IP응답.(Locator -> Client)


// filter.
#define MC_RESPONSE_BLOCK_COUNTRYCODE			50001	/// 블럭된 국가코드의 IP가 접속시 통보용.
#define MC_LOCAL_UPDATE_USE_COUNTRY_FILTER		50002
#define MC_LOCAL_GET_DB_IP_TO_COUNTRY			50003
#define MC_LOCAL_GET_DB_BLOCK_COUNTRY_CODE		50004
#define MC_LOCAL_GET_DB_CUSTOM_IP				50005
#define MC_LOCAL_UPDATE_IP_TO_COUNTRY			50006
#define MC_LOCAL_UPDATE_BLOCK_COUTRYCODE		50007
#define MC_LOCAL_UPDATE_CUSTOM_IP				50008
#define MC_LOCAL_UPDATE_ACCEPT_INVALID_IP		50009

#define MC_TEST_BIRDTEST1					60001	///< 버드 테스트1
#define MC_TEST_PEERTEST_PING				60002	///< Target Peer Test
#define MC_TEST_PEERTEST_PONG				60003	///< Target Peer Test


/// 커맨드 세부 옵션

// MC_MATCH_CHANNEL_REQUEST_ALL_PLAYER_LIST의 세부옵션
#define MCP_MATCH_CHANNEL_REQUEST_ALL_PLAYER_LIST_NORMAL		0		
#define MCP_MATCH_CHANNEL_REQUEST_ALL_PLAYER_LIST_NONCLAN		1
#define MCP_MATCH_CHANNEL_REQUEST_ALL_PLAYER_LIST_MYCLAN		2


#endif