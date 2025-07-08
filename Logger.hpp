#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <cstdio>
#include <cstdarg>
#include <mutex>
#include <windows.h>

namespace Logger {

    enum class LogLevel {
        Info,
        Error
    };

    //使用互斥锁防止多线程调用出现异常
    inline std::mutex log_mutex;

   /**
   * HANDLE GetStdHandle(DWORD nStdHandle) 获取标准输入、输出或错误设备的句柄（屏幕缓冲区句柄）
   * 
   *   nStdHandle 标准设备类型，可以是以下值：
   *   - STD_INPUT_HANDLE   (DWORD)-10  // 标准输入（键盘）的句柄
   *   - STD_OUTPUT_HANDLE  (DWORD)-11  // 标准输出（控制台屏幕）的句柄
   *   - STD_ERROR_HANDLE   (DWORD)-12  // 标准错误（错误信息输出）的句柄
   *
   * 成功返回设备句柄（HANDLE），失败返回 INVALID_HANDLE_VALUE
   * 
   * Windows 控制台颜色 (用于 SetConsoleTextAttribute)
   *
   * 颜色采用 4-bit 编码，格式为 0xXY：
   *   - X (高4位): 背景色
   *   - Y (低4位): 前景色(文字颜色)
   *
   * 基础颜色值 (可组合使用):
   *   0x0: 黑色       0x1: 深蓝色
   *   0x2: 深绿色     0x3: 深青色
   *   0x4: 深红色     0x5: 深紫色
   *   0x6: 深黄色     0x7: 灰色(默认)
   *   0x8: 深灰色     0x9: 蓝色
   *   0xA: 绿色       0xB: 青色
   *   0xC: 红色       0xD: 紫色
   *   0xE: 黄色       0xF: 白色
   *
   * 常用组合示例：
   *   0x07: 灰字黑底(默认)    0x70: 黑字灰底(反色)
   *   0x0F: 白字黑底         0xF0: 黑字白底
   *   0x04: 红字黑底         0x40: 黑字红底
   *   0x0A: 绿字黑底         0xA0: 黑字绿底
   *   0x0E: 黄字黑底         0xE0: 黑字黄底
   *
   * 特殊组合：
   *   - 添加 FOREGROUND_INTENSITY(0x08) 使前景色变亮
   *   - 添加 BACKGROUND_INTENSITY(0x80) 使背景色变亮
   *   例如：0x0C 是深红色，0x0F 是亮白色

   */


    inline void Print(LogLevel level, const char* fmt, va_list args) {
        std::lock_guard<std::mutex> lock(log_mutex);//这里加锁防止线程冲突
        switch (level) {
        case LogLevel::Info:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x7);//设置为默认灰色
            printf("[");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x2);//设置为绿色
            printf("Info");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x7);//设置为默认灰色
            printf("] ");
            break;
        case LogLevel::Error:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x7);//设置为默认灰色
            printf("[");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x4);//设置为红色
            printf("Error");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x7);//设置为默认灰色
            printf("] ");
            break;
        default:
            printf("[Log] ");
            break;
        }
        vprintf(fmt, args);
        printf("\n");
    }

    inline void Info(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        Print(LogLevel::Info, fmt, args);
        va_end(args);
    }

    inline void Error(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        Print(LogLevel::Error, fmt, args);
        va_end(args);
    }

} // namespace Logger

#endif // LOGGER_HPP
