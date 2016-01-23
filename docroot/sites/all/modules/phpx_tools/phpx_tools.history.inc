<?php

/**
 * @file
 * PHP X History Records CRUD.
 */

/**
 * Loads history records based on $conditions.
 *
 * @param array $conditions
 *   Array of conditions (field values to filter by).
 *
 * @return mixed
 *   Script array or false.
 */
function phpx_tools_history_load(array $conditions = array()) {
  $select = db_select('phpx_history', 'ph')->fields('ph');

  foreach ($conditions as $field => $value) {
    $select->condition($field, $value);
  }

  $select->orderBy('executed', 'desc');

  $script = $select->execute()->fetchAll(PDO::FETCH_ASSOC);

  return $script;
}

/**
 * Saves a given script into history table.
 *
 * @param array $script
 *   The script to be saved.
 *
 * @return bool|int
 *   Result of drupal_write_record operation.
 */
function phpx_tools_history_save(array $script) {
  $script += array(
    'id'  => NULL,
    'uid'      => $GLOBALS['user']->uid,
    'created'  => REQUEST_TIME,
    'executed' => REQUEST_TIME,
  );

  $script['id'] = phpx_tools_get_existing_history($script);

  // Insert or update the affiliate.
  $status = drupal_write_record('phpx_history', $script, (!empty($script['id']) ? 'id' : array()));

  return $status;
}

/**
 * Delete script identified by $conditions.
 *
 * @param array $conditions
 *   Conditions to identify record.
 *
 * @return bool
 *   Success indicator.
 */
function phpx_tools_history_delete(array $conditions = array()) {
  $query = db_delete('phpx_history');
  foreach ($conditions as $field => $value) {
    $query->condition($field, $value);
  }

  return $query->execute();
}

/**
 * Get script by its contents.
 *
 * @param string $script
 *   Script name.
 *
 * @return array|null
 *   Script if exists or NULL otherwise.
 */
function phpx_tools_get_existing_history($script) {
  $existing = phpx_tools_history_load(array('script' => $script['script']));

  return $existing ? $existing[0]['id'] : NULL;
}

/**
 * Get all scripts from history.
 *
 * @return mixed
 *   Return all history records.
 */
function phpx_tools_get_history() {
  $history = db_select('phpx_history', 'ph')
    ->fields('ph')
    ->orderBy('executed', 'DESC')
    ->execute()
    ->fetchAll(PDO::FETCH_ASSOC);

  return $history;
}

/**
 * Return the table containing the history script records.
 *
 * @return string
 *   Table HTML
 */
function phpx_tools_build_history_table() {

  // Define the table head.
  $head = array(t('PHP'), t('Executed'), t('Load'));
  $rows = array();

  // Get all history entries.
  $history = phpx_tools_get_history();

  // Build the table rows.
  if ($history) {
    foreach ($history as $h) {
      $rows[] = array(
        '<span id="script-' . $h['id'] . '">' . htmlspecialchars($h['script']) . '</span>',
        $h['executed'] ? format_date($h['executed'], 'custom', 'd.m.Y H:i:s') : '',
        '<a class="script-load" rel="script-' . $h['id'] . '" href="#">load</a>',
      );
    }
  }
  else {
    return t("The history is squeaky clean right now. Let's run some code and watch it magically appear here!");
  }

  // Return the drupal table.
  return theme('table', array(
    'header' => $head,
    'rows' => $rows,
    'attributes' => array(
      'class' => array('phpx-history-table'),
    ),
  ));
}
