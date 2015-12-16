<?php
/**
 * @file
 * Rules for Fool's Rules.
 */

/**
 * Implements hook_rules_event_info().
 */
function fools_rules_rules_event_info() {
  return array(
    'fools_rules_event_bundle_alter' => array(
      'label' => t('Bundle is viewed.'),
      'group' => t('Fool\'s Rules'),
    ),
    'fools_rules_event_form_alter' => array(
      'label' => t('Form is viewed.'),
      'group' => t('Fool\'s Rules'),
    ),
    'fools_rules_event_page_alter' => array(
      'label' => t('Page is viewed.'),
      'group' => t('Fool\'s Rules'),      
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function fools_rules_rules_action_info() {
  return array(
    'fools_rules_action_bundle_alter' => array(
      'label' => t('Alter bundle'),
      'group' => t('Fool\'s Rules'),
      'access' => 'administer fools rules',
      'parameter' => array(
        'bundle_id' => array(
          'label' => t('Bundle ID'),
          'type' => 'text',
          'restriction' => 'input',
          'description' => t('Name of the bundle. For example: <code>article</code>.'),
        ),
        'property' => array(
          'label' => t('Property to modify'),
          'type' => 'text',
          'restriction' => 'input',
          'description' => t('Available values are Drupal bundle array key
            names, separated by a colon. For example: to modify
            <code>[\'summary\'][\'member_for\'][\'#title\']</code> found in the
            <code>user</code> bundle, use <code>summary:member_for:#title</code>.'),
        ),
        'value' => array(
          'label' => t('Value to set'),
          'type' => 'text',
          'restriction' => 'input',
          'description' => _fools_rules_settype_info()
        ),
      ),
    ),
    'fools_rules_action_form_alter' => array(
      'label' => t('Alter form'),
      'group' => t('Fool\'s Rules'),
      'access' => 'administer fools rules',
      'parameter' => array(
        'form_id' => array(
          'label' => t('Form ID'),
          'type' => 'text',
          'restriction' => 'input',
          'description' => t('Name of the form. For example:
            <code>system_site_information_settings</code>.'),
        ),
        'property' => array(
          'label' => t('Property to modify'),
          'type' => 'text',
          'restriction' => 'input',
          'description' => t('Available values are Drupal form array key
            names, separated by a colon. For example: to modify
            <code>$form[\'site_information\'][\'#title\']</code>, use
            <code>site_information:#title</code>.'),
        ),
        'value' => array(
          'label' => t('Value to set'),
          'type' => 'text',
          'restriction' => 'input',
          'description' => _fools_rules_settype_info(),
        ),
      ),
    ),
    'fools_rules_action_page_alter' => array(
      'label' => t('Alter page'),
      'group' => t('Fool\'s Rules'),
      'access' => 'administer fools rules',
      'parameter' => array(
        'property' => array(
          'label' => t('Property to modify'),
          'type' => 'text',
          'restriction' => 'input',
          'description' => t('Available values are Drupal render array key
            names, separated by a colon. For example: to modify
            <code>$page[\'footer\'][\'system_powered-by\'][\'#markup\']</code>,
            use <code>footer:system_powered-by:#markup</code>.'),
        ),
        'value' => array(
          'label' => t('Value to set'),
          'type' => 'text',
          'restriction' => 'input',
          'description' => _fools_rules_settype_info(),
        ),
      ),
    ),
  );
}

/**
 * Action: bundle_alter.
 */
function fools_rules_action_bundle_alter($bundle_id, $property, $value) {
  $bundle_alter = &drupal_static('fools_rules_action_bundle_alter', array());

  // Populate $bundle_alter for hook_entity_view_alter().
  $index['bundle_id'] = check_plain($bundle_id);
  $index['property'] = check_plain($property);
  $index['value'] = _fools_rules_settype($value);
  $bundle_alter[] = $index;
}

/**
 * Action: form_alter.
 */
function fools_rules_action_form_alter($form_id, $property, $value) {
  $form_alter = &drupal_static('fools_rules_action_form_alter', array());

  // Populate $form_alter for hook_form_alter().
  $index['form_id'] = check_plain($form_id);
  $index['property'] = check_plain($property);
  $index['value'] = _fools_rules_settype($value);
  $form_alter[] = $index;
}

/**
 * Action: page_alter.
 */
function fools_rules_action_page_alter($property, $value) {
  $page_alter = &drupal_static('fools_rules_action_page_alter', array());

  // Populate $page_alter for hook_page_alter().
  $index['property'] = check_plain($property);
  $index['value'] = _fools_rules_settype($value);
  $page_alter[] = $index;
}
