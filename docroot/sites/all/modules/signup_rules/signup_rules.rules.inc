<?php

/**
 * @file
 * Rules hook implementations for Signup Rules module.
 */

/**
 * Implements hook_rules_file_info().
 */
function signup_rules_rules_file_info() {
  return array('signup_rules.eval');
}

/**
 * Implements hook_rules_event_info().
 */
function signup_rules_rules_event_info() {
  $defaults = array(
    'group' => t('Signup'),
    'module' => 'signup',
    'access callback' => 'signup_rules_signup_integration_access',
  );

  $events = array();
  $events['signup_rules_event_insert'] = $defaults + array(
    'label' => t('User signed up for content'),
    'variables' => _signup_rules_event_variables(),
    'help' => t('This event is triggered when a user first signs up for content.'),
  );
  $events['signup_rules_event_update'] = $defaults + array(
    'label' => t('Signup was changed'),
    'variables' => _signup_rules_event_variables(),
    'help' => t('This event is triggered when the user has updated a signup.'),
  );
  $events['signup_rules_event_cancel'] = $defaults + array(
    'label' => t('Signup was cancelled'),
    'variables' => _signup_rules_event_variables(),
    'help' => t('This event is triggered when an individual user\'s signup has been cancelled.'),
  );
  $events['signup_rules_event_open'] = $defaults + array(
    'label' => t('Signups were opened for content'),
    'variables' => array(
      'node' => array('type' => 'node', 'label' => t('Content for which signups were opened')),
    ),
    'help' => t('This event is triggered when the status of signups for content is changed to open.'),
  );
  $events['signup_rules_event_close'] = $defaults + array(
    'label' => t('Signups were closed for content'),
    'variables' => array(
      'node' => array('type' => 'node', 'label' => t('Content for which signups were closed')),
    ),
    'help' => t('This event is triggered when the status of signups for content is changed to closed.'),
  );
  $events['signup_rules_event_display'] = $defaults + array(
    'label' => t('Signup form is being displayed'),
    'variables' => array(
      'user' => array('type' => 'user', 'label' => t('User viewing content')),
      'node' => array('type' => 'node', 'label' => t('Content')),
      'form_display_status' => array('type' => 'form_display_status', 'label' => t('Signup form display status')),
    ),
    'help' => 'Triggered when the signup form is being prepared for display.',
  );
  $events['signup_rules_event_signup_node'] = $defaults + array(
    'label' => t('User signed up from content signup form'),
    'variables' => _signup_rules_event_variables(),
    'help' => 'Triggered when a user signs up from the signup form on content.',
  );
  $events['signup_rules_event_signup_register'] = $defaults + array(
    'label' => t('User signed up from registration form'),
    'variables' => _signup_rules_event_variables(),
    'help' => 'Triggered when a user signs up from the user registration form.',
  );
  $events['signup_rules_event_attended'] = $defaults + array(
    'label' => t('Signup was marked attended'),
    'variables' => _signup_rules_event_variables(),
    'help' => 'Triggered when a user\'s signup has been marked attended.',
  );
  $events['signup_rules_event_not_attended'] = $defaults + array(
    'label' => t('Signup was marked not attended'),
    'variables' => _signup_rules_event_variables(),
    'help' => 'Triggered when a user\'s signup has been marked not attended.',
  );
  return $events;
}

/**
 * Helper function for providing common event variables.
 */
function _signup_rules_event_variables() {
  return array(
    'user' => array('type' => 'user', 'label' => t('User who is signed up for content')),
    'node' => array('type' => 'node', 'label' => t('Content to which the user is signed up')),
    'signup' => array('type' => 'signup', 'label' => t('Signup data')),
  );
}

/**
 * Implements hook_rules_condition_info().
 */
