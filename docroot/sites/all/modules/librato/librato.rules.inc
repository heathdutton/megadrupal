<?php
/**
 * @file
 * This files contains rules actions
 */

/**
 * Implements hook_rules_action_info().
 */
function librato_rules_action_info() {
  $actions['librato_add_incremental_count'] = array(
    'label' => t('Add incremental count to Librato'),
    'group' => t('Statistics'),
    'module' => 'librato',
    'parameter' => array(
      'name' => array(
        'type' => 'text',
        'restriction' => 'input',
        'default mode' => 'input',
        'label' => t('Metrics name'),
      ),
    ),
    'callbacks' => array(
      'validate' => 'librato_rules_action_naming_validation',
      'help' => 'librato_action_help',
      'form_alter' => 'librato_rules_action_name_form_alter',
    ),
  );

  $actions['librato_add_count_with_value'] = array(
    'label' => t('Add incremental count to Librato with given value'),
    'group' => t('Statistics'),
    'module' => 'librato',
    'parameter' => array(
      'name' => array(
        'type' => 'text',
        'restriction' => 'input',
        'default mode' => 'input',
        'label' => t('Metrics name'),
      ),
      'value' => array(
        'type' => array('decimal', 'integer', 'text'),
        'label' => t('Value to report'),
        'default mode' => 'selector',
      ),
    ),
    'callbacks' => array(
      'validate' => 'librato_rules_action_naming_validation',
      'help' => 'librato_action_help',
      'form_alter' => 'librato_rules_action_name_form_alter',
    ),
  );

  return $actions;
}

/**
 * Alter the defult rules action form for Metrics name.
 *
 * @see librato_rules_action_info()
 */
function librato_rules_action_name_form_alter(&$form, &$form_state, $options, RulesAbstractPlugin $element) {
  $form['parameter']['name']['settings']['help'] = NULL;
  $form['parameter']['name']['settings']['name']['#title'] = t('Metrics name');
  $form['parameter']['name']['settings']['name']['#type'] = 'textfield';
  if (!empty($form['parameter']['value'])) {
    $form['parameter']['value']['#title'] = t('Value to report');
  }
  $form['parameter']['name']['#type'] = 'container';
}

/**
 * Add naming validation.
 *
 * @see librato_rules_action_info()
 */
function librato_rules_action_naming_validation($element) {
  if (empty($element->settings['name'])) {
    throw new RulesIntegrityException(
      t('Invalid Metrics name specified.'),
      array(
        $element,
        'parameter',
        'name',
      )
    );
  }

  $element->settings['name'] = _librato_check_format($element->settings['name']);

  if (substr_count($element->settings['name'], '.') < 2) {
    throw new RulesIntegrityException(
      t('Missing dots in Metrics name, please specify at least two like this: system.error.count'),
      array(
        $element,
        'parameter',
        'name',
      )
    );
  }
}

/**
 * Add help text in top of action.
 *
 * @see librato_rules_action_info()
 */
function librato_action_help() {
  $help_text = '<h2>Metrics instrument</h1>Here you define the name of metrics.<br/><br/>
  It is important that you have in mind this is like the "machine name" of the event you wish to record, it has to be short and still human readable.<br/><br/>
  An example could be: system.error.count<br/><br/>
  We require the use of two dots to ensure this (type).(function).(action) relationship.<br/><br/>';

  return $help_text;
}
