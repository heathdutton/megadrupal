<?php

/**
 * @file
 * This template handles the layout of Hover Card.
 *
 * Variables available:
 * - $details: An array of the user details.
 * - $details['name']: Use this to display the name of the user.
 * - $details['mail']: Use this to display the registered e-mail of the user.
 * - $details['picture']: Array which can be used to display the user picture.
 * - $details['role']: Use this to display the role of the user.
 */
?>
<div class="user-details">
  <?php
  $user_picture = $details['picture'];
  if (!empty($user_picture)):
  ?>
  <div class="user-image">
  <?php print_r($user_picture); ?>
  </div>
  <?php endif; ?>
  <div class="user-name"><strong><?php print t('Username'); ?>:</strong> <?php print $details['name']; ?></div>
  <?php if (variable_get('hover_card_user_email_display_status', TRUE)): ?>
  <div class="user-mail"><strong><?php print t('Email'); ?>:</strong> <a href="mailto:<?php print $details['mail']; ?>"><?php print $details['mail']; ?></a></div>
  <?php endif; ?>
  <div class="user-role"><strong><?php print t('Roles'); ?>:</strong> <?php print $details['roles']; ?></div>
</div>
