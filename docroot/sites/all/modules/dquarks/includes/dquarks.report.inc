<?php

/**
 * @file
 * This file includes helper functions for creating reports for dquarks.module
 */

// All functions within this file need the dquarks.submissions.inc.
require_once  drupal_get_path('module', 'dquarks') . '/includes/dquarks.submissions.inc';

/**
 * Retrieve lists of submissions for a given dquarks.
 */
function dquarks_results_submissions($node, $user_filter, $pager_count) {
  global $user;
  if (isset($_GET['results']) && is_numeric($_GET['results'])) {
    $pager_count = $_GET['results'];
  }

  $header = theme('dquarks_results_submissions_header', array('node' => $node));
  if ($user_filter) {
    if ($user->uid) {
      drupal_set_title(t('Submissions for %user', array('%user' => $user->name)), PASS_THROUGH);
    }
    else {
      drupal_set_title(t('Your submissions'));
      dquarks_disable_page_cache();
    }
    dquarks_set_breadcrumb($node);
    $submissions = dquarks_get_submissions(array('nid' => $node->nid, 'uid' => $user->uid), $header, $pager_count);
    $count = dquarks_get_submission_count($node->nid, $user->uid);
  }
  else {
    $submissions = dquarks_get_submissions($node->nid, $header, $pager_count);
    $count = dquarks_get_submission_count($node->nid);
  }

  $operation_column = end($header);
  $operation_total = $operation_column['colspan'];

  $rows = array();
  foreach ($submissions as $sid => $submission) {
    $row = array(
      $submission->is_draft ? t('@sid (draft)', array('@sid' => $sid)) : $sid,
      format_date($submission->submitted, 'small'),
    );
    if (dquarks_results_access($node, $user)) {
      $row[] = theme('username', array('account' => $submission));
      $row[] = $submission->remote_addr;
    }
    $destination = drupal_get_destination();
    $row[] = l(t('View'), "node/$node->nid/submission/$sid");
    $operation_count = 1;

    if (dquarks_submission_access($node, $submission, 'delete', $user)) {
      $row[] = l(t('Delete'), "node/$node->nid/submission/$sid/delete", array('query' => $destination));
      $operation_count++;
    }
    if ($operation_count < $operation_total) {
      $row[count($row) - 1] = array('data' => $row[count($row) - 1], 'colspan' => $operation_total - $operation_count + 1);
    }
    $rows[] = $row;
  }

  $element['#theme'] = 'dquarks_results_submissions';
  $element['#node'] = $node;
  $element['#submissions'] = $submissions;
  $element['#total_count'] = $count;
  $element['#pager_count'] = $pager_count;
  $element['#attached']['library'][] = array('dquarks', 'admin');

  $element['table']['#theme'] = 'table';
  $element['table']['#header'] = $header;
  $element['table']['#rows'] = $rows;
  $element['table']['#operation_total'] = $operation_total;

  return drupal_render($element);
}

/**
 * Theme the list of links for selecting the number of results per page.
 *
 * @param array $variables
 *   An array contain the total number of results available and The current
 * number of results displayed per page.
 */
function theme_dquarks_results_per_page($variables) {
  $total_count = $variables['total_count'];
  $pager_count = $variables['pager_count'];
  $output = '';

  // Create a list of results-per-page options.
  $counts = array(
    '20' => '20',
    '50' => '50',
    '100' => '100',
    '200' => '200',
    '500' => '500',
    '1000' => '1000',
    '0' => t('All'),
  );

  $count_links = array();

  foreach ($counts as $number => $text) {
    if ($number < $total_count) {
      $count_links[] = l($text, $_GET['q'], array(
        'query' => array('results' => $number),
        'attributes' => array(
          'class' => array($pager_count == $number ? 'selected' : ''),
        ),
              )
      );
    }
  }

  $output .= '<div class="dquarks-results-per-page">';
  if (count($count_links) > 1) {
    $output .= t('Show !count results per page.', array('!count' => implode(' | ', $count_links)));
  }
  else {
    $output .= t('Showing all results.');
  }
  if ($total_count > 1) {
    $output .= ' ' . t('@total results total.', array('@total' => $total_count));
  }
  $output .= '</div>';

  return $output;
}

/**
 * Theme the header of the submissions table.
 *
 * This is done in it's own function so that dquarks can retrieve the header and
 * use it for sorting the results.
 */
function theme_dquarks_results_submissions_header($variables) {
  $node = $variables['node'];

  $columns = array(
    array('data' => t('#'), 'field' => 'sid', 'sort' => 'desc'),
    array('data' => t('Submitted'), 'field' => 'submitted'),
  );
  if (dquarks_results_access($node)) {
    $columns[] = array('data' => t('User'), 'field' => 'name');
    $columns[] = array('data' => t('IP Address'), 'field' => 'remote_addr');
  }
  $columns[] = array(
    'data' => t('Operations'),
    'colspan' => module_exists('print') ? 5 : 3,
  );

  return $columns;
}

/**
 * Preprocess function for dquarks-results-submissions.tpl.php.
 */
