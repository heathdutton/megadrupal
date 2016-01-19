<?php

/**
 * @file
 * taxonomy field menu pages callback.
 */

/**
 * Page callback for taxonomy field menu custom page.
 *
 * This page is almost the same page with default term page. The only
 * difference is it only contains nodes which belong to current taxonomy
 * field menu.
 *
 * There's another feature for this term page, that it could include child
 * terms' nodes as well, if this taxonomy field menu has been set to
 * inlcude them.
 *
 * @param object $taxonomy_field_menu
 *   A taxonomy_field_menu object associated with taxonomy field menu name.
 *
 * @param object $term
 *   A term object this page is showing.
 */
function taxonomy_field_menu_term_custom_page($taxonomy_field_menu, $term) {
  // Code comes from taxonomy_term_page, with some adjusting.
  // If there is a menu link to this term, the link becomes the last part of
  // the active trail, and the link name becomes the page title. Thus, we must
  // explicitly set the page title to be the term title.
  drupal_set_title($term->name);

  // Build breadcrumb based on the hierarchy of the term.
  $current = (object) array(
    'tid' => $term->tid,
  );
  // @Todo This overrides any other possible breadcrumb and is a pure
  // hard-coded presumption. Make this behavior configurable per vocabulary
  // or term.
  $breadcrumb = array();
  while ($parents = taxonomy_get_parents($current->tid)) {
    $current = array_shift($parents);
    $breadcrumb[] = l($current->name, 'taxonomy_field_menu/' . $taxonomy_field_menu->menu_name . '/' . $current->tid);
  }
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb = array_reverse($breadcrumb);
  drupal_set_breadcrumb($breadcrumb);

  // Here we build nodes.
  $build = array();

  // Get all nids connected to this term inside the menu.
  $nids = taxonomy_field_menu_get_term_all_nodes($term, $taxonomy_field_menu);

  if (!empty($nids)) {
    $nodes = node_load_multiple($nids);
    $build['nodes'] = node_view_multiple($nodes);
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
 * Taxonomy field menus overview page.
 */
function taxonomy_field_menu_overview_page() {
  $output = t('This list shows all taxonomy field menus in site.<br />');
  $taxonomy_field_menus = _taxonomy_field_menu_get_menus();
  if (empty($taxonomy_field_menus)) {
    $output .= t("There's no taxonomy field menu enabled in site now. You may enable taxonomy field menu for taxonomy field at taxonomy field's editing page, e.g., admin/structure/types/manage/article/fields/field_test_term.");
    return $output;
  }

  $header = array(
    t('Field name'),
    t('Taxonomy field menu'),
    t('Used in'),
  );
  $rows = array();
  $fields = field_info_field_map();
  $menu_path = 'admin/structure/menu/manage/';
  $type_path = 'admin/structure/types/manage/';
  foreach ($taxonomy_field_menus as $taxonomy_field_menu) {
    $bundles = $fields[$taxonomy_field_menu->field_name]['bundles']['node'];
    $types = '';
    $count = count($bundles);
    foreach ($bundles as $key => $bundle) {
      $type = node_type_load($bundle);
      $types .= l(check_plain($type->name), $type_path . $bundle . '/fields/' . $taxonomy_field_menu->field_name);
      if ($count != $key + 1) {
        $types .= ', ';
      }
    }
    $menu = menu_load($taxonomy_field_menu->menu_name);
    $data = array(
      'field_name' => $taxonomy_field_menu->field_name,
      'taxonomy_field_menu' => l(check_plain($menu['title']), $menu_path . $taxonomy_field_menu->menu_name),
      'used_in' => $types,
    );
    $rows[] = array('data' => $data);
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}
