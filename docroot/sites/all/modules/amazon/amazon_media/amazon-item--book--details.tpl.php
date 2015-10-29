<?php

/**
 * @file
 * Theme the 'amazon-item' 'book' 'details' style.
 * Many, many available variables. See template_preprocess_amazon_item().
 * Everything that gets put into $variables there is available.
 */
?>
<div class="<?php print $classes; ?>">
<?php if (!empty($smallimage)) { print $smallimage; } ?>
<div><strong><?php print l($title, $detailpageurl, array('html' => TRUE, 'attributes' => array('rel' => 'nofollow'))); ?></strong></div>
<div><strong><?php print t('Author'); ?>:</strong> <?php print $author; ?></div>
<div><strong><?php print t('Publisher'); ?>:</strong> <?php print $publisher; ?> (<?php print $publicationyear; ?>)</div>
<div><strong><?php print t('Binding'); ?>:</strong> <?php print t('@binding, @pages pages', array('@binding' => $binding, '@pages' => !empty($numberofpages) ? $numberofpages : "")); ?></div>
</div>
