<?php
/**
 * @file
 * Displays the extra data to display on the moon phase page
 * 
 * Available variables:
 * - $to_full: The number of days until the next full moon
 * - $to_new: The number of days until the next new moon
 * - $illum: The percentage illuminated of the current moon
 * 
 * @ingroup themeable
 */
?>
<section id="moon-extra">
  <div class="extra-row">
    <div class="left">Days Until Next Full Moon</div>
    <div class="right"><?php print $to_full; ?></div>
    <div class="moonclear"></div>
  </div>
  <div class="extra-row">
    <div class="left">Days Until Next New Moon</div>
    <div class="right"><?php print $to_new; ?></div>
    <div class="moonclear"></div>
  </div>
  <div class="extra-row">
    <div class="left">Percentage of Illumination</div>
    <div class="right"><?php print $illum; ?></div>
    <div class="moonclear"></div>
  </div>
</section>