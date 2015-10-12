<?php
/**
 * @file
 * Maximage.tpl.php - Beta version - March 2014.
 *
 * Parameters:
 * $images - array: contains all maximage images to display
 * $htmlbody - contains text of maximage item
 * $htmlcustom - contains the custom HTML
 * print formatted text to be included in Javascript
 */
?>
<?php $arr_images = $variables['images']; ?>
<?php $htmlbody = $variables['htmlbody']; ?>
<?php $htmlcustom = $variables['htmlcustom']; ?>

<div id="maximage">
<?php for ($i = 0; $i < count($arr_images); $i++) : ?>
<?php $url = file_create_url($arr_images[$i]['uri']); ?>
<?php $url = parse_url($url); ?>
  <div class="maximage-item">
    <img src="<?php print $url['path']; ?>">
    <div class="maximage-text">
      <h3><?php print $arr_images[$i]['image_field_caption']['value']; ?></h3>
    </div>
  </div>
<?php endfor; ?></div>
<?php if ($htmlbody != '') : ?>
  <div class="maximage-body"><?php print $htmlbody; ?></div>
<?php endif; ?>
<?php if ($htmlcustom != '') : ?>
  <div class="maximage-custom-html"><?php print $htmlcustom; ?></div>
<?php endif; ?>
