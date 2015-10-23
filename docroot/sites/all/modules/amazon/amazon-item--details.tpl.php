<?php

/**
 * @file
 * Theme the 'amazon-item' 'details' style.
 * Many, many available variables. See template_preprocess_amazon_item().
 * Everything that gets put into $variables there is available.
 */
?>
<div class="<?php print $classes; ?>">
<?php if (!empty($invalid_asin)) { print "<div class='invalid_asin'>This item is no longer valid on Amazon.</div>"; } ?>
<?php if (!empty($smallimage)): ?>
  <?php print $smallimage; ?>
<?php endif; ?>
<div><strong><?php print l($title, $detailpageurl, array('html' => TRUE, 'attributes' => array('rel' => 'nofollow'))); ?></strong></div>
<div><strong><?php print t('Manufacturer'); ?>:</strong> <?php if (!empty($manufacturer)) { print $manufacturer; } ?></div>
<div><strong><?php print t('Part Number'); ?>:</strong> <?php if (!empty($mpn)) {print $mpn; }?></div>
<div><strong><?php print t('Price'); ?>:</strong> <?php if (!empty($listpriceformattedprice)) { print $listpriceformattedprice; }?></div>
</div>
