<?php
/**
 * Template file for jqzoom element
 *
 * Variables:
 *
 *  $element - the whole element
 *
 *  $rendered
 *  The renderd jqzoom image area, or remove it, but care to
 *  drupal_render($element['#element']['items']);
 *
 *  $title The field title
 */
?>
<div class="jqzoom-element">
  <div class="jqzoom-element-title">
    <?php print $title ?>
  </div>
  <div class="jqzoom-element-image">
    <?php print $rendered ?>
  </div>
</div>
