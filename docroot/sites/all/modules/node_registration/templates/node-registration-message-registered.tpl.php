<?php
/**
 * @file
 * Contains the block message for when the user is already registered.
 *
 * Available variables:
 * - object $user -- session user (might be anonymous)
 * - object $node -- owner node
 * - object $registration -- owner registration
 */

$uri = node_registration_uri($registration);
?>

<p><?php print t("You've already been registered. !view_link", array(
  '!view_link' => l(t('Click here to see it.'), $uri['path']),
)); ?></p>