function signup_rules_rules_condition_info() {
  $defaults = array(
    'group' => 'Signup',
    'access callback' => 'signup_rules_signup_integration_access',
  );

  $conditions = array();
  $conditions['signup_total'] = $defaults + array(
    'label' => t('Content signup total has value'),
    'parameter' => array(
      'node' => array('type' => 'node', 'label' => t('Content')),
      'operator' => array(
        'type' => 'text',
        'label' => t('Signup total is'),
        'options list' => 'signup_rules_operator_options',
        'restriction' => 'input',
        'description' => t('Whether the content signup total should be less than, greater than, or equal to the given numeric value.'),
      ),
      'input' => array(
        'type' => 'integer',
        'label' => t('value'),
        'description' => t('The numeric value with which to compare the content signup total.'),
      ),
    ),
    'base' => 'signup_rules_condition_signup_total',
    'help' => t('Evaluate to TRUE if the signup total for content is less than, greater than, or equal to a given numeric value.'),
    
  );
  $conditions['user_is_signed_up'] = $defaults + array(
    'label' => t('User is signed up for content'),
    'parameter' => array(
      'node' => array('type' => 'node', 'label' => t('Content')),
      'user' => array('type' => 'user', 'label' => t('User')),
    ),
    'base' => 'signup_rules_condition_user_is_signed_up',
    'help' => t('Eveluates to TRUE if the selected user is signed up to content.'),
  );
  $conditions['signups_open'] = $defaults + array(
    'label' => t('Signups are open for content'),
    'parameter' => array(
      'node' => array('type' => 'node', 'label' => t('Content')),
    ),
    'base' => 'signup_rules_condition_signups_open',
    'help' => t('Evaluates to TRUE if signups are open for content.'),
  );
  $conditions['signup_attended'] = $defaults + array(
    'label' => t('Signup is marked attended'),
    'parameter' => array(
      'signup' => array('type' => 'signup', 'label' => t('Signup data')),
    ),
    'base' => 'signup_rules_condition_signup_attended',
    'help' => t('Evaluates to TRUE if a signup has been marked \'attended\'.'),
  );
  $conditions['signup_not_attended'] = $defaults + array(
    'label' => t('Signup is marked not attended'),
    'parameter' => array(
      'signup' => array('type' => 'signup', 'label' => t('Signup data')),
    ),
    'base' => 'signup_rules_condition_signup_not_attended',
    'help' => t('Evaluates to TRUE if a signup has been marked \'not attended\'.'),
  );
  return $conditions;
}

/**
 * Provides operator options for condition: Content signup total has value.
 */
