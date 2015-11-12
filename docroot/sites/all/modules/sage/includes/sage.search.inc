<?php

/**
 * @file
 * Contains the form to build the search page.
 */

// Include inc files.
module_load_include('inc', 'sage', 'includes/sage.functions');
module_load_include('inc', 'sage', 'includes/sage.xmlstrtoarray');

/**
 * The (Advanced) Search form for SAGE API.
 *
 * @param array $form
 *   Nested array of form elements that comprise the form.
 * @param array &$form_state
 *   A keyed array containing the current state of the form.
 */
function sage_form_search(array $form, array &$form_state) {

  // Help texts.
  $form['sage_form_search_help'] = array(
    '#type' => 'markup',
    '#markup' => variable_get('sage_api_search_help', ""),
    '#prefix' => "<div class='search-help'>",
    '#suffix' => '</div>',
  );
  // General options.
  $form['sage_form_search_category'] = array(
    '#type' => 'select',
    '#title' => t('Category:'),
    '#default_value' => sage_get_url_parameter("category"),
    '#options' => sage_get_categories_array(),
    '#description' => t("Search by Category."),
  );
  $form['sage_form_search_keywords'] = array(
    '#type' => 'textfield',
    '#title' => t('Keywords:'),
    '#default_value' => sage_get_url_parameter("keywords"),
    '#size' => 128,
    '#maxlength' => 128,
    '#description' => t("Search by Keywords."),
  );
  $form['sage_form_search_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Color:'),
    '#default_value' => sage_get_url_parameter("color"),
    '#size' => 128,
    '#maxlength' => 128,
    '#description' => t("Search by Color."),
  );
  $form['sage_form_search_spc'] = array(
    '#type' => 'textfield',
    '#title' => t('Item #:'),
    '#default_value' => sage_get_url_parameter("spc"),
    '#size' => 128,
    '#maxlength' => 128,
    '#description' => t("Search by Item Number."),
  );
  // Price group.
  $form['sage_form_search_price'] = array(
    '#type' => 'fieldset',
    '#title' => t('Price:'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['sage_form_search_price']['sage_form_search_price_low'] = array(
    '#type' => 'textfield',
    '#title' => t('Price (From):'),
    '#default_value' => sage_get_url_parameter("lop"),
    '#size' => 4,
    '#maxlength' => 4,
    '#element_validate' => array('sage_element_validate_positive_number'),
    '#description' => t("Minimum price to search for."),
  );
  $form['sage_form_search_price']['sage_form_search_price_high'] = array(
    '#type' => 'textfield',
    '#title' => t('Price (To):'),
    '#default_value' => sage_get_url_parameter("hip"),
    '#size' => 4,
    '#maxlength' => 4,
    '#element_validate' => array('sage_element_validate_positive_number'),
    '#description' => t("Maximum price to search for."),
  );
  $form['sage_form_search_quantity'] = array(
    '#type' => 'textfield',
    '#title' => t('Qty:'),
    '#default_value' => sage_get_url_parameter("lop"),
    '#size' => 6,
    '#maxlength' => 6,
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t("Minimum quantity."),
  );
  // Other.
  $form['sage_form_search_other'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other:'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['sage_form_search_other']['sage_form_search_madeinusa'] = array(
    '#type' => 'checkbox',
    '#title' => t('Made in USA'),
    '#default_value' => sage_get_url_parameter("madeinusa"),
    '#description' => t("Made in USA."),
  );
  $form['sage_form_search_other']['sage_form_search_envfriendly'] = array(
    '#type' => 'checkbox',
    '#title' => t('Environmentally Friendly'),
    '#default_value' => sage_get_url_parameter("envfriendly"),
    '#description' => t("Environmentally Friendly."),
  );
  // Form actions.
  $form['actions']['search'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
    '#weight' => 13,
    '#submit' => array('sage_search_form_submit'),
  );
  $form['actions']['clear'] = array(
    '#type' => 'submit',
    '#value' => t('Clear'),
    '#weight' => 10,
    '#submit' => array('sage_search_form_reset'),
  );

  return $form;
}

/**
 * Submit callback for SAGE Search form.
 *
 * Sends the user to a different page.
 *
 * @param array $form
 *   Nested array of form elements that comprise the form.
 * @param array &$form_state
 *   A keyed array containing the current state of the form.
 */
function sage_search_form_submit(array $form, array &$form_state) {

  $query = array(
    'category' => $form_state['values']['sage_form_search_category'],
    'keywords' => $form_state['values']['sage_form_search_keywords'],
    'color' => $form_state['values']['sage_form_search_color'],
    'spc' => $form_state['values']['sage_form_search_spc'],
    'lop' => $form_state['values']['sage_form_search_price_low'],
    'hip' => $form_state['values']['sage_form_search_price_high'],
    'qty' => $form_state['values']['sage_form_search_quantity'],
    'madeinusa' => sage_boolean_zero($form_state['values']['sage_form_search_madeinusa']),
    'envfriendly' => sage_boolean_zero($form_state['values']['sage_form_search_envfriendly']),
  );
  drupal_goto('sage/search/results', array('query' => $query));
}

/**
 * Reset form by removing any query parameters.
 *
 * Sends the user to the same page but with no parameters.
 *
 * @param array $form
 *   Nested array of form elements that comprise the form.
 * @param array &$form_state
 *   A keyed array containing the current state of the form.
 */
function sage_search_form_reset(array $form, array &$form_state) {
  drupal_goto(current_path());
}

/**
 * Validate positive float 
 */
function sage_element_validate_positive_number($element, &$form_state) {
  $value = $element['#value'];
  if ($value != '' && (!is_numeric($value) || $value < 0)) {
    form_error($element, t('%name must be a positive number.', array('%name' => $element['#title'])));
  }
}
