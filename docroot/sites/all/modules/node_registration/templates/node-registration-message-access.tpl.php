<?php
/**
 * @file
 * Contains the block message for when the user doesn't have sufficient access.
 *
 * Available variables:
 * - object $user -- session user (might be anonymous)
 * - object $node -- owner node
 */
?>

<p><?php print t("You don't have sufficient access to register. !login_link", array(
  '!login_link' => l(t('Log in here.'), 'user/login'),
)); ?></p>