function signup_rules_operator_options() {
  return array(
    'equal' => t('equal to'),
    'greater' => t('greater than'),
    'less' => t('less than'),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function signup_rules_rules_action_info() {
  $defaults = array(
    'group' => t('Signup'),
    'access callback' => 'signup_rules_signup_integration_access',
  );

  $actions = array();
  $actions['load_signup'] = $defaults + array(
    'label' => t('Load signup data'),
    'parameter' => array(
      'node' => array('type' => 'node', 'label' => t('Content')),
      'user' => array('type' => 'user', 'label' => t('User')),
    ),
    'provides' => array(
      'signup_loaded' => array('type' => 'signup', 'label' => t('Loaded signup data')),
    ),
    'base' => 'signup_rules_action_load_signup',
  );
  $actions['signup_user'] = $defaults + array(
    'label' => t('Sign up user to content'),
    'parameter' => array(
      'user' => array('type' => 'user', 'label' => t('User')),
      'node' => array('type' => 'node', 'label' => t('Content')),
      'force' => array(
        'type' => 'boolean',
        'label' => t('Force signup'),
        'restriction' => 'input',
        'description' => t('The signup status of the content will be ignored. i.e. if the content signup status is closed the user will still be signed up. However, signups still must be enabled for the content type.'),
      ),
    ),
    'base' => 'signup_rules_action_signup_user',
    'help' => 'Signs up a user to content. This action will only take place if signups are open for the content.',
  );
  $actions['cancel_node_signups'] = $defaults + array(
    'label' => t('Cancel all signups for content'),
    'parameter' => array(
      'node' => array('type' => 'node', 'label' => t('Content')),
    ),
    'base' => 'signup_rules_action_cancel_node_signups',
    'help' => 'Cancels all signups for a piece of content.',
  );
  $actions['cancel_user_signups'] = $defaults + array(
    'label' => t('Cancel all signups for user'),
    'parameter' => array(
      'user' => array('type' => 'user', 'label' => t('User')),
    ),
    'base' => 'signup_rules_action_cancel_user_signups',
    'help' => 'Cancels all signups for a given user.',
  );
  $actions['open_signups'] = $defaults + array(
    'label' => t('Open signups for content'),
    'parameter' => array(
      'node' => array('type' => 'node', 'label' => t('Content'), 'save' => TRUE),
    ),
    'base' => 'signup_rules_action_open_signups',
    'help' => 'Opens signups for a piece of content. This action will only take place if signups are enabled for the content type.',
  );
  $actions['close_signups'] = $defaults + array(
    'label' => t('Close signups for content'),
    'parameter' => array(
      'node' => array('type' => 'node', 'label' => t('Content'), 'save' => TRUE),
    ),
    'base' => 'signup_rules_action_close_signups',
    'help' => 'Closes signups for a piece of content. This action will only take place if signups are enabled for the content type.',
  );
  $actions['mark_attended'] = $defaults + array(
    'label' => t('Mark signup as attended'),
    'parameter' => array(
      'signup' => array('type' => 'signup', 'label' => t('Signup data')),
    ),
    'base' => 'signup_mark_attended_action',
    'help' => 'Marks a signup as attended.',
  );
  $actions['mark_not_attended'] = $defaults + array(
    'label' => t('Mark signup as not attended'),
    'parameter' => array(
      'signup' => array('type' => 'signup', 'label' => t('Signup data')),
    ),
    'base' => 'signup_mark_not_attended_action',
    'help' => 'Marks a signup as not attended.',
  );
  $actions['cancel_signup'] = $defaults + array(
    'label' => t('Cancel a signup'),
    'parameter' => array(
      'signup' => array('type' => 'signup', 'label' => t('Signup data')),
    ),
    'base' => 'signup_cancel_action',
    'help' => 'Cancels an individual signup.',
  );
  $actions['send_mail'] = $defaults + array(
    'label' => t('Send mail to users signed up to content'),
    'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Content'),
      ),
      'limit' => array(
        'type' => 'text',
        'label' => t('Send mail to'),
        'options list' => 'signup_rules_limit_options',
        'restriction' => 'input',
        'description' => t('Limit the mail to specific users sign up to this content.'),
      ),
      'subject' => array(
        'type' => 'text',
        'label' => t('Subject'),
        'description' => t('The mail\'s subject.'),
      ),
      'message' => array(
        'type' => 'text',
        'label' => t('Message'),
        'description' => t('The mail\'s message body.'),
      ),
      'from' => array(
        'type' => 'text',
        'label' => t('From'),
        'description' => t('The mail\'s from address. Leave it empty to use the site-wide configured address.'),
        'optional' => TRUE,
      ),
    ),
    'base' => 'signup_rules_action_send_mail',
  );
  $actions['suppress_form'] = $defaults + array(
    'label' => t('Prevent display of signup form'),
    'parameter' => array(
      'form_display_status' => array('type' => 'form_display_status', 'label' => t('Signup form display status')),
    ),
    'base' => 'signup_rules_action_suppress',
    'help' => 'Prevents the display of the signup form.',
  );
  return $actions;
}

/**
 * Provides options for limiting a mail to signed up users form.
 */
function signup_rules_limit_options() {
  return array(
    'all' => t('All signed up users'),
    'attended' => t('Users who attended'),
    'not attended' => t('Users who did not attended'),
  );
}

/**
* Signup integration access callback.
*/
function signup_rules_signup_integration_access($type, $name) {
  if ($type == 'event' || $type == 'condition') {
    return user_access('view all signups');
  }
  return user_access('administer all signups');
}

/**
 * Implements hook_rules_data_info().
 */
function signup_rules_rules_data_info() {
  return array(
    'signup' => array(
      'label' => t('signup data'),
      'group' => t('Signup'),
      'token type' => 'signup',
      'property info' => array(),
    ),
    'form_display_status' => array(
      'label' => t('Signup form display status'),
      'group' => t('Signup'),
      'property info' => array(),
    ),
  );
}