function template_preprocess_dquarks_results_submissions(&$vars) {
  $vars['node'] = $vars['element']['#node'];
  $vars['submissions'] = $vars['element']['#submissions'];
  $vars['table'] = $vars['element']['table'];
  $vars['total_count'] = $vars['element']['#total_count'];
  $vars['pager_count'] = $vars['element']['#pager_count'];
  $vars['is_submissions'] = (arg(2) == 'submissions') ? 1 : 0;

  unset($vars['element']);
}

/**
 * Create a table containing all submitted values for a dquarks node.
 */
function dquarks_results_table($node, $pager_count = 0) {
  if (isset($_GET['results']) && is_numeric($_GET['results'])) {
    $pager_count = $_GET['results'];
  }

  // Get all the submissions for the node.
  $header = theme('dquarks_results_table_header', array('node' => $node));
  $submissions = dquarks_get_submissions($node->nid, $header, $pager_count);
  $total_count = dquarks_get_submission_count($node->nid);

  $output = theme('dquarks_results_table', array(
    'node' => $node,
    'components' => $node->dquarks['components'],
    'submissions' => $submissions,
    'total_count' => $total_count,
    'pager_count' => $pager_count,
          )
  );
  if ($pager_count) {
    $output .= theme('pager');
  }
  return $output;
}

/**
 * Format the output.
 *
 * @param array $variables
 *   Contains the current node
 *
 * @return array
 *   The fields to display
 */
function theme_dquarks_results_table_header($variables) {
  return array(
    array('data' => t('#'), 'field' => 'sid', 'sort' => 'desc'),
    array('data' => t('Submitted'), 'field' => 'submitted'),
    array('data' => t('User'), 'field' => 'name'),
    array('data' => t('IP Address'), 'field' => 'remote_addr'),
    array('data' => t('Score'), 'field' => 'score'),
  );
}

/**
 * Theme the results table displaying all the submissions for a particular node.
 *
 * @param array $variables
 *   Contain the node whose results are being displayed, the associative array
 *  of the components for this dquarks, an array of all submissions for this
 *  dquarks,the total number of submissions to this dquarks and The number of
 *  results to be shown per page.
 */
function theme_dquarks_results_table($variables) {
  drupal_add_library('dquarks', 'admin');

  $node = $variables['node'];
  $submissions = $variables['submissions'];
  $total_count = $variables['total_count'];
  $pager_count = $variables['pager_count'];

  $header = array();
  $rows = array();
  $cell = array();

  // This header has to be generated seperately so we can add the SQL necessary.
  // to sort the results.
  $header = theme('dquarks_results_table_header', array('node' => $node));

  // Generate a row for each submission.
  foreach ($submissions as $sid => $submission) {
    $cell[] = l($sid, 'node/' . $node->nid . '/submission/' . $sid);
    $cell[] = format_date($submission->submitted, 'small');
    $cell[] = theme('username', array('account' => $submission));
    $cell[] = $submission->remote_addr;
    $cell[] = $submission->score . '/' . $submission->max_score;
    $component_headers = array();

    foreach ($node->dquarks['components'] as $component) {
      $table_function = '_dquarks_table_data_' . $component['type'];
      if (function_exists($table_function)) {
        $data = isset($submission->data[$component['cid']]) ? $submission->data[$component['cid']] : NULL;
        $submission_output = $table_function($data, $component);
        if ($submission_output !== NULL) {
          $component_headers[] = $component['name'];
          $cell[] = $submission_output;
        }
      }
    }

    $rows[] = $cell;
    unset($cell);
  }
  if (!empty($component_headers)) {
    $header = array_merge($header, $component_headers);
  }

  if (count($rows) == 0) {
    $rows[] = array(
      array(
        'data' => t('There are no submissions for this quiz. <a href="!url">View this quiz</a>.', array('!url' => url('node/' . $node->nid))),
        'colspan' => 5,
      ),
    );
  }

  $output = '';
  $output .= theme('dquarks_results_per_page', array(
    'total_count' => $total_count,
    'pager_count' => $pager_count,
          ));
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}

/**
 * Delete all submissions for a node.
 *
 * @param int $nid
 *   The node id whose submissions will be deleted.
 */
function dquarks_results_clear($nid) {
  $node = node_load($nid);
  $submissions = dquarks_get_submissions($nid);
  foreach ($submissions as $submission) {
    dquarks_submission_delete($node, $submission);
  }
}

/**
 * Confirmation form to delete all submissions for a node.
 *
 * @param object $node
 *   ID of nodewhose submissions will be cleared.
 */
function dquarks_results_clear_form($form, $form_state, $node) {
  drupal_set_title(t('Clear Form Submissions'));

  $form = array();
  $form['nid'] = array('#type' => 'value', '#value' => $node->nid);
  $question = t('Are you sure you want to delete all submissions for this form?');

  return confirm_form($form, $question, 'node/' . $node->nid . '/dquarks-results', NULL, t('Clear'), t('Cancel'));
}

/**
 * Clear results.
 *
 * @param array $form
 *   An array of form items of submission.
 * @param array $form_state
 *   An array of form items of submission.
 */
