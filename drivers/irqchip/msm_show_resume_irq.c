// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2011, 2014-2016, 2018, The Linux Foundation. All rights reserved.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

/*Huaqin added for DRSTRANGE-639 by wuhongwen at 2020/07/08 start */
int msm_show_resume_irq_mask = 0x01;
/*Huaqin added for DRSTRANGE-639 by wuhongwen at 2020/07/08 end */
module_param_named(
	debug_mask, msm_show_resume_irq_mask, int, 0664);
