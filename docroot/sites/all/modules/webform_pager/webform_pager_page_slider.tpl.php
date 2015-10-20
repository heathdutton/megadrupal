<?php

/**
 * @file
 * Theme the webform pager page slider.
 *
 * Available variables:
 * - $items: array containing the page slider items
 */
?>
<div class="<?php print $classes; print $custom_class; ?>">
  <?php if ($items): ?>
    <?php foreach($items as $item): ?>
      <?php print render($item); ?>
    <?php endforeach; ?>
  <?php endif; ?>
</div>
