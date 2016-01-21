<?php
/**
 * @file
 * Callbacks and theme functions for settings_menu_link.module.
 */

/**
 * Menu callback.
 *
 * Renders an overview page form the childeren.
 */
function settings_menu_link_overview() {

  // Add the CSS.
  drupal_add_css(drupal_get_path('module', 'settings_menu_link') . '/settings_menu_link.overview.css');

  $parent = menu_get_item();
  $menu_link = db_select('menu_links')
      ->fields('menu_links')
      ->condition('router_path', $parent['path'])
      ->execute()
      ->fetchAssoc();
  $parent += $menu_link;
  if ($content = settings_menu_link_menu_block($parent)) {
    $root_items = array();
    $blocks = array();
    $blocks[] = '';
    foreach ($content as $item) {
      if ($item['module'] == 'settings_menu_link') {
        if ($children = settings_menu_link_menu_block($item)) {
          $item['content'] = theme('settings_menu_link_overview_block_content', array('items' => $children));
        }
        $blocks[] = theme('settings_menu_link_overview_block', $item);
      }
      else {
        $root_items[] = $item;
      }
    }
    if (count($root_items)) {
      $options = unserialize($menu_link['options']);
      if (!empty($options['attributes']['title'])) {
        $parent['description'] = t($options['attributes']['title']);
      }
      $parent['content'] = theme('settings_menu_link_overview_block_content', array('items' => $root_items));
      $blocks[0] = theme('settings_menu_link_overview_block', $parent);
    }

    $output = theme('settings_menu_link_overview_page', array('blocks' => $blocks));
  }
  else {
    $output = t('You do not have any settings items.');
  }
  return $output;
}

function theme_settings_menu_link_overview_page($variables) {
  $html = '<div id="settings-menu-link-overview" class="' . (count($variables['blocks'])>1?'blocks-multiple':'blocks-single') . '">';
  foreach ($variables['blocks'] as $block) {
    $html .= $block;
  }
  $html .= '</div>';
  return $html;
}

function theme_settings_menu_link_overview_block($variables) {
  $output = '<div class="admin-panel">';
  if (!empty($variables['title'])) {
    $output .= '<h3>' . $variables['title'] . '</h3>';
  }
  if (!empty($variables['description'])) {
    $output .= '<div class="description">' . $variables['description'] . '</div>';
  }
  if (!empty($variables['content'])) {
    $output .= '<div class="body">' . $variables['content'] . '</div>';
  }

  $output .= '</div>';

  return $output;
}


function theme_settings_menu_link_overview_block_content($variables) {
  $items = $variables['items'];
  $output = '';
  if (!empty($items)) {
    $output .= '<dl class="admin-list">';
    foreach ($items as $item) {
      $output .= '<dt>' . l($item['title'], $item['href'], $item['localized_options']) . '</dt>';
      if (isset($item['description'])) {
        $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
      }
    }
    $output .= '</dl>';
  }
  return $output;
}