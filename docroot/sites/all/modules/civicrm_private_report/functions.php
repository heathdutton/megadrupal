<?php
/**
 * @file
 * Non-hook functions for civicrm_private_report module.
 */

/**
 * Records whether a given report instance is or is not a user-private report.
 *
 * @param $instance_id
 *  The relevant CiviCRM report instance ID.
 * @param $is_private
 *  Boolean: is private, or is not.
 *
 * @return Boolean: TRUE on succes, FALSE on fialure.
 */
function _civicrm_private_report_set_private_report($instance_id, $is_private) {
  if ($is_private) {
    global $user;
    $data = array(
      'instance_id' => $instance_id,
      'uid' => $user->uid,
    );
    $ret = drupal_write_record('civicrm_private_report', $data);
  }
  else {
    $ret = db_delete('civicrm_private_report')
      ->condition('instance_id', $instance_id)
      ->execute();
  }
  return (bool) $ret;
}

/**
 * Checks wither a given report is private, and whether it belongs to the
 * given user, if any.
 * @param $instance_id
 *  The relevant CiviCRM report instance ID.
 * @param $uid
 *  Optional. If given, the uid of the user whose ownership we're testing.
 *
 * @return Boolean
 *  When only $intance-id is given: TRUE if the report instance is a user-private
 *  report, or FALSE if not.
 *  When the optional $uid is also given: TRUE if the report is private and
 *  owned by that user, or FALSE otherwise.
 */
function _civicrm_private_report_is_private($instance_id, $uid = NULL) {
  $query = db_select('civicrm_private_report', 'pr')
    ->fields('pr', array('id'))
    ->condition('instance_id', $instance_id);
  if (isset($uid)) {
    $query->condition('uid', $uid);
  }
  $array = array();
  $array = $query->execute()->fetchAllKeyed();
  return !empty($array);
}

/**
 * Gets data from civicrm_private_report table.
 * @param $select
 *  A single-dimensional array of columns to select. If empty, all columns
 *  will be selected.
 * @param $where
 *  Optional. An assocative array of column_name => value pairs to use in a
 *  WHERE clause.
 * @param $order
 *  Optional. Array of ORDER BY settings. Each item in the array can be a simple
 *  value, which will be converted to an ASCENDING sort; or it can be a
 *  string-keyed item, in which the key is the column name, and the value is
 *  either "ASC" or "DESC".
 * @param $single_column
 *  Boolean. If TRUE, values will be returned as a single-dimensional array. If
 *  FALSE (the default), values will be returned as a numerically-indexed array
 *  of associative arrays of column_name => value pairs.
 *
 * @return Array
 *  The matching rows. See $single_column parameter.
 */
function _civicrm_private_report_get_rows($select = array(), $where = array(), $order = array(), $single_column = FALSE) {
  // Initialize the query object.
  $query = db_select('civicrm_private_report', 'pr');

  // Define valid columns.
  $valid_columns = array(
    'id',
    'instance_id',
    'uid',
  );
  // Ensure $select contains only valid columns.
  if (is_array($select) && !empty($select)) {
    $select = array_intersect($select, $valid_columns);
  }
  else {
    $select = $valid_columns;
  }
  // Add select fields to query object.
  $query->fields('pr', $select);

  // Set up WHERE clause.
  if (is_array($where) && count($where)) {
    $wheres = $params = array();
    foreach ($valid_columns as $column) {
      if (array_key_exists($column, $where)) {
        // Add conditional to query object.
        $query->condition($column, $where[$column]);
      }
    }
  }

  // Set up ORDER BY clause.
  if (is_array($order) && !empty($order)) {
    $orderbys = array();
    foreach ($order as $column => $direction) {
      if (drupal_strtolower($direction) != 'desc' && drupal_strtolower($direction) != 'asc') {
        $column = $direction;
        $direction = 'ASC';
      }
      if (in_array($column, $valid_columns)) {
        $query->orderBy($column, $direction);
      }
    }
  }

  // Execute the query object.
  $result = $query->execute();
  $rows = array();
  while ($row = $result->fetchAssoc()) {
    if ($single_column) {
      $rows[] = $row[$select[0]];
    }
    else {
      $rows[] = $row;
    }
  }
  return $rows;
}

