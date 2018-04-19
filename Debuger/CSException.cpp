#include "StdAfx.h"
#include "CSException.h"


CCSException::CCSException(void)
{
}

CCSException::CCSException( char *msg ):m_msg(msg)
{

}

CCSException::CCSException( const ExceptType t ):m_msg("")
{

}


CCSException::~CCSException(void)
{
}

void CCSException::Throw()
{
	USES_CONVERSION;
	AfxMessageBox(A2T(m_msg.StrVal()));
}

void CCSException::AppendMsg( CCSVar msg )
{
	m_msg += msg;
}

CCSVar CCSException::GetMsg() const
{
	return m_msg;
}
