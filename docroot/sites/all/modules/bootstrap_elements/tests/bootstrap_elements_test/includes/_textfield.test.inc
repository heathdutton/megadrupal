<?php

/**
 * @file
 * Textfield tests.
 */

/**
 * Helper function to generate textfield FAPI elements.
 */
function _boostrap_elements_textfield(&$form) {

  $form['textfield'] = array(
    '#type' => 'fieldset',
    '#title' => t('Textfields'),
  );

  $form['textfield']['default'] = array(
    '#type' => 'textfield',
    '#title' => t('Textfield Default'),
    '#description' => t('Example description'),
    '#attributes' => array(
      'placeholder' => t('Example placeholder'),
    ),
  );

  $form['textfield']['horizontal_inline'] = array(
    '#type' => 'textfield',
    '#title' => t('Textfield Horizontal (Inline)'),
    '#description' => t('Example description'),
    '#attributes' => array(
      'placeholder' => t('Example placeholder'),
    ),
    '#form_type' => 'horizontal',
    '#title_display' => 'inline',
    '#title_grid' => array(
      'columns' => array(
        'md' => 4,
      ),
    ),
    '#element_grid' => array(
      'columns' => array(
        'md' => 6,
      ),
    ),
  );

  $form['textfield']['horizontal_before'] = array(
    '#type' => 'textfield',
    '#title' => t('Textfield Horizontal (Before)'),
    '#description' => t('Example description'),
    '#attributes' => array(
      'placeholder' => t('Example placeholder'),
    ),
    '#form_type' => 'horizontal',
    '#title_display' => 'before',
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

  $form['textfield']['horizontal_after'] = array(
    '#type' => 'textfield',
    '#title' => t('Textfield Horizontal (After)'),
    '#description' => t('Example description'),
    '#attributes' => array(
      'placeholder' => t('Example placeholder'),
    ),
    '#form_type' => 'horizontal',
    '#title_display' => 'after',
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

  $form['textfield']['inline'] = array(
    '#type' => 'textfield',
    '#title' => t('Textfield Inline'),
    '#description' => t('Example description'),
    '#attributes' => array(
      'placeholder' => t('Example placeholder'),
    ),
    '#form_type' => 'inline',

  );

}
