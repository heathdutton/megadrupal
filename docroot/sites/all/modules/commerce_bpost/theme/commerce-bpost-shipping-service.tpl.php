<?php
/**
 * @file
 *
 * bpost shipping pane
 *
 * Available $element attributes :
 * - title: shipping service display title
 * - price: shipping service price
 * - bpack: bpack service details
 */
?>
<div id="bpost-shipping">
  <div><?php print $content['title']; ?> <?php print $content['price']; ?></div>
  <?php if (isset($content['point_details'])) : ?>
  <div>
    <div><?php print t('Delivery point details:'); ?></div>
    <div><?php print $content['point_details']; ?></div>
  </div>
  <?php endif; ?>
</div>
