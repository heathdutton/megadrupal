<?php


/**
 * Implements hook_rules_event_info().
 */
function computing_rules_event_info() {
  $events = array(
    'computing_event_created' => array(
      'label' => t('A new computing record is created'),
      'group' => t('Computing'),
      'variables' => array(
        'computing_record' => array(
          'type' => 'computing_record',
          'label' => t('Computing record'),
        ),
      ),
    ),
    'computing_event_updated' => array(
      'label' => t('A computing record is updated'),
      'help' => t('This event is not supposed to work for "claim" and "finish" operations. Use corresponded events instead.'),
      'group' => t('Computing'),
      'variables' => array(
        'computing_record' => array(
          'type' => 'computing_record',
          'label' => t('Computing record'),
        ),
      ),
    ),
    'computing_event_claimed' => array(
      'label' => t('A  computing record is claimed by agent'),
      'group' => t('Computing'),
      'variables' => array(
        'computing_record' => array(
          'type' => 'computing_record',
          'label' => t('Computing record'),
        ),
      ),
    ),
    'computing_event_finished' => array(
      'label' => t('A computing record is finished by agent'),
      'group' => t('Computing'),
      'variables' => array(
        'computing_record' => array(
          'type' => 'computing_record',
          'label' => t('Computing record'),
        ),
      ),
    ),
    'computing_event_released' => array(
      'label' => t('A  computing record is released back to Drupal'),
      'group' => t('Computing'),
      'variables' => array(
        'computing_record' => array(
          'type' => 'computing_record',
          'label' => t('Computing record'),
        ),
      ),
    ),
  );
  return $events;
}



/**
 * Implements hook_rules_action_info().
 * Possible actions in the future: create, update, claim, finish a record.
 */
function computing_rules_action_info() {
  $actions = array(
    'computing_action_handle_output' => array(
      'label' => t('Process output data from agent.'),
      'group' => t('Computing'),
      'parameter' => array(
        'computing_record' => array(
          'type' => 'computing_record',
          'label' => t('Computing record'),
        ),
      ),
    ),
  );
  return $actions;
}


/**
 * Implements hook_rules_condition_info().
 * This is not needed for now. Possible conditions: application/command is ...
 */
//function computing_rules_condition_info() {
//  $conditions = array(
//  );
//  return $conditions;
//}


function computing_action_handle_output($record) {
  // TODO: $record->command might not be found in hook_computing_data() if the real "command" was defined.
  $command_data = computing_fetch_data($record->application, $record->command);

  // load file if necessary.
  if ($command_data && isset($command_data['file']) && is_array($command_data['file'])) {
    call_user_func_array('module_load_include', $command_data['file']);
  }

  if ($command_data && isset($command_data['result callback']) && function_exists($command_data['result callback'])) {
    // if 'result callback' is defined, use it.
    $output = isset($record->output) ? $record->output : NULL;
    return call_user_func($command_data['result callback'], $output, $record);
  }

  return FALSE;
}