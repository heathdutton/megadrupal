<?php
/**
 * @file
 * Provide Webform component for MyNewsletterBuilder list signups
 */

/**
 * Implementation of _webform_defaults_component().
 */
function _webform_defaults_mnb_signup() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'lists' => array(),
      'email_field' => NULL,
      'description' => '',
    ),
  );
}

/**
 * Implementation of _webform_edit_component().
 */
function _webform_edit_mnb_signup($component) {
  $form = array();

  $lists = array();
  foreach (mnb_webform_get_lists() as $list) {
    $lists[$list['id']] = sprintf('%s (ID: %s, Subscribers: %s)', $list['name'], $list['id'], $list['subscribers']);
  }

  $form['extra']['lists'] = array(
    '#type' => 'radios',
    '#title' => t('List'),
    '#options' => $lists,
    '#default_value' => !empty($component['extra']['lists']) ? $component['extra']['lists'] : '',
    '#description' => t('Select the list submitters should be able to sign up for.'),
    '#weight' => 0,
    '#required' => TRUE,
  );

  $email_fields = array();
  $name_fields = array();
  $other_fields = array();
  $result = db_query("SELECT cid, form_key, name, type FROM {webform_component} WHERE type <> 'mnb_signup' AND nid = :nid ORDER BY weight, name", array(':nid' => $component['nid']));
  while ($c = $result->fetchAssoc()) {
    if ($c['type'] == 'email') {
      $email_fields[$c['cid']] = sprintf('%s (%s)', $c['name'], $c['form_key']);
    }
    elseif ($c['type'] == 'textfield') {
      $name_fields[$c['cid']] = sprintf('%s (%s)', $c['name'], $c['form_key']);
    }
    else {
      $other_fields[$c['cid']] = sprintf('%s (%s)', $c['name'], $c['form_key']);
    }
  }
  $other_fields += $name_fields;

  $form['extra']['email_field'] = array(
    '#type' => 'radios',
    '#title' => t('Email field'),
    '#options' => $email_fields,
    '#default_value' => (isset($component['extra']['email_field'])) ? $component['extra']['email_field'] : array_shift(array_keys($email_fields)),
    '#description' => t('Select the field which represents the email which should be signed up'),
    '#weight' => 0,
    '#required' => TRUE,
  );

  $form['extra']['name_field'] = array(
    '#type' => 'radios',
    '#title' => t('Name field'),
    '#options' => $name_fields,
    '#default_value' => (isset($component['extra']['name_field'])) ? $component['extra']['name_field'] : array_shift(array_keys($name_fields)),
    '#description' => t('Select the field which represents the name of the user'),
    '#weight' => 0,
    '#required' => FALSE,
  );

  $form['extra']['custom_fields'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Custom fields'),
    '#options' => $other_fields,
    '#default_value' => (isset($component['extra']['custom_fields'])) ? array_keys(array_filter($component['extra']['custom_fields'])) : array_keys($other_fields),
    '#description' => t('Select which fields should be sent as additional subscriber data'),
    '#weight' => 0,
    '#required' => FALSE,
  );

  $form['value'] = array(
    '#type' => 'checkbox',
    '#title' => $component['name'],
    '#default_value' => (isset($component['value'])) ? $component['value'] : FALSE,
    '#description' => t("Set the default state for the subscribe checkbox."),
  );

  return $form;
}

/**
 * Implementation of _webform_render_component().
 */
function _webform_render_mnb_signup($component, $value = NULL) {
  $form_item = array(
    '#type' => 'checkbox',
    '#title' => $component['name'],
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#description'   => _webform_filter_descriptions($component['extra']['description']),
    '#default_value' => isset($value) ? $value[0] : $component['value'],
    '#prefix' => '<div class="webform-component-' . $component['type'] . '" id="webform-component-' . $component['form_key'] . '">',
    '#suffix' => '</div>',
  );

  return $form_item;
}

/**
 * Implementation of _webform_submit_component().
 */
function _webform_submit_mnb_signup($component, $value) {
  return $value;
}


/**
 * Implementation of _webform_display_component().
 */
function _webform_display_mnb_signup($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme_wrappers' => $format == 'html' ? array('webform_element', 'webform_element_wrapper') : array('webform_element_text'),
    '#post_render' => array('webform_element_wrapper'),
    '#format' => $format,
    '#value' =>  ($value[0] == 1) ? t('Subscribed') : t('Not subscribed'),
    '#webform_component' => $component,
  );
}


function _webform_table_mnb_signup($component, $value) {
  return ($value[0] == '1') ? t('Subscribed') : t('Not subscribed');
}
