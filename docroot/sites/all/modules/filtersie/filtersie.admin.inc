<?php

/**
 * @file
 * Admin filters config pages.
 */

/**
 * Generic kernel convolution config form.
 */
function filtersie_generic_form($data) {

  $form['help'] = array(
    '#markup' => t('Take a look at the folling page for an overview and some example matrices about convolution: !p.', array(
      '!p' => l(t('http://docs.gimp.org/en/plug-in-convmatrix.html'), t('http://docs.gimp.org/en/plug-in-convmatrix.html'))
    )),
  );

  if (isset($data['matrix'])) {
    $matrix_values = $data['matrix']['entries'];
  }
  else {
    $matrix_values = array_fill(0, 3, array_fill(0, 3, 1));
  }

  // kernel matrix inputs
  $form['matrix'] = array(
    '#type' => 'item',
    '#title' => t('Kernel matrix'),
    '#description' => t('Entries sum: ') . '<span class="filtersie-matrix-sum">0</span>.',
    '#collapset' => FALSE,
    '#required' => TRUE,
    '#prefix' => '<div class="filtersie-matrix-wrapper">',
    '#suffix' => '</div>',
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'filtersie') . '/filtersie.admin.css'),
      'js' => array(
        drupal_get_path('module', 'filtersie') . '/filtersie.admin.js',
//        array(
//          'type' => 'setting',
//          'data' => array('matrix' => $matrix_values),
//        ),
      )
    )
  );
  $form['matrix']['entries'] = array();
  for ($i = 0; $i < 3; $i++) {
    $form['matrix']['entries'][$i] = array(
      '#type' => 'fieldset',
    );
    for ($j = 0; $j < 3; $j++) {
      $form['matrix']['entries'][$i][$j] = array(
        '#type' => 'textfield',
        '#title' => t('Matrix entry') . " ($i,$j)",
        '#title_display' => 'invisible',
        '#default_value' => $matrix_values[$i][$j],
        '#size' => 3,
        '#required' => TRUE,
        '#element_validate' => array('filtersie_numericInput_validate'),
      );
    }
  }

  // divisor input
  $form['divisor'] = array(
    '#type' => 'textfield',
    '#title' => t('Divisor'),
    '#description' => t('Typically the matrix entries sum (normalization).'),
    '#default_value' => isset($data['divisor']) ? $data['divisor'] : 1,
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => TRUE,
    '#element_validate' => array('filtersie_numericInput_validate'),
  );

  // offset input
  $form['offset'] = array(
    '#type' => 'textfield',
    '#title' => t('Offset'),
    '#description' => t('This value is added to the division result.'),
    '#default_value' => isset($data['offset']) ? $data['offset'] : 0,
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => TRUE,
    '#element_validate' => array('filtersie_numericInput_validate'),
  );

  // filter label input
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#description' => t('A label to identify this filter effect.'),
    '#default_value' => isset($data['label']) ? $data['label'] : '',
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Sharpen config form.
 */
function filtersie_sharpen_form($data) {
  $form['sharpenlevel'] = array(
    '#type' => 'textfield',
    '#title' => t('Sharpen level'),
    '#description' => t('Typically 1 - 50.'),
    '#default_value' => isset($data['sharpenlevel']) ? $data['sharpenlevel'] : 10,
    '#required' => TRUE,
    '#size' => 3,
    '#element_validate' => array('image_effect_integer_validate'),
    '#allow_negative' => FALSE,
  );
  return $form;
}

/**
 * Sharpen (using Unsharp masking technique) config form.
 */
function filtersie_sharpenUSM_form($data) {
  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount'),
    '#description' => t('Typically 50 - 200.'),
    '#default_value' => isset($data['amount']) ? $data['amount'] : 80,
    '#required' => TRUE,
    '#size' => 3,
    '#element_validate' => array('image_effect_integer_validate'),
    '#allow_negative' => FALSE,
  );
  $form['radius'] = array(
    '#type' => 'textfield',
    '#title' => t('Radius'),
    '#description' => t('Typically 0.5 - 1.'),
    '#default_value' => isset($data['radius']) ? $data['radius'] : 0.5,
    '#required' => TRUE,
    '#size' => 3,
    '#element_validate' => array('filtersie_numericInput_validate'),
    '#allow_negative' => FALSE,
  );
  $form['threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Threshold'),
    '#description' => t('Typically 0 - 5.'),
    '#default_value' => isset($data['threshold']) ? $data['threshold'] : 3,
    '#required' => TRUE,
    '#size' => 3,
    '#element_validate' => array('filtersie_numericInput_validate'),
    '#allow_negative' => FALSE,
  );
  return $form;
}

/**
 * Validate input type number (int, float or double).
 */
function filtersie_numericInput_validate($element, &$form_state) {
  $value = empty($element['#allow_negative']) ? $element['#value'] : preg_replace('/^-/', '', $element['#value']);
  if ($element['#value'] != '' && (!is_numeric($value))) {
    if (empty($element['#allow_negative'])) {
      form_error($element, t('!name must be a number.', array('!name' => $element['#title'])));
    }
    else {
      form_error($element, t('!name must be a positive number.', array('!name' => $element['#title'])));
    }
  }
}
