<?php
/**
 *  @file
 *  This will format a view to display several images in a Galleria style, for
 *  Views jQFX: Galleria.
 *
 * - $view: The view object.
 * - $options: Style options. See below.
 * - $rows: The output for the rows.
 * - $title: The title of this group of rows.  May be empty.
 * - $id: The unique counter for this view.
 */
?>

  <div id="views-jqfx-galleria-<?php print $id; ?>" class="views-jqfx-galleria">

    <div id="views-jqfx-galleria-images-<?php print $id; ?>" class="<?php print $classes; ?>">
      <?php foreach ($rows as $image): ?>
        <?php print $image ."\n"; ?>
      <?php endforeach; ?>
    </div>

  </div>
