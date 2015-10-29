<?php
/**
 * @file
 * User-facing page callbacks for the menutree module.
 *
 */


/**
 * Menu callback; Display a menu tree for a single specified menu.
 *
 * @param $menu
 *  The menu to display.
 */
function menutree_display_page($menu) {
  return menutree_display($menu);
}

/**
 * Menu callback; Display a fully-expanded version of all flagged menus.
 *
 */
function menutree_display_all() {
  $trees = array();

  foreach (menu_get_menus() as $menu_name => $menu_title) {
    $weight = trim(variable_get('menutree_all_weight_' . $menu_name, '<none>'));
    if (is_numeric($weight)) {
      $trees[$menu_name] = menutree_display(menu_load($menu_name), MENUTREE_TITLE_BODY);
      // Don't display empty trees on the main index.
      if (empty($trees[$menu_name]['#tree'])) {
        unset($trees[$menu_name]);
      }
      else {
        $trees[$menu_name]['#weight'] = (int) $weight;
      }
    }
  }

  return $trees;
}

/**
 * Display a fully-expanded version of the menu specified on the path.
 *
 * @param $menu
 *   The menu to display.
 * @param $title_display
 *   How to handle the display of the title.  This is a bitmask that can take
 *   multiple values.
 *     - MENUTREE_TITLE_PAGE: Set the title of this menu as the page title.
 *     - MENUTREE_TITLE_BODY: Include the title of the menu in the body of the page.
 */
function menutree_display($menu, $title_display = MENUTREE_TITLE_PAGE) {
  // The title could be displayed in various ways.
  $title = variable_get('menutree_title_' . $menu['menu_name'], '');
  if (!$title) {
    $title = $menu['title'];
  }
  $title = check_plain($title);

  if ($title_display & MENUTREE_TITLE_PAGE) {
    drupal_set_title($title);
    $title = '';
  }

  // Output custom intro text.
  $intro = variable_get('menutree_intro_text_' . $menu['menu_name'], '');
  $description = '';
  if (!empty($intro)) {
    $description = check_markup($intro, FILTER_FORMAT_DEFAULT, FALSE);
  }

  $tree = menu_tree_output(menu_tree_all_data($menu['menu_name']));

  return array(
    '#theme' => 'menutree_tree__' . str_replace('-', '_', $menu['menu_name']),
    '#menu_name' => $menu['menu_name'],
    '#title' => $title,
    '#description' => $description,
    '#tree' => $tree,
  );
}

/**
 * Preprocess variables for menutree trees.
 */
function template_preprocess_menutree_tree(&$variables) {
  $variables['classes_array'][] = 'menutree-' . $variables['menu_name'];
}
