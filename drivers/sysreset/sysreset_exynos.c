// SPDX-License-Identifier: GPL-2.0+
/*
 * Samsung Exynos PMU sysreset driver.
 *
 * Used on Exynos 9610/9611-class parts where the boot chain doesn't expose
 * PSCI: write 1 to the PMU SWRESET register (typically 0x11860000 + 0x400)
 * and the SoC takes a full-system warm reset.
 */

#include <asm/io.h>
#include <dm.h>
#include <errno.h>
#include <linux/err.h>
#include <regmap.h>
#include <syscon.h>
#include <sysreset.h>

#define EXYNOS_DEFAULT_SWRESET_OFFSET	0x0400

struct exynos_sysreset_priv {
	struct regmap *pmu;
	u32 offset;
};

static int exynos_sysreset_request(struct udevice *dev, enum sysreset_t type)
{
	struct exynos_sysreset_priv *priv = dev_get_priv(dev);

	regmap_write(priv->pmu, priv->offset, 0x1);

	/* SWRESET is fire-and-forget; if we return, the write didn't take. */
	return -EINPROGRESS;
}

static int exynos_sysreset_probe(struct udevice *dev)
{
	struct exynos_sysreset_priv *priv = dev_get_priv(dev);

	priv->pmu = syscon_regmap_lookup_by_phandle(dev, "samsung,pmureg");
	if (IS_ERR(priv->pmu))
		return PTR_ERR(priv->pmu);

	priv->offset = dev_read_u32_default(dev, "offset",
					    EXYNOS_DEFAULT_SWRESET_OFFSET);

	return 0;
}

static struct sysreset_ops exynos_sysreset_ops = {
	.request = exynos_sysreset_request,
};

static const struct udevice_id exynos_sysreset_ids[] = {
	{ .compatible = "samsung,exynos-sysreset" },
	{ }
};

U_BOOT_DRIVER(exynos_sysreset) = {
	.name		= "exynos_sysreset",
	.id		= UCLASS_SYSRESET,
	.of_match	= exynos_sysreset_ids,
	.probe		= exynos_sysreset_probe,
	.priv_auto	= sizeof(struct exynos_sysreset_priv),
	.ops		= &exynos_sysreset_ops,
};
