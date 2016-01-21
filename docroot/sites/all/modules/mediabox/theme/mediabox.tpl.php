<div class="content"<?php print $content_attributes; ?>>
  <?php
  // We hide the comments and links now so that we can render them later.
  print drupal_render($content);
  ?>
</div>