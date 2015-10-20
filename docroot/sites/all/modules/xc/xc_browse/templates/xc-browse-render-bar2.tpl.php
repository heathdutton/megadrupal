<?php
/**
 * @file templates/xc-browse-render-bar.tpl.php
 * Default theme implementation of a single navigation bar on the browse form
 *
 * Available variables:
 * - $id: The id of the bar
 * - $label: The label of the bar
 * - $items (array): The list of displayable items (the content of the bar)
 * - $type (String): additional class suffix
 * - $show_bar (boolean): show or hide the content
 *
 * @see xc_browse_prepare_bar_data()
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
?>
<div id="xc-navigation-bar-<?php print $id ?>" class="xc-navigation-bar2">

  <?php if (!empty($label)) : ?>
    <div class="xc-navigation-bar-label"><a href="#" onclick="return XCBrowse.showContent();"><?php print $label ?></a></div>
  <?php endif ?>

  <div class="xc-navigation-bar-content2 text_exposed_<?php print ($show_bar ? 'show' : 'hide'); ?>">
    <table>
      <tr valign="top">
      <?php
        $limit = intval(count($items) / 3);
        for ($i=0; $i<3; $i++) :
          $offset = ($limit * $i);
          $slimit = ($i == 2) ? NULL : $limit;
      ?>
        <td width="33%">
          <?php print theme('item_list', array('items' => array_slice($items, $offset, $slimit), 'title' => NULL, 'type' => 'ul',
                'attributes' => array('class' => array('xc-navigation-bar-list2' . (is_null($type) ? '' : '-' . $type))))); ?>
        </td>
      <?php endfor; ?>
      </tr>
    </table>
  </div>
</div>
