<?php

/**
 * @file
 * Provide views data and handlers for site_disclaimer.module
 */

/**
 * @defgroup site_disclaimer.module handlers
 *
 * @{
 */

/**
 * Implements hook_views_data().
 */
function site_disclaimer_views_data() {
  $data['users']['tou_version'] = array(
    'title' => t('Site Disclaimer Version'),
    'real field' => 'data',
    'field' => array(
      'sortable' => FALSE,
      'click sortable' => FALSE,
      'handler' => 'site_disclaimer_handler_field_tou_version',
      'help' => t('Version of Site Disclaimer the User has last agreed to / accepted.'),
    ),
//    'sort' => array(
//      'handler' => 'site_disclaimer_handler_sort_tou_version',
//    ),
//    'argument' => array(
//      'handler' => 'views_handler_argument_numeric',
//    ),
  );

  return $data;
}



/**
 * Implementation of hook_views_handlers().
 *
 * Register information about all of the views handlers provided by the
 * site_disclaimer module.
 */
function site_disclaimer_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'site_disclaimer') . '/views/handlers',
    ),
    'handlers' => array(
      'site_disclaimer_handler_field_tou_version' => array(
        'parent' => 'views_handler_field_prerender_list',
      ),
//      'site_disclaimer_handler_sort_tou_version' => array(
//        'parent' => 'views_handler_sort',
//      ),
    ),
  );
}

/**
 * Sort functions for usort().
 */
function site_disclaimer_sort($a, $b) {
  $acmp = (is_object($a) && isset($a->site_disclaimer_sortby)) ? $a->site_disclaimer_sortby : '';
  $bcmp = (is_object($b) && isset($b->site_disclaimer_sortby)) ? $b->site_disclaimer_sortby : '';
  return strcmp($acmp, $bcmp);
}
function site_disclaimer_rsort($a, $b) {
  $acmp = (is_object($a) && isset($a->site_disclaimer_sortby)) ? $a->site_disclaimer_sortby : '';
  $bcmp = (is_object($b) && isset($b->site_disclaimer_sortby)) ? $b->site_disclaimer_sortby : '';
  return -strcmp($acmp, $bcmp);
}

/**
 * Implementation of hook views_pre_render().
 * Performs data sort after the query is executed.
 */
function UNUSED_site_disclaimer_views_pre_render(&$view) {
  $query = $view->query;
  if ($query->base_table == 'users' && isset($query->site_disclaimer_handler_sort_tou_version)) {
    $rows = &$view->result;
    $option = $query->site_disclaimer_handler_sort_tou_version[0];
    // FIXME: should we iterate and sort by each option? foreach ($query->site_disclaimer_handler_sort_tou_version as $option) {...
    // The fundamental problem is that we will not be able to maintain other sort orders performed by query.

//$option['form_data_fieldname'] = 'site_disclaimer';
    if (isset($option['form_data_fieldname'])) {

      // Extract field from user data into the recordset, so we can sort by it
      $field_alias = isset($option['field_alias']) ? $option['field_alias'] : $option['table'] . '_' . 'data';

//      $desc = (stripos($query->orderby[0], 'DESC') !== false);
      // FIXME: this [0] is not going to work with multi-sort queries.
      $desc = ($option['order'] == 'DESC');

//watchdog('debug','IN tou_sort field_alias='.$field_alias.' query=<pre>'.print_r($query,1).'</pre>');
      foreach ($rows as $key => $result) {
        $form_data = unserialize($result->{$field_alias});
//        if ($form_data) {
//          foreach ($form_data as $k => $v) {
//            if ($k == $option['form_data_fieldname']) {
$v = isset($form_data[$option['form_data_fieldname']]) ? $form_data[$option['form_data_fieldname']] : '';
              $rows[$key]->site_disclaimer_sortby = drupal_strtolower($v); // Case-insensitive
//            }
//          }
//        }
      }

      // Now sort the data
      usort($rows, $desc ? 'site_disclaimer_rsort' : 'site_disclaimer_sort');
    }
//watchdog('debug','tou_sort desc='.$desc.' result=<pre>'.print_r($view->result,1).'</pre> query=<pre>'.print_r($query,1).'</pre>');
  }
}

/**
 * @}
 */
