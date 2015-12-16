<?php

/**
 * @file
 * Displays the slider.
 *
 * Available variables:
 * - $attributes: Element attributes like id, class.
 * - $items: The rendered items for the slider.
 * - $progress: The rendered progress bar for the slider.
 * - $prevnext: The rendered previous/next elements for the slider.
 * - $pagination: The rendered pagination element for the slider.
 *
 * @see template_preprocess()
 * 
 * @ingroup themeable
 */
?>
<div<?php print $attributes; ?>>
  <ul>
    <?php print $items; ?>
  </ul>
  <?php print $progress; ?>
  <?php print $prevnext; ?>
  <?php print $pagination; ?>
</div>
