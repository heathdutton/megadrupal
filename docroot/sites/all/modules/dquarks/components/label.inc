<?php

/**
 * @file
 * Quiz module label component.
 * @author IP-TECH
 */

/**
 * Create a default label component.
 */
function _dquarks_defaults_label() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'mandatory' => 0,
    'email' => 1,
    'extra' => array(
      'description' => '',
      'attributes' => array(),
      'disabled' => FALSE,
    ),
  );
}

/**
 * Create a set of form items to be displayed on the form.
 * @return array
 *   An array of form items to be displayed on the edit component page.
 */
function _dquarks_edit_label($currfield) {
  $edit_fields = array();
  $edit_fields['value'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => $currfield['value'],
    '#description' => t('The text message.') . theme('dquarks_token_help'),
    '#size' => 60,
    '#maxlength' => 127,
    '#weight' => 0,
  );
  $edit_fields['extra']['description'] = array(
    '#type' => 'hidden',
    '#value' => '',
  );
  $edit_fields['extra']['disabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disabled'),
    '#return_value' => 1,
    '#description' => t('Make this field non-editable. Useful for setting an unchangeable default value.'),
    '#weight' => 3,
    '#default_value' => $currfield['extra']['disabled'],
  );
  return $edit_fields;
}

/**
 * Build a form item array containing all the properties of this component.
 *
 * @param array $component
 *   An array of information describing the component, directly correlating to
 *   the dquarks_component database schema.
 *
 * @return array
 *   An array of a form item to be displayed on the client-side dquarks.
 */
function _dquarks_render_label($component) {
  $form_item = array(
    '#type' => 'item',
    '#title' => '',
    '#value' => _dquarks_filter_values($component['value']),
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#attributes' => $component['extra']['attributes'],
    '#prefix' => '<div class="dquarks-component-' . $component['type'] . '" id="dquarks-component-' . $component['form_key'] . '">',
    '#suffix' => '</div>',
  );

  return $form_item;
}

/**
 * Module specific instance of hook_theme().
 */
function _dquarks_theme_label() {
  return array(
    'dquarks_mail_label' => array(
      'arguments' => array('data' => NULL, 'component' => NULL),
    ),
  );
}

/**
 * Module specific instance of hook_help().
 */
function _dquarks_help_label($section) {
  switch ($section) {
    case 'admin/settings/dquarks#label_description':
      return t('Basic label type.');
  }
}