/**
 * Gets a list of distinct uid => name pairs for users having user-private reports.
 *
 * @param $uid
 *  Optional. If given, the list is limited to the given user ID. (Note that
 *  this can result in an empty array being returned, if that user has no
 *  user-private reports.)
 *
 * @return Array of uid => name pairs.
 */
function _civicrm_private_report_get_users($uid = 0) {
  $users = array();
  $query = db_select('users', 'u');
  $query->innerJoin('civicrm_private_report', 'pr', 'u.uid = pr.uid');
  $query->fields('u', array('name', 'uid'));
  if ($uid) {
    $query->condition('u.uid', $uid);
  }
  $query->orderBy('name');
  $result = $query->execute();
  while ($row = $result->fetchAssoc()) {
    $users[$row['uid']] = $row['name'];
  }
  return $users;
}

/**
 * Gets the page output for menu item "civicrm_private_report/menu/list/all".
 */
function _civicrm_private_report_menu_list_all() {
  $output = drupal_render(drupal_get_form('_civicrm_private_report_form_list_filter'));
  $output .= _civicrm_private_report_build_list_all();
  return $output;
}

/**
 * Gets the page output for menu item "civicrm_private_report/menu/list".
 */
function _civicrm_private_report_menu_list() {
  $output = '<p>' . t('This page lists all CiviCRM user-private reports currently available for you.') . '</p>';
  global $user;
  $output .= '<div id="crm-container">' . _civicrm_private_report_user_list($user->uid) . '</div>';
  return $output;
}

/**
 * Form constructor for the user filter form.
 *
 * @see _civicrm_private_report_form_list_filter_validate()
 * @ingroup forms
 */
function _civicrm_private_report_form_list_filter($form, $form_state) {
  $form = array();

  // Build list of users having private reports.
  $options = array('0' => '- ' . t('Show All') . ' -');
  $options += _civicrm_private_report_get_users();

  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter users'),
  );
  $form['filters']['#theme'] = 'civicrm_private_report_user_filter';
  $form['filters']['uid'] = array(
    '#type' => 'select',
    '#title' => 'Select a user',
    '#options' => $options,
    '#default_value' => isset($_SESSION['civicrm_private_report_list_filter']) ? $_SESSION['civicrm_private_report_list_filter'] : 0,
  );
  $form['filters']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('List'),
  );

  return $form;
}

/**
 * Form submission handler for the user filter form.
 */
function _civicrm_private_report_form_list_filter_submit($form, &$form_state) {
  $filter = &$_SESSION['civicrm_private_report_list_filter'];
  $filter = $form_state['values']['uid'];
}

/**
 * Gets page output for listing the user-private reports of one given user.
 */
