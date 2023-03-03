//
// Created by liuhy on 2023/2/24.
//

#ifndef GSM_UTIL_H
#define GSM_UTIL_H

#define CITY_DISTANCE 300
#define VALLEY_DISTANCE 1000
#define HIGHWAY_DISTANCE 5000

extern vector<mobile *> mobileInfo;
extern vector<fake *> fakeInfo;
// 根据time获取时间戳
string getTimeStamp(double time , mobile m,tm* t);
// 读取移动终端信息
vector<mobile *> readMobileInfo();
vector<fake *> readFakeInfo();
vector<fake *> fakeFilter(vector<fake *> f,mobile m);
// util初始化
string initUtil();
void showChoice();
#endif //GSM_UTIL_H
