<?php
/**
 * @file
 * Error reporting/handling
 */

$plugin = array (
  'title' => t('Blocks'),
  'description' => t('Library for working with blocks.'),
  'xu_type' => 'library'
);

/**
 * Save a block to the database
 *
 * @param $delta
 *  The block delta
 * @param $config
 *  An array of configuration parameters
 */
function xu_block_save($delta, $config) {

  foreach (list_themes() as $key => $theme) {
    if ($theme->status) {
      db_query("INSERT INTO {blocks} (visibility, pages, custom, title, module, theme, status, weight, delta, cache) VALUES(%d, '%s', %d, '%s', '%s', '%s', %d, %d, %d, %d)", $config['visibility'], trim($config['pages']), $config['custom'], $config['title'], $config['module'], $theme->name, 0, 0, $delta, BLOCK_NO_CACHE);
    }
  }

  foreach (array_filter($config['roles']) as $rid) {
    db_query("INSERT INTO {blocks_roles} (rid, module, delta) VALUES (%d, '%s', '%s')", $rid, $config['module'], $delta);
  }

  drupal_set_message(t('The block has been created.'));
  cache_clear_all();
}

/**
 * Delete a block
 *
 * @param $delta
 * @param $module
 * @param null $name
 *
 * @internal param $
 */
function xu_block_delete($delta, $module, $name = NULL) {
  db_query("DELETE FROM {blocks} WHERE module = '%s' AND delta = %d", $module, $delta);
  db_query("DELETE FROM {blocks_roles} WHERE module = '%s' AND delta = %d", $module, $delta);
  if (isset($name)) {
    drupal_set_message(t('The "%name" block has been removed.', array('%name' => $name)));
  }
  cache_clear_all();
}