function _civicrm_private_report_user_list($uid) {
  global $user;
  $private_instance_ids = _civicrm_private_report_get_rows(array('instance_id'), array('uid' => $uid), array(), TRUE);
  // Build a list of private reports with relevant info for the template listing.
  _civicrm_private_report_civicrm_html_head();
  if (!empty($private_instance_ids)) {
    require_once 'CRM/Report/Page/InstanceList.php';
    require_once 'CRM/Core/Smarty.php';

    $private_list = array();
    $info = CRM_Report_Page_InstanceList::info();
    foreach ($info as $category => $instances) {
      $category_private = array();
      foreach ($instances as $instance_id => $instance) {
        if (in_array($instance_id, $private_instance_ids)) {
          // If current user is report owner, alter delete URL to point to this
          // module's delete path.
          if ($user->uid == $uid) {
            $instance['deleteUrl'] = url('civicrm_private_report/delete/' . $instance_id);
          }
          // If current user is NOT report owner, hide the delete URL entirely.
          else {
            unset($instance['deleteUrl']);
          }
          // Add the instance to the list of private reports.
          $category_private[$instance_id] = $instance;
        }
      }
      if (!empty($category_private)) {
        $private_list[$category] = $category_private;
      }
    }
  }

  // If we actually have anything in the private reports list, parse the template and display output.
  if (!empty($private_list)) {
    $tpl = CRM_Core_Smarty::singleton();
    $tpl->assign('list', $private_list);
    $tpl_output = $tpl->fetch('CRM/Report/Page/InstanceList.tpl');
    $output = $tpl_output;
    drupal_add_js(drupal_get_path('module', 'civicrm_private_report') . "/resource/js/CRM_Report_Page_InstanceList-private.js");
  }
  else {
    if (user_access('access CiviReport')) {
      $args = array(
        '!link' => l(t('view the list of all CiviCRM Reports'), 'civicrm/report/list', array('query' => array('reset' => '1'))),
      );
      $view_reports = ' ' . t('You can !link.', $args);
    }
    $output = '<p>' . t('No user-private reports were found for you.') . $view_reports . '</p>';
  }
  return $output;
}

/**
 * Deletes the given report instance, both in CiviCRM and as a user-private report.
 *
 * @param $instance_id
 *  The relevant CiviCRM report instance ID.
 *
 * @return Boolean: TRUE on success, FALSE on error.
 */
function _civicrm_private_report_delete($instance_id) {

  // Delete the CiviCRM report.
  civicrm_initialize();
  require_once 'CRM/Report/DAO/Instance.php';
  $dao = new CRM_Report_DAO_Instance();
  $dao->id = $instance_id;
  $result = $dao->delete();
  if ($result === FALSE) {
    return FALSE;
  }

  // Delete the private report record
  $result = _civicrm_private_report_set_private_report($instance_id, FALSE);
  if ($result === FALSE) {
    return FALSE;
  }

  return TRUE;
}

/**
 * Menu access callback for path "civicrm_private_report/delete".
 */
