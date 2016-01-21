<?php
/**
 * @file
 *
 * Display the background for the stone wall.
 *
 * - $content: The content of the box.
 */
?>
<div class="parchment-scroll">
  <div class="ps-wrapper">
    <div class="ps-top-edge">
      <div class="ps-left-corner"></div>
      <div class="ps-right-corner"></div>
    </div>
    <div class="ps-left-edge">
      <div class="ps-right-edge clearfix">
        <div class="ps-background">
          <?php print $content; ?>
        </div>
      </div>
    </div>
    <div class="ps-bottom-edge">
      <div class="ps-left-corner"></div>
      <div class="ps-right-corner"></div></div>
  </div>
</div>
