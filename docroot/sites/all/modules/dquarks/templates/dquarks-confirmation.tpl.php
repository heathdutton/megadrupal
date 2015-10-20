<?php
/**
 * @file
 * Customize confirmation screen after successful submission.
 *
 * This file may be renamed "dquarks-confirmation-[nid].tpl.php" to target a
 * specific dquarks e-mail on your site. Or you can leave it
 * "dquarks-confirmation.tpl.php" to affect all dquarks confirmations on your
 * site.
 *
 * Available variables:
 * - $node: The node object for this dquarks.
 * - $confirmation_message: The confirmation message input by the dquarks
 * author.
 * - $sid: The unique submission ID of this submission.
 */
?>

<div class="dquarks-confirmation">
  <p><?php print t('Thank you, your submission has been received.'); ?></p>
</div>

<div class="links">
  <a href="<?php print url('node/' . $node->nid) ?>">
    <?php print t('Go back to the form') ?>
  </a>
</div>
