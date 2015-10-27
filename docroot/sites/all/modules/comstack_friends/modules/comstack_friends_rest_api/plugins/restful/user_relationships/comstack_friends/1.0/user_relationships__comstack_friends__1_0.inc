<?php

$plugin = array(
  'label' => t('Friends'),
  'resource' => 'cs-fr/friends',
  'name' => 'user_relationships__comstack_friends__1_0',
  'entity_type' => 'user_relationships',
  'bundle' => 'comstack_friends',
  'description' => t('Get and manipulate friend relationships.'),
  'class' => 'ComstackFriendsFriendsResource__1_0',
  'authentication_types' => TRUE,
  'authentication_optional' => FALSE,
  'formatter' => 'comstack_json',
);
