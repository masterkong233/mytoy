#include <iostream>
#include <curl/curl.h>
// #include "zlib.h"
#include <vector>
#include <string>
// #include <unistd.h>
#include <io.h>
#include <process.h>
#include <memory.h>
// #include <json/json.h>
#include <sstream>
#include <cstddef>
#include "cqsdk/cqsdk.h"
#include "cqsdk/target.h"
// using namespace std;

namespace app = cq::app; // 插件本身的生命周期事件和管理
namespace event = cq::event; // 用于注册 QQ 相关的事件处理函数
namespace api = cq::api; // 用于调用酷 Q 提供的接口
namespace logging = cq::logging; // 用于日志
namespace message = cq::message; // 提供封装了的 Message 等类

class CommonTools{
    private:
        CommonTools(){};
        // static CommonTools* getInstance();
        CommonTools(const CommonTools&);
	    CommonTools& operator=(const CommonTools&);
        int event;
        static CommonTools* instance;
        static cq::PrivateMessageEvent m_event;
    public:
        static CommonTools* getInstance(){
            return instance;
        };
        ~CommonTools(){};
    public:
        static size_t receive_data(void *contents, size_t size, size_t nmemb, void *stream);
        // HTTP 下载文件的回掉函数
        // static size_t writedata2file(void *ptr, size_t size, size_t nmemb, FILE *stream);
        // 文件下载接口
        // static int download_file(const char* url, const char outfilename[FILENAME_MAX]);
        // http get 请求
        // static CURLcode HttpGet(const std::string & strUrl, std::string & strResponse,int nTimeout);
        // htpp post 请求
        // static CURLcode HttpPost(const std::string & strUrl, std::string szJson,std::string & strResponse,int nTimeout);
        int setEvent(cq::PrivateMessageEvent e);
        cq::PrivateMessageEvent getEvent();
        
};
