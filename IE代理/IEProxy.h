#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <string>


#ifndef IEPROXY_H
#define IEPROXY_H

#include <Wininet.h>
#pragma comment(lib, "WinInet.lib")


namespace ie_proxy {

	typedef struct ProxyConfig {

		ProxyConfig() : auto_detect_settings(false) {};

		std::wstring auto_config_url;
		bool auto_detect_settings;

		std::wstring proxy_server;
		std::wstring bypass_domain_list;

	} ProxyConfig;


	bool setProxyConfig(ProxyConfig *config){

		bool result;

		INTERNET_PER_CONN_OPTION options[] = {
			{ INTERNET_PER_CONN_FLAGS, 0 },
			{ INTERNET_PER_CONN_AUTOCONFIG_URL, 0 },
			{ INTERNET_PER_CONN_PROXY_SERVER, 0 },
			{ INTERNET_PER_CONN_PROXY_BYPASS, 0 },
		};


		INTERNET_PER_CONN_OPTION_LIST list;

		int list_size = sizeof(list);

		list.dwSize = list_size;
		list.pszConnection = NULL;
		list.dwOptionCount = 4;
		list.dwOptionError = 0;
		list.pOptions = options;//new INTERNET_PER_CONN_OPTION[3];

		// pointer to options
		//INTERNET_PER_CONN_OPTION *options = list.pOptions;

		// setting INTERNET_PER_CONN_FLAGS
		options[0].Value.dwValue = PROXY_TYPE_DIRECT;

		if (!config->auto_config_url.empty())
		{
			// append INTERNET_PER_CONN_AUTOCONFIG_URL to INTERNET_PER_CONN_FLAGS 
			options[0].Value.dwValue |= PROXY_TYPE_AUTO_PROXY_URL;

			// pszValue takes in LPTSTR 
			options[1].Value.pszValue = (TCHAR*)config->auto_config_url.c_str();
		}

		if (config->auto_detect_settings)
		{
			// setting INTERNET_PER_CONN_FLAGS
			options[0].Value.dwValue |= PROXY_TYPE_AUTO_DETECT;
		}

		if (!config->proxy_server.empty())
		{
			// setting INTERNET_PER_CONN_FLAGS
			options[0].Value.dwValue |= PROXY_TYPE_PROXY;

			// setting INTERNET_PER_CONN_PROXY_SERVER
			options[2].Value.pszValue = (TCHAR*)config->proxy_server.c_str();
		}

		if (!config->bypass_domain_list.empty())
		{
			// INTERNET_PER_CONN_PROXY_BYPASS
			options[3].Value.pszValue = (TCHAR*)config->bypass_domain_list.c_str();
		}

		result = InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &list, list_size);
		//delete[] list.pOptions;

		return result;
	}

	bool getProxyConfig(ProxyConfig *config){

		INTERNET_PER_CONN_OPTION options[] = {
			{ INTERNET_PER_CONN_FLAGS, 0 },
			{ INTERNET_PER_CONN_AUTOCONFIG_URL, 0 },
			{ INTERNET_PER_CONN_PROXY_SERVER, 0 },
			{ INTERNET_PER_CONN_PROXY_BYPASS, 0 },
		};

		INTERNET_PER_CONN_OPTION_LIST list;

		DWORD size = sizeof(INTERNET_PER_CONN_OPTION_LIST);

		list.dwSize = size;
		list.pszConnection = NULL;
		list.dwOptionCount = 4;
		list.dwOptionError = 0;
		list.pOptions = options;

		if (InternetQueryOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &list, &size))
		{
			DWORD flags = list.pOptions[0].Value.dwValue;

			if (flags & PROXY_TYPE_AUTO_DETECT)
			{
				config->auto_detect_settings = true;
			}

			if (flags & INTERNET_PER_CONN_AUTOCONFIG_URL)
			{
				config->auto_config_url = std::wstring(list.pOptions[1].Value.pszValue);
			}

			if (flags & INTERNET_PER_CONN_PROXY_SERVER)
			{
				config->proxy_server = list.pOptions[2].Value.pszValue;
			}

			if (flags & INTERNET_PER_CONN_PROXY_BYPASS)
			{
				wchar_t *url_list = list.pOptions[3].Value.pszValue;

				if (url_list != NULL)
				{
					config->bypass_domain_list = std::wstring(url_list);
				}
			}

			return true;
		}

		return false;
	}

	bool refreshSettings(){

		InternetSetOption(0, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
		InternetSetOption(0, INTERNET_OPTION_REFRESH, NULL, 0);

		return true;
	}

}

#endif