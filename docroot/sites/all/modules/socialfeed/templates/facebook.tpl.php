<?php

/**
 * @file
 * This template handles the layout of Social Feed facebook block.
 *
 * Variables available:
 * - $facebook_feed: An array of the Facebook feeds.
 * - $facebook_feed['message']: Use this to display Facebook feed
 * - $facebook_feed['created_stamp']: Use this to display feed date
 * - $facebook_feed['picture']: Use this display pictures when post type Photo
 * is selected
 */
?>

<?php if (variable_get('socialfeed_facebook_app_id') != ''): ?>
  <ul>
  <?php foreach ($facebook as $facebook_feed): ?>
    <li>
      <div class="fb-message"><?php print $facebook_feed['message']; ?></div>
      <?php if (isset($facebook_feed['created_stamp']) && !empty($facebook_feed['created_stamp'])): ?>
        <div class="fb-time">
          <?php print $facebook_feed['created_stamp']; ?>
        </div>
      <?php endif; if (isset($facebook_feed['picture']) && !empty($facebook_feed['picture'])):
?>
        <div class="fb-pic">
          <img src="<?php print $facebook_feed['picture']; ?>">
        </div>
      <?php endif; ?>    
    </li>
  <?php endforeach; ?>
  </ul>
<?php endif;
