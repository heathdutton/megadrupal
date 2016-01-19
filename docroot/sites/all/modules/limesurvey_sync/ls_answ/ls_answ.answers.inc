<?php
/**
 * @file
 * functions for displaying and selecting answers form a content type.
 */

/**
 * Form builder displaying all answers for a survey in a view page.
 *
 * Display answers links and synchronization datas into a table and allow to
 * select some answers to compare.
 *
 * @param $node_survey
 *   An object. The survey node.
 * @param $number_results
 *   A numeric. The number of results per page. Default is 10.
 */
function ls_answ_answ_list($form, &$form_state, $node_survey, $number_results = 10) {

  // Define a list of columns.
  $columns = array(
    'title' => t('Title'),
    'ls_last_import' => '',
    'uid' => t('Author'),
    'ls_status' => '',
    'ls_sync' => '',
  );

  // Create sorted headers from column list.
  $header = array();
  $i = 0;
  module_load_include('inc', 'ls_answ', 'ls_answ');
  foreach ($columns as $field_name => $col_name) {
    $header[$i] = array(
      'data' => (!in_array($field_name, array('uid', 'title'))) ? ls_answ_display_sync_headers($field_name) : $col_name,
      'field' => $field_name,
    );
    // Set the default sort field.
    if ($field_name == 'ls_last_import') {
      $header[$i]['sort'] = 'desc';
    }
    $i++;
  }

  // Retrieve default settings.
  $published = (isset($form_state['values']['published'])) ? $form_state['values']['published'] : 1;
  $ignore_virgin = (isset($form_state['values']['ignore_virgin'])) ? $form_state['values']['ignore_virgin'] : 1;
  $results_per_page = (int) (isset($form_state['values']['results_per_page'])) ? $form_state['values']['results_per_page'] : $number_results;

  $results = array();
  module_load_include('inc', 'ls_survey', 'ls_survey');
  $answ_type = ls_survey_find_answ_type($node_survey->ls_orig_type);

  // Help one http://blog.worldempire.ch/story/howto-convert-module-dbtng.

  $query = db_select('node', 'n')->extend('PagerDefault')->extend('TableSort');
  $query->join('ls_answ', 'lsa', 'lsa.nid = n.nid AND lsa.vid = n.vid');
  $query->join('users', 'u', 'n.uid = u.uid');
  $query->fields('n', array('title', 'uid', 'nid'));
  $query->fields('lsa', array('ls_last_import', 'ls_status', 'ls_sync'));
  $query->fields('u', array('name'));
  $query->condition('n.type', check_plain($answ_type->type));

  // Option filters :
  $query->limit($results_per_page);
  if ($published) {
    $query->condition('n.status', NODE_PUBLISHED);
  }
  if ($ignore_virgin) {
    $query->condition('lsa.ls_status', LS_STATUS_VIRGIN, '<>');
  }

  $query->orderByHeader($header);

  // Sort as '1;9;10;20' instead of '1;10;20;9'
  global $databases;
  switch (drupal_strtolower($databases['default']['default']['driver'])) {
    case 'mysql':
      // I don't know if cast expression is supported by all database types.
      $query->addExpression('CAST(n.nid AS UNSIGNED)', 'nid_sort');
      $query->orderBy('nid_sort', 'DESC');
      break;
      // @TODO: add more database types.
    default:
      $query->orderBy('n.nid', 'DESC');
  }

  $query->addTag('node_access');
  $result = $query->execute();
  while ($row = $result->fetchAssoc()) {
    $cell = array();

    // Handle each column.
    foreach ($columns as $field_name => $col_name) {
      switch ($field_name) {
        case 'title':
          $value = l($row[$field_name], 'node/' . $row['nid']);
          break;

        case 'uid':
          $value = l($row['name'], 'user/' . $row[$field_name]);
          break;

        case 'ls_last_import':
        case 'ls_status':
        case 'ls_sync':
          $value = ls_answ_display_sync_fields($field_name, $row[$field_name]);
          break;

        default:
          $value = $row[$field_name];
      }

      $cell[] = $value;
    }
    $results[$row['nid']] = $cell;
    $cell = array();
  }

  $form['type'] = array(
    '#type' => 'value',
    '#value' => $answ_type->type,
  );

  // Add filter.
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter results'),
    '#collapsible' => 1,
    '#collapsed' => 1,
  );

  $form['filters']['published'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show only published answers'),
    '#default_value' => $published,
    '#return_value' => 1,
    '#access' => (user_access('bypass node access') || user_access('administer nodes') || user_access('view own unpublished content')),
  );

  $form['filters']['ignore_virgin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ignore virgin answers'),
    '#default_value' => $ignore_virgin,
    '#return_value' => 1,
  );

  $form['filters']['results_per_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Set the number of answers per page'),
    '#default_value' => $results_per_page,
    '#size' => 5,
  );
  // Load button.
  $form['filters']['actions'] = array('#type' => 'actions');
  $form['filters']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );

  // Load the answer nodes.
  $compare_access = user_access('compare answers');
  if ($compare_access) {
    $form['answers'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $results,
      '#empty' => t('No respond yet'),
    );
  }
  else {
    $no_node = array(
      'data' => t('No respond yet'),
      'colspan' => count($header),
      'style' => 'text-align:center',
    );
    $cells = (count($results)) ? $results : $no_node;
    $form['answers'] = array(
      '#type' => 'markup',
      '#prefix' => '<div>',
      '#markup' => theme('table', array('header' => $header, 'rows' => $cells)),
      '#suffix' => '</div>',
    );
  }

  global $pager_total_items;
  pager_default_initialize($pager_total_items[0], $results_per_page, 0);
  $form['pager'] = array('#markup' => theme('pager'));

  // Load button.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Compare answers'),
    '#access' => $compare_access,
  );

  return $form;
}

