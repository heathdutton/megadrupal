<?php
/**
 * @file
 *    Template that contains the content of the page displayed when a user
 *    unsubscribes from a mailing.
 */

global $user;
?>
<p>
  <?php if ($outcome == 'success'): ?>
    The email address <em><?php print $mail; ?></em> has been unsubscribed and
    will no longer receive emails from this site.
  <?php elseif ($outcome == 'noaction'): ?>
    The email address <em><?php print $mail; ?></em> is not currently subscribed
    to receive any emails from this site.
  <?php elseif ($outcome == 'invalidtoken'): ?>
    The unsubscribe link used does not correspond to any currently subscribed
    user. It may have already been used to unsubscribe previously.
  <?php else: ?>
    There was a problem while unsubscribing. The link you followed might no
    longer be valid, or might have been truncated by your email program
    (especially if it was split onto two lines).
  <?php endif; ?>

  <?php if ($user->uid): ?>
    <?php print l(t('Manage your email preferences'), 'user'); ?>.
  <?php else: ?>
    Please <?php print l(t('log in'), 'user/login'); ?> to manage your email
    preferences.
  <?php endif; ?>
</p>
