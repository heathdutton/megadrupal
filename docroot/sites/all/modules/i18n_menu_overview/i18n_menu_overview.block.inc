<?php
/**
 * @file
 * Contains block logic. For now contextual links.
 */

/**
 * Implements hook_block_view_alter().
 */
function i18n_menu_overview_block_view_alter(&$data, $block) {
  $allowed_navigation_blocks_delta = array(
    'navigation',
  );

  $allowed_navigation_blocks_module = array(
    'superfish',
  );

  if (in_array($block->module,  $allowed_navigation_blocks_module)
      || in_array($block->delta, $allowed_navigation_blocks_delta)) {

    list($menu_name, $language)
      = _i18n_menu_overview_get_correct_menu_by_block($block);

    $data['content']['#contextual_links']['i18n_menu_overview']
      = array('admin/structure/i18n_menu/',
              array($menu_name, $language),
    );
  }
}

/**
 * Retrieves the correct menu name and to redirect to.
 */
function _i18n_menu_overview_get_correct_menu_by_block($block) {
  global $language;
  // ie: admin/structure/main-menu_en

  switch ($block->module) {
    case 'superfish':

      list($menu_name, $mlid)
        = explode(':', variable_get('superfish_menu_' . $block->delta,
        'main-menu:0'));

      break;

    default:
      $menu_name = 'main-menu';
  }

  return array($menu_name, $language->language);
}
