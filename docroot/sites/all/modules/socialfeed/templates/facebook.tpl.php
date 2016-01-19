<?php

/**
 * @file
 * This template handles the layout of Social Feed facebook block.
 *
 * Variables available:
 * - $facebook_feed: An array of the Facebook feeds.
 * - $facebook_feed['message']: Display Facebook feed.
 * - $facebook_feed['created_stamp']: Display feed date.
 * - $facebook_feed['picture']: Display pictures when post type is Photo & Show
 * Post Pictures is checked.
 * - $facebook_feed['full_feed_link']: Display direct post link.
 * - $facebook_feed['video']: Display videos when post type is Video & Show
 * Post Video is checked.
 */
?>

<?php if (variable_get('socialfeed_facebook_app_id') != '') : ?>
  <ul>
    <?php foreach ($facebook as $facebook_feed): ?>
    <li>
      <!-- Feed -->
        <?php if (isset($facebook_feed['message']) && !empty($facebook_feed['message'])) : ?>
        <div class="fb-message">
            <?php print $facebook_feed['message']; ?>
        </div>
      <!-- Full post link -->
        <?php endif; if (isset($facebook_feed['full_feed_link']) && !empty($facebook_feed['full_feed_link'])) : ?>
        <div class="teaser-link">
            <?php print $facebook_feed['full_feed_link']; ?>
        </div>
      <!-- Time -->
        <?php endif; if (isset($facebook_feed['created_stamp']) && !empty($facebook_feed['created_stamp'])) : ?>
        <div class="fb-time">
            <?php print $facebook_feed['created_stamp']; ?>
        </div>
      <!-- Picture -->
        <?php endif; if (isset($facebook_feed['picture']) && !empty($facebook_feed['picture'])) : ?>
        <div class="fb-pic">
          <img src="<?php print $facebook_feed['picture']; ?>">
        </div>
      <!-- Video -->
        <?php endif; if (isset($facebook_feed['video']) && !empty($facebook_feed['video'])) : ?>
        <div class="fb-video">
          <a href="<?php print $facebook_feed['video']; ?>">See Video</a>
        </div>
        <?php endif; ?>
    </li>
    <?php endforeach; ?>
  </ul>
<?php endif;
