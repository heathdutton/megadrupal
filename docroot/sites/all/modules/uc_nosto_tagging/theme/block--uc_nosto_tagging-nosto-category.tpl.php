<?php

/**
 * @file
 * Block template for Nosto category tagging.
 *
 * Available variables:
 * - $category_path: Full path to the current category.
 *
 * @see uc_nosto_tagging_block_view()
 * @see uc_nosto_tagging_theme()
 * @see uc_nosto_tagging_preprocess_nosto_category()
 */
?>

<?php if (!empty($category_path)): ?>
  <div class="nosto_category" style="display:none"><?php print check_plain($category_path); ?></div>
<?php endif; ?>
