<?php

/**
 * @file
 * Theme the 'amazon_inline_item' type.
 * Available variables:
 * - $item: The cleaned amazon_item with all data.
 */
?>
<span class="<?php print _amazon_item_classes($item) ?> amazon-item-inline">
  <?php print l($item['title'], $item['detailpageurl'], array('html' => TRUE, 'attributes' => array('rel' => 'nofollow'))); ?>
</span>
