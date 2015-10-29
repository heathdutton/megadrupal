<?php
/**
 * @file
 * Views integration for the Zeitgeist module.
 *
 * @copyright (c) 2005-2013 Ouest Systemes Informatiques (OSInet)
 *
 * @license Licensed under the General Public License version 2 or later.
 */

/**
 * Implements hook_views_data().
 *
 * Describe our data to Views. Note that Views 3.x normally expects any
 * base table to have a PK, and ZG doesn't, but no ill consequence seems to
 * arise out of this. Please open an issue is a problem is identified.
 */
function zeitgeist_views_data() {
  $data = array();

  $data['zeitgeist'] = array(
    'table'    => array(
      // Do NOT translate!
      'group'    => 'Zeitgeist',
      'base'     => array(
        'field'    => 'ts',
        'title'    => t('Zeitgeist search log'),
        'help'     => t('A list of searches which have been recently applied to this site.'),
      ),
    ),
    'search'   => array(
      'title'    => t('Search'),
      'help'     => t('The pattern searched for.'),
      'field'    => array(
        'click sortable' => FALSE,
      ),
    ),
    'category' => array(
      'title'    => t('Search kind'),
      'help'     => t('The kind of search requested, like "node", or "user".'),
      'field'    => array(
        'click sortable' => FALSE,
      ),
    ),
    'ts'       => array(
      'title'    => t('Timestamp'),
      'help'     => t('The instant at which the search was requested.'),
      'field'    => array(
        'click sortable' => TRUE,
        'handler'        => 'views_handler_field_date',
      ),
      'filter'   => array(
        'handler'        => 'views_handler_filter_date',
      ),
      'sort'     => array(
        'handler'        => 'views_handler_sort_date',
      ),
    ),
  );

  return $data;
}
