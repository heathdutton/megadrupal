<?php
/**
 * @file
 * Contains contextual link logic.
 */

/**
 * Implements hook_contextual_links_view_alter().
 */
function i18n_menu_overview_contextual_links_view_alter(&$element, $items) {
  if (isset($element['#element']['#config'])) {
    global $language;

    $menu_name = $element['#element']['#config']['menu_name'];
    $lang = $language->language;

    $element['#links']['i18n_menu_overview'] = array(
      'title' => 'i18n menu overview: ' . $menu_name . ' - ' . $lang,
      'href' => 'admin/structure/' . $menu_name . '_' . $lang
    );
  }
}
