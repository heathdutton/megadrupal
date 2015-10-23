<?php
/**
 * @file
 * SimpleCrop widget markup template.
 *
 * @see template_preprocess()
 * @see template_preprocess_simplecrop_widget()
 */
?><div class="<?php print $classes; ?>">

  <?php if (!empty($element['source_image'])): ?>
    <div class="original-image">
      <?php print render($element['source_image']); ?>
    </div>
  <?php endif; ?>

  <?php if (!empty($element['cropped_image'])): ?>
    <div class="cropped-image">
      <?php print render($element['cropped_image']); ?>
    </div>
  <?php endif; ?>

  <div class="simplecrop-widget-data">
    <?php print drupal_render_children($element); ?>
  </div>

</div>
