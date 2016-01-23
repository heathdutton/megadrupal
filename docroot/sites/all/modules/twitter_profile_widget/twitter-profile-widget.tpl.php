<?php
/**
 * @file
 * Block content template for twitter widget
 *
 * @param $twitter_script
 *   A JavaScript with widget settings
 * @param $follow_us_enabled
 *   Boolean. Shows whether include follow us button or not
 * @param $follow_us_link
 *   Link to follow
 */
?>
<div class = "twitter_profile_widget">

  <?php if ($twitter_script): ?>
    <script type = "text/javascript"><?php print $twitter_script; ?></script>
  <?php endif; ?>
  
  <?php if ($follow_us_enabled): ?>
    <div class = "twp_bottom">
      <?php print $follow_us_link; ?>
    </div>
  <?php endif; ?>
  
</div>