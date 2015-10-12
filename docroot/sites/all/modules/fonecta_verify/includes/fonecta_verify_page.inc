<?php

 /**
  * @file
  *
  * Code for the logic for dipalying the form for selecting a company
  */

/**
 * Custom page; Create the page in which stuff will be themed.
 *
 */
function fonecta_verify_page() {

  $content = '';

  if (arg(1) == 'verify') {

    $form = drupal_render(drupal_get_form('fonecta_verify_confirm_form'));
  }
  else {

    unset($_SESSION['company_name']);

    $form = drupal_render(drupal_get_form('fonecta_verify_form'));
  }

  return theme('main_page', array('content' => $content, 'form' => $form));
}

/**
 * Implements hook_form().
 */
function fonecta_verify_form($form) {

  cache_clear_all();

  $form = array();

  $form['#prefix'] = t('Please enter the company name in the field below for verification against the Fonecta database');

  $form['fonecta_verify_company'] = array(
    '#type' => 'textfield',
    '#title' => t('Company name'),
    '#default_value' => '',
    '#required' => TRUE,
  );

  $form['fonecta_verify_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit search'),
  );

  return $form;
}

/**
 * Implements hook_form_validate().
 */
function fonecta_verify_form_validate($form, &$form_state) {

  $file = new Fonecta();

  $code = $file->getCompanyList($form_state['values']['fonecta_verify_company']);

  if ($code['header']->MESSAGE_CODE == 'ER1010') {
    form_set_error('fonecta', t('Please enter a company name!'));
  }
  elseif ($code['header']->MESSAGE_CODE == 'ER9100') {
    form_set_error('fonecta', t('Too many search results. Please refine your search.'));
  }
  elseif ($code['header']->MESSAGE_CODE == 'ER9090' || $code['content'] == NULL) {
    form_set_error('fonecta', t('The company name that you entered could not be found in the database. Please re-enter the name!'));
  }
  else {
    $_SESSION['company_name'] = $form_state['values']['fonecta_verify_company'];
  }
}

/**
 * Implements hook_form().
 */
function fonecta_verify_confirm_form() {

  $file = new Fonecta();

  $company = $file->getCompanyList($_SESSION['company_name']);

  if ($company['content'] != NULL) {
    $options = array();

    foreach ($company['content'] as $val) {
      $options[str_replace(' ', '_', $val->SUBS_NAME)] = check_plain($val->SUBS_NAME) . "<br/><table>
                <tr><td class='title'>" . t('Street address') . "</td><td class='info'>" . check_plain($val->STREET_ADDRESS) . "</td></tr>
                <tr><td class='title'>" . t('Post office') . "</td><td class='info'>" . check_plain($val->ADDRESS_POSTOFFICE) . "</td></tr>
                <tr><td class='title'>" . t('City') . "</td><td class='info'>" . check_plain($val->LEVEL_2_LOCALITY) . "</td></tr>
                <tr><td class='title'>" . t('Phone') . "</td><td class='info'>" . check_plain($val->COMM_ADDRESS->VALUE) . "</td></tr>
                <tr><td class='title'>" . t('Longitude') . "</td><td class='info'>" . check_plain($val->LONGITUDE) . "</td></tr>
                <tr><td class='title'>" . t('Latitude') . "</td><td class='info'>" . check_plain($val->LATITUDE) . "</td></tr>
                </table>";
    }

    $form = array();

    $form['fonecta_verify_select'] = array(
      '#type' => 'radios',
      '#title' => t('Company name'),
      '#default_value' => '',
      '#required' => TRUE,
      '#options' => $options,
    );

    $form['fonecta_verify_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save company'),
    );
  }
  else {

    drupal_goto('company_search');
  }

  return $form;
}

/**
 * Implements hook_form_submit().
 */
function fonecta_verify_confirm_form_submit(&$form, $form_state) {

  $file = new Fonecta();

  $company = $file->getCompanyList(str_replace('_', ' ', $form_state['values']['fonecta_verify_select']));

  $node = new stdClass();

  $node->type = 'fonecta_company';

  node_object_prepare($node);

  $node->title = check_plain((string)$company['content'][0]->SUBS_NAME);
  $node->language = LANGUAGE_NONE;

  $node->body[$node->language][0]['value'] = '';
  $node->body[$node->language][0]['summary'] = text_summary('');
  $node->body[$node->language][0]['format']  = 'filtered_html';

  $node->field_streetaddress['und'][0]['value'] = check_plain($company['content'][0]->STREET_ADDRESS);
  $node->field_postoffice['und'][0]['value'] = check_plain($company['content'][0]->ADDRESS_POSTOFFICE);
  $node->field_city['und'][0]['value'] = check_plain($company['content'][0]->LEVEL_2_LOCALITY);
  $node->field_phonenumber['und'][0]['value'] = check_plain($company['content'][0]->COMM_ADDRESS->VALUE);
  $node->field_latitude['und'][0]['value'] = check_plain($company['content'][0]->LATITUDE);
  $node->field_longitude['und'][0]['value'] = check_plain($company['content'][0]->LONGITUDE);

  $path = 'content/' . strtolower(str_replace(' ', '_', $node->title));

  $node->path = array('alias' => check_plain($path));

  node_save($node);

  global $base_url;

  drupal_set_message(t('Company created successfully. You may visit your newly created company page at @url', array('@url' => '<a href="' . $base_url . '/' . $node->path['alias'] . '">' . $base_url . '/' . $node->path['alias'] . '</a>')));
}

/**
 * Implements hook_form_submit().
 */
function fonecta_verify_form_submit(&$form, $form_state) {

  drupal_goto('company_search/verify');
}
