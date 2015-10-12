<?php

 /**
  * @file
  * Custom page; Create the page in which stuff will be themed.
  *
  */
function prh_search_page() {

  $content = '';

  if (arg(1) != 'verify') {

    $form = drupal_render(drupal_get_form('prh_search_form'));
  }
  else {

    $form = drupal_render(drupal_get_form('prh_search_confirm_form'));
  }

  return theme('main_page', array('content' => $content, 'form' => $form));
}

/**
 * Implements hook_form().
 */
function prh_search_form($form) {

  cache_clear_all();

  $form = array();

  $form['#prefix'] = t('Please enter the association name in the field below for verification against the PRH database');

  $form['prh_search_association'] = array(
    '#type' => 'textfield',
    '#title' => t('Association name'),
    '#default_value' => '',
    '#required' => TRUE,
  );

  $form['prh_search_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit search'),
  );

  return $form;
}

/**
 * Implements hook_form_validate().
 */
function prh_search_form_validate($form, &$form_state) {

  $file = new PRH();

  $code = $file->getValidAssociationFromPRH($form_state['values']['prh_search_association'], NULL);

  if (count($code) == NULL) {

    form_set_error('prh', t('The association name that you entered could not be found in the database. Please re-enter the name!'));
  }

  else {

    $_SESSION['association_name'] = $form_state['values']['prh_search_association'];
  }
}

/**
 * Implements hook_form().
 */
function prh_search_confirm_form() {

  $file = new PRH();

  $association = $file->getValidAssociationFromPRH($_SESSION['association_name'], NULL);

  if (count($association) != NULL) {
    $options = array();

    foreach ($association as $key => $val) {

      $prefix = "<p>" . t('If the details given below are correct, click the Submit button to store the association info to the site.') . "</p><table>
                <tr><td class='title'>" . t('Association name') . "</td><td class='info'>" . check_plain($val->nimi) . "</td></tr>
                <tr><td class='title'>" . t('Register number') . "</td><td class='info'>" . check_plain($val->rekisterinumero) . "</td></tr>
                <tr><td class='title'>" . t('Home city') . "</td><td class='info'>" . check_plain($val->kotipaikka) . "</td></tr>
                <tr><td class='title'>" . t('Registration date') . "</td><td class='info'>" . check_plain($val->rekisterointipaiva) . "</td></tr>
                <tr><td class='title'>" . t('Last registration date') . "</td><td class='info'>" . check_plain($val->viim_rekpaiva) . "</td></tr>
                </table>";
    }

    $form = array();

    $form['#prefix'] = $prefix;

    $form['prh_search_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save association'),
    );
  }
  else {
    drupal_goto('association_search');
  }

  return $form;
}

/**
 * Implements hook_form_submit().
 */
function prh_search_confirm_form_submit(&$form, $form_state) {

  $file = new PRH();

  $association = $file->getValidAssociationFromPRH($_SESSION['association_name'], NULL);

  $node = new stdClass();

  $node->type = 'prh_association';

  node_object_prepare($node);

  $node->title = check_plain((string)$association->nimi);
  $node->language = LANGUAGE_NONE;

  $node->body[$node->language][0]['value'] = '';
  $node->body[$node->language][0]['summary'] = text_summary('');
  $node->body[$node->language][0]['format']  = 'filtered_html';

  $node->field_register_number['und'][0]['value'] = check_plain($association->rekisterinumero);
  $node->field_registration_date['und'][0]['value'] = check_plain($association->rekisterointipaiva);
  $node->field_homecity['und'][0]['value'] = check_plain($association->kotipaikka);
  $node->field_last_registration_date['und'][0]['value'] = check_plain($association->viim_rekpaiva);

  $path = 'content/' . strtolower(str_replace(' ', '_', $node->title));

  $node->path = array('alias' => check_plain($path));

  node_save($node);

  global $base_url;

  drupal_set_message(t('Association created successfully. You may visit your newly created association page at !url', array('!url' => l($base_url . '/' . $node->path['alias'], $base_url . '/' . $node->path['alias']))));
}

/**
 * Implements hook_form_submit().
 */
function prh_search_form_submit(&$form, $form_state) {

  drupal_goto('association_search/verify');
}
