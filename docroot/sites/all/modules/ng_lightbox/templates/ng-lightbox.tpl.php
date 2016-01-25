<?php

/**
 * @file
 * Lightbox template.
 */

// Gross hack because wrapper_attributes won't work?
$classes = isset($element['#wrapper_attributes']['class']) ? implode(' ', $element['#wrapper_attributes']['class']) : '';
$class_modifier = $element['#modifier'];
?>

<div class="lightbox lightbox--<?php print $class_modifier; ?>">
  <div class="lightbox__overlay <?php print $classes; ?>">

    <div class="lightbox__content">
      <h2 class="lightbox__header"><?php print $element['#title']; ?></h2>
      <div class="lightbox__body">
        <?php print $element['#children']; ?>
      </div>
    </div>
  </div>
</div>
