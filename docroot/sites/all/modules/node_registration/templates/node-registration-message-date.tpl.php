<?php
/**
 * @file
 * Contains the block message for when outside the registration period.
 *
 * Available variables:
 * - object $user -- session user (might be anonymous)
 * - object $node -- owner node
 */

$registration_time_passed = $node->registration->max_registration_time_passed();
?>

<?php if ($registration_time_passed): ?>

  <p><?php print t("Registrations have ended on @date.", array(
    '@date' => format_date($node->registration->max_registration_time()),
  )); ?></p>

<?php else: ?>

  <p><?php print t("Registration for this event is not open yet."); ?></p>

<?php endif; ?>
