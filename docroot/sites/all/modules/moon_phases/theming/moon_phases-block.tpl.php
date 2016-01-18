<?php
/**
 * @file
 * Displays the Moon Phase block
 * 
 * Available variables:
 * - $id: The phase ID
 * - $image: The HTML image of the moon phase
 * - $link: A link to the phase page
 * - $illum: Percentage of moon that is visible
 * - $days: The number of days until the next New moon
 * 
 * @ingroup themeable
 */
?>
<div class="moon-block" id="<?php echo $id; ?>" align="center">
  <?php echo $image; ?>
  <h5 align="center"><?php echo $link; ?></h5>
  
  <?php if ( $illum ) : ?>
    <h6 align="center"><?php echo $illum; ?></h6>
  <?php endif; ?>
  
  <?php if ( $days ) : ?>
    <h6 align="center"><?php echo $days; ?></h6>
  <?php endif; ?>
</div>