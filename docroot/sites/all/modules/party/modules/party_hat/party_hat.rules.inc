<?php
/**
 * @file
 * Rules integration with party hats.
 */

/**
 * Implements hook_rules_data_info() {
 */
function party_hat_rules_data_info() {
  $data['party_hat_name'] = array(
    'label' => t('Hat'),
    'ui class' => 'RulesDataUIPartyHatName',
    'module' => 'party_hat',
  );
  return $data;
}

/**
 * Provide Events for assigning and unassigning hats
 */
function party_hat_rules_event_info() {
  $events = array(
    'party_hat_assign_hats' => array(
      'group' => t('Party'),
      'module' => 'party_hat',
      'label' => t('After assigning some hats to a party'),
      'variables' => array(
        'party' => array('type' => 'party', 'label' => t('party')),
        'hats' => array('type' => 'list<party_hat_name>', 'label' => t('assigned hats')),
      ),
    ),
    'party_hat_unassign_hats' => array(
      'group' => t('Party'),
      'module' => 'party_hat',
      'label' => t('After unassigning some hats from a party'),
      'variables' => array(
        'party' => array('type' => 'party', 'label' => t('party')),
        'hats' => array('type' => 'list<party_hat_name>', 'label' => t('unassigned hats')),
      ),
    ),
  );

  return $events;
}

/**
 * Provide the action to assign an unassign a hat
 */
function party_hat_rules_action_info() {
  $actions = array(
    'party_assign_hat' => array(
      'label' => t('Assign a Hat to a Party'),
      'group' => t('Party'),
      'parameter' => array(
        'party' => array(
          'type' => 'party',
          'label' => t('Party'),
          'save' => TRUE,
        ),
        'hat_name' => array(
          'type' => 'party_hat_name',
          'label' => t('Hat'),
          'description' => t('The Hat to Assign'),
        ),
      ),
      //'access callback' => 'rules_user_integration_access'
      'base' => 'party_hat_rules_assign_hat',
    ),
    'party_unassign_hat' => array(
      'label' => t('Unassign a Hat from a Party'),
      'group' => t('Party'),
      'parameter' => array(
        'party' => array(
          'type' => 'party',
          'label' => t('Party'),
          'save' => TRUE,
        ),
        'hat_name' => array(
          'type' => 'party_hat_name',
          'label' => t('Hat'),
          'description' => t('The Hat to Unassign'),
        ),
      ),
      //'access callback' => 'rules_user_integration_access'
      'base' => 'party_hat_rules_unassign_hat',
    ),
  );

  return $actions;
}

/**
 * Implement the assign hat rule.
 *
 * @todo: Get party loading as a Party object not as stdClass
 */
function party_hat_rules_assign_hat($party, $hat_name, $settings, $state, $element) {
  $hats = array($hat_name);

  party_hat_hats_assign($party, $hats);
}

/**
 * Implement the unassign hat rule.
 *
 * @todo: Get party loading as a Party object not as stdClass
 */
function party_hat_rules_unassign_hat($party, $hat_name, $settings, $state, $element) {
  $hats = array($hat_name);

  party_hat_hats_unassign($party, $hats);
}

/**
 * Adds a hat chooser form to the enabling action.
 */
class RulesDataUIPartyHatName extends RulesDataUI implements RulesDataDirectInputFormInterface {
  public static function getDefaultMode() {
    return 'input';
  }

  public static function inputForm($name, $info, $settings, RulesPlugin $element) {
    // Get a list of all the hats.
    $options = party_hat_options_list();

    $form[$name] = array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $settings[$name],
      '#required' => empty($info['optional']),
    );

    return $form;
  }

  public static function render($value) {
    return array();
  }
}