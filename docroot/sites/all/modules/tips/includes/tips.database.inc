<?php

/**
 * @file
 * tips.database.inc
 *
 * Database associated functions.
 */

/**
 * Load all tooltips.
 */
function tips_load_all() {
  $result = db_query('SELECT *
    FROM {tips_registry} AS tr');
  $tips = $result->fetchAll();

  return $tips;
}

/**
 * Load a single tooltip by id.
 *
 * Use tip_add instead for 99% of use cases. This is
 * simply a helper function.
 */
function tips_load($tip_id = NULL) {
  $result = db_query('SELECT tr.selector, tc.content, ts.library, ts.settings
    FROM {tips_registry} AS tr
    JOIN {tips_content_registry} AS tc ON tr.content_id = tc.content_id
    JOIN {tips_settings_registry} AS ts ON tr.settings_id = ts.settings_id
    WHERE tr.tip_id = :tip_id', array(':tip_id' => $tip_id));
  $tips = $result->fetchAssoc();

  return $tips;
}

/**
 * Load a tooltip settings group.
 *
 * @todo, Add title/ name; some sort of friendly identifier.
 */
function tips_load_settings($settings_id = NULL) {  
  $result = db_query('SELECT tr.tip_id, ts.library, ts.settings 
    FROM {tips_registry} AS tr 
    JOIN {tips_settings_registry} AS ts ON tr.settings_id = ts.settings_id
    WHERE ts.settings_id = :settings_id', array(':settings_id' => $settings_id));
  $settings = $result->fetchAssoc();

  // Format setting values.
  $settings['settings'] = unserialize($settings['settings']);

  return $settings;
}

/**
 * Update a tooltip settings group.
 *
 * @param(int)
 *  ID of settings group to update.
 * @param(array)
 *  Array of settings to insert.
 */
function tips_update_settings($settings_id, $settings) {
  $result = db_update('tips_settings_registry')
    ->fields(array(
      'settings' => serialize($settings),
    ))
  ->condition('settings_id', $settings_id)
  ->execute();
}

/**
 * Insert new settings group into database.
 */
function tips_add_settings_group($library, $settings) {
  $result = db_insert('tips_settings_registry')
    ->fields(array(
      'library' => $library,
      'settings' => serialize($settings),
    )
  )
  ->execute();
}

/**
 * Delete settings group with id condition.
 */
function tips_delete_settings_group($settings_id) {
  if (is_numeric($settings_id)) {
    db_delete('tips_settings_registry')
      ->condition('settings_id', $settings_id)
      ->execute();
  }
}

/**
 * Insert new tooltip collection into database.
 */
function tips_add_tooltip_group($values) {
  $result = db_insert('tips_registry')
    ->fields(array(
      'selector' => $values['selector'],
      'pages' => $values['pages'],
      'settings_id' => $values['settings_id'],
      'content_id' => $values['content_id'],
    )
  )
  ->execute();
}

/**
 * Delete selector group.
 */
function tips_delete_selector($tip_id) {
  if (is_numeric($tip_id)) {
    db_delete('tips_registry')
      ->condition('tip_id', $tip_id)
      ->execute();
  }
}

/**
 * Loads a tooltip content group.
 */
function tips_load_content($content_id) {
  $result = db_query('SELECT tc.content
    FROM {tips_content_registry} AS tc
    WHERE tc.content_id = :content_id', array(':content_id' => $content_id));
  $content = $result->fetchAssoc();

  return $content['content'];
}

/**
 * Update or save tooltip content group.
 */
function tips_update_content($content_id = NULL, $content) {
  if ($content_id && is_numeric($content_id)) {
    db_update('tips_content_registry')
      ->fields(array(
      'content' => $content,
    ))
    ->condition('content_id', $content_id)
    ->execute();
  }
  else {
    db_insert('tips_content_registry')
      ->fields(array(
        'content' => $content,
    ))
    ->execute();
  }
}

/**
 * Delete settings group with id condition.
 */
function tips_delete_content_group($content_id) {
  if (is_numeric($content_id)) {
    db_delete('tips_content_registry')
      ->condition('content_id', $content_id)
      ->execute();
  }
}
