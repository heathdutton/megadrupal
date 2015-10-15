<?php
/**
 * @file
 * Template for an entity collection.
 */
?>
<div class="<?php print $classes; ?>" <?php print drupal_attributes($attributes_array); ?>>
<?php print render($title_prefix); ?>
  <?php if ($show_title): ?>
    <h2><?php print $title; ?></h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
  <?php foreach ($collection as $item): ?>
      <?php print render($item); ?>
  <?php endforeach; ?>
</div>
