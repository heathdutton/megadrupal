<?php
/**
 * @file
 * Overrides the default region template for the 'Content Asside' region.
 *
 * @param $content
 *  The output of the region if there are blocks using that region.
 * @param $classes
 *  Classes added to the region.
 */

?>
<?php if (!empty($content)): ?>
  <aside class="<?php print $classes; ?>">
    <?php print $content; ?>
  </aside>
<?php endif; ?>
