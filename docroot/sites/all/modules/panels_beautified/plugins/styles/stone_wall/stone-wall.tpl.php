<?php
/**
 * @file
 *
 * Display the background for the stone wall.
 *
 * - $content: The content of the box.
 */
?>
<div class="stone-wall">
  <div class="sw-wrapper">
    <div class="sw-top-edge">
      <div class="sw-left-corner"></div>
      <div class="sw-right-corner"></div>
    </div>
    <div class="sw-left-edge">
      <div class="sw-right-edge clearfix">
        <div class="sw-background">
          <?php print $content; ?>
        </div>
      </div>
    </div>
    <div class="sw-bottom-edge">
      <div class="sw-left-corner"></div>
      <div class="sw-right-corner"></div></div>
  </div>
</div>
