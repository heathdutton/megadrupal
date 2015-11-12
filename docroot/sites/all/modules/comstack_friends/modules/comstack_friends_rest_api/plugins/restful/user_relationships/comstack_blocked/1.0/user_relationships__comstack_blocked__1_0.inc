<?php

$plugin = array(
  'label' => t('Blocked'),
  'resource' => 'cs-fr/blocked',
  'name' => 'user_relationships__comstack_blocked__1_0',
  'entity_type' => 'user_relationships',
  'bundle' => 'comstack_blocked',
  'description' => t('Get and manipulate block relationships.'),
  'class' => 'ComstackFriendsBlockedResource__1_0',
  'authentication_types' => TRUE,
  'authentication_optional' => FALSE,
  'formatter' => 'comstack_json',
);
