<?php

/**
 * @file
 * Theme implementation for comment number link.
 *
 * Available variables:
 * - $link: The comment number, as a link.
 * - $tips: The text that will appear next to the comment number.
 *
 * These variables are provided for context:
 * - $comment: Full comment object.
 *
 *
 * @see template_preprocess_comment_easy_reply_comment_number_link()
 */
?>
<span class="comment-easy-reply-number-link-wrapper">
<?php print $link; ?>
<?php print $tips; ?>
</span>
