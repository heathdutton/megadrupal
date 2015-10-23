<?php

/**
 * @file
 * Default theme implementation for a reply.
 */
?>
<div id="reply-<?php print $id ?>" class="<?php print $classes ?>">
  <div class="reply-body"><?php print render($content) ?></div>
  <div class="reply-links"><?php print render($links) ?></div>
</div>
