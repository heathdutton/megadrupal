<?php

/**
 * @file
 * Entity List Themer pages.
 */

/**
 * AJAX callback for pager.
 */
function entity_lister_pageturner_callback() {

  $list_class = $_POST['list_class'];

  // Is $list_class one of our allowed classes?
  if ('EntityLister' !== $list_class) {
    $allowed_classes = variable_get('entity_lister_allowed_classes', NULL);
    if (!in_array($list_class, $allowed_classes)) {
      print '';
      drupal_exit();
    }
  }

  // For tabular output.
  if (!empty($_POST['config']['table_sort'])) {
    if ($_POST['order'] && $_POST['sort']) {
      // Make EntityFieldQuery aware of the latest order and sort.
      $_GET['order'] = $_POST['order'];
      $_GET['sort'] = $_POST['sort'];
    }
  }

  // Though the delta does wind up in the output, we don't need check_plain
  // because casting as an int ensures that nothing malicious can sneak through.
  $delta = (int) $_POST['delta'];
  // This prefix becomes part of the output, so call check_plain().
  $id_prefix = check_plain($_POST['id_prefix']);

  $obj = new $list_class($_POST['config'], $delta, $id_prefix);
  $list = $obj->getList(urldecode($_POST['page']));
  $element = $obj->element($list);
  print drupal_render($element);

  drupal_exit();

}

/**
 * View a list of cache entries.
 */
function entity_lister_cache_entries() {

  $array['#attached']['css'][] = drupal_get_path('module', 'entity_lister') . '/cache.css';

  drupal_set_title(t('Entity Lister: Cache Entries'));

  $order = isset($_GET['order']) ? $_GET['order'] : 'created';
  $sort = isset($_GET['sort']) ? $_GET['sort'] : 'DESC';

  $result = db_select('cache', 'c')
    ->fields('c', array('cid', 'created'))
    ->condition('cid', 'entity_lister:%', 'LIKE')
    ->orderBy("c." . $order, $sort)
    ->execute();

  $header[] = array(
    'data' => t('CID'),
    'field' => 'c.cid',
  );
  $header[] = array(
    'data' => t('Created'),
    'field' => 'c.created',
    'sort' => 'desc',
  );

  $rows = array();
  if ($result) {
    while ($record = $result->fetchAssoc()) {
      $record['cid'] = l($record['cid'], 'admin/reports/entity_lister/cache/entry/' . $record['cid']);
      $record['created'] = $record['created'] . '<div class="entity-lister-date">' . date('F j, Y g:i:sa', $record['created']) . '</div>';
      $rows[] = $record;
    }
  }

  $array['table'] = array(
    '#markup' => theme('table', array('header' => $header, 'rows' => $rows)),
  );

  return $array;

}

/**
 * View a list of cache entries.
 */
function entity_lister_cache_entry($cid) {

  $array['#attached']['css'][] = drupal_get_path('module', 'entity_lister') . '/cache.css';

  drupal_set_title(t('Entity Lister: Cache Entry Detail'));

  $result = db_select('cache', 'c')
    ->fields('c', array('cid', 'created', 'data'))
    ->condition('cid', $cid)
    ->execute()
    ->fetchAssoc();

  $output = '<div id="entity-lister-full-entry">';

  if ($result) {
    $output .= '<div id="entity-lister-cid"><label>Cache ID</label>' . $result['cid'] . '</div>';
    $output .= '<div id="entity-lister-created"><label>Created</label>' . $result['created'] . ' :: <span class="entity-lister-date">' . date('F j, Y @ g:i:sa', $result['created']) . '</span></div>';
    $output .= '<div id="entity-lister-data"><label>Data</label><textarea>' . $result['data'] . '</textarea></div>';
  }

  $output .= '</div>';

  $array['output'] = array('#markup' => $output);

  return $array;

}

/**
 * Get confirmation that an administrator has seen a breaking change warning.
 */
