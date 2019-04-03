#include "cqsdk/cqsdk.h"
#include <string>
#include "include/curl/curl.h"
#include <iostream>
#include <fstream>
#include "curltools/ctools.h"
using namespace std;

namespace app = cq::app; // 插件本身的生命周期事件和管理
namespace event = cq::event; // 用于注册 QQ 相关的事件处理函数
namespace api = cq::api; // 用于调用酷 Q 提供的接口
namespace logging = cq::logging; // 用于日志
namespace message = cq::message; // 提供封装了的 Message 等类
// using namespace std;

// 初始化 App Id
CQ_INITIALIZE("com.masterkongapp.cookiezi");

// 插件入口，在静态成员初始化之后，app::on_initialize 事件发生之前被执行，用于配置 SDK 和注册事件回调
CQ_MAIN {
    cq::config.convert_unicode_emoji = true; // 配置 SDK 自动转换 Emoji 到 Unicode（默认就是 true）

    app::on_enable = [] {
        // logging、api、dir 等命名空间下的函数只能在事件回调函数内部调用，而不能直接在 CQ_MAIN 中调用
        logging::debug(u8"启用", u8"插件已启动");
    };

    event::on_private_msg = [](const cq::PrivateMessageEvent &e) {
        logging::debug(u8"消息", u8"收到私聊消息：" + e.message + u8"，发送者：" + to_string(e.user_id));

        try {
            api::send_private_msg(e.user_id, e.message); // echo 回去

            api::send_msg(e.target, e.message); // 使用 e.target 指定发送目标

            const auto code = app::auth_code;
            cq::Message msg1 = u8"code:" + to_string(code);
            msg1.send(e.target);
            // const auto cook = api::get_cookies();
            // api::send_msg(e.target, cook);
            // auto info = api::get_group_member_info(637377281,1345578556);
            // api::send_msg(e.target,info.nickname);

            // auto ginfo = api::get_group_list();
            // for(vector<cq::Group>::size_type i = 0; ginfo.size(); i++)
            // {
            //    api::send_msg(e.target,(ginfo[i].group_name));
            // }


            
            CURL *pCURL = nullptr;
            pCURL = curl_easy_init();
            CURLcode res;
            string res_buf = "ok";
            if (pCURL != nullptr){
                auto co = CommonTools::getInstance();
                co->setEvent(e);
                curl_easy_setopt(pCURL, CURLOPT_URL, "http://www.baidu.com");
                // curl_easy_setopt(pCURL, CURLOPT_POSTFIELDS, POSTFIELDS);
                curl_easy_setopt(pCURL, CURLOPT_POST, 1); 
            //     curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3); // set transport and time out time
		    //     curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
                curl_easy_setopt(pCURL, CURLOPT_NOSIGNAL, 1L);
                curl_easy_setopt(pCURL, CURLOPT_TIMEOUT, 2);
                curl_easy_setopt(pCURL, CURLOPT_NOPROGRESS, 1L);
                curl_easy_setopt(pCURL,CURLOPT_WRITEFUNCTION,  CommonTools::getInstance()->receive_data);
                curl_easy_setopt(pCURL,CURLOPT_WRITEDATA,(void*)&res_buf);
                // curl_easy_setopt(pCURL, CURLOPT_FOLLOWLOCATION, 1L);
                res = curl_easy_perform(pCURL);
                if (res != CURLE_OK) {
                    cq::Message msgbegin = u8"curl test failed!" + to_string(res);
                    api::send_msg(e.target,msgbegin);
                }else{
                    cq::Message msgover = u8"curl test success!";
                    api::send_msg(e.target,msgover);
                }
                
            }
            curl_easy_cleanup(pCURL);

            

            // MessageSegment 类提供一些静态成员函数以快速构造消息段
            // cq::Message msg = cq::MessageSegment::contact(cq::MessageSegment::ContactType::USER, e.user_id);
            // msg.send(e.target); // 使用 Message 类的 send 成员函数
        } catch (const cq::exception::ApiError &err) {
            // API 调用失败
            logging::debug(u8"API", u8"调用失败，错误码：" + to_string(err.code));
        }

        e.block(); // 阻止事件继续传递给其它插件
    };

    event::on_group_msg = [](const auto &e /* 使用 C++ 的 auto 关键字 */) {
        // const auto memlist = api::get_group_member_list(e.group_id); // 获取数据接口
        // cq::Message msg = u8"本群一共有 "; // string 到 Message 自动转换
        // msg += to_string(memlist.size()) + u8" 个成员"; // Message 类可以进行加法运算
        // message::send(e.target, msg); // 使用 message 命名空间的 send 函数

        //获取e.group_id,获取自己的群列表api::get_group_list(),遍历并通过id获取群name
    };

    event::on_group_member_increase = [](const auto &e) {
        // const auto from_qq = e.operator_id;
        // const auto be_operator_qq = e.user_id;
        const auto memlist = api::get_group_member_list(e.group_id); // 获取数据接口
        cq::Message msg = u8"本群一共有 "; // string 到 Message 自动转换
        msg += to_string(memlist.size()) + u8" 个成员"; // Message 类可以进行加法运算
        message::send(e.target, msg); // 使用 message 命名空间的 send 函数

        if (2 == e.sub_type)
        {
            cq::Message msg1 = cq::MessageSegment::at(e.operator_id) + u8"与" + cq::MessageSegment::at(e.user_id) + u8"进行了py交易";
            message::send(e.target, msg1);
        }else{
            cq::Message msg1 = cq::MessageSegment::at(e.operator_id) + u8"新纳了一位后宫" + cq::MessageSegment::at(e.user_id);
            message::send(e.target, msg1); 
        }
        
        cq::Message msg2 = cq::MessageSegment::at(e.user_id);
        cq::Message msg3 = u8"欢迎来到花果山";
        msg2 = msg2 + msg3;
        
        message::send(e.target, msg2);
    };

    event::on_group_member_decrease = [](const auto &e) {
        const auto memlist = api::get_group_member_list(e.group_id); // 获取数据接口
        cq::Message msg = u8"本群一共有 "; // string 到 Message 自动转换
        msg += to_string(memlist.size()) + u8" 个成员"; // Message 类可以进行加法运算
        message::send(e.target, msg); // 使用 message 命名空间的 send 函数

        cq::Message msg1 = u8"";
        switch (e.sub_type)
        {
            case 1/* 1/群员离开 */:
                msg1 += to_string(e.user_id) + u8"伤心欲绝，离开了花果山";
                break;
            case 2: // 2/群员被踢
                msg1 += to_string(e.user_id) + u8"boy♂next♂door";
                break;
            case 3: // 3/自己(即登录号)被踢
                break;
            default:
                msg1 += to_string(e.user_id) + u8"已经离开本群";
                break;
        }
        msg1.send(e.target);
    };
}

// 添加菜单项，需要同时在 <appid>.json 文件的 menu 字段添加相应的条目，function 字段为 menu_demo_1
CQ_MENU(menu_demo_1) {
    logging::info(u8"菜单", u8"点击了示例菜单1");
    try {
        api::send_private_msg(10000, "hello");
    } catch (const cq::exception::ApiError &) {
        logging::warning(u8"菜单", u8"发送失败");
    }
}

// 不像 CQ_INITIALIZE 和 CQ_MAIN，CQ_MENU 可以多次调用来添加多个菜单
CQ_MENU(menu_demo_2) {
    logging::info(u8"菜单", u8"点击了示例菜单2");
    cq::config.convert_unicode_emoji = !cq::config.convert_unicode_emoji;
}
