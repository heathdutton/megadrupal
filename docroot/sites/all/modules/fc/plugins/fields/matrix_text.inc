<?php
/**
 * @file
 * Field Complete - Provides field-based completeness for any entity - matrix (ext) field plugin.
 */

if (!defined('MATRIX_COMPLETE_ANY')) {
  define('MATRIX_COMPLETE_ANY', 1);
  define('MATRIX_COMPLETE_ALL', 2);
  define('MATRIX_COMPLETE_ROW', 3);
  define('MATRIX_COMPLETE_COL', 4);
}

$plugin = array(
  'title' => t('FC: Matrix (text) field plugin'),
  'can be empty' => TRUE,
  'zero can be empty' => TRUE,

  'completeness check' => array(
    'file' => 'matrix.inc',
    'path' => drupal_get_path('module', 'fc') . '/plugins/fields',
  	'function' => 'fc_matrix_field_is_complete',
  ),
  'cardinality check' => array(
    'file' => 'matrix.inc',
    'path' => drupal_get_path('module', 'fc') . '/plugins/fields',
  	'function' => 'fc_matrix_field_cardinality',
  ),

  'incomplete process' => array(
    'file' => 'matrix.inc',
    'path' => drupal_get_path('module', 'fc') . '/plugins/fields',
  	'function' => 'fc_matrix_incomplete_process',
  ),

  'field form' => array(
    'fc_check_cells' => array(
      '#type' => 'radios',
      '#title' => t('Check matrix for completeness'),
      '#options' => array(
        MATRIX_COMPLETE_ANY => t('Any cell has a value'),
        MATRIX_COMPLETE_ALL => t('All cells have a value'),
        MATRIX_COMPLETE_ROW => t('At least one row has cells with a value'),
        MATRIX_COMPLETE_COL => t('At least one column has cells with a value'),
      ),
    ),
  ),
);

