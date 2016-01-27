<?php


/**
 * Implements hook_dqx_adminmenu_items()
 */
function locale_dqx_adminmenu_items() {
  if (drupal_multilingual()) {
    $path = 'admin';
    $links = language_negotiation_get_switch_links('language', $path);
    if (isset($links->links)) {
      $items = array();
      foreach ($links->links as $lang => $link) {
        $item = array(
          'link_path' => $link['href'],
          'title' => $link['title'],
        );
        $item['localized_options']['language'] = $link['language'];
        $item['localized_options']['attributes']['class'] = 'dqx_adminmenu-link-variation';
        $items['dqx-adminmenu-user/language/' . $lang] = $item;
      }
      if (!empty($items)) {
        $item = array(
          'title' => t('Languages'),
          'link_path' => 'admin/config/regional/language',
          'weight' => -5,
        );
        $item['item_attributes']['class'] = 'dqx-adminmenu-languages';
        $item['localized_options']['attributes']['title'] = t('Languages');
        $items['dqx-adminmenu-user/language'] = $item;
        return $items;
      }
    }
  }
}
