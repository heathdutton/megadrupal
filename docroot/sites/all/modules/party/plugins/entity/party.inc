<?php
/**
 * @file
 * Definition of the party plugin.
 */

$plugin = array(
  'handler' => 'PanelizerEntityParty',
  'uses page manager' => FALSE,
  'hooks' => array(
    'menu' => TRUE,
    'admin_paths' => TRUE,
    'permission' => TRUE,
    'panelizer_defaults' => TRUE,
    'form_alter' => TRUE,
    'views_data_alter' => TRUE,
  ),
);
