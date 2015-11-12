<?php

/**
 * @file
 * Template for Taxonomy term: Nutrition Types in Grid view mode.
 */
?>
<div class="<?php print $classes; ?>">
  <?php print render($title_suffix); ?>
  <?php if (!empty($content['field_nutrition_types_icon'])): ?>
    <?php print render($content['field_nutrition_types_icon']); ?>
  <?php endif; ?>
  <?php print $term_name; ?>
</div>
