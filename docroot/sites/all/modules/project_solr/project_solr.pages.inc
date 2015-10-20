<?php

function project_solr_browse_page($project_type) {
  $project_type = node_type_get_type($project_type);
  // Set the page title and breadcrumb.
  drupal_set_title($project_type->name);
  $breadcrumb = menu_get_active_breadcrumb();
  drupal_set_breadcrumb($breadcrumb);

  // Set some default solr params
  $params = array(
    'q' => isset($_GET['text']) ? $_GET['text'] : '',
    'qf' => 'content',
    'fl' => array(
      'id',
      'entity_type',
      'entity_id',
      'label',
      'content',
      'is_comment_count',
      'bundle',
      'ds_created',
      'ds_changed',
      'url',
      'is_uid',
      'ss_name',
      'iss_project_release_usage',
      'ds_project_latest_release',
      'ds_project_latest_activity',
      'hash',
    ),
    'rows' => variable_get('apachesolr_rows', 10),
    'facet' => 'true',
    'facet.mincount' => 1,
    'facet.sort' => 'true',
    'facet.field' => array(
      'bundle',
      'im_project_release_api_tids',
    ),
    'facet.limit' => 200,
  );
  $params['start'] = (isset($_GET['page']) ? $_GET['page'] : 0) * $params['rows'];

  // This is the object that does the communication with the solr server.
  $env_id = apachesolr_default_environment();
  $solr = apachesolr_get_solr($env_id);
  if (isset($_GET['solrsort'])) {
    $solrsort = $_GET['solrsort'];
  }
  else {
    if (module_exists('project_usage')) {
      $solrsort = 'iss_project_release_usage desc';
    }
    else {
      $solrsort = 'sort desc';
    }
  }
  $query = new SolrBaseQuery($env_id, $solr, $params, $solrsort, $_GET['q']);
  if (is_null($query)) {
    throw new Exception(t('Could not construct a Solr query.'));
  }

  // We add addFilter() parameters here to include all the constant filters for
  // the query -- project nodes of the given top-level type that have releases
  // (if project_release is enabled). We use addFilter() rather than
  // $params['fq'] so that our filters are correctly passed to anything that
  // uses our cached query.
  $query->addFilter('bundle', $project_type->type);

  // Apply sort value.
  $query->setAvailableSort('ss_name', array('title' => t('Author'), 'default' => 'asc'));
  $query->setAvailableSort('ds_created', array('title' => t('Created date'), 'default' => 'desc'));
  $query->setAvailableSort('ds_project_latest_release', array('title' => t('Last release'), 'default' => 'desc'));
  $query->setAvailableSort('ds_project_latest_activity', array('title' => t('Last build'), 'default' => 'desc'));
  if (module_exists('project_usage')) {
    $query->setAvailableSort('iss_project_release_usage', array('title' => t('Most installed'), 'default' => 'desc'));
  }
  list($sort_field, $sort_direction) = explode(' ', $solrsort);
  $query->setSolrsort($sort_field, $sort_direction);

  // Allow modules to alter the query prior to statically caching it.
  // This can e.g. be used to add available sorts.
  foreach (module_implements('apachesolr_query_prepare') as $module) {
    $function_name = $module . '_apachesolr_query_prepare';
    $function_name($query, $params, 'project_solr_browse_page');
  }

  // Cache the built query. Since all the built queries go through
  // this process, all the hook_invocations will happen later.
  apachesolr_current_query($env_id, $query);

  if ($query) {
    $response = $query->search(isset($_GET['text']) ? $_GET['text'] : '');

    // The response is cached so that it is accessible to the blocks and anything
    // else that needs it beyond the initial search.
    apachesolr_static_response_cache($query->getSearcher(), $response);
    apachesolr_has_searched($env_id, TRUE);

    $nids = array();
    foreach (apachesolr_search_process_response($response, $query) as $result) {
      $nids[] = $result['node']->entity_id;
    }
    $results = array();
    foreach (node_load_multiple($nids) as $node) {
      $results[] = node_view($node, 'teaser');
    }
  }

  $output = array();

  $form_state = array(
    'method' => 'get',
    'always_process' => TRUE,
    'no_redirect' => TRUE,
    'build_info' => array('args' => array($project_type)),
  );
  $form = drupal_build_form('project_solr_browse_projects_form', $form_state);
  unset($form['#build_id'], $form['form_build_id'], $form['form_id']);
  $output['project_solr_browse_projects_form'] = $form;

  // Print the description to the page
  if (!empty($project_type->description)) {
    $output['description']['#markup'] = '<p>' . filter_xss($project_type->description) . '</p>';
  }

  $output['results'] = $results;
  $output['pager']['#markup'] = theme('pager');

  return $output;
}

/**
 * Get a page of categories of projects.
 *
 * @param $project_type
 *   Node type name.
 * @param $vocab
 *   Vocabulary object.
 *
 * @return
 *   Array for drupal_render().
 */
function project_solr_category_page($project_type, $vocab) {
  // todo Simplify and move into drupalorg.
  $tree = taxonomy_get_tree($vocab->vid);
  if (empty($tree)) {
    return drupal_not_found();
  }
  // Limit category page to 100 terms. If this is ever exceeded we may need a pager.
  $tree = array_slice($tree, 0, 100);

  $project_type = node_type_get_type($project_type);

  $categories = array();
  foreach ($tree as $category_term) {
    $items = project_solr_fetch_category_items($project_type, $category_term);
    if (!empty($items)) {
      $categories[$category_term->tid] = array(
        'title' => check_plain($category_term->name),
        'items' => $items,
      );
    }
  }

  $form_state = array('method' => 'get', 'always_process' => TRUE, 'no_redirect' => TRUE, 'build_info' => array('args' => array('project/' . $project_type->type . '/' . $vocab->machine_name, t('Filter by compatibilty'))));
  $form = drupal_build_form('project_solr_version_form', $form_state);
  unset($form['#build_id'], $form['form_build_id'], $form['form_id']);

  return array(
    'project_solr_version_form' => $form,
    'results' => array(
      '#theme' => 'project_solr_category_page',
      '#categories' => $categories,
    ),
  );
}
