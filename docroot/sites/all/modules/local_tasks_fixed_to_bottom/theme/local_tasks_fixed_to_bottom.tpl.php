<?php

/**
 * @file
 * Theme "Local tasks fixed to bottom" wrapper.
 */
?>

<?php if (!empty($local_tasks)): ?>
  <div id="local-tasks-fixed-to-bottom" class="clearfix">
    <div class="inner"><?php print $local_tasks; ?></div>
  </div>
<?php endif; ?>
