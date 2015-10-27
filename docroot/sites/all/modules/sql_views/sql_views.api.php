<?php

/**
 * @file
 * Hooks provided by the SQL Views module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allows modules to define SQL Views.
 *
 * The definition is an array structure representing one or more
 * SQL views provided by the module. SQL views are defined by
 * hook_sql_views() which must live in your module's .install file.
 * Note that the views will not be dropped automatically when the module
 * has been removed.
 *
 * @return array
 *   Associative array defining SQL Views provided by the module.
 *   The key of each element of the array is the name of the SQL View.
 *   The corresponding array value is an associative array that may contain
 *   the following key-value pairs:
 *   - "query": Required. SelectQuery object that provides the definition
 *     of the view.
 *   - "human_name": Required. A localized human name of the view.
 *   - "description": Required. A description of what the view does.
 *   - "field_types": Optional. An associative array describing fields for
 *     the Views module. The key of each element is the field name from
 *     the query. The value is suggested field type. The following values
 *     are possible:
 *     - string (default)
 *     - numeric
 *     - float
 *     - boolean
 *     - date
 *     Note that in most cases you need not describe field types because their
 *     definitions will be copied from original table. The only exception is an
 *     expression that returns non string value. See example below.
 *
 * @see SelectQuery
 * @see hook_views_data()
 */
function hook_sql_views() {

  // Authors statistics.
  $query = db_select('users', 'u')
    // It is necessary you to specify all required field explicitly.
    // Otherwise they won't be available in Drupal Views.
    ->fields('u', array('uid', 'name'))
    ->condition('u.uid', 0, '>')
    ->groupBy('u.uid');
  $query->addExpression('COUNT(*)', 'count');
  $query->addExpression('100 * (COUNT(*) / (SELECT COUNT(*) FROM node))', 'percentage');
  $query->join('node', 'n', 'n.uid = u.uid');

  // It is a good practice to prefix the view name by
  // the name of of the module.
  $items['module_name_authors'] = array(
    // SelectQuery instance.
    'query' => $query,
    'human_name' => t('Authors'),
    'description' => 'The best authors on the site.',
    // We describe only expressions fields because other definitions
    // can be cloned from original tables.
    'field_types' => array(
      'count' => 'numeric',
      'percentage' => 'float',
    ),
  );

  return $items;
}

/**
 * Alter the SQL views after hook_sql_views() is invoked.
 *
 * This hook is invoked by sql_views_get_sql_views(). The SQL views definitions
 * are passed in by reference. Each element of the $items array is one
 * SQL view returned by a module from hook_sql_views(). Additional items may
 * be added, or existing items altered.
 *
 * @param array $items
 *   Associative array of SQL views definitions.
 *
 * @see hook_sql_views()
 */
function hook_sql_views_alter(array &$items) {
  // Lets count only published content.
  if (isset($items['module_name_authors'])) {
    $items['module_name_authors']['query']->condition('n.status', 1);
  }
}

/**
 * @} End of "addtogroup hooks".
 */
