<?php

/**
 * @file
 * Theme implementation for comment referrer link tooltip.
 *
 * Available variables:
 * - $tips: The text that will appear inside the tooltip.
 *
 * These variables are provided for context:
 * - $comment: Full comment object.
 *
 *
 * @see template_preprocess_comment_easy_reply_comment_referrer_tips()
 */
?>
<dl class="<?php print $classes; ?>" style="display:none;">
<dt><?php print $tips; ?></dt>
</dl>
