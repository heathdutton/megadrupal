<?php

/**
 * @file
 * Building of custom add content page.
 */

/**
 * Displays add content links given custom_add_content menu configuration.
 *
 * Redirects to node/add/[type] if only one content type is available.
 *
 * @see node_menu()
 */
function custom_add_content_node_add_page() {

  $tree = menu_tree_all_data('custom_add_content_page');

  if (is_array($tree) && count($tree) == 0) {
    $output = '<p>' . t('Please, make sure custom_add_content_page menu has links.') . '</p>';
  }
  else {
    $output = theme('custom_add_content_page_add', array('tree' => $tree));
  }

  return $output;
}

/**
 * Render of custom_add_content_page menu items.
 *
 * @ingroup themeable
 */
function theme_custom_add_content_page_add($variables) {
  $tree = $variables['tree'];

  if (is_array($tree) && count($tree) > 0) {

    $renderer = variable_get('custom_add_content_renderer', 1);

    if ($renderer == 0) {
      // Drupal's menu_tree_output function.
      $menu = menu_tree_output($tree);
      $output = drupal_render($menu);
    }
    else {
      // Module's custom renderer.
      $output = custom_add_content_renderer($tree);
    }

  }
  else {
    $output = '<p>' . t('You have not created any content types yet. Go to the <a href="@create-content">content type creation page</a> to add a new content type.', array('@create-content' => url('admin/structure/types/add'))) . '</p>';
  }

  return $output;
}

/**
 * Module's custom renderer.
 *
 * @see theme_node_add_list()
 */
function custom_add_content_renderer($tree) {
  $output = '<ul class="admin-list">';
  foreach ($tree as $item) {
    $link = $item['link'];
    $below = $item['below'];
    if ($link['hidden'] != 1) {
      $output .= '<li>' . l($link['title'], $link['href'], $link['localized_options']) . '</li>';
      if (count($below) > 0) {
        $output .= custom_add_content_renderer($below);
      }
    }
  }
  $output .= '</ul>';

  return $output;
}
