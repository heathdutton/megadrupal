<?php

$plugin = array(
  'schema' => 'brick',
  'access' => 'administer brick',
  'create access' => 'create brick',

  'menu' => array(
    'menu item' => 'brick',
    'menu title' => 'Bricks',
    'menu description' => 'Add, edit or delete bricks, which can be used as blocks.',
  ),

  'title singular' => t('brick'),
  'title singular proper' => t('Brick'),
  'title plural' => t('bricks'),
  'title plural proper' => t('Bricks'),

  'handler' => array(
    'class' => 'brick_ui',
    'parent' => 'ctools_export_ui',
  ),

  'use wizard' => TRUE,
  'form info' => array(
    'order' => array(
      'basic' => t('Settings'),
      'context' => t('Context'),
      'content_type' => t('Content type'),
      'rules' => t('Visibility rules'),
    ),
  ),
);

