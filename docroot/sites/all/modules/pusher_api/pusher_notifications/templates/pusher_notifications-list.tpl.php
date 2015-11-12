<?php
/**
 * @file pusher_notifications-list.tpl.php
 * Theme a notification list wrapper
 */
?>

<div id="pusher-notifications-list" class="element-invisible">
  <h2 id="pusher-notificaitons-list-title"><?php print t('Notifications'); ?></h2>
  <div class="pusher-notificaitons-list-content"><?php print t('Loading...'); ?></div>
  <div class="pusher-notificaitons-list-more"><?php print l(t('See all notifications'), 'notifications/list'); ?></div>
</div>


