<?php
/**
 * @file
 * Views Slideshow: Single Frame template file.
 *
 * - $rows: The themed rows from the view.
 * - $settings: The settings for the views slideshow type.
 * - $view: The view object.
 * - $vss_id: The views slideshow id.
 */

$i = 0;
?>
<div id="views_slideshow_liquid_slider_content_<?php print $vss_id; ?>" class="liquid-slider">
  <?php foreach ($rows as $row): ?>
    <div>
	  <?php $tab_name = $view->render_field($settings['caption_field'], $i++); ?>
	  <?php if (isset($tab_name)): ?>
	  <h2 class="title"><?php print $tab_name; ?></h2>
	  <?php endif; ?>
	  <?php if (isset($row)): ?>
	  <?php print $row; ?>
	  <?php endif; ?>
    </div>
  <?php endforeach; ?>
</div>
