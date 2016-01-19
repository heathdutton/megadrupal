<?php

/**
 * @file
 * Block template.
 *
 * @ingroup pay_with_a_tweet
 */
?>
<?php if ($configuration->show == '1' && !empty($configuration->text)) : ?>
<div class="pay-with-a-tweet-before-text pay-with-a-tweet-before-text-<?php print $pay_with_a_tweet->pid; ?>">
  <?php print $configuration->text; ?>
</div>
<?php endif; ?>

<div class="pay-with-a-tweet-button pay-with-a-tweet-button-<?php print $pay_with_a_tweet->pid; ?>">
  <?php print $pay_with_a_tweet->html; ?>
</div>

<?php if ($configuration->show == '0' && !empty($configuration->text)) : ?>
<div class="pay-with-a-tweet-after-text pay-with-a-tweet-after-text-<?php print $pay_with_a_tweet->pid; ?>">
  <?php print $configuration->text; ?>
</div>
<?php endif; ?>
