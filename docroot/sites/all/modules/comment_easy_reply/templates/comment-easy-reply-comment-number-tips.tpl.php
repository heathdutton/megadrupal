<?php

/**
 * @file
 * Theme implementation for comment number link tooltip.
 *
 * Available variables:
 * - $tips: The text that will appear inside the tooltip.
 * - $classes: The CSS classes.
 *
 * These variables are provided for context:
 * - $comment: Full comment object.
 * - $comment_num: Comment number.
 *
 *
 * @see template_preprocess_comment_easy_reply_comment_number_tips()
 */
?>
<span class="<?php print $classes;?>" style="display:none;">
<?php print $tips; ?>
</span>
