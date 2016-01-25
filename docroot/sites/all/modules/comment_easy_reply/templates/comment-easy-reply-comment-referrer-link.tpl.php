<?php

/**
 * @file
 * Theme implementation for comment referrer link.
 *
 * Available variables:
 * - $link: A link to show referred comment number.
 * - $tips: The text that will appear next to the referred comment link.
 *
 * These variables are provided for context:
 * - $comment: Full comment object.
 *
 *
 * @see template_preprocess_comment_easy_reply_comment_referrer_link()
 */

?>
<em class="comment-easy-reply-referrer-link-wrapper">
<?php print $link; ?>
</em>
