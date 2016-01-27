<?php
/**
 * @file
 * Template file for field_slideshow
 */
?>
<ul id="zoundation-orbit-<?php print $id; ?>" class="<?php print $classes; ?>" <?php print drupal_attributes(array('data-options' => $options));?>data-orbit>
  <?php print drupal_render($slides); ?>
</ul>
