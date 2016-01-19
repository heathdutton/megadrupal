<?php
/**
 * @file
 * Implementations of rules hooks.
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_rules_pane_rules_event_info() {
  return array(
    'determine_rules_checkout_pane_content' => array(
      'label' => t('Determine rules checkout pane content'),
      'group' => t('Commerce Checkout'),
      'variables' => array(
        'commerce_order' => array(
          'type' => 'commerce_order',
          'label' => t('Completed order', array(), array('context' => 'a drupal commerce order')),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_rules_pane_rules_action_info() {
  return array(
    'commerce_rules_pane_set_message' => array(
      'label' => t('Set the completion message'),
      'group' => t('Commerce Checkout'),
      'parameter' => array(
        'commerce_order' => array(
          'type' => 'commerce_order',
          'label' => t('Order in checkout'),
          'skip save' => TRUE,
        ),
        'content' => array(
          'type' => 'text',
          'label' => t('Message'),
          'description' => t('The text to display in the checkout pane.'),
          'translatable' => TRUE,
        ),
      ),
    ),
    'commerce_rules_pane_set_entity' => array(
      'label' => t('Set an entity to be displayed as completion message'),
      'group' => t('Commerce Checkout'),
      'parameter' => array(
        'commerce_order' => array(
          'type' => 'commerce_order',
          'label' => t('Order in checkout'),
        ),
        'entity' => array(
          'type' => 'entity',
          'label' => t('Select the entity to be displayed'),
        ),
        'view_mode' => array(
          'type' => 'text',
          'label' => t('View mode'),
          'options list' => 'commerce_rules_pane_set_entity_view_modes',
          'restriction' => 'input',
        ),
      ),
      'callbacks' => array(
        'form_alter' => 'commerce_rules_pane_set_entity_form_alter',
      ),
    ),

  );
}

/**
 * Rules action: save a custom message.
 *
 * Saves a custom message to current order to be later used in the
 * commerce_rules_pane.
 */
function commerce_rules_pane_set_message($commerce_order, $message) {
  $commerce_order->data['checkout_messages'][] = array(
    '#markup' => filter_xss($message, variable_get('commerce_rules_pane_message_tags', array(
      'a', 'strong', 'ul', 'ol', 'li', 'br', 'p',
    ))),
  );
}

/**
 * Rules action: commerce_rules_pane_set_entity.
 *
 * Saves an entity to be displayed as a completion message in the
 * commerce_rules_pane.
 *
 * @param object $commerce_order
 *   The EntityDrupalWrapper object with the order.
 * @param string $entity
 *   The EntityDrupalWrapper object selected for display.
 */
function commerce_rules_pane_set_entity($commerce_order, $entity, $view_mode) {
  $commerce_order->data['checkout_messages'][] = array(
    'content' => entity_view($entity->type(), array($entity->value()), $view_mode),
  );
}

/**
 * Form alter callback for commerce_rules_pane_set_entity action.
 */
function commerce_rules_pane_set_entity_form_alter(&$form, &$form_state, $options, RulesAbstractPlugin $element) {
  // Which step are we in?
  $step = (empty($element->settings['entity:select'])) ? 1 : 2;
  // Add a new button, to reload the form.
  $form['reload'] = array(
    '#weight' => 5,
    '#type' => 'submit',
    '#name' => 'reload',
    '#value' => ($step == 1) ? t('Continue') : t('Reload form'),
    '#limit_validation_errors' => array(
      array('parameter', 'commerce_order'),
      array('parameter', 'entity'),
    ),
    '#submit' => array('commerce_rules_pane_set_entity_view_mode_submit_rebuild'),
  );
  // If we are on step 1 hide the view mode and the submit button.
  if ($step == 1) {
    $form['parameter']['view_mode']['#access'] = FALSE;
    $form['submit']['#access'] = FALSE;
  }

}

/**
 * Option list callback for commerce_rules_pane_set_entity.
 */
function commerce_rules_pane_set_entity_view_modes(RulesPlugin $element, $param_name) {
  $options = array();
  // Get the available entity view modes for the selected content type.
  if ($element->applyDataSelector($element->settings['entity:select'])) {
    $info = $element->applyDataSelector($element->settings['entity:select'])->info();
    $entity_info = entity_get_info($info['type']);
    foreach ($entity_info['view modes'] as $key => $view_mode) {
      $options[$key] = $view_mode['label'];
    }
  }
  // In case we don't have an entity to display, show none to prevent getting
  // the illegal option error.
  else {
    $options['none'] = t('--none--');
  }
  return $options;
}
