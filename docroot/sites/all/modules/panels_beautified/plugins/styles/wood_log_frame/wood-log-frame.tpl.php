<?php
/**
 * @file
 *
 * Display the background for the stone wall.
 *
 * - $content: The content of the box.
 */
?>
<div class="wood-log-frame">
  <div class="wrapper">
    <div class="top-edge">
      <div class="left-corner"></div>
      <div class="right-corner"></div>
    </div>
    <div class="left-edge">
      <div class="right-edge clearfix">
        <div class="background">
          <?php print $content; ?>
        </div>
      </div>
    </div>
    <div class="bottom-edge">
      <div class="left-corner"></div>
      <div class="right-corner"></div></div>
  </div>
</div>
