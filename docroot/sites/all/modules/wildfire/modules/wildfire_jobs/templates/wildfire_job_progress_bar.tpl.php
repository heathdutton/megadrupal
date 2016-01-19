<?php
/**
 * @file wildfire_job_progress_bar.tpl.php
 * Theme functions for the progress bar on Wildfire Jobs pages.
 *
 * @author Craig Jones <craig@tiger-fish.com>
 */
?>
<span class="wildfire-job-progress-bar">
  <span class="wildfire-job-progress-bar-inner" style="width:<?php print $progress; ?>%">
  </span>
</span>
<span class="wildfire-job-progress-percent">
  <?php printf('(%d%%)', $progress); ?>
</span>
