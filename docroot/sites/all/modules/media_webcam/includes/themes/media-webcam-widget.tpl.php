<?php

/**
 * @file media_webcam/includes/themes/media-webcam-widget.tpl.php
 *
 * Template file for theme('media_webcam_widget').
 *
 * Available variables:
 */
?>
<div id="<?php print $wrapper_id; ?>" class="media-webcam">
  <?php print $form_elements; ?>

  <div id="<?php print $flash_id; ?>" class="media-webcam-flash">
    <?php print $no_flash; ?>
  </div>

  <?php if ($buttons) : ?>
    <div class="media-webcam-buttons">
      <?php print $buttons; ?>
    </div>
  <?php endif; ?>

</div>
