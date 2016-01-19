<?php

/**
 * @file
 * Contains the theme's functions to manipulate Drupal's default markup.
 */

/**
 * @mainpage
 *
 * A responsive theme for the website @link http://pirckheimer-gymnasium.de
 * pirckheimer-gymnasium.de @endlink made with zen grids, based on drupal.
 *
 * @section id_and_css CSS id and class structure
 *
 * - #page
 *   - #header
 *     - #head-banner
 *       - #secondary-menu
 *       - #top
 *         - .region-top-first
 *         - .region-top-second
 *         - .region-top-third
 *     - #identity
 *       - a#logo
 *       - #name-and-slogan
 *         - #site-name
 *         - #site-slogan
 *     - #section
 *       - .region-section-first
 *       - .region-section-second
 *     - #main-menu
 *   - #main
 *     - .region-feature
 *     - #content
 *       - .region-highlighted
 *       - .region-content
 *     - #sidebars
 *       - .region-sidebar-first
 *       - .region-sidebar-second
 *   - #footer
 *     - .region-footer
 *     - #bottom
 *       - .region-bottom-first
 *       - .region-bottom-second
 *     - #tertiary-menu
 *
 * @section colors Colors
 *
 * - orange: #d67c1c
 * - dark-blue: #485873
 * - light-blue: #78adb8
 * - green: #a2c037
 * - light-grey: #eee
 * - grey: #8a95a7
 * - dark-grey: #727b8a
 * - white: #fff
 *
 * - color-default: dark-grey
 * - color-1: orange
 * - color-2: dark-blue
 * - color-3: green
 * - color-4: light-blue
 *
 * @section fonts Fonts
 *
 * - font-normal: PGNSansRegular, Arial, Tahoma, Sans-serif
 * - font-bold: PGNSansBlack, Arial Black
 * - courier:  Courier New, DejaVu Sans Mono, monospace, sans-serif
 *
 * @section files Files
 *
 * @subsection settings Settings
 *
 * - @link template.php template.php @endlink
 * - @link theme-settings.php theme-settings.php @endlink
 *
 * @subsection templates Templates
 *
 * - @link templates/page.tpl.php page.tpl.php @endlink
 * - @link templates/node--pgn-group.tpl.php node--pgn-group.tpl.php @endlink
 * - @link templates/book-navigation.tpl.php book-navigation.tpl.php @endlink
 * - @link templates/panels-pane.tpl.php panels-pane.tpl.php @endlink
 */

/**
 * Implements hook_css_alter().
 */
function willibald_css_alter(&$css) {
  unset($css['modules/system/system.menus.css']);
  unset($css['modules/system/system.messages.css']);

  unset($css['modules/book/book.css']);
  unset($css['modules/comment/comment.css']);
  unset($css['sites/all/modules/custom_search/custom_search.css']);
  unset($css['sites/all/modules/logintoboggan/logintoboggan.css']);
  unset($css['sites/all/modules/print/css/printlinks.css']);
}

/**
 * Add addtional informations as CSS classes to a menu item.
 *
 * Add the menu link ID (mlid) and depth count of the level as CSS classes to
 * all menu items.
 *
 * @see theme_menu_link()
 *
 * @ingroup themable
 */
