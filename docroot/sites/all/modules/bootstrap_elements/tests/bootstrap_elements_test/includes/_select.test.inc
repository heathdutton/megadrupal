<?php
/**
 * @file
 * Select field tests.
 */

/**
 * Helper function to generate select elements.
 */
function _boostrap_elements_select(&$form) {

  $form['select'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select Boxes'),
  );

  $form['select']['default'] = array(
    '#type' => 'select',
    '#title' => t('Select Default'),
    '#description' => t('Example description'),
    '#options' => drupal_map_assoc(array('apples', 'oranges')),

  );

  $form['select']['horizontal'] = array(
    '#type' => 'select',
    '#title' => t('Select Horizontal'),
    '#description' => t('Example description'),
    '#options' => drupal_map_assoc(array('apples', 'oranges')),
    '#form_type' => 'horizontal',
    '#title_grid' => array(
      'columns' => array(
        'md' => 3,
      ),
    ),
    '#element_grid' => array(
      'columns' => array(
        'md' => 9,
      ),
    ),
  );

  $form['select']['inline'] = array(
    '#type' => 'select',
    '#title' => t('Select Inline'),
    '#description' => t('Example description'),
    '#options' => drupal_map_assoc(array('apples', 'oranges')),
    '#form_type' => 'inline',

  );

}
