#ifdef PSCPDLL_EXPORTS
#define PSCPDLL_API __declspec(dllexport)
#else
#define PSCPDLL_API __declspec(dllimport)
#endif

typedef int (*callback_exec)(void* param);

PSCPDLL_API int scp(const char *szIp, const char *szUser, const char *szPwd, 
	const char *szSource, const char *szDes , callback_exec func);

PSCPDLL_API int sun_test(int num1, int num2);
