<?php

/**
 * @file
 * Default theme implementation to display a region.
 */
?>
<?php if ($content): ?>
  <footer id="<?php print $region; ?>" class="<?php print $classes; ?> clearfix">
    <?php print $content; ?>
  </footer>
<?php endif; ?>
