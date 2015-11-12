<?php

/**
 * @file
 * Default theme implementation to display a region.
 */
?>
<?php if ($content): ?>
  <div id="<?php print $region; ?>-wrapper" class="<?php print $classes; ?>">
    <section id="<?php print $region; ?>">
      <?php print $content; ?>
    </section>
  </div>
<?php endif; ?>
