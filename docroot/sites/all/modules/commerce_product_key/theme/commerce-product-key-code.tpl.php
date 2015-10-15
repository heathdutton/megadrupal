<?php

/**
 * @file
 * Default theme implementation to present the SKU on a product page.
 *
 * Available variables:
 * - $sku: The SKU to render.
 * - $label: If present, the string to use as the SKU label.
 *
 * Helper variables:
 * - $product: The fully loaded product object the SKU represents.
 */
?>
<?php if ($code): ?>
  <div class="commerce-product-key-code">
    <?php if ($label): ?>
      <div class="code-label">
        <?php print $label; ?>
      </div>
    <?php endif; ?>
    <?php print $code; ?>
  </div>
<?php endif; ?>