<?php
/**
 * @file wildfire_jobs_current_page.tpl.php
 * Template for the current jobs page.
 *
 * @author Craig Jones <craig@tiger-fish.com>
 */
?>
<h2><?php print t('Active jobs'); ?></h2>
<div id="active-jobs">
  <?php print drupal_render($active); ?>
</div>

<h2><?php print t('Pending jobs'); ?></h2>
<div id="pending-jobs">
  <?php print drupal_render($pending); ?>
</div>

<h2><?php print t('Latest jobs'); ?></h2>
<div id="latest-jobs">
  <?php print drupal_render($latest); ?>
</div>
