<?php
/**
 * @file
 * Default theme implementation to display workbench menu settings table.
 *
 * Available Variables:
 *
 * Content Variables:
 * - $table: An array containing the menu items table variables.
 * - $add_path: A url path for performing the item add operation.
 */
?>
<div class="workbench_menu menu_settings-table-wrapper">
  <?php print(theme('table', $menu_items_table)); ?>

  <div class="menu_settings-add_item_link">
    <?php print(l(t("Add new menu item."), $add_path)); ?>
  </div>

  <?php if (!empty($disabled_items_table['rows'])) print(theme('table', $disabled_items_table)); ?>
</div>