function willibald_menu_link(array $variables) {
  $element = $variables['element'];

  $element['#attributes']['class'][] = 'menu-' . $element['#original_link']['mlid'];
  $element['#attributes']['class'][] = 'level-' . $element['#original_link']['depth'];

  $sub_menu = '';
  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/**
 * Preprocessor for html.tpl.php template file.
 */
function willibald_preprocess_html(&$vars) {

  // Load css files only if module exists and is enabled.
  $module_support = array(
    'book',
    'comment',
    'custom_search',
    'field_group',
    'forum',
    'logintoboggan',
    'print',
    'taxonomy'
  );

  foreach ($module_support as $module) {
    _willibald_add_css($module);
  }
}

/**
 * Add css files only if module exists and is enbled.
 *
 * @param string $module
 *   Name of the module.
 */
function _willibald_add_css($module) {
  $path = drupal_get_path('theme', 'willibald') . '/css/module-support/';
    if (module_exists('taxonomy')) {
    drupal_add_css($path . 'willibald.' . str_replace('_', '-', $module) . '.css');
  }
}

/**
 * Preprocessor for page.tpl.php template file.
 */
function willibald_preprocess_page(&$variables, $hook) {

  $page = $variables['page'];

  // Top
  $variables['top_first']  = render($page['top_first']);
  $variables['top_second'] = render($page['top_second']);
  $variables['top_third'] = render($page['top_third']);

  if ($variables['top_first'] || $variables['top_second'] || $variables['top_third']) {
    $variables['show_top'] = TRUE;
  }
  else {
    $variables['show_top'] = FALSE;
  }

  // Head banner
  if ($variables['secondary_menu'] || $variables['show_top']) {
    $variables['show_head_banner'] = TRUE;
  }
  else {
    $variables['show_head_banner'] = FALSE;
  }

  // Identity
  if ($variables['logo'] || $variables['site_name'] || $variables['site_slogan']) {
    $variables['show_identity'] = TRUE;
  }
  else {
    $variables['show_identity'] = FALSE;
  }

  // Section
  $variables['section_first'] = render($page['section_first']);
  $variables['section_second'] = render($page['section_second']);

  if ($variables['section_first'] || $variables['section_second']) {
    $variables['show_section'] = TRUE;
  }
  else {
    $variables['show_section'] = FALSE;
  }

  // Main menu
  if (theme_get_setting('toggle_main_menu')) {
    $menu_tree = menu_tree_all_data('main-menu');
    $variables['main_menu'] = _willibald_render_main_menu($menu_tree, $variables);
  }
  else {
    $variables['main_menu'] = NULL;
  }

  // Sidebars
  $variables['sidebar_first'] = render($page['sidebar_first']);
  $variables['sidebar_second'] = render($page['sidebar_second']);

  if ($variables['sidebar_first'] || $variables['sidebar_second']) {
    $variables['show_sidebars'] = TRUE;
  }
  else {
    $variables['show_sidebars'] = FALSE;
  }

  // Bottom
  $variables['bottom_first'] = render($page['bottom_first']);
  $variables['bottom_second'] = render($page['bottom_second']);

  if ($variables['bottom_first'] || $variables['bottom_second']) {
    $variables['show_bottom'] = TRUE;
  }
  else {
    $variables['show_bottom'] = FALSE;
  }

  // Secondary menu
  $secondary_links = variable_get('menu_secondary_links_source', 'user-menu');
  if ($secondary_links) {
    $menus = function_exists('menu_get_menus') ? menu_get_menus() : menu_list_system_menus();
    $variables['secondary_menu_heading'] = $menus[$secondary_links];
  }
  else {
    $variables['secondary_menu_heading'] = '';
  }

  if ($variables['secondary_menu']) {
    $variables['secondary_menu_rendered'] = _willibald_inline_menu($variables['secondary_menu'], $variables['secondary_menu_heading']);
  }
  else {
    $variables['secondary_menu_rendered'] = '';
  }

  // Tertiary menu
  if (theme_get_setting('toggle_tertiary_menu')) {
    $tertiary_links = theme_get_setting('tertiary_menu');
    $variables['tertiary_menu'] = menu_navigation_links($tertiary_links);
    $menus = function_exists('menu_get_menus') ? menu_get_menus() : menu_list_system_menus();
    $variables['tertiary_menu_heading'] = $menus[$tertiary_links];
    $variables['tertiary_menu_rendered'] = _willibald_inline_menu($variables['tertiary_menu'], $variables['tertiary_menu_heading']);
  }
  else {
    $variables['tertiary_menu'] = '';
    $variables['tertiary_menu_heading'] = '';
    $variables['tertiary_menu_rendered'] = '';
  }

}

/**
 * Render inline menu links for the secondary and tertiary menu.
 *
 * @param array $menu
 *   Menu links array. Output from menu_navigation_links().
 * @param string $menu_heading
 *   The heading of menu
 *
 * @return
 *   The rendered menu.
 */
function _willibald_inline_menu($menu, $menu_heading) {
  return theme('links__system_secondary_menu', array(
      'links' => $menu,
      'attributes' => array(
        'class' => array('links', 'inline', 'clearfix'),
      ),
      'heading' => array(
        'text' => $menu_heading,
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    )
  );
}

/**
 * Puts as first menu entry a link to the home of the website.
 *
 * This can be disable in the theme settings.
 *
 * @param array $menu_tree
 *   A menu tree array.
 * @param array $variables
 *   Value array from the theme function.
 *
 * @return
 *   Return a rendered output of the main menu.
 */
function _willibald_render_main_menu($menu_tree, $variables) {
  $tree = menu_tree_output($menu_tree);

  foreach ($tree as $key1 => $level1) {
    if (!empty($level1['#below'])) {
      $count = array();
      foreach ($level1['#below'] as $key2 => $level2) {
        if (!empty($level2['#below'])) {
          $count[] = count($level2['#below']) - 2;
        }
      }
      sort($count);
      $tree[$key1]['#attributes']['class'][] = 'max-' . array_pop($count);
    }
  }

  $output = render($tree);

  if (theme_get_setting('main_menu_home')) {
    $search = '<ul class="menu">';
    $home = t('Home');
    $link = '<a href="' . $variables['front_page'] . '" title="' . $home . '" rel="home">' . $home . '</a>';
    $link = '<ul class="menu">' . '<li class="level-1 front">' . $link . '</li>';

    return preg_replace('#' . $search . '#', $link, $output, 1);
  }
  else {
    return $output;
  }
}

/**
 * Implements theme_field__field_type().
 */
function willibald_field__taxonomy_term_reference($variables) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<h3 class="field-label">' . $variables['label'] . ': </h3>';
  }

  // Render the items.
  $output .= ($variables['element']['#label_display'] == 'inline') ? '<ul class="links inline">' : '<ul class="links">';
  foreach ($variables['items'] as $delta => $item) {
    $output .= '<li class="taxonomy-term-reference-' . $delta . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</li>';
  }
  $output .= '</ul>';

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . (!in_array('clearfix', $variables['classes_array']) ? ' clearfix' : '') . '"' . $variables['attributes'] .'>' . $output . '</div>';

  return $output;
}
