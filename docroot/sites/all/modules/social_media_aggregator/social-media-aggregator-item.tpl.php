<?php
/**
 * @file
 * Standard TPL for social media atoms.
 */
?>
<div class="social-feed-wrapper">
  <div class="social-feed-header">
    <div class="social-feed-icon social-icon-<?php print $source; ?>"></div>
    <div class="social-feed-timestamp"><?php print $time; ?></div>
  </div>
  <div class="social-feed-item ">
    <?php print $content_image; ?>
    <div class="social-feed-item-content">
      <div class="social-feed-account-details">
        <div class="social-feed-profile-pic"><?php print $profile_pic; ?></div>
        <div class="social-feed-account-name"><?php print $account_name; ?></div>
        <div class="social-feed-account-handle"><?php print $account_handle; ?></div>
      </div>
      <p class="social-feed-content"><?php print $content; ?></p>
    </div>
  </div>
</div>
