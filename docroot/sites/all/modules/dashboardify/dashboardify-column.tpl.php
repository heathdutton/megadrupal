<?php

/**
 * @file
 * Default theme implementation to display a dashboard.
 *
 * Available variables:
 * - $blocks: Blocks available in given column.
 * - $width: Width for given column.
 *
 * @ingroup themeable
 */
?>

<div class="<?php print $classes ?>" style="width: <?php print $width; ?>%">
  <?php print $blocks; ?>
</div>
