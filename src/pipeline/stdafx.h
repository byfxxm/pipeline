﻿#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <thread>
#include <vector>
#include <functional>
#include <cassert>
#include <future>

#define PIPELINE_EXPORTS
namespace pipeline {}
using namespace pipeline;