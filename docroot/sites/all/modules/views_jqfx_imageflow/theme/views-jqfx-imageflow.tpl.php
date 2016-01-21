<?php
/**
 *  @file
 *  This will format a view to display several images in a ImageFlow/CoverFlow
 *  style.
 *
 *  VERY IMPORTANT:
 *  The View MUST return a set of fields composed of Images only.
 *  Anything else WILL break the output.
 *
 * - $view: The view object.
 * - $options: Style options. See below.
 * - $rows: The output for the rows.
 * - $title: The title of this group of rows.  May be empty.
 * - $id: The unique counter for this view.
 * - $images: An array of images that have been stripped from $rows.
 */
?>
<div id="views-jqfx-imageflow-<?php print $id; ?>" class="views-jqfx-imageflow"; <?php if ($styles) print $styles; ?>>

  <div id="views-jqfx-imageflow-images-<?php print $id; ?>" class="views-jqfx-imageflow-images imageflow">
    <?php foreach ($images as $image): ?>
     <?php print $image ."\n"; ?>
    <?php endforeach; ?>
  </div>

</div>

