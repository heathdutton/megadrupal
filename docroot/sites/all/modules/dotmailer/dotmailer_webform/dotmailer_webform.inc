<?php

/**
 * @file
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_dotmailer() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'mandatory' => 0,
    'extra' => array(
      'description' => NULL,
      'dotmailer_display' => NULL,
      'dotmailer_mailing_list' => NULL,
      'dotmailer_email_field' => NULL,
      'private' => FALSE,
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_dotmailer($component) {
  $form = array();

  $form['extra']['dotmailer_display'] = array(
    '#type' => 'select',
    '#default_value' => $component['extra']['dotmailer_display'],
    '#options' => array(
      DOTMAILER_BOOKTYPE_REQUIRED => 'Hidden',
      DOTMAILER_BOOKTYPE_OPTIN => 'Checkbox',
    ),
    '#title' => t('Element display'),
    '#description' => t('Choose required to force the user to subscribe to the mailing list without displaying any UI on the form.
      Choose checkbox to have a checkbox displayed allowing the user to opt-in. Hint: You can choose checkbox and make the element mandatory
      to ensure the user explicitly confirms their desire to sign up'),
    '#required' => TRUE,
  );

  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default state'),
    '#default_value' => $component['value'],
    '#description' => t('If empty, then the default value is "unchecked". If not empty or you provide a token, and that token is not empty when the form is displayed then the default will be "checked"') . theme('webform_token_help'),
    '#states' => array(
      'visible' => array(
        ':input[name="extra[dotmailer_display]"]' => array('value' => DOTMAILER_BOOKTYPE_OPTIN),
      ),
    ),
  );

  // Note we are not filtering the list. We preusme that the form could be filled
  // out by anonymous users and therefore they should be given the chance to sign
  // up to required address books.
  $options = array();
  foreach (dotmailer_get_active_address_books() as $address_book) {
    $options[$address_book->Id] = $address_book->Name;
  }
  $form['extra']['dotmailer_mailing_list'] = array(
    '#type' => 'select',
    '#default_value' => $component['extra']['dotmailer_mailing_list'],
    '#options' => $options,
    '#title' => t('dotMailer mailing list'),
    '#description' => t('Choose the mailing list the user should be subscribed to.'),
    '#required' => TRUE,
  );

  // Field mappings - we are just doing email for now.
  $node = node_load($component['nid']);
  $email_components = array();
  foreach ($node->webform['components'] as $cid => $_component) {
    if ($_component['type'] !== 'email') {
      continue;
    }
    $email_components[$cid] = $_component['name'];
  }

  if (empty($email_components)) {
    drupal_set_message(t('The dotmailer component requires your form to have at least one email field to use when subscribing users.'), 'error');
  }

  $form['extra']['dotmailer_email_field'] = array(
    '#type' => 'select',
    '#default_value' => $component['extra']['dotmailer_email_field'],
    '#options' => $email_components,
    '#title' => t('Email field'),
    '#description' => t('Select the e-mail field to be used when subscribing the user to the address book.'),
    '#required' => TRUE,
  );


  // Hide the 'display' fieldset because we don't need it.
  $form['display']['#type'] = 'markup';

  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_dotmailer($component, $value = NULL, $filter = TRUE) {

  $type = ($component['extra']['dotmailer_display'] == DOTMAILER_BOOKTYPE_REQUIRED) ? 'value' : 'checkbox';

  $element = array(
    '#type' => $type,
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#description' => $filter ? _webform_filter_descriptions($component['extra']['description']) : $component['extra']['description'],
    '#return_value' => $component['extra']['dotmailer_mailing_list'],
    '#element_validate' => array('_dotmailer_webform_validate_component'),
  );


  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;
  if (isset($value[0])) {
    $default_value = $value[0];
  }
  else{
    $default_value = $filter ? _webform_filter_values($component['value'], $node) : $component['value'];
  }

  if ($type == 'checkbox' && $default_value) {
    $element['#default_value'] = $element['#return_value'];
  }

  if ($type == 'value') {
    $element['#value'] = $component['extra']['dotmailer_mailing_list'];
  }

  return $element;
}

/**
 * Clever old webform disables Drupal's built in validation - for reasons I don't
 * fully understand - and replaces it with something that can't validate checkboxes
 */
function _dotmailer_webform_validate_component($element) {
  // Check the element is ticked by comparing it's submitted value to the value
  // it would have if it where ticked.
  if ($element['#required'] && $element['#value'] !== $element['#return_value']) {
    form_error($element, t('!name is required.', array('!name' => check_plain($element['#title']))));
  }
}

function _webform_display_dotmailer($component, $value, $format = 'html') {

  $answer = 'No';
  if (!empty($value[0])) {
    $answer = 'Yes';
  }

  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_textfield',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#post_render' => array('webform_element_wrapper'),
    '#field_prefix' => '',
    '#field_suffix' => '',
    '#component' => $component,
    '#format' => $format,
    '#value' => $answer,
  );
}

//Render - for building the form
//Display - for displaying the results
//Submit - an alter hook?
//More is welcome

