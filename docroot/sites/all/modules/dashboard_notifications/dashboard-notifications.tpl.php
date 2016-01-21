<?php
/**
 * @file
 * Displays a user alert.
 *
 * Available variables:
 * - $alert_label: The label of the alert, as set in the User Alerts settings.
 * - $body: The user alert message.
 *
 * @ingroup themeable
 */
?>

<div id="dashboard-notification-<?php print $aid; ?>" class="dashboard-notification">
  <div class="dashboard-notification-type icon icon-<?php print $notification->type; ?>"></div>
  <div class="dashboard-notification-message"><strong><?php print $notification->type; ?> - </strong><?php print $message; ?></div>
  <div class="dashboard-notification-close"><a href="javascript:;" rel="<?php print $aid; ?>">x</a></div>
</div>