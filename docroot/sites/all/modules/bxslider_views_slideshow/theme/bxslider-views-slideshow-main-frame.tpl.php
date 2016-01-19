<?php
/**
 * @file
 * Displays a single BxSlider.
 *
 * Available variables:
 * - $view: The view object.
 * - $options: Style options. See below.
 * - $rows: The output for the rows.
 * - $title: The title of this group of rows. May be empty.
 * - $id: The unique counter for this view.
 */
?>
<div id="views-slideshow-bxslider-<?php print $id; ?>" class="views-slideshow-bxslider">
  <?php if (!empty($title)) : ?>
  <h3><?php print $title; ?></h3>
  <?php endif; ?>
  <ul id="views-slideshow-bxslider-images-<?php print $id; ?>" class="<?php print $classes; ?>">
    <?php foreach ($rendered_rows as $row): ?>
      <?php print $row . "\n"; ?>
    <?php endforeach; ?>
  </ul>
</div>
