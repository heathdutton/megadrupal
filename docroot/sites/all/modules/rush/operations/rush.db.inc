<?php
/**
 * @file
 * DB operation functions for Drush Rush.
 * @see operations
 */

/**
 * @addtogroup operations Operations
 * @{
 */
/**
 * @addtogroup db-operations DB Operations
 * @brief DB operation functions for Drush Rush.
 * @details Includes functions to:
 * - Dump a db snapshot to an sql file
 * - Run sql queries, including queries in an sql file
 * - Create or destroy a database
 *
 * Currently all DB Operation functions use rush_invoke which in turn uses
 * drush_invoke_process to execute Drush commands necessary to execute the
 * operation.
 * @{
 */

/**
 * Export a database to the specified file.
 *
 * Wrapper for `drush sql-dump`.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - array $params['build']['site_alias_record']
 *   Required. An drush site alias record.
 *   - string $params['db']['file_path']
 *   Required. String containing the path to file to write the dump to.
 *   - string $params['db']['url']
 *   Required. A string containing the db url.
 *
 * @return bool
 *   TRUE on success, FALSE on failure via drush_set_error().
 */
function rush_op_sql_dump($params) {

  if (empty($params['build']['site_alias_record'])) {
    return rush_empty_error('$params[\'build\'][\'site_alias_record\']', __FUNCTION__);
  }
  if (empty($params['db']['file_path'])) {
    return rush_empty_error('$params[\'db\'][\'file_path\']', __FUNCTION__);
  }
  if (empty($params['db']['url'])) {
    return rush_empty_error('$params[\'db\'][\'url\']', __FUNCTION__);
  }

  $alias     = $params['build']['site_alias_record'];
  $args      = array();
  $options   = array();
  $options[] = '--result-file=' . $params['db']['file_path'];
  $options[] = '--db-url=' . $params['db']['url'];
  $options[] = '-y';

  return rush_invoke($alias, 'sql-dump', $args, $options, __FUNCTION__);

}

/**
 * Run the sql in the file specified by $params['db']['file_path'].
 *
 * Uses `drush sql-query`.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - array $params['build']['site_alias_record']
 *   Required. A drush site alias record.
 *   - string $params['db']['file_path']
 *   Required. String containing the path to file containing the sql query
 *   to run.
 *   - string $params['db']['url']
 *   Required. A string containing the db url.
 *
 * @return bool
 *   TRUE on success, FALSE on failure via drush_set_error().
 */
function rush_op_sql_query_file($params) {

  if (empty($params['build']['site_alias_record'])) {
    return rush_empty_error('$params[\'build\'][\'site_alias_record\']', __FUNCTION__);
  }
  if (empty($params['db']['file_path'])) {
    return rush_empty_error('$params[\'db\'][\'file_path\']', __FUNCTION__);
  }
  if (empty($params['db']['url'])) {
    return rush_empty_error('$params[\'db\'][\'url\']', __FUNCTION__);
  }

  $msg = dt('[START DB IMPORT]: This may take some time.');
  drush_log($msg, 'status');

  $alias     = $params['build']['site_alias_record'];
  $args      = array();
  $options   = array();
  $options[] = '--file=' . $params['db']['file_path'];
  $options[] = '--db-url=' . $params['db']['url'];
  $options[] = '-y';

  return rush_invoke($alias, 'sql-query', $args, $options, __FUNCTION__);

}

/**
 * Run a specified query using `drush sql-query`.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - array $params['build']['site_alias_record']
 *   Required. An drush site alias record.
 *   - string $params['db']['query']
 *   Required. A string containing the query to run.
 *   - string $params['db']['url']
 *   Required. A string containing the db url.
 *
 * @return bool
 *   TRUE on success, FALSE on failure via drush_set_error().
 * @backlog Appears to return true when sql is executed correctly even if there
 * was an sql error.
 */
function rush_op_sql_query($params) {

  if (empty($params['build']['site_alias_record'])) {
    return rush_empty_error('$params[\'build\'][\'site_alias_record\']', __FUNCTION__);
  }
  if (empty($params['db']['url'])) {
    return rush_empty_error('$params[\'db\'][\'url\']', __FUNCTION__);
  }
  if (empty($params['db']['query'])) {
    return rush_empty_error('$params[\'db\'][\'query\']', __FUNCTION__);
  }

  $alias     = $params['build']['site_alias_record'];
  $args      = array();
  $args[]    = $params['db']['query'];
  $options   = array();
  $options[] = '--db-url=' . $params['db']['url'];
  $options[] = '-y';

  return rush_invoke($alias, 'sql-query', $args, $options, __FUNCTION__);
}

/**
 * Delete the specified data base.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - array $params['build']['site_alias_record']
 *   Required. A drush site alias record.
 *   - string $params['db']['name']
 *   Required. A string containing the name of the db to destroy.
 *   - string $params['db']['url']
 *   Required. A string containing the db url.
 *
 * @return bool
 *   TRUE on success, FALSE on failure via drush_set_error().
 */
function rush_op_sql_destroy_db($params) {

  if (empty($params['build']['site_alias_record'])) {
    return rush_empty_error('$params[\'build\'][\'site_alias_record\']', __FUNCTION__);
  }
  if (empty($params['db']['url'])) {
    return rush_empty_error('$params[\'db\'][\'url\']', __FUNCTION__);
  }
  if (empty($params['db']['name'])) {
    return rush_empty_error('$params[\'db\'][\'name\']', __FUNCTION__);
  }
  // Change the db_query param to drop the database.
  $params['db']['query'] = 'DROP DATABASE IF EXISTS ' . $params['db']['name'];
  $result                = rush_op_sql_query($params);
  if ($result === FALSE) {
    $msg = dt('[ERROR]: *** !operation failed to run: !cmd_string ***', array(
      '!cmd_string' => $params['db']['query'],
      '!operation'  => __FUNCTION__,
    ));

    return drush_set_error('RUSH_SQL_OP_ERROR', $msg);
  }
  else {
    $msg = dt('[RUN DRUSH]: !cmd_string', array('!cmd_string' => $params['db']['query']));
    drush_log($msg, 'success');

    return TRUE;
  }
}

/**
 * Create a database.
 *
 * Wrapper for `drush sql-create`.  
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - array $params['build']['site_alias_record']
 *   Required. A drush site alias record.
 *   - string $params['db']['url']
 *   Required. A string containing database URL.
 *
 * @return bool
 *   TRUE on success, FALSE on failure via drush_set_error().
 */
function rush_op_sql_create($params) {

  if (empty($params['build']['site_alias_record'])) {
    return rush_empty_error('$params[\'build\'][\'site_alias_record\']', __FUNCTION__);
  }
  if (empty($params['db']['url'])) {
    return rush_empty_error('$params[\'db\'][\'url\']', __FUNCTION__);
  }

  $alias     = $params['build']['site_alias_record'];
  $args      = array();
  $options   = array();
  $options[] = '--db-url=' . $params['db']['url'];
  $options[] = '-y';

  return rush_invoke($alias, 'sql-create', $args, $options, __FUNCTION__);

}
/** @}*/
/** @}*/
