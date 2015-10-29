<?php
/**
 * @file
 * Template for a custom layout: 3 columns and 9 columns in a 12 column susy framework
 */
?>
<div class="dv-left-col dv-pane">
  <div class="dv-left-col-wrapper clearfix">
    <div class="content">
      <?php print $content['content']; ?>
    </div>
    <div class="left-sidebar">
      <?php print $content['left']; ?>
    </div>
  </div>
  <div class="footer">
    <?php print $content['footer']; ?>
  </div>
</div>