<?php
/**
 * @file
 *
 * Display the box for rounded corners.
 *
 * - $content: The content of the box.
 */
?>

<div class="corner-wrapper corner-gray-bg">
  <div class="corner-inner">
    <div class="corner-top"><div class="corner-top-right corner"></div><div class="corner-top-left corner"></div></div><!-- /corner-top -->
    <div class="corner-content clearfix">
      <div class="corner-content-inner">
        <?php print $content; ?>      
      </div><!-- /corner-content-inner -->
    </div><!-- /corner-content -->
  <div class="corner-bottom"><div class="corner-bottom-right corner"></div><div class="corner-bottom-left corner"></div></div><!-- /corner-bottom -->
  </div><!-- /corner-inner -->
</div><!-- /corner-wrapper -->