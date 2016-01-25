<?php
/**
 * @file
 * Default theme implementation of tweetable text.
 *
 * - $text: The actual text to be displayed.
 * - $tweet_url: The URL that will take the user to the Tweet form.
 * - $icon: The Twitter icon to put inline.
 * - $content_url: The URL pointing to this peice of content.
 * - $short_url: The shortened URL pointing to this peice of content.
 * - $alt: (Optional) The text to be tweeted if different than $text.
 * - $hashtag: (Optional) Space seperated list of hashtags.
 * 
 * @see template_preprocess_tweetable_text()
 *
 * @ingroup themable
 */
?>
<span class='tweetable-text'><a href="<?php print $tweet_url ?>"><?php print $text; ?><?php if ($icon) { echo '&thinsp;' . $icon; } ?></a><span class="tweetable-text-sharebuttons"><a href="<?php print $tweet_url ?>">TWEET</a></span></span>
