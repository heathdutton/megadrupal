<?php

/**
 * @file
 * Default theme implementation for viewerjs_field_formatter_view.
 *
 * Available variables:
 * - $color : The class to display widget in built-in color.
 * - $thumbnail : Thumbnail of file extension or image.
 * - $icon : File type icon.
 * - $filename : The filename or file description if exist.
 * - $size : The size of the document.
 * - $preview_link : Display link to preview document with ViewerJS.
 *
 * @see template_preprocess()
 * @see template_preprocess_viewerjs()
 * @see template_process()
 */
?>

<div class="viewerjs-field-item">
  <div class="viewerjs-item-wrapper <?php print $color; ?>"
       style="<?php ($thumbnail) ? print $thumbnail : print ''; ?>">
    <header>
      <div class="name">
        <?php print $icon; ?><b><?php print $filename; ?></b>
      </div>
      <div class="size">
        <ul class="actions">
          <li><?php print $size; ?></li>
          <li><?php print render($download_link); ?></li>
          <?php if ($preview_link): ?>
            <li><?php print render($preview_link); ?></li>
          <?php endif; ?>
        </ul>
        </div>
    </header>
  </div>

</div>