/**
 * Validate the answers view list.
 */
function ls_answ_answ_list_validate($form, &$form_state) {
  if (isset($form_state['values']['results_per_page']) && (!is_numeric($form_state['values']['results_per_page']) || $form_state['values']['results_per_page'] < 2)) {
    form_set_error('results_per_page', t('The number of answers per page is not valid'));
  }
  elseif ($form_state['values']['op'] != t('Compare answers')) {
    $form_state['rebuild'] = TRUE;
  }

  if ($form_state['values']['op'] == t('Compare answers')) {
    $error = FALSE;
    $type = check_plain($form_state['values']['type']);
    $nids = array_filter($form_state['values']['answers']);
    if (count($nids) < 2) {
      form_set_error('answers', t('You must check at least 2 answers to compare'));
      $error = TRUE;
    }
    // Limit the amount of answers to compare regarding the generate url.
    // The maximum lenght url shoud be 2048.
    // http://www.boutell.com/newfaq/misc/urllength.html
    $prefix_url = 'node/compare/' . $type . '/';
    $delta = ls_answ_compare_check_max_url($nids, $prefix_url);
    if ($delta < 0) {
      form_set_error('answers', t('To many answers to compare. Uncheck aproximatively %number answers.', array('%number' => abs($delta))));
      $error = TRUE;
    }

    // Check access rights, compatibility, etc...
    if (!$error && !ls_answ_compare_access($type, implode('-', $nids))) {
      form_set_error('answers', t('You can not compare thoses answers'));
    }
  }
}

/**
 * Submit the answers view list.
 */
function ls_answ_answ_list_submit($form, &$form_state) {
  if ($form_state['values']['op'] == t('Compare answers')) {
    $checked = array_filter($form_state['values']['answers']);
    // redirect to the new url :
    $form_state['redirect'] = 'node/compare/' . $form_state['values']['type'] . '/' . implode('-', $checked);
  }
}

/**
 * Display the answers comparision page.
 *
 * Compare answers node from disctinct answers content types, languages, even
 * for desynchronized languages. They only need to share the same ls_sid and a
 * valid ls_aid. The display options including language are provided by the
 * $type parameter.
 *
 * @param $type
 *   The survey node object.
 * @param $nids
 *   A string. The list of nids from answer nodes to compare. Glue is '-'.
 *
 * @return
 *   An html string. The answers comparaison.
 */
function ls_answ_compare($type, $nids) {
  // Add css.
  $element = array(
    '#tag' => 'link', // The #tag is the html tag - <link />
    '#attributes' => array(// Set up an array of attributes inside the tag
      // Use the file_create_url() function for compatibility with the locale
      // module.
      'href' => file_create_url(drupal_get_path('module', 'ls_answ') . '/ls_answ.css'),
      'rel' => 'stylesheet',
      'type' => 'text/css',
      'media' => 'all',
    ),
  );
  drupal_add_html_head($element, 'ls_answ_css');

  module_load_include('inc', 'ls_survey', 'ls_survey.type');
  $options = variable_get('ls_answ_options_' . $type, ls_survey_answ_default_options());
  module_load_include('inc', 'ls_answ', 'ls_answ');
  $node_survey = ls_answ_find_survey($type);

  $all_nids = explode('-', $nids);
  $nodes = node_load_multiple($all_nids);
  foreach ($nodes as $node) {
    if (!isset($ls_sid)) {
      $ls_sid = $node->ls_sid;
    }
    // Resync each node to make sure they are up-to-date.
    $sync_node = ls_answ_sync_node($node);
    // Reload the node if it has been updated.
    if (!is_string($sync_node) && $sync_node !== $node) {
      $node = $sync_node;
    }
    if (!empty($node->ls_aid)) {
      $ls_aid_s[$node->ls_aid] = l($node->title, 'node/' . $node->nid);
    }
  }

  // Create a fake node.
  $node_compare = array();
  $node_compare['ls_sid'] = $ls_sid;
  $node_compare['ls_lang'] = $node_survey->ls_lang;
  $node_compare['type'] = $type;
  $node_compare['nid'] = NULL;
  $node_compare['body'] = NULL;
  $node_compare = (object) $node_compare;
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_answ');
  $answers = limesurvey_sync_answ_import_answer($node_compare, $ls_aid_s, $options);
  module_load_include('inc', 'ls_survey', 'ls_survey');
  $output = ls_survey_theme_box(t('Maching score'), t('The matching score is %score%', array('%score' => $answers['score'])));
  $output .= $answers['body'];
  return $output;
}

/**
 * Check the amount of answers to compare regarding to the length url generated.
 *
 * @param $nids
 *   An array. The list of nids from the answer nodes to compare.
 *
 * @param $prefix_url
 *   A string. The url append to the $nids variable. It can be a relative or
 *   an absolute url.
 *
 * @return
 *   A signed integer. The number of answers than can be add to the prefix url.
 *   If the value is negative : the url is too long.
 */
function ls_answ_compare_check_max_url($nids, $prefix_url) {
  // The maximum length url shoud be 2048.
  // http://www.boutell.com/newfaq/misc/urllength.html
  $maximum_length_url = 2048;

  $prefix_url_absolute = url($prefix_url, array('absolute' => TRUE));
  $max_suffix_url_len = $maximum_length_url - drupal_strlen($prefix_url_absolute);
  $suffix_url = implode('-', $nids);
  $delta = $max_suffix_url_len - drupal_strlen($suffix_url);

  // Calculate the average length for one answer.
  $n_nids = count($nids);
  $nids_length = array_map('drupal_strlen', $nids);
  $nid_length_moy = (array_sum($nids_length) + $n_nids) / $n_nids;

  return round($delta / $nid_length_moy);
}

