<?php
/**
 * @file
 * Template file for the plagiarize teaser to be embedded into a node body.
 */
?>
<div <?php print drupal_attributes($content_attributes_array); ?>>
  <h2 <?php print drupal_attributes($title_attributes_array); ?>><?php print $title; ?></h2>
  <div class="summary">
    <?php print $summary; ?>
  </div>
</div>
