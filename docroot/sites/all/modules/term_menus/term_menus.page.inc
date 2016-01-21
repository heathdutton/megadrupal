<?php

/**
 * @file
 * Term menu pages callback.
 */

/**
 * Page callback for term menu custom page.
 *
 * This page is almost the same page with default term page. The only
 * difference is it only contains nodes which belong to current term menu.
 *
 * There's another feature for this term page, that it could include child
 * terms' nodes as well, if this term menu has been set to inlcude them.
 *
 * @param object $term_menu
 *   A term_menu object associated with term menu name.
 *
 * @param object $term
 *   A term object this page is showing.
 */
function term_menus_term_custom_page($term_menu, $term) {
  // Code comes from taxonomy_term_page, with some adjusting.
  // If there is a menu link to this term, the link becomes the last part of
  // the active trail, and the link name becomes the page title. Thus, we must
  // explicitly set the page title to be the term title.
  drupal_set_title($term->name);

  // Build breadcrumb based on the hierarchy of the term.
  $current = (object) array(
    'tid' => $term->tid,
  );
  // @todo This overrides any other possible breadcrumb and is a pure hard-coded
  //   presumption. Make this behavior configurable per vocabulary or term.
  $breadcrumb = array();
  while ($parents = taxonomy_get_parents($current->tid)) {
    $current = array_shift($parents);
    $breadcrumb[] = l($current->name, 'term_menus/' . $term_menu->menu_name . '/' . $current->tid);
  }
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb = array_reverse($breadcrumb);
  drupal_set_breadcrumb($breadcrumb);

  // Here we build nodes.
  $build = array();

  // Get all nids connected to this term inside the menu.
  $nids = term_menus_get_term_all_nodes($term, $term_menu);

  if (!empty($nids)) {
    $nodes = node_load_multiple($nids);
    $build += node_view_multiple($nodes);
    $build['pager'] = array(
      '#theme' => 'pager',
      '#weight' => 5,
    );
  }
  else {
    $build['no_content'] = array(
      '#prefix' => '<p>',
      '#markup' => t('There is currently no content classified with this term.'),
      '#suffix' => '</p>',
    );
  }
  return $build;
}

/**
 * Term menus overview page.
 */
function term_menus_overview_page() {
  $output = t('This list shows all term menus in site.<br />');
  $term_menus = _term_menus_get_menus();
  if (empty($term_menus)) {
    $output .= t("There's no term menus enabled in site now. You may enable term menu for taxonomy field at taxonomy field's editing page, e.g., admin/structure/types/manage/article/fields/field_test_term.");
    return $output;
  }

  $header = array(
    t('Field name'),
    t('Term menu'),
    t('Used in'),
  );
  $rows = array();
  $fields = field_info_field_map();
  $menu_path = 'admin/structure/menu/manage/';
  $type_path = 'admin/structure/types/manage/';
  foreach ($term_menus as $term_menu) {
    $bundles = $fields[$term_menu->field_name]['bundles']['node'];
    $types = '';
    $count = count($bundles);
    foreach ($bundles as $key => $bundle) {
      $type = node_type_load($bundle);
      $types .= l(check_plain($type->name), $type_path . $bundle . '/fields');
      if ($count != $key + 1) {
        $types .= ', ';
      }
    }
    $menu = menu_load($term_menu->menu_name);
    $data = array(
      'field_name' => $term_menu->field_name,
      'term_menu' => l(check_plain($menu['title']), $menu_path . $term_menu->menu_name),
      'used_in' => $types,
    );
    $rows[] = array('data' => $data);
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}
