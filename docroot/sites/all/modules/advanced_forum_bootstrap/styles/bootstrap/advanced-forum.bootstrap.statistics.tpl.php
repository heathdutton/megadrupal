<?php
/**
 * @file
 *
 * Theme implementation: Template for each forum forum statistics section.
 *
 * Available variables:
 * - $current_total: Total number of users currently online.
 * - $current_users: Number of logged in users.
 * - $current_guests: Number of anonymous users.
 * - $online_users: List of logged in users.
 * - $topics: Total number of nodes (threads / topics).
 * - $posts: Total number of nodes + comments.
 * - $users: Total number of registered active users.
 * - $latest_users: Linked user names of latest active users.
 */
?>

<div class="row">
  <div class="col-md-6">
    <h4><?php print t("What's Going On?") ?></h4>
    <?php print t('Currently active users: !current_total', array('!current_total' => $current_total)) ?>
    <?php if (!empty($online_users)) print $online_users ?>
  </div>

  <div class="col-md-6">
    <h4><?php print t('Statistics') ?></h4>
    <?php print t('Topics: !topics, Posts: !posts, Users: !users', array('!topics' => $topics, '!posts' => $posts, '!users' => $users)) ?>
    <?php print t('Welcome to our latest members: !users', array('!users' => $latest_users)) ?>
  </div>
</div>