function dquarks_results_clear_form_submit($form, &$form_state) {
  dquarks_results_clear($form_state['values']['nid']);
  $node = node_load($form_state['values']['nid']);
  $title = $node->title;
  $message = t('dquarks %title entries cleared.', array('%title' => $title));
  drupal_set_message($message);
  watchdog('dquarks', $message);
  $form_state['redirect'] = 'node/' . $form_state['values']['nid'] . '/dquarks-results';
}

/**
 * Form to configure the download of excel files.
 */
function dquarks_results_download_form($form, &$form_state, $node) {
  require_once  drupal_get_path('module', 'dquarks') . '/includes/dquarks.export.inc';
  require_once  drupal_get_path('module', 'dquarks') . '/includes/dquarks.components.inc';

  $form = array();

  $form['node'] = array(
    '#type' => 'value',
    '#value' => $node,
  );

  $form['format'] = array(
    '#type' => 'radios',
    '#title' => t('Export format'),
    '#options' => dquarks_export_list(),
    '#default_value' => 'excel',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Download'),
  );

  return $form;
}

/**
 * Validate handler for dquarks_results_download_form().
 */
function dquarks_results_download_form_submit(&$form, &$form_state) {
  dquarks_results_export($form_state['values']['node'], $form_state['values']['format'], $form_state['values']);
}

/**
 * Generate a Excel-readable file containing all submissions for a dquarks.
 *
 * The structure of the file is:
 *   Heading Line 1: Gives group overviews padded by empty cells to the
 *                   next group.  A group may be a question and corresponds
 *                   to a component in the dquarks philosophy. Each group
 *                   overview will have a fixed number of columns beneath it.
 *   Heading line 2: gives column headings
 *   Data line 1 .....
 *   Data line 2 .....
 */
function dquarks_results_export($node, $format = 'delimited', $options = array()) {
  global $user;
  require_once  drupal_get_path('module', 'dquarks') . '/includes/dquarks.export.inc';
  require_once  drupal_get_path('module', 'dquarks') . '/includes/dquarks.components.inc';

  if (empty($options)) {
    $options = array(
      'delimiter' => variable_get('dquarks_csv_delimiter', '\t'),
      'format' => variable_get('dquarks_export_format', 'delimited'),
    );
  }


  // Open a new dquarks exporter object.
  $exporter = dquarks_export_create_handler($format, $options);

  $file_name = drupal_tempnam('temporary://', 'dquarks_');

  _dquarks_sanitize_node($node);
  // The @ suppresses errors.
  $handle = @fopen($file_name, 'w');

  $exporter->setContent($file_name, $node, $format, $options);

  $export_name = _dquarks_safe_name($node->title);
  $exporter->setHeaders($export_name);
  // The @ makes it silent.
  @readfile($file_name);
  // Clean up, the @ makes it silent.
  @unlink($file_name);
  exit();
}

/**
 * Function used to sanitize node's title before sending them to PHPExcel.
 *
 * @param object $node
 *   The dquarks node object.
 */
function _dquarks_sanitize_node(&$node) {
  $forbidden_char = array(
    '*', ':', '/', '\\', '?', '[', ']',
  );

  $node->title = str_replace($forbidden_char, '', $node->title);
}

/**
 * Provides a simple analysis of all submissions to a dquarks.
 *
 * @param object $node
 *   The dquarks node on which to generate the analysis.
 * @param array $sids
 *   An array of submission IDs to which this analysis may be filtered. May be
 *   used to generate results that are per-user or other groups of submissions.
 * @param array $analysis_component
 *   A dquarks component. If passed in, additional information may be returned
 *   relating specifically to that component's analysis, such as a list of
 *   "Other" values within a select list.
 */
function dquarks_results_analysis($node, $sids = array(), $analysis_component = NULL) {

  $rows = array();
  $question_number = 0;

  $headers = array(
    t('Q'),
    array('data' => t('responses'), 'colspan' => '10'),
  );
  // Load all component types.
  dquarks_load_components();
  foreach ($node->dquarks['components'] as $component) {

    // Do component specific call.
    $analysis_function = '_dquarks_analysis_rows_' . $component['type'];
    if (function_exists($analysis_function)) {
      $question_number++;
      $crows = $analysis_function($component, $sids);
      if (is_array($crows)) {
        $row[0] = array(
          'data' => '<strong>' . $question_number . '</strong>',
          'rowspan' => count($crows) + 1, 'valign' => 'top',
        );
        $row[1] = array(
          'data' => '<strong>' . $component['name'] . '</strong>',
          'colspan' => '10',
        );
        $rows = array_merge($rows, array_merge(array($row), $crows));
      }
    }
  }

  if (count($rows) == 0) {
    $rows[] = array(
      array(
        'data' => t('There are no submissions for this quiz. <a href=\"!url\">View this quiz</a>.', array(
          '!url' => url('node/' . $node->nid),
                )
        ),
        'colspan' => 21,
      ),
    );
  }

  return theme('table', array('header' => $headers, 'rows' => $rows));
}
