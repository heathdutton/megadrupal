<?php
/**
 * @file
 * Default theme implementation to display a single workbench menu list item.
 *
 * 'data': an array with the following keys:
 *   - 'menu_id': The id of the menu in which an item belongs to.
 *   - 'even_menu': A boolean representing whether or not the item's menu is
 *   even or odd.
 *   - 'menu_count': An auto-incremented number assigned to the menu.
 *   - 'settings': An array containing all settings associated with the menu
 *   item.
 *   - 'even_item': A boolean representing whether or not this item is even or
 *   odd.
 *   - 'count_item': An auto-incremented number assigned to the item.
 *   - 'even_item_local': An auto-incremented number assigned to the item that
 *   is reset between recursive calls.
 *   - 'count_item_local':
 *   - 'depth': An auto-incremented number that is incremented each time this
 *   function recurses.
 *   - 'trail_is_active': A boolean representing whether or not the item or its
 *   children is active and thus the trail is active.
 *   - 'invalid_trail_active': A boolean representing that the trail is active
 *   but there is no actual active item because the path is invalid (or has no
 *   item associated with it).
 *   - 'clickable_label': A boolean representing that the item is a clickable
 *   label and so the paths are not active, but should be made available for
 *   the client (via js and/or css) to make it active or not.
 *   - 'path_current': A string containing the complete url path that is
 *   currently active.
 *   - 'active_text': an additional string used to improve accessibility.
 *   - 'child_list': an already processsed child list string.
 *
 * @see workbench_menu_render_menus()
 */

if (!empty($data['settings'])) {
  $settings = & $data['settings'][$data['id']];
  $classes = implode(' ', (array) $settings['attributes']['class']);

  $custom = $settings['attributes'];
  $custom['class'] = array('menu_item-text');
  $attributes = ' ' . drupal_attributes($custom);
?>
<li class="<?php print($classes); ?>">
  <?php if (!empty($settings['attributes']['href'])) { ?>
    <a<?php print($attributes); ?>><?php print($settings['prefix']); ?><?php print($settings['label'] . $data['active_text']); ?><?php print($settings['postfix']); ?></a>
  <?php } else { ?>
    <span<?php print($attributes); ?>><?php print($settings['prefix']); ?><?php print($settings['label'] . $data['active_text']); ?><?php print($settings['postfix']); ?></span>
  <?php } ?>

  <?php if (!empty($data['child_list'])) {
    $list = array();
    $list['items'] = $data['child_list'];
    $list['attributes'] = array();
    $list['attributes']['class'] = array();
    $list['attributes']['class'][] = 'menu_item-children';

    print(theme('workbench_menu_list', array('list' => $list, 'data' => $data, 'child' => TRUE)));
  } ?>
</li>
<?php } ?>
