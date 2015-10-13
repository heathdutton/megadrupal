<?php

/**
 * @file personalize_ga.admin.inc
 * Provides functions needed for the admin UI.
 */

/**
 * Admin form for configuring Google Analytics Integration.
 */
function personalize_ga_admin_form($form, &$form_state) {
  $google_analytics_link = l(t('Google Analytics account'), 'http://www.google.com/analytics/', array('attributes' => array('target' => '_blank')));
  $web_property_link = l(t('documentation'), 'https://developers.google.com/analytics/resources/concepts/gaConceptsAccounts', array('fragment' => 'webProperty', 'attributes' => array('target' => '_blank')));

  $form['personalize_ga_tracking_id'] = array(
    '#title' => t('Web Property ID'),
    '#type' => 'textfield',
    '#default_value' => variable_get('personalize_ga_tracking_id'),
    '#attributes' => array(
      'placeholder' => 'UA-xxxxxxx-yy',
    ),
    '#size' => 15,
    '#maxlength' => 20,
    '#required' => TRUE,
    '#description' => t('You can reuse an existing tracking id, or separate your data by using a new one just for Personalization. You can register or manage your tracking id\'s in your !google_analytics_link. For more information, see !web_property_link.', array('!google_analytics_link' => $google_analytics_link, '!web_property_link' => $web_property_link)),
  );
  $form['personalize_ga_personalize_decisions'] = _personalize_ga_get_admin_form_personalize_decisions();
  $form['personalize_ga_visitor_actions'] = _personalize_ga_get_admin_form_visitor_actions();

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  $form['#submit'][] = 'personalize_ga_admin_form_submit';
  $form['#attached']['js'][] = drupal_get_path('module', 'personalize_ga') . '/js/personalize_ga.admin.js';

  return $form;
}

function _personalize_ga_get_admin_form_personalize_decisions() {
  $subform = array(
    '#title' => t('Personalize module'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );

  $subform['personalize_ga_send_personalize_decisions'] = array(
    '#title' => t('Personalize module\'s decisions'),
    '#type' => 'radios',
    '#options' => array(
      'all' => t('Send all'),
      'none' => t('Don\'t send'),
      'some' => t('Send only the following'),
    ),
    '#default_value' => variable_get('personalize_ga_send_personalize_decisions', 'all'),
    '#attributes' => array(
      'class' => array('personalize-ga-personalize-decisions-checkboxes-control'),
    ),
  );

  $tracked_personalize_decisions = variable_get('personalize_ga_tracked_personalize_decisions', array());
  $personalizations = personalize_agent_load_multiple(array(), array(), TRUE);
  $options = array();
  foreach ($personalizations as $name => $personalization) {
    $option_sets = personalize_option_set_load_by_agent($name);
    foreach($option_sets as $option_set) {
      $osid = 'osid-' . $option_set->osid;
      $options[$osid] = $personalization->label . ': ' . $option_set->label;
    }
  }
  $subform['personalize_ga_tracked_personalize_decisions'] = array(
    '#title' => empty($options) ? t('Currently no trackable decisions') : t('Trackable decisions'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#attributes' => array(
      'class' => array('personalize-ga-personalize-decisions-checkboxes'),
    ),
  );
  foreach ($options as $osid => $label) {
    $default_value = isset($tracked_personalize_decisions[$osid]) ? $tracked_personalize_decisions[$osid] : FALSE;
    $subform['personalize_ga_tracked_personalize_decisions'][$osid] = array(
      '#type' => 'checkbox',
      '#default_value' => $default_value,
      '#title' => $label,
    );
  }

  return $subform;
}

function _personalize_ga_get_admin_form_visitor_actions() {
  $subform = array(
    '#title' => t('Visitor Actions module'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );

  $subform['personalize_ga_send_visitor_actions'] = array(
    '#title' => t('Visitor Actions module\'s actions'),
    '#type' => 'radios',
    '#options' => array(
      'all' => t('Send all'),
      'none' => t('Don\'t send'),
      'some' => t('Send only the following'),
    ),
    '#default_value' => variable_get('personalize_ga_send_visitor_actions', 'all'),
    '#attributes' => array(
      'class' => array('personalize-ga-visitor-actions-checkboxes-control'),
    ),
  );

  $tracked_visitor_actions = variable_get('personalize_ga_tracked_visitor_actions', array());
  $visitor_actions = visitor_actions_get_actions();
  $options = array();
  foreach ($visitor_actions as $name => $info) {
    $options[$name] = $info['label'];
  }
  $subform['personalize_ga_tracked_visitor_actions'] = array(
    '#title' => empty($options) ? t('Currently no trackable actions') : t('Trackable actions'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#attributes' => array(
      'class' => array('personalize-ga-visitor-actions-checkboxes'),
    ),
  );
  foreach ($options as $name => $label) {
    $default_value = isset($tracked_visitor_actions[$name]) ? $tracked_visitor_actions[$name] : FALSE;
    $subform['personalize_ga_tracked_visitor_actions'][$name] = array(
      '#type' => 'checkbox',
      '#default_value' => $default_value,
      '#title' => $label,
    );
  }

  return $subform;
}

/**
 * Submit handler for the configuration form.
 */
function personalize_ga_admin_form_submit($form, &$form_state) {
  variable_set('personalize_ga_tracking_id', $form_state['values']['personalize_ga_tracking_id']);
  variable_set('personalize_ga_send_personalize_decisions', $form_state['values']['personalize_ga_personalize_decisions']['personalize_ga_send_personalize_decisions']);
  if (isset($form_state['values']['personalize_ga_personalize_decisions']['personalize_ga_tracked_personalize_decisions'])) {
    variable_set('personalize_ga_tracked_personalize_decisions', $form_state['values']['personalize_ga_personalize_decisions']['personalize_ga_tracked_personalize_decisions']);
  }
  variable_set('personalize_ga_send_visitor_actions', $form_state['values']['personalize_ga_visitor_actions']['personalize_ga_send_visitor_actions']);
  if (isset($form_state['values']['personalize_ga_visitor_actions']['personalize_ga_tracked_visitor_actions'])) {
    variable_set('personalize_ga_tracked_visitor_actions', $form_state['values']['personalize_ga_visitor_actions']['personalize_ga_tracked_visitor_actions']);
  }
}
