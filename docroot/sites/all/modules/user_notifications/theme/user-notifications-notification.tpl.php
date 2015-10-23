<?php
/**
 * @file user-notifications-notification.tpl.php
 * Template for notification shown on user list page
 *
 * - $notification full raw object
 * - $message - contains 'time', 'subject' & 'body' rendered
 *
 * @see template_preprocess_user_notifications_notification()
 */
?>
<div class="notification-item clear-block <?php echo $classes; ?>">
  <div class="container-inline">
    <div class="notification-item-subject"><h2><?php echo $message['subject'];?></h2></div>
    <div class="notification-item-time"><?php echo $message['time']; ?></div>
	  <div class="notification-item-body"><?php echo $message['body']; ?></div>
  </div>
</div>