function entity_lister_changes() {

  $array['warning'] = array(
    '#prefix' => '<p>',
    '#suffix' => '</p>',
    '#markup' => t('Please confirm that you have read about API-breaking changes in the <a href="@link_rc2"><strong>release notes for rc2</strong></a> and <a href="@link_rc3"><strong>rc3</strong></a>. These changes may affect output on your site if you have upgraded Entity Lister from any version before rc3.', array('@link_rc2' => 'https://drupal.org/node/2067893', '@link_rc3' => 'https://drupal.org/node/2073045')),
  );

  $array['can_ignore'] = array(
    '#prefix' => '<p>',
    '#suffix' => '</p>',
    '#markup' => t('If you never installed rc2 or earlier versions of Entity Lister on this site, you can ignore this warning and follow the link below to stop the warning from appearing.'),
  );

  $query = array('confirm' => 'yes');
  if (isset($_GET['destination'])) {
    $query['destination'] = $_GET['destination'];
  }
  else {
    $query['destination'] = '';
  }
  $array['confirm'] = array(
    '#prefix' => '<p>',
    '#suffix' => '</p>',
    '#markup' => l(t('Got it.  Take me back to the page I was viewing.'), $_GET['q'], array('query' => $query)),
  );

  if (isset($_GET['confirm']) && 'yes' == $_GET['confirm']) {
    // Turn off the nag.
    variable_set('entity_lister_changes_nag', 'off');
    if (isset($_GET['destination'])) {
      drupal_goto($_GET['destination']);
    }
  }

  return $array;

}

/**
 * API Demo.
 *
 * See the constructor in EntityLister for documentation of config variables,
 * including some not used here.
 */
function entity_lister_demo() {

  $array['#attached'] = array(
    'css' => array(drupal_get_path('module', 'entity_lister') . '/demo.css'),
  );

  $array['heading'] = array(
    '#markup' => '<h2>' . t('Multiple Lists on One Page') . '</h2>',
  );

  $array['summary'] = array(
    '#markup' => '<p>' . t("Depending on what's in your database, this demo page
                           will display up to three lists.  If you've just
                           installed Drupal, you won't see much here unless you
                           add a couple articles.") . '</p>',
  );

  /*
   * Example #1
   */

  $config['type'] = 'user';
  $config['numitems'] = '5';

  // Don't cache the list.
  $config['cache'] = '0';

  // '0' for no pager, '1' for standard pager, '2' for ajax pager.
  $config['pager'] = '2';

  // '0' for above, '1' for below, '2' for both.
  $config['pager_position'] = '1';

  // Sort by username in reverse alphabetical order.
  $sort = array(
    array(
      'method' => 'propertyOrderBy',
      'args' => array(
        'property' => 'name',
        'direction' => 'DESC',
      ),
    ),
    /* Any number of calls to propertyOrderBy or fieldOrderBy.
     * array(
     *  'method' => 'fieldOrderBy',
     *  'args' => array(
     *    'field' => 'field_something',
     *    'column' => 'some_column',
     *    'direction' => 'ASC',
     *   ),
     * ),
     */
  );

  $config['sort'] = $sort;

  // The $delta must be unique among all lists appearing
  // on this page, and must be an integer.
  $delta = 0;

  // All params are optional, but in this case we set $config and $delta.
  $obj = new EntityLister($config, $delta);

  // Passing the $page to getList() is only required when using the standard
  // pager. Since this list uses the AJAX pager, we could omit the
  // call, but it's here for demo purposes.
  $page = $obj->getPageNum();
  // Get the themed list output.
  $list = $obj->getList($page);
  // Place the list in a renderable element.
  $array['entity-lister-' . $delta] = $obj->element($list);

  /*
   * Example #2
   */

  $sort = array(
    array(
      'method' => 'propertyOrderBy',
      'args' => array(
        'property' => 'title',
        'direction' => 'ASC',
      ),
    ),
  );

  $config = array(
    'cache' => '1',
    'bundle' => array('article'),
    'numitems' => '1',
    'pager_st_offset' => '50',
    'get_total' => '1',
    'sort' => $sort,
  );
  $delta = 1;
  $obj = new EntityLister($config, $delta);
  $list = $obj->getList();
  // The element() call is only necessary when you are using the AJAX pager,
  // but is a good idea regardless, for consistency's sake.
  $array['entity-lister-' . $delta] = $obj->element($list);

  /*
   * Example #3
   */

  // Note that the config here is the same as above, but we
  // add a property condition, remove the bundle config and disable caching.
  $config['property_condition'][] = array(
    'column' => 'uid',
    'value' => 1000,
    'operator' => '<',
  );
  unset($config['bundle']);
  $config['cache'] = '0';
  $delta = 2;
  $obj = new EntityLister($config, $delta);
  $list = $obj->getList();
  $array['entity-lister-' . $delta] = $obj->element($list);

  return $array;

}
