<?php
/**
 * @file
 * rules fonctions for the ls_answ module.
 */

/**
 * Implements hook_rules_event_info().
 *
 * Create 4 events for the rule module : when an answer is completed, when a
 * answer is being edited and about synchronization.
 */
function ls_answ_rules_event_info() {
  $events = array();
  $default = array(
    'group' => t('LimeSurvey Sync'),
    'class' => 'RulesLsanswEventHandler',
    'module' => 'ls_answ',
    'variables' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Answer'),
      ),
    ),
  );

  $events['ls_answ_sync_completed'] = $default + array(
    'label' => t('Response has been completed'),
  );

  $events['ls_answ_sync_editing'] = $default + array(
    'label' => t('Response is being edited'),
  );

  $events['ls_answ_sync_desynchronized'] = $default + array(
    'label' => t('Response is desynchronized'),
  );

  $events['ls_answ_sync_resynchronized'] = $default + array(
    'label' => t('Response is resynchronized'),
  );

  $events['ls_answ_ask_new_answer'] = $default + array(
    'label' => t('Ask for creating a new response'),
  );

  return $events;
}

/**
 * Implements hook_rules_condition_info().
 *
 */
function ls_answ_rules_condition_info() {
  $conditions = array();
  $default = array(
    'module' => 'ls_answ',
    'arguments' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('The answer node'),
      ),
    ),
    'module' => 'ls_answ',
    'group' => t('LimeSurvey Sync'),
  );

  $conditions['ls_answ_condition_answer_exists'] = $default + array(
    'label' => t('An answer does exists for that user'),
  );
  $conditions['ls_answ_condition_answer_exists']['arguments']['account'] = array(
    'type' => 'user',
    'label' => t('The selected user'),
  );

  $conditions['ls_answ_condition_node_is_answer'] = $default + array(
    'label' => t('The node is a LimeSurvey answer'),
    'base' => 'ls_answ_condition_node_is_answer',
  );

  return $conditions;
}

/**
 * return TRUE if an answer node from a specific content type does exists for the provided user.
 */
function ls_answ_condition_answer_exists($node, $account) {
  $query = db_query('SELECT nid FROM {node} WHERE uid = :uid AND type = :type', array(':uid' => $account->uid, ':type' => $node->type));
  return ($query->rowCount() > 0);
}

/**
 * return TRUE if the node is an answer.
 *
 * This condition automatically load the ls_fields.
 */
function ls_answ_condition_node_is_answer($node) {
  return (!empty($node->type)) ? ls_answ_type_is_answer($node->type) : FALSE;
}

/**
 * Assert that the node is an answer, add the ls_fields.
 */
function ls_answ_condition_node_is_answer_assertions($element) {
  if (empty($element->settings['bundle'])) {
    $bundles = ls_answ_types_options_list();
    if (count($bundles) > 0) {
      $ls_types = array_keys($bundles);
      $bundle = $ls_types[0];
      $assertions = array();
      $assertions['node']['bundle'] = $bundle;
      return $assertions;
    }
  }
}

/**
 * Implements hook_rules_action_info().
 *
 */
function ls_answ_rules_action_info() {
  $actions = array();
  $default = array(
    //'module' => 'ls_answ',
    'group' => t('LimeSurvey Sync'),
    'arguments' => array(
      'account' => array(
        'type' => 'user',
        'label' => t('The selected user'),
      ),
      'node' => array(
        'type' => 'node',
        'label' => t('The answer node'),
      ),
    ),
  );

  $actions['ls_answ_recent_answer_variable'] = $default + array(
    'label' => t('Provide recent answer variable from that author'),
    'provides' => array(
      'most_recent_answer' => array(
        'type' => 'node',
        'label' => t('The most recent created answer for that author'),
      ),
    ),
  );

  $actions['ls_answ_create_invitation'] = $default + array(
    'label' => t('Set the token invitation if needed for filling/editing an answer'),
    'provides' => array(
      'answer_ls_url' => array(
        'type' => 'uri',
        'label' => t('The url for filling the survey'),
      ),
    ),
  );

  return $actions;
}

/**
 * return the most recent answer node from a specific content type and a provided user.
 *
 * This function create a new rule variable : the most_recent_answer.
 */
function ls_answ_recent_answer_variable($account, $node) {
  $query = db_query('SELECT nid FROM {node} WHERE uid = :uid AND type = :type ORDER BY created DESC', array(':uid' => $account->uid, ':type' => $node->type));
  $nid = $query->fetchField();
  return array('most_recent_answer' => node_load($nid));
}

/**
 * This function prepare the LimeSurvey token and return the LimeSurvey url as a new variable.
 */
function ls_answ_create_invitation($account, $node) {
  $form_state = array();
  module_load_include('inc', 'node', 'node.pages');
  if (empty($node)) {
    $node = new stdClass(); // We create a new node object
    //$node->type = $ls_type;
  }
  //$node->type must be provided.
  $form_state['values']['node'] = $node;
  $form_state['values']['name'] = $account->name;
  $form_state['values']['language'] = 'und';
  $form_state['values']['edit_answer'] = TRUE;
  $form_state['values']['op'] = t('Save');
  $form_state['programmed_bypass_access_check'] = FALSE;
  drupal_form_submit($node->type . '_node_form', $form_state, $node);
  // Do not display the success message.
  drupal_get_messages('status', TRUE);
  // Retrieve the Nid for the create node answer.
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_answ');
  // return the survey survey page url.
  return array('answer_ls_url' => limesurvey_sync_answ_surveyurl(node_load($form_state['values']['nid'])));
}

/**
 * Event handler support node bundle event settings.
 */
class RulesLsanswEventHandler extends RulesEventHandlerEntityBundle {

  /**
   * Implements RulesEventHandlerInterface::__construct()
   */
  public function __construct($event_name, $info) {
    RulesEventHandlerBase::__construct($event_name, $info);
    $this->entityType = 'node';
    $this->entityInfo = entity_get_info($this->entityType);
    if (!$this->entityInfo) {
      throw new InvalidArgumentException('Unsupported event name passed.');
    }
  }

  /**
   * Implements RulesEventHandlerInterface::buildForm()
   */
  public function buildForm(array &$form_state) {
    $form['bundle'] = array(
      '#type' => 'select',
      '#title' => t('Restrict by @bundle', array('@bundle' => $this->getBundlePropertyLabel())),
      '#description' => t('If you need to filter for multiple values, either add multiple events or use the "The node is a LimeSurvey answer" condition instead.'),
      '#default_value' => $this->settings['bundle'],
      '#empty_value' => '',
    );
    $form['bundle']['#options'] = ls_answ_types_options_list();
    return $form;
  }

  /**
   * Returns the label to use for the bundle property.
   *
   * @return string
   */
  protected function getBundlePropertyLabel() {
    return t('Answers content type');
  }

  /**
   * Implements RulesEventHandlerInterface::validate()
   */
  public function validate() {
    // If bundle is provide make sure it exists.
    parent::validate();
    $bundles = ls_answ_types_options_list();
    if (!empty($this->settings['bundle'])) {
      if (!array_key_exists($this->settings['bundle'], $bundles)) {
        throw new RulesIntegrityException(t('The @bundle is not valid.',
          array(
          '@bundle' => $this->getBundlePropertyLabel(),
        )), array(NULL, 'bundle'));
      }
    }
  }
}
