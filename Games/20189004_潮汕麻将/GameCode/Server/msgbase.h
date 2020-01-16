#ifndef MSG_BASE
#define MSG_BASE

#include <fstream>
#include <map>
#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
#include <string>
using namespace std;
#pragma pack( 1 )

struct MsgHead
{
	int		_msgID;
	int		_msgLen;
	int		_stationLen;
	__int64	_time;
	
	MsgHead( )
	{
		memset( this, 0, sizeof( MsgHead ));
	}
};

struct UserInfor
{
	char		szNickName[61];			//�ǳ�
	char		szheadUrl[256];			//ͷ���ַ
	UINT		nLogoID;				//ͷ��ID
	__int64		i64UserMoney;			//��ҽ��
	bool		bBoy;					///�Ա�
	long        uId ;
};

struct RecordUserInfo
{

	BYTE		bWatchDeskView;						//�ۿ��������Ϊ

	char		szDeskPassWord[20];					//��������

	bool		bVIPRoom;							//�Ƿ�VIP����
	bool		bBuyRoom;							//�Ƿ�ɹ��򷿼�

	bool		bHasPassword;						//�Ƿ�������

	int			nPlayCount;							//�����Ŀ

	UINT		nComType;							//��Ϸ����
	UINT		nKindID;							//��Ϸ����ID
	UINT		nNameID;							//��Ϸ����ID
	UINT		dwRoomRule;							//��Ϸ�������

	int			nBasePoint;							//��������

	int			iVipGameCount;						//�������Ӿ���

	int			iRunGameCount;						//��ǰ���о���

	int			iPayType;							//֧����ʽ

	int			iClubID;							//���ֲ�ID

	bool		bfree;								//�Ƿ����

	UserInfor	userinfo[PLAY_COUNT];

	RecordUserInfo( )
	{
		memset( this, 0, sizeof( RecordUserInfo ));
	}
};

#pragma pack( )

/*
*\brief�ļ�������, �������ļ���������Ϣ����ȡ��Ϣ
*/
class zNewFileOperator
{
private:
	FILE*				m_file;					//�ļ���
	char*				m_fileName;				//�ļ���
public:
	zNewFileOperator( )
	{
		m_file = NULL;
	}
	virtual ~zNewFileOperator( )
	{

	}
	/*
	*\brief �����ļ���
	*para fileName�����ļ�������
	*/
	void	setFileName( char *fileName )
	{
		if ( m_fileName != NULL)
		{
			free( m_fileName );
			m_fileName = NULL;
		}

		int iLen			= strlen( fileName );
		m_fileName		= ( char * )malloc( ( iLen + 1 ) * sizeof( char ) );
		memcpy( m_fileName , fileName, iLen * sizeof( char ));
		m_fileName[iLen]	= '\0';
	}
	/*
	*\brief�����ļ�����֤�ļ���פ�ڴ�
	*para write ��дģʽ
	*/
	bool	createFile( bool write = true )
	{
		char Mode[2];
		if ( write )
		{
			sprintf( Mode, "wb" );
		}
		else
		{
			sprintf( Mode, "rb" );
		}
		m_file = fopen( m_fileName, Mode );
		return m_file != NULL;
	}
	/*
	*\brief���ļ���������Ϣ��
	*para property ��Ҫ��ӵ���Ϣ��
	*/
	void	appendProperty( int nMsgId, void *buffer, int nLen, void *stationBuffer, int nStationLen )
	{
		if ( m_file == NULL )
		{
			return ;
		}
		MsgHead head;
		head._msgID			= nMsgId;
		head._time			= time( NULL );
		head._msgLen		= nLen;
		head._stationLen	= nStationLen;
		fwrite( &head, sizeof( head ), 1, m_file ); // д1���ṹ
		if ( nStationLen )
		{
			fwrite( stationBuffer, nStationLen, 1, m_file ); // д1���ṹ
		}
		fwrite( buffer, nLen, 1, m_file ); // д1���ṹ
		//fflush( file );
	}

	void CloseFile( )
	{
		if ( m_file != NULL )
		{
			fclose( m_file );
			m_file = NULL;
		}
	}

	void RenameFile( char *_newFileName )
	{
		rename( m_fileName, _newFileName );
	}
	void RemoveFile( )
	{
		remove( m_fileName );
	}
	void CutAndReName( char* _newFileName )
	{
		RenameFile( _newFileName );
		RemoveFile( );
	}
};

#endif