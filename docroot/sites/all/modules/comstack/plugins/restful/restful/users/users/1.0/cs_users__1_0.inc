<?php

$plugin = array(
  'label' => t('Users'),
  'resource' => 'cs/users',
  'name' => 'cs_users__1_0',
  'description' => t('Get a specific user formatted as per Comstack REST API documentation.'),
  'class' => 'ComstackUsersResource__1_0',
  'major_version' => 1,
  'minor_version' => 0,
  'entity_type' => 'user',
  'bundle' => 'user',
  'authentication_types' => TRUE,
  'authentication_optional' => FALSE,
  'autocomplete' => array(
    'enable' => FALSE,
  ),
  'formatter' => 'comstack_json',
);
