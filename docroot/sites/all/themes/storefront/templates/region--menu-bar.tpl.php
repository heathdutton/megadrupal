<?php
/**
 * @file
 * Overrides the default region template for the 'Menu Bar' region.
 *
 * @param $content
 *  The output of the region if there are blocks using that region.
 * @param $classes
 *  Classes added to the region.
 */

?>
<?php if (!empty($content)): ?>
  <div id="menu-bar" class="nav clearfix">
    <?php print $content; ?>
  </div>
<?php endif; ?>
