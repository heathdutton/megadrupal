<?php

/**
 * @file
 * Theme the 'amazon-item' 'software' 'details' style.
 * Many, many available variables. See template_preprocess_amazon_item().
 * Everything that gets put into $variables there is available.
 */
?>
<div class="<?php print $classes; ?>">
<?php if (!empty($smallimage)) { print $smallimage; } ?>
<div><strong><?php print l($title, $detailpageurl, array('html' => TRUE, 'attributes' => array('rel' => 'nofollow'))); ?></strong></div>
<div><strong><?php print t('Publisher'); ?>:</strong> <?php print $publisher; ?></div>
<?php if (!empty($operatingsystem)):?>
<div><strong><?php print t('Operating System'); ?>:</strong> <?php print $operatingsystem; ?></div>
<?php endif; ?>
<?php if (!empty($listpriceformattedprice)): ?>
<div><strong><?php print t('List Price'); ?>:</strong> <?php print $listpriceformattedprice; ?></div>
<?php endif; ?>
</div>
