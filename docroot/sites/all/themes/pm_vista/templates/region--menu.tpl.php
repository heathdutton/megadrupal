<?php
/**
 * @file
 * region menu template file
 */
?>
<div<?php print $attributes; ?>>
  <div<?php print $content_attributes; ?>>
    <?php if ($content): ?>
    <nav class="navigation">
        <?php print $content; ?>
    </nav>
    <?php endif; ?>
  </div>
</div>
