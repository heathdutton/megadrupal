<?php
/**
 * @file pusher_notifications-notification.tpl.php
 * Theme a single pusher notification
 *
 * - $notification: The notification array.
 * - $markup: Rendered markup returned by the notification callback.
 */
?>

<div class="pusher-notification-wrapper" id="notification-<?php print $notification['nfid']; ?>">
  <?php if ($picture): ?>
    <div class="pusher-notification-picture">
      <?php print $picture; ?>
    </div>
  <?php endif; ?>
  <div class="pusher-notification">
    <?php print $markup; ?>
    <span class="pusher-notification-date">
      <?php print format_date($notification['created']); ?>
    </span>
  </div>
</div>
