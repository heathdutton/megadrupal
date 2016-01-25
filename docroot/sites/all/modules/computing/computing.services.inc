<?php

/**
 * Implements hook_services_resources().
 */
function computing_services_resources() {
  $resources = array(
    'computing' => array(
      'operations' => array(
        'retrieve' => array(
          'help' => t('Retrieves a computing record.'),
          //'file' => array('type' => 'module', 'module' => 'computing', 'name' => 'computing'),
          'callback' => 'computing_load',
          'args' => array(
            array(
              'name' => 'id',
              'optional' => FALSE,
              'source' => array('path' => 0),
              'type' => 'int',
              'description' => 'The id of the computing record to load',
            ),
          ),
          'access callback' => 'computing_access',
          'access arguments' => array(array('administer computing records', 'access computing services endpoints')),
          //'access callback file' => array('type' => 'inc', 'module' => 'computing', 'name' => 'computing.entity'),
        ),
        'create' => array(
          'help' => 'Creates a computing record.',
          'file' => array('type' => 'inc', 'module' => 'computing', 'name' => 'computing.services'),
          'callback' => 'computing_create_services_wrapper',
          'args' => array(
            array(
              'name' => 'application',
              'optional' => FALSE,
              'source' => array('data' => 'application'),
              'description' => 'The application that this computing record belongs to.',
              'type' => 'string',
            ),
            array(
              'name' => 'command',
              'optional' => FALSE,
              'source' => array('data' => 'command'),
              'description' => 'The command that this computing record will execute on.',
              'type' => 'string',
            ),
            array(
              'name' => 'label',
              'optional' => TRUE,
              'source' => array('data' => 'label'),
              'description' => 'The human-readable label of the computing record.',
              'type' => 'string',
            ),
            array(
              'name' => 'input',
              'optional' => TRUE,
              'source' => array('data' => 'input'),
              'description' => 'JSON encoded string for input.',
              'type' => 'string',
            ),
//            array(
//              'name' => 'options',
//              'optional' => TRUE,
//              'source' => array('data' => 'options'),
//              'description' => 'JSON encoded string as extra options.',
//              'type' => 'string',
//            ),
            array(
              'name' => 'data',
              'optional' => FALSE,
              'source' => 'data',
              'description' => 'The computing record data from remote caller.',
              'type' => 'array',
            )
          ),
          'access callback' => 'computing_access',
          'access arguments' => array(array('administer computing records', 'access computing services endpoints')),
        ),
        'update' => array(
          'help' => 'Update a computing record.',
          'callback' => 'computing_update_services_wrapper',
          'file' => array('type' => 'inc', 'module' => 'computing', 'name' => 'computing.services'),
          'args' => array(
            array(
              'name' => 'id',
              'optional' => FALSE,
              'source' => array('path' => 0),
              'type' => 'int',
              'description' => 'The id of the computing record to update',
            ),
            array(
              'name' => 'data',
              'optional' => FALSE,
              'source' => 'data',
              'type' => 'array',
              'description' => 'The computing record to be updated.',
            ),
          ),
          'access callback' => 'computing_access',
          'access arguments' => array(array('administer computing records', 'access computing services endpoints')),
        ),
      ),
      'actions' => array(
        'info' => array(
          'help' => t('Get basic system info.'),
          'callback' => 'computing_site_info',
          'access callback' => 'computing_access',
          'access arguments' => array('access computing services endpoints'),
        ),
        'claim' => array(
          'help' => 'Claim an unprocessed computing record to process.',
          'callback' => 'computing_claim',
          'args' => array(
            array(
              'name' => 'application',
              'optional' => FALSE,
              'source' => array('data' => 'application'),
              'type' => 'string',
              'description' => 'The application from which to claim the record.',
            ),
          ),
          'access callback' => 'computing_access',
          'access arguments' => array(array('administer computing records', 'access computing services endpoints')),
        ),
      ),
      'targeted_actions' => array(
        'field' => array(
          'help' => 'Update a single field of a computing record.',
          'callback' => 'computing_update_field',
          'args' => array(
            array(
              'name' => 'id',
              'optional' => FALSE,
              'source' => array('path' => 0),
              'type' => 'int',
              'description' => 'The id of the computing record to update',
            ),
            array(
              'name' => 'field_name',
              'optional' => FALSE,
              'source' => array('data' => 'name'),
              'type' => 'string',
              'description' => 'The field to be updated.',
            ),
            array(
              'name' => 'field_value',
              'optional' => FALSE,
              'source' => array('data' => 'value'),
              'type' => 'mixed', // it seems this is only a memo, not anything enforced in services.module.
              'description' => 'The field value to be updated.',
            ),
          ),
          'access callback' => 'computing_access',
          'access arguments' => array(array('administer computing records', 'access computing services endpoints')),
        ),
        'finish' => array(
          'help' => 'Mark a computing record as done.',
          'callback' => 'computing_finish',
          'args' => array(
            array(
              'name' => 'id',
              'optional' => FALSE,
              'source' => array('path' => 0),
              'type' => 'int',
              'description' => 'The id of the computing record to update',
            ),
            array(
              'name' => 'status',
              'optional' => FALSE,
              'source' => array('data' => 'status'),
              'type' => 'string',
              'description' => 'The status of the computing record to update.',
            ),
            array(
              'name' => 'message',
              'optional' => FALSE,
              'source' => array('data' => 'message'),
              'type' => 'string',
              'description' => 'The message form agent.',
            ),
            array(
              'name' => 'output',
              'optional' => TRUE,
              'source' => array('data' => 'output'),
              'type' => 'mixed',
              'description' => 'The output from agent.',
            ),
            array(
              'name' => 'options',
              'optional' => TRUE,
              'source' => array('data' => 'options'),
              'type' => 'mixed',
              'description' => 'Extra data to write to computing record.',
            ),
          ),
          'access callback' => 'computing_access',
          'access arguments' => array(array('administer computing records', 'access computing services endpoints')),
        ),
      ),
    ),
  );
  return $resources;
}


function computing_create_services_wrapper($app_name, $command, $label, $input, $data) {
  //drupal_debug(func_get_args());
  unset($data['application']);
  unset($data['command']);
  unset($data['label']);
  unset($data['input']);
  $return = computing_create($app_name, $command, $label, $input, $data);
  return $return;
}


function computing_update_services_wrapper($id, $data) {
  //drupal_debug(func_get_args(), "computing_update_services_wrapper");
  if ($id != $data['id']) {
    return FALSE;
  }
  return computing_update((object) $data);
}