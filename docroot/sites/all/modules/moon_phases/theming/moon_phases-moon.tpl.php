<?php
/**
 * @file
 * Displays the image of the moon phase
 * 
 * Available variables:
 * - $moondata: An array of data about the moon phase
 *   - class: either "day" or "night"
 *   - left: an array
 *     image: the overlay image
 *   - right: an array
 *     image: the overlay image
 * 
 * @ingroup themeable
 */
?>
<div class="moon">
  <div class="moonmask <?php echo $moondata[ 'class' ]?>">
    <div class="overlay left">
      <?php echo $moondata[ 'left' ][ 'image' ]?>
    </div>
    <div class="overlay right">
      <?php echo $moondata[ 'right' ][ 'image' ]?>
    </div>
  </div>
</div>