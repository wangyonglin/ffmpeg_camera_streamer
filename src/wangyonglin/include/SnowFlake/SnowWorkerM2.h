/*
 * 版权属于：yitter(yitter@126.com)
 * 代码翻译：amuluowin
 * 代码修订：yitter
 * 开源地址：https://gitee.com/yitter/idgenerator
 */
#pragma once

#include <stdlib.h>

#include <SnowFlake/SnowWorkerM1.h>
#include <SnowFlake/IdGenOptions.h>
extern uint64_t WorkerM2NextId(SnowFlakeWorker *worker);
