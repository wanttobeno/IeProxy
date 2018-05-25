
#ifndef PROXYSETTING_H__
#define PROXYSETTING_H__ 1
#include <tchar.h>
namespace Proxy
{

	bool QueryProxy();

	void SetProxy(bool enableproxy,TCHAR *pProxyServer);
}



#endif  //  PROXYSETTING_H__


