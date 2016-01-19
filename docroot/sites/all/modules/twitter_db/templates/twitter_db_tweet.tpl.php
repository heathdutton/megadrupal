<?php
//$Id
/**
 * @file
 * Filename:   twitter_db_tweet.tpl.php
 * Author  :   dhavyd@dhavyd.com
 * Date    :   Mars 8, 2012
 * Default theme implementation to display a tweet.
 *
 * Available variables:
 * - $tid: Twitter ID.
 * - $created: the relative date and time the tweet was created, ex: "2 days ago".
 * - $text: the tweet text, with parsed links to any URLs that hans been written by the Twitter user.
 * - $source: Source.
 * - $replyto: The screen name to reply to.
 * - $type: "User" or "hashtag".
 * - $user_id: The Twitter user id.
 * - $user_name: The Twitter user name.
 * - $user_profile_pic: URL to the Twitter user picture.
 * 
*/
?>

<span id="tweet-<?php print $tid; ?>" class="tweet <?php if ($user_profile_pic): ?>profile-pic<?php endif; ?>">
  <?php if ($user_profile_pic): ?>
    <span class="picture">
      <img src="<?php print $user_profile_pic ?>" title="<?php print $user_name; ?>" />
    </span>
  <?php endif; ?>
  <?php if ($created): ?>
    <span class="created">
      <?php print $created; ?>
    </span>
  <?php endif; ?>
  <span class="text">
    <?php print $text; ?>
  </span>
</span>

  

