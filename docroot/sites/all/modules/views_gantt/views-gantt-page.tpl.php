<?php

/**
 * @file
 * Template file for page that is used in our gantt dialog.
 */
?>
<div id="gantt-dialog-page">
  <?php if (isset($messages)): print $messages; endif; ?>
  <?php print render($page['content']); ?>
</div>
