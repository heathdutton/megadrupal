<?php
/**
 * Defines property info for watchdog log entries, used by the log entry data
 * type to provide an useful metadata wrapper.
 */
function _pane_rules_watchdog_log_entry_info() {
  return array(
    'name' => array(
      'type' => 'text',
      'label' => t('The machine name of the pane'),
    ),
    'title' => array(
      'type' => 'text',
      'label' => t('Title of the pane'),
      'sanitized' => TRUE,
    ),
  );
}

/**
 * Implements hook_rules_event_info
 */
function pane_rules_event_info() {
  $defaults = array(
    'group' => t('pane'),
    'module' => 'pane',
    'access callback' => 'rules_pane_integration_access',
    'property info' => _pane_rules_watchdog_log_entry_info(),
    'wrap' => TRUE,
  );
  return array(
    'pane_update' => $defaults + array(
      'label' => t('After updating a Pane'),
      'variables' => array(
        'pane' => array('type' => 'pane', 'label' => t('updated pane')),
      ),
    ),
    'pane_view' => $defaults + array(
      'label' => t('A pane is viewed'),
      'variables' => array(
        'pane' => array('type' => 'pane', 'label' => t('viewed pane')),
      ),
    ),
  );
}

/**
 * Comment integration access callback.
 */
function rules_pane_integration_access($type, $name) {
  if ($type == 'event' || $type == 'condition') {
    return TRUE;
  }
}