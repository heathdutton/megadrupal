<?php

/**
 * @file
 * Template for Taxonomy term: Menu Categories in Stacked view mode.
 */
?>
<div class="<?php print $classes; ?>">
  <?php print render($title_suffix); ?>
  <div class="row">
    <div class="col-md-8">
      <?php if (!empty($content['field_menu_categories_image'])): ?>
        <?php print render($content['field_menu_categories_image']); ?>
      <?php endif; ?>
    </div>
    <div class="col-md-4">
      <?php if (!empty($content['description'])): ?>
        <?php print render($content['description']); ?>
      <?php endif; ?>
    </div>
  </div>

</div>
