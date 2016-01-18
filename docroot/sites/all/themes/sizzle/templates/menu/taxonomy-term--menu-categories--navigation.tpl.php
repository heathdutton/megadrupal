<?php

/**
 * @file
 * Template for Taxonomy term: Menu Categories in Navigation view mode.
 */
?>
<div class="<?php print $classes; ?>" data-menu-category="<?php print $term_machine_name; ?>">
  <a href="<?php print $term_url; ?>">
    <?php if (!empty($content['field_menu_categories_icon'])): ?>
      <?php print render($content['field_menu_categories_icon']); ?>
    <?php endif; ?>
    <?php print $term_name; ?>
  </a>
  <?php print render($title_suffix); ?>
</div>
