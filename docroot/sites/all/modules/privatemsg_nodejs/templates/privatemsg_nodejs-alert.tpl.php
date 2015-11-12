<?php

/**
 * @file
 * Alert template file.
 */
?>
<div class="wrapper-alert">
  <div class="close-button"></div>
  <div class="picture">
    <?php print $picture; ?>
  </div>
  <div class="body">
    <span class="username">
      <?php print $name; ?>
    </span>
    <?php print $body; ?>
    <div class="links">
      <?php print $links; ?>
    </div>
  </div>
</div>
