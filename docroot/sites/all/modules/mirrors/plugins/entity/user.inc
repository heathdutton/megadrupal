<?php

/**
 * @file
 * Mirrors Node entity plugin declaration.
 */

/**
 * Entity Declaration: user.
 */
$plugin = array(
  'title' => t('User'),
  'category' => 'Entity',
  'views' => array(
    'base_table' => 'users',
    'filters' => array(),
    'sorts' => array(
      'uid' => 'users',
    ),
  ),
  'feeds' => array(
    'processor' => 'FeedsUserProcessor',
  ),
  'properties' => array(
    'name' => array(
      'type' => 'text',
      'feeds' => array(
        'unique' => TRUE,
      ),
      'views' => array(
        'link_to_user' => FALSE,
      ),
    ),
    'mail' => array(
      'type' => 'text',
      'views' => array(
        'link_to_user' => '0',
      ),
    ),
    'created' => array(
      'type' => 'date',
    ),
    'status' => array(
      'type' => 'boolean',
    ),
    'language' => array(
      'type' => 'text',
      'views' => array(
        'link_to_user' => FALSE,
      ),
    ),
  ),
);
