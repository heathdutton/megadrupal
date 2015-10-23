<?php
/**
 * @file templates/xc-browse-render-bar.tpl.php
 * Default theme implementation of a single navigation bar on the browse form
 *
 * Available variables:
 * - $id: The id of the bar
 * - $lable: The label of the bar
 * - $items (array): The list of displayable items (the content of the bar)
 * - $type (String): additional class suffix
 *
 * @see xc_browse_prepare_bar_data()
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
?>
<div id="xc-navigation-bar-<?php print $id ?>" class="xc-navigation-bar">
  <?php if (!empty($label)) : ?>
    <div class="xc-navigation-bar-label"><?php print $label ?></div>
  <?php endif ?>

  <div class="xc-navigation-bar-content">
    <?php print theme('item_list', array('items' => $items, 'title' => NULL, 'type' => 'ul',
      'attributes' => array(
        'class' => array('xc-navigation-bar-list' . (is_null($type) ? '' : '-' . $type))))); ?>
  </div>
</div>
