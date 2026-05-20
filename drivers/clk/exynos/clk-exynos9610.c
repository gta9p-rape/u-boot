// SPDX-License-Identifier: GPL-2.0-only
/*
 * Samsung Exynos9610 clock driver.
 *
 * A CMU_TOP provider that replaces the fixed-clock stubs the uboot1st DTS
 * was using. S-Boot/EPBL leaves the PLL tree, muxes and dividers configured
 * before we run, so this driver only reports stable rates that DM-aware
 * drivers (UART today, UFS/USB later) can bind against.
 *
 * No MUX/DIV register reads yet. Add them when a clock here needs to vary
 * at runtime.
 */

#include <asm/io.h>
#include <dm.h>
#include <dt-bindings/clock/samsung,exynos9610-cmu.h>
#include <linux/clk-provider.h>

#include "clk.h"

enum exynos9610_cmu_id {
	CMU_TOP,
};

#define EXYNOS9610_OSCCLK_RATE		26000000

/*
 * Rates the previous-stage bootloader leaves us with on gta4xl. UART0 baud
 * clock and PCLK ride directly on OSCCLK (matches CONFIG_DEBUG_UART_CLOCK=
 * 26000000 in the defconfig). Treat them as fixed for now.
 */
static const struct samsung_fixed_rate_clock top_frate_clks[] = {
	FRATE(CLK_EXYNOS9610_OSCCLK,			"clock-oscclk",
	      EXYNOS9610_OSCCLK_RATE),
	FRATE(CLK_EXYNOS9610_DOUT_PERI_UART,		"dout_peri_uart",
	      EXYNOS9610_OSCCLK_RATE),
	FRATE(CLK_EXYNOS9610_DOUT_PERI_UART_USER,	"dout_peri_uart_user",
	      EXYNOS9610_OSCCLK_RATE),
	FRATE(CLK_EXYNOS9610_GOUT_PERI_UART0_PCLK,	"gout_peri_uart0_pclk",
	      EXYNOS9610_OSCCLK_RATE),
	FRATE(CLK_EXYNOS9610_GOUT_PERI_UART0_UCLK,	"gout_peri_uart0_uclk",
	      EXYNOS9610_OSCCLK_RATE),
};

static const struct samsung_clk_group top_cmu_clks[] = {
	{ S_CLK_FRATE, top_frate_clks, ARRAY_SIZE(top_frate_clks) },
};

static int exynos9610_cmu_top_probe(struct udevice *dev)
{
	return samsung_register_cmu(dev, CMU_TOP, top_cmu_clks,
				    exynos9610_cmu_top);
}

static const struct udevice_id exynos9610_cmu_top_ids[] = {
	{ .compatible = "samsung,exynos9610-cmu-top" },
	{ }
};

SAMSUNG_CLK_OPS(exynos9610_cmu_top, CMU_TOP);

U_BOOT_DRIVER(exynos9610_cmu_top) = {
	.name		= "exynos9610-cmu-top",
	.id		= UCLASS_CLK,
	.of_match	= exynos9610_cmu_top_ids,
	.ops		= &exynos9610_cmu_top_clk_ops,
	.probe		= exynos9610_cmu_top_probe,
	.flags		= DM_FLAG_PRE_RELOC,
};
