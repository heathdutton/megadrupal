<?php

/**
 * @file
 * Message template file.
 */
?>
<div class="rules-message">
  <div class="close-button"></div>
  <?php if ($picture): ?>
    <div class="picture">
      <?php print drupal_render($picture); ?>
    </div>
  <?php endif; ?>
  <div class="body">
    <div class="subject">
      <?php print $subject; ?>
    </div>
    <?php print $body; ?>
  </div>
</div>
