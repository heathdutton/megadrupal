<?php

/**
 * @file
 * Provides rules integration for character sheets.
 */

/**
 * Implements hook_rules_event_info().
 */
function character_sheet_rules_event_info() {
  $events = array();
  $events['character_sheet_event_updated'] = array(
    'label' => t('A character sheet is updated'),
    'group' => t('Character sheet'),
    'module' => 'character_sheet',
    'variables' => array(
      'sheet' => array(
        'type' => 'node',
        'label' => t('sheet'),
      ),
      'player' => array(
        'type' => 'user',
        'label' => t('player'),
      ),
    ),
  );
  return $events;
}