function _civicrm_private_report_menu_access_delete($instance_id) {
  global $user;
  if (!user_access('administer own CiviCRM reports') || !_civicrm_private_report_is_private($instance_id, $user->uid)) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Menu access callback for path "civicrm_private_report/list"
 */
function _civicrm_private_report_menu_access_list() {
  return (user_access('administer own CiviCRM reports') && user_access('access Report Criteria'));
}

/**
 * Sets up JavaScript and CSS for CiviCRM-related content.
 * Essentially copied from civicrm_html_head() in civicrm.module, with the
 * exception that this function doesn't work only under the civicrm/ path.
 *
 * @return a style tag that indicates what file browsers should import
 */
function _civicrm_private_report_civicrm_html_head() {

  if (! civicrm_initialize()) {
    return;
  }

  $head = NULL;

  require_once 'CRM/Core/Config.php';
  $config = CRM_Core_Config::singleton();

  $template = &CRM_Core_Smarty::singleton();
  $buffer = $template->fetch('CRM/common/jquery.files.tpl');
  $lines  = preg_split('/\s+/', $buffer);
  foreach ($lines as $line) {
    $line = trim($line);
    if (empty($line)) {
      continue;
    }
    if (strpos($line, '.js') !== FALSE) {
      drupal_add_js(drupal_get_path('module', 'civicrm') . '/../' . $line);
    }
    elseif (strpos($line, '.css') !== FALSE) {
      drupal_add_css(drupal_get_path('module', 'civicrm') . '/../' . $line);
    }
  }

  // add localized calendar js
  $localisation   = explode('_', $config->lcMessages);
  $localization_file = drupal_get_path('module', 'civicrm') . '/../packages/jquery/jquery-ui-1.8.11/development-bundle/ui/i18n/jquery.ui.datepicker-' . $localisation[0] . '.js';

  if (file_exists($localization_file)) {
    drupal_add_js($localization_file);
  }

  // add Common.js
  drupal_add_js(drupal_get_path('module', 'civicrm') . '/../js/Common.js');

  // add the final assignment
  drupal_add_js('var cj = jQuery.noConflict(); $ = cj;', array('type' => 'inline', 'scope' => JS_DEFAULT));

  if (isset($config->customCSSURL) && ! empty($config->customCSSURL)) {
    $head  = "<style type=\"text/css\">@import url({$config->customCSSURL});</style>\n";
  }
  else {
    drupal_add_css(drupal_get_path('module', 'civicrm') . '/../css/deprecate.css');
    drupal_add_css(drupal_get_path('module', 'civicrm') . '/../css/civicrm.css');
    drupal_add_css(drupal_get_path('module', 'civicrm') . '/../css/extras.css');
  }
  return $head;
}

/**
 * Gets page content for the list of all users' user-private reports, based on
 * the filter settings defined in the user filter form.
 */
function _civicrm_private_report_build_list_all() {
  $filter = &$_SESSION['civicrm_private_report_list_filter'];
  $filter = isset($filter) ? $filter : 0;

  $users = _civicrm_private_report_get_users($filter);

  $output = '<div id="crm-container">';

  foreach ($users as $uid => $name) {
    $output .= '<h3>' . check_plain($name) . '</h3>';
    $output .= _civicrm_private_report_user_list($uid);
  }

  $output . '</div>';
  return $output;
}

/**
 * Returns HTML for the user filter form.
 *
 * @param $form
 *  The user filter form.
 *
 * @ingroup themeable
 */
function theme_civicrm_private_report_user_filter($variables) {
  $form = $variables['form'];
  $output = '';
  $output .= '<div>';
  $output .= '<label style="float:left; line-height: 3em; margin-right: .25em;">' . $form['uid']['#title'] . '</label>';
  unset($form['uid']['#title']);
  $output .= '<div style="float:left; line-height: 3em; margin-right: .25em;">' . drupal_render($form['uid']) . '</div>';
  $output .= '<div style="float:left; line-height: 3em; margin-right: .25em;">' . drupal_render($form['submit']) . '</div>';
  $output .= '<div style="clear:both"></div>';
  $output .= '</div>';
  return $output;
}

/**
 * Form builder for delete confirmation form.
 *
 * @param $form Drupal form.
 * @param $form_state Drupal form state.
 * @param $instance_id Internal CiviCRM ID of the report to be deleted.
 */ 
function _civicrm_private_report_menu_delete_confirm_form($form, &$form_state, $instance_id) {
  // Get report title for display on the delete confirm form.
  civicrm_initialize();
  require_once 'CRM/Report/BAO/Instance.php'; 
  $bao = new CRM_Report_BAO_Instance();
  $bao->id = $instance_id;
  $bao->find();
  $bao->fetch();
  $title = $bao->title;

  $args = array(
    '%title' => $title,
  );
  $question = t('Are you sure you want to delete the report %title?', $args);

  $form = array(
    'instance_id' => array(
      '#type' => 'value',
      '#value' => $instance_id,
    ),
    'title' => array(
      '#type' => 'value',
      '#value' => $title,
    ),
  );

  return confirm_form($form, $question, 'civicrm_private_report/list');
}

/**
 * Form submit handler for the delete confirmation form.
 */
function _civicrm_private_report_menu_delete_confirm_form_submit($form, &$form_state) {
  $instance_id = $form_state['values']['instance_id'];

  $args = array(
    '%title' => $form_state['values']['title'],
  );
  if (_civicrm_private_report_delete($instance_id)) {
    drupal_set_message(t('The private report %title has been deleted.', $args));
  }
  else {
    drupal_set_message(t('Some problems were encountered in deleting the report. Please review this list to ensure it was deleted.', 'error'));
  }
  $form_state['redirect'] = 'civicrm_private_report/list';
}
