<?php
/**
 * @file
 * Custom theme functions for the Lyris module.
 */
/**
 * Returns HTML for a list of available mailing lists for content creation.
 *
 * @param $vars
 *   An associative array containing:
 *   - content: An array of lists.
 *
 * @ingroup themeable
 */
function theme_lyris_add_page($vars) {
  $content = $vars['content'];
  $output = '';

  if ($content) {
    $output = '<dl class="node-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href'], $item['localized_options']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    $output = '<p>' . t('You have not created or imported any lists yet. Go to the <a href="@create-content">list create/import page</a> to add a list.', array('@create-content' => url('admin/structure/lyris'))) . '</p>';
  }
  return $output;
}

/**
 * Lyris Admin Links Block
 */
function theme_lyris_admin_links_block($vars) {
  $list = $vars['list'];
  $lc   = $vars['content'];
  $out = '';

  $options = array('query' => drupal_get_destination());

  if ($list) {
    $title = t('List Management') . '<br /><small>' . $list->getDisplayName() . '</small>';

    $items = array();
    $items[] = l(t('List Overview'),            "admin/structure/lyris/lists/{$list->eid}", $options);
    $items[] = l(t('Edit List Settings'),       "admin/structure/lyris/lists/{$list->eid}/edit", $options);
    $items[] = l(t('Default Content Settings'), "admin/structure/lyris/lists/{$list->eid}/defaults", $options);
    $items[] = l(t('View List Members'),        "admin/structure/lyris/lists/{$list->eid}/members", $options);
    $items[] = l(t('View List Content'),        "admin/content/lyris/{$list->eid}", $options);
    if (module_exists('field_api')) {
      $items[] = l(t('Content Fields'),  "admin/structure/lyris/lists/{$list->eid}/fields", $options);
      $items[] = l(t('Content Display'), "admin/structure/lyris/lists/{$list->eid}/display", $options);
    }
    $out .= theme('item_list', array('items' => $items, 'title' => $title));
  }

  if ($lc) {
    $title = t('Content Management') . '<br /><small>' . $lc->getDisplayName() . '</small>';

    $items = array();
    $items[] = l(t('View Content'),      "lyris/content/{$lc->eid}", $options);
    $items[] = l(t('Edit Content'),      "lyris/content/{$lc->eid}/edit", $options);
    $items[] = l(t('Preview/Send Mail'), "lyris/content/{$lc->eid}/mailings", $options);
    $out .= theme('item_list', array('items' => $items, 'title' => $title));
  }

  return $out;
}

/**
 * Overriding theme_item_list to remove wrapping div.
 */
function theme_item_list__lyris_helper_links($variables) {
  $items = $variables['items'];
  $title = $variables['title'];
  $type = $variables['type'];
  $attributes = $variables['attributes'];

  // Only output the list container and title, if there are any list items.
  // Check to see whether the block title exists before adding a header.
  // Empty headers are not semantic and present accessibility challenges.
  $output = '<span class="links">';
  if (isset($title) && $title !== '') {
    $output .= '<h3>' . $title . '</h3>';
  }

  if (!empty($items)) {
    $output .= "<$type" . drupal_attributes($attributes) . '>';
    $num_items = count($items);
    foreach ($items as $i => $item) {
      $attributes = array();
      $children = array();
      $data = '';
      if (is_array($item)) {
        foreach ($item as $key => $value) {
          if ($key == 'data') {
            $data = $value;
          }
          elseif ($key == 'children') {
            $children = $value;
          }
          else {
            $attributes[$key] = $value;
          }
        }
      }
      else {
        $data = $item;
      }
      if (count($children) > 0) {
        // Render nested list.
        $data .= theme_item_list(array('items' => $children, 'title' => NULL, 'type' => $type, 'attributes' => $attributes));
      }
      if ($i == 0) {
        $attributes['class'][] = 'first';
      }
      if ($i == $num_items - 1) {
        $attributes['class'][] = 'last';
      }
      $output .= '<li' . drupal_attributes($attributes) . '>' . $data . "</li>\n";
    }
    $output .= "</$type>";
  }
  $output .= '</span>';
  return $output;
}

/**
 * Returns HTML for a node type description for the content type admin overview page.
 *
 * @param $vars
 *   An associative array containing:
 *   - list: The Lyris list entry from the lyris_list table.
 *
 * @ingroup themeable
 */
function theme_lyris_list_admin_overview($vars) {
  $list = $vars['list'];

  $output = check_plain($list['title']);
  $output .= ' <small>' . t('(Lyris List: @type)', array('@type' => $list[llf('ListName')])) . '</small>';
  $output .= '<div class="description">' . filter_xss_admin($list[llf('ShortDescription')]) . '</div>';
  return $output;
}

/**
 * Theme the created/imported column of the list admin page.
 */
function theme_lyris_list_admin_created($vars) {
  $origin  = $vars['origin'];
  $created = $vars['created'];
  $creator = $vars['creator'];

  $args = array(
    '%created' => format_date($created, 'short'),
    '!creator' => theme('username', array('account' => $creator)),
  );

  return '<div class="meta-created">' . ($origin == LYRIS_LIST_NEW ? t('Created %created<br />by !creator', $args) : t('Imported %created<br />by !creator', $args)) . '</div>';
}

/**
 * Theme the updated column of the list admin page.
 */
function theme_lyris_list_admin_changed($vars) {
  $changed = $vars['changed'];
  $changer = $vars['changer'];

  $args = array(
    '%changed' => format_date($changed, 'short'),
    '!changer' => theme('username', array('account' => $changer)),
  );

  return '<div class="meta-changed">' . t('Updated %changed<br />by !changer', $args) . '</div>';
}

/**
 * Theme the optional mode indicator for admin pages.
 */
function theme_lyris_mode_indicator($vars) {
  drupal_add_css(LYRIS_MOD_PATH . '/lyris.css');
  $attributes = $vars['attributes'];
  $mode = variable_get('lyris_mode', LYRIS_MODE_DEFAULT);

  $attributes['class'] += array('lyris-mode-indicator', 'lyris-mode-' . $mode);

  return '<div ' . drupal_attributes($attributes) . '>' . t('Lyris Mode: %mode', array('%mode' => _lyris_options('lyris_mode', $mode))) . '</div>';
}
