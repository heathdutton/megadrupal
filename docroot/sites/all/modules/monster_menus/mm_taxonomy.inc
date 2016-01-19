<?php
// $Id: mm_taxonomy.inc 4216 2010-06-25 18:36:37Z dan $

/**
 * @file
 * Versions of taxonomy module functions which take MM permissions into account.
 * Without these, the user gets results that can't be viewed.
 */

/**
 * Return nodes attached to a term across all field instances.
 * Originally from taxonomy_select_nodes() in taxonomy.module.
 *
 * This function requires taxonomy module to be maintaining its own tables,
 * and will return an empty array if it is not. If using other field storage
 * methods alternatives methods for listing terms will need to be used.
 *
 * @param $tid
 *   The term ID.
 * @param $pager
 *   Boolean to indicate whether a pager should be used.
 * @param $limit
 *   Integer. The maximum number of nodes to find.
 *   Set to FALSE for no limit.
 * @order
 *   An array of fields and directions.
 *
 * @return
 *   An array of nids matching the query.
 */
function mm_taxonomy_select_nodes($tid, $pager = TRUE, $limit = FALSE, $order = array('t.sticky' => 'DESC', 't.created' => 'DESC')) {
  if (!variable_get('taxonomy_maintain_index_table', TRUE)) {
    return array();
  }
  $query = db_select('taxonomy_index', 't');
  $query->addTag('node_access');
  $query->condition('tid', $tid);

  // LOCALMOD START: filter out recycled items.
  $query->leftJoin('mm_recycle', 'r', 'r.id = t.nid AND r.type = :type', array(':type' => 'node'));
  $query->isNull('r.id');
  // LOCALMOD END: filter out recycled items.

  if ($pager) {
    $count_query = clone $query;
    $count_query->addExpression('COUNT(t.nid)');

    $query = $query->extend('PagerDefault');
    if ($limit !== FALSE) {
      $query = $query->limit($limit);
    }
    $query->setCountQuery($count_query);
  }
  else {
    if ($limit !== FALSE) {
      $query->range(0, $limit);
    }
  }
  $query->addField('t', 'nid');
  $query->addField('t', 'tid');
  foreach ($order as $field => $direction) {
    $query->orderBy($field, $direction);
    // ORDER BY fields need to be loaded too, assume they are in the form
    // table_alias.name
    list($table_alias, $name) = explode('.', $field);
    $query->addField($table_alias, $name);
  }

  // LOCALMOD: Check MM user access.
  // return $query->execute()->fetchCol();
  $nids = $query->execute()->fetchCol();
  $nids_visible = array();
  foreach ($nids as $nid) {
    if (mm_content_user_can_node($nid, MM_PERMS_READ))
      $nids_visible[] = $nid;
  }
  return $nids_visible;
}

/**
 * Menu callback; displays all nodes associated with a term.
 * Originally from taxonomy_term_page() in taxonomy.pages.inc.
 *
 * @param $term
 *   The taxonomy term.
 * @return
 *   The page content.
 */
function mm_taxonomy_term_page($term) {
  // Build breadcrumb based on the hierarchy of the term.
  $current = (object) array(
    'tid' => $term->tid,
  );
  // @todo This overrides any other possible breadcrumb and is a pure hard-coded
  //   presumption. Make this behavior configurable per vocabulary or term.
  $breadcrumb = array();
  while ($parents = taxonomy_get_parents($current->tid)) {
    $current = array_shift($parents);
    $breadcrumb[] = l($current->name, 'taxonomy/term/' . $current->tid);
  }
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb = array_reverse($breadcrumb);
  drupal_set_breadcrumb($breadcrumb);
  drupal_add_feed('taxonomy/term/' . $term->tid . '/feed', 'RSS - ' . $term->name);

  $build = array();

  $build['term_heading'] = array(
    '#prefix' => '<div class="term-listing-heading">',
    '#suffix' => '</div>',
    'term' => taxonomy_term_view($term, 'full'),
  );

  // LOCALMOD: was taxonomy_select_nodes
  if ($nids = mm_taxonomy_select_nodes($term->tid, TRUE, variable_get('default_nodes_main', 10))) {
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
 * Generate the content feed for a taxonomy term.
 * Originally from taxonomy_term_feed() in taxonomy.pages.inc.
 *
 * @param $term
 *   The taxonomy term.
 */
function mm_taxonomy_term_feed($term) {
  if (is_object($term) && !empty($term->tid)) {
    $channel['link'] = url('taxonomy/term/' . $term->tid, array('absolute' => TRUE));
    $channel['title'] = variable_get('site_name', 'Drupal') . ' - ' . $term->name;
    // Only display the description if we have a single term, to avoid clutter and confusion.
    // HTML will be removed from feed description.
    $channel['description'] = check_markup($term->description, $term->format, '', TRUE);
    // LOCALMOD: was taxonomy_select_nodes
    $nids = mm_taxonomy_select_nodes($term->tid, FALSE, variable_get('feed_default_items', 10));

    node_feed($nids, $channel);
  }
}