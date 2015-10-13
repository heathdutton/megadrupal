<?php

/**
 * @file
 * Default theme implementation to display an oauth application.
 */

?>
<div id="oauth-<?php print $oauth->oid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <div class="content"<?php print $content_attributes; ?>>
    <?php print render($content); ?>
  </div>

</div>