#ifndef SHARED_NET
#define SHARED_NET
#define ERROR(condition, reason, err_code) do {if (condition) {printf ("Error:%s, line:%d\n",reason, __LINE__); return err_code;} }while(false);
#endif // SHARED_NET