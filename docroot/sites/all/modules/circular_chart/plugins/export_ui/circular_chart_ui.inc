<?php
/**
 * @file
 * Define this Export UI plugin.
 */

$plugin = array(
  'schema' => 'circular_chart_preset',
  'access' => 'administer circular_chart settings',

  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/config/media',
    'menu item' => 'circular-charts',
    'menu title' => 'Circular Chart Presets',
    'menu description' => 'Configure circular chart styles for front end display.',
  ),

  // Define user interface texts.
  'title singular' => t('circular chart'),
  'title plural' => t('circular charts'),
  'title singular proper' => t('Circular chart'),
  'title plural proper' => t('Circular charts'),

  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'circular_chart_preset_config',
    'validate' => 'circular_chart_preset_config_validate',
    'submit' => 'circular_chart_preset_config_submit',
  ),
);
