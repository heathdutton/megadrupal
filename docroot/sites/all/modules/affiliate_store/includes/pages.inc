<?php

/**
 * @file
 * Callback pages and forms.
 */

/**
 * Menu callback; Return a page with list of affiliate store products.
 *
 * @param string $str_tids
 *   (optional) Term ID format string of products to display. Default to empty
 *   meaning display all.
 */
function affiliate_store_page($str_tids = '') {
  $active = affiliate_store_terms_parse_string($str_tids);
  $breadcrumb = array(l(t('Home'), NULL));
  $breadcrumb[] = l(affiliate_store_title(t('Store')), 'affiliate-store');
  $browse_title = t('Category');
  $browse_items = array();
  // The category tree is used for generating browse links.
  $tree = affiliate_store_get_category_tree();

  // Store the currently selected term, its term name or synonym, and its
  // vocabulary.
  $term = NULL;
  $name = NULL;
  $vocab = NULL;
  // Generate trail from active terms.
  // Additional term IDs in URL are the first term ID group's parents.
  for ($len = count($active), $i = $len - 1; $i >= 0; --$i) {
    $term = taxonomy_term_load($active[$i][0]);
    if (!$term) {
      continue;
    }
    $vocab = affiliate_store_get_vocabulary_name($term->vid);
    if (!$vocab) {
      continue;
    }
    $name = affiliate_store_get_term_synonym($term, $term->name);

    $path = implode('+', $active[$i]);
    // Add parent paths.
    for ($j = $i + 1; $j < $len; ++$j) {
      $path .= ',' . implode('+', $active[$j]);
    }
    $breadcrumb[] = l($name, "affiliate-store/term/$path");
  }

  $nids = array();
  // Category, subcategory, or subsubcategory term selected.
  $categories = array('category', 'subcategory', 'subsubcategory');
  if (in_array($vocab, $categories, TRUE)) {
    $browse_title = check_plain($name);
    $browse_items = _affiliate_store_select_browse_links($tree, $vocab);
    $nids = _affiliate_store_select_nodes($active);
  }
  // Term other than category and subcategory has been selected, e.g. network
  // or merchant term. Display nodes the same way as taxonomy do for single
  // term.
  elseif ($term) {
    $nids = _affiliate_store_select_nodes(array(array($term->tid)));
  }
  // No term selected, display all categories.
  else {
    $tids = array();
    foreach ($tree as $category) {
      $tids = array_merge($tids, $category['tids']);
      $browse_items[] = l($category['name'], $category['path']);
    }
    $tids = array_unique($tids);
    if ($tids) {
      $nids = _affiliate_store_select_nodes(array($tids));
    }
  }

  drupal_set_breadcrumb($breadcrumb);

  $build = array(
    '#theme' => 'affiliate_store',
    '#browse_title' => $browse_title,
    '#browse_items' => $browse_items,
    '#term' => $term,
  );
  $build['message'] = array(
    '#markup' => filter_xss_admin(variable_get('affiliate_store_message', '')),
  );
  $build['browse'] = array(
    '#type' => 'fieldset',
    '#title' => $browse_title,
  );
  $build['browse']['items'] = array(
    '#theme' => 'item_list',
    '#items' => $browse_items,
  );

  if ($nids) {
    $build += node_view_multiple(node_load_multiple($nids));
  }
  else {
    $build['nodes'] = array(
      '#prefix' => '<p>',
      '#markup' => $term ?
        t('There are currently no products in this category.') :
        t('There are currently no products.'),
      '#suffix' => '</p>',
    );
  }
  $build['pager'] = array('#theme' => 'pager');

  $path = drupal_get_path('module', 'affiliate_store');
  $build['#attached']['css'][] = "$path/css/affiliate_store.css";
  return $build;
}

/**
 * Find all active child terms of a parent term, and format them as browse
 * links.
 *
 * @param array &$tree
 *   A reference to category tree.
 * @param string $vocabulary
 *   Vocabulary logical name of parent term.
 *
 * @return array
 *   Active child terms formatted as browse links.
 *
 * @see affiliate_store_vocabularies()
 */
function _affiliate_store_select_browse_links(&$tree, $vocabulary) {
  $result = array();
  foreach ($tree as $branch) {
    if (!empty($branch['parent']['active'])
      && ($branch['parent']['vocabulary'] === $vocabulary)
    ) {
      $result[] = l($branch['name'], $branch['path']);
    }
    if (isset($branch['children'])) {
      $func = __FUNCTION__;
      $result = array_merge($result, $func($branch['children'], $vocabulary));
    }
  }
  return $result;
}

/**
 * Finds all nodes that match selected terms for used with a pager.
 *
 * @param array $tids
 *   An array of term ID groups.
 * @param array $order
 *   (optional) An associative array of orders with fields as keys and
 *   directions as values. The field must be in the format
 *   {table_alias}.{field_name}. Available table aliases are:
 *   - n: node
 *   - p: affiliate_store_product
 *   - t: taxonomy_index
 *   Default to n.sticky => DESC, n.title => ASC, p.price => ASC.
 *
 * @return array
 *   Node IDs.
 *
 * @see affiliate_store_terms_parse_string()
 */
function _affiliate_store_select_nodes($tids,
$order = array('n.sticky' => 'DESC', 'n.title' => 'ASC', 'p.price' => 'ASC')) {
  $query = db_select('node', 'n')
    ->addTag('node_access')
    ->extend('PagerDefault')
    ->limit(variable_get('default_nodes_main', 10))
    ->distinct()
    ->fields('n', array('nid'))
    ->condition('n.status', 1)
    ->condition('n.type', 'affiliate_store_product');
  $query->join('affiliate_store_product', 'p', 'p.vid = n.vid');
  foreach ($tids as $idx => $group) {
    $query->join('taxonomy_index', "t$idx", "t$idx.nid = n.nid");
    $query->condition("t$idx.tid", $group);
  }
  foreach ($order as $field => $direction) {
    $query->orderBy($field, $direction);
    // ORDER BY fields need to be selected too.
    list($table_alias, $name) = explode('.', $field);
    $query->addField($table_alias, $name);
  }
  return $query->execute()->fetchCol();
}
