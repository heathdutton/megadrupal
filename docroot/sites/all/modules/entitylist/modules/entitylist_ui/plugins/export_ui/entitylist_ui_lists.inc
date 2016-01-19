<?php

$plugin = array(
  'schema' => 'entitylist_lists',
  'access' => 'administer entity lists',

  'menu' => array(
    'menu prefix' => 'admin/structure/entitylists',
    'menu item' => 'lists',
    'menu title' => 'Lists',
    'menu description' => 'Manage the abstraction of content lists.',
  ),

  'export' => array(
    'admin_title' => 'title',
    'admin_description' => 'description',
  ),

  'title singular' => t('list'),
  'title singular proper' => t('List'),
  'title plural' => t('lists'),
  'title plural proper' => t('Lists'),

  'use wizard' => TRUE,

  'handler' => array(
    'class' => 'entitylist_ui_lists',
    'parent' => 'ctools_export_ui',
  ),

  'form info' => array(
    'order' => array(
      'basic' => t('Basics'),
      'settings' => t('Settings'),
      'preview' => t('Preview'),
    ),
  ),
);
