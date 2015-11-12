<?php
/**
 * @file
 * Projekktor template file
 *
 * @TODO: the player should be rendered via a theme function and the
 * carousel should be sorted by weight.
 * @TODO: classes should be added in the render array.
 */
?>
<div id="projekktor-wrapper-<?php print $id; ?>" class="container-outer projekktor-container <?php if (isset($position)) print 'jcarousel-' . $position ?>">
  <?php if (isset($carousel_first)): ?>
    <?php print render($jcarousel); ?>
  <?php endif; ?>
  <?php print render($projekktor); ?>
  <?php if (isset($carousel_second)): ?>
    <?php print render($jcarousel); ?>
  <?php endif; ?>
</div>
