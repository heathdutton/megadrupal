<?php

/**
 * @file
 * Theme file to handle slider display.
 *
 * Variables passed:
 * - $flickity_slider_items: An array of slider items.   
 */
?>

<div class="gallery-main">
  <div class="gallery">
    <?php  for ($i = 0; $i < count($flickity_slider_items); $i++):
      $img_url = image_style_url('flickity_slider_style', $flickity_slider_items[$i]['img_uri']);
    ?>
	<img alt = "<?php echo $flickity_slider_items[$i]['img_alt']; ?>" src="<?php echo $img_url; ?>" /> 
    <?php endfor; ?>
  </div>
</div>
