<?php //

/**
 * @file
 * Rules integration for the Invites module.
 */

/**
 * Implementation of hook_rules_event_info().
 */
function invites_rules_event_info() {
  $items = array();
  $items['invites_approval'] = array(
    'label' => t('An invites is in approval'),
    'help' => t("An email invites link is clicked and get to the site page approval."),
    'variables' => array(
      'invites' => array(
        'type' => 'invites',
        'label' => t("The invites"),
      ),
    ),
    'group' => t('Invites'),  
  );
  
  return $items;
}

/**
 * Implements hook_rules_condition_info().
 */
function invites_rules_condition_info() {
  $items = array();  
  $items['invites_user_is_register'] = array(
    'label' => t('User is register'),
    'group' => t('Invites'),
    'parameter' => array(
      'mail' => array(
        'type' => 'text',
        'label' => t('Mail'),
        'wrapped' => TRUE,
      ),     
    ),
  );
  
  $items['invites_is_of_type'] = array(
    'label' => t('Invites is of type'),
    'group' => t('Invites'),
    'help' => t('Evaluates to TRUE if the given Invites is of one of the selected Invites types.'),
    'parameter' => array(
      'invites' => array(
        'type' => 'invites', 
        'label' => t('Invites')
      ),
      'types' => array(
        'type' => 'list<text>',
        'label' => t('Invites types'),
        'restriction' => 'input',  
        'options list' => '_invites_field_info_bundles',
        'description' => t('The Invites type(s) to check for.'),    
      )
    ),
    
  );
  return $items;
}

/**
 * Condition: Invites user is register.
 */
function invites_user_is_register($mail) {
  if (!db_query("SELECT COUNT(*) FROM {users} WHERE mail = :mail", array(':mail' => $mail))->fetchField()) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Condition: Invites is of type.
 */
function _invites_field_info_bundles() {
  $info = entity_get_info('invites');
  foreach ($info['bundles'] as $key => $bundle) {
    $list[$key] = $bundle['label'];
  }
  return $list;
}

/**
 * Condition: Invites user is register.
 */
function invites_is_of_type($invites, $types) {
  return in_array($invites->type, $types);
}