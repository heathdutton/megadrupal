<?php
/**
 * @file
 * API for taxonomy_override module.
 *
 * taxonomy_override can't work out of the box.
 * You need to implements hooks on your own module.
 */

/**
 * Function hook_taxonomy_override_define_callback
 * Define the callback to use on vocabulary page settings
 * @return array
 */
function hook_taxonomy_override_define_callback() {
  return array('mymodule_taxonomy_page' => 'My description title of callback');
}


/**
 * Copy of taxonomy.pages.inc taxonomy_term_page function
 *
 * @param $term
 * @return array
 */
function mymodule_taxonomy_page($term) {
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
    $breadcrumb[] = l($current->name, 'taxonomy/term/' . $current->tid);
  }
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb = array_reverse($breadcrumb);
  drupal_set_breadcrumb($breadcrumb);

  // Set the term path as the canonical URL to prevent duplicate content.
  $uri = entity_uri('taxonomy_term', $term);
  drupal_add_html_head_link(array('rel' => 'canonical', 'href' => url($uri['path'], $uri['options'])), TRUE);
  // Set the non-aliased path as a default shortlink.
  drupal_add_html_head_link(array('rel' => 'shortlink', 'href' => url($uri['path'], array_merge($uri['options'], array('alias' => TRUE)))), TRUE);

  // Normally we would call taxonomy_term_show() here, but for backwards
  // compatibility in Drupal 7 we do not want to do that (it produces different
  // data structures and HTML markup than what Drupal 7 released with). Calling
  // taxonomy_term_view() directly provides essentially the same thing, but
  // allows us to wrap the rendered term in our desired array structure.
  $build['term_heading'] = array(
    '#prefix' => '<div class="term-listing-heading">',
    '#suffix' => '</div>',
    'term' => taxonomy_term_view($term, 'full'),
  );

  if ($nids = taxonomy_select_nodes($term->tid, TRUE, 20)) {
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