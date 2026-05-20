/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Samsung Exynos9610 clock IDs for U-Boot.
 *
 * Only the IDs U-Boot needs during uboot1st bring-up: OSCCLK, the
 * UART clock chain, and the two UART0 IP gates. The kernel binding
 * covers the full SoC clock tree and lives upstream.
 */

#ifndef _DT_BINDINGS_CLOCK_EXYNOS9610_CMU_H
#define _DT_BINDINGS_CLOCK_EXYNOS9610_CMU_H

/* CMU_TOP outputs */
#define CLK_EXYNOS9610_OSCCLK			1
#define CLK_EXYNOS9610_DOUT_PERI_UART		10
#define CLK_EXYNOS9610_DOUT_PERI_UART_USER	11

/* Per-IP gates */
#define CLK_EXYNOS9610_GOUT_PERI_UART0_PCLK	20
#define CLK_EXYNOS9610_GOUT_PERI_UART0_UCLK	21

#endif /* _DT_BINDINGS_CLOCK_EXYNOS9610_CMU_H */
