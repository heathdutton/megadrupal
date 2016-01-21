<?php
/**
 * @file
 * Template file for displaying a collection of media items in a Nivo Slider
 *
 * The template-specific available variables are:
 *
 * - $slider_id: The id of the nivo slider.
 * - $images: The collection of images to be in the slider.
 * - $image: Individual image item to be included in the slider.
 *    - $image['link']: Location for the image to link to.
 *    - $image['image]: Image tag with caption in the title attribute if it was provided.
 * - $slider_theme: Theme to use for the slider.
 * - $ribbon: Boolean for whether or not to print the ribbon div.
 *
 */
?>

<div class="slider-wrapper theme-<?php echo $slider_theme; ?>">
  <?php 
    if ($ribbon) {
      echo '<div class="ribbon"></div>';
    }
  ?>
  <div id="<?php echo $slider_id; ?>-media-nivo-slider" class="nivoSlider">

    <?php 
      foreach($images as $image){
        if (isset($image['link'])) {
          echo '<a href="' . $image['link'] . '">' . $image['image'] . '</a>'; 
        }
        else {
          echo $image['image']; 
         }
      }
    ?>
  </div>
</div>
