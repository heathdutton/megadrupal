<?php
// $Id: simple_contest_rules.rules.inc,v 1.1.2.4 2010/02/22 14:34:43 vinoth.3v Exp $

/**
 * @file
 * Provides rules integration
 */
/**
 * Implementation of hook_rules_event_info().
 */


/**
 * Implementation of hook_rules_event_info().
 */
function simple_contest_rules_rules_event_info() {
  
  return array(
    'simple_contest_rules_contest_completed' => array(
       'label' => t('Contest was completed and winners were finalized'),
       'variables' => array(
          'contest' => array('type' => 'node', 'label' => t('Contest node')),          
       ),
       'group' => 'Simple contest',
    ),
    'simple_contest_contest_status_changed' => array(
       'label' => t('Contest status was changed'),
       'variables' => array(
          'contest' => array('type' => 'node', 'label' => t('Contest node')),
          'contest_author' => array('type' => 'user', 'label' => t('Contest author'), 'handler' => 'simple_contest_argument_load_contest_user'),
       ),
       'group' => 'Simple contest',
    ),
  );
}

function simple_contest_argument_load_contest_user($args){
	return user_load($args['contest']->uid);
}

/**
 * Rules actions
 */

/**
 * Implementation of hook_rules_action_info().
 */
function simple_contest_rules_rules_action_info() {
  return array(
    'simple_contest_rules_action_change_status' => array(
      'label' => t('Change the contest status'),
      'parameter' => array(
        'contest' => array('type' => 'node', 'label' => t('Contest node')),
        'status' => array(
          'type' => 'integer',
          'label' => t('Change the contest status to'),
          'options list' => 'simple_contest_status_array',
          'optional' => FALSE,
        ),
      ),
      'group' => 'Simple contest',
    ),
    'simple_contest_rules_action_finalize_results' => array(
      'label' => t('Complete the contest and finalize the winners'),
      'parameter' => array(
          'contest' => array('type' => 'node', 'label' => t('Contest node'))
      ),
      'group' => 'Simple contest',
    ),
    'simple_contest_rules_action_send_mail_to_winners' => array(
      'label' => t('Send E-mail to all winners'),
      'parameter' => array(
        'contest' => array('type' => 'node', 'label' => t('Contest node')),
        'subject' => array('type' => 'text', 'label' => t('E-mail subject'), 'sanitize' => TRUE,),
        'from' => array('type' => 'text', 'label' => t('E-mail from'), 'sanitize' => TRUE,),
        'message' => array('type' => 'text', 'label' => t('Message'), 'sanitize' => TRUE,),
      ),
      'help' => t('IMPORTANT: disable the default buitin email notification to winners.'),
      'group' => 'Simple contest',
    ),
    'simple_contest_rules_action_create_new_contest' => array(
      'label' => t('Create new contest'),
      'parameter' => array(
        'contest_type' => array('type' => 'token', 'label' => t('Contest content type'), 'options list' => 'simple_contest_get_types_names'),    	
        'title' => array('type' => 'text', 'label' => t('Contest title'), 'sanitize' => TRUE,),
        'body' => array('type' => 'text', 'label' => t('Contest body'), 'sanitize' => TRUE,),
        'state_date' => array('type' => 'date', 'label' => t('Contest start date')),
        'end_date' => array('type' => 'date', 'label' => t('Contest end date')),
        'node_user' => array('type' => 'user', 'label' => t('Contest author')),
        'status' => array(
          'type' => 'integer',
          'label' => t('Change the contest status to'),
          'options list' => 'simple_contest_status_array',
          'optional' => FALSE,
        ),
      ),
      'new variables' => array(
        'created_contest' => array(
          'type' => 'node',
          'label' => t('New contest'),
        ),
      ),
      'group' => 'Simple contest',
    ),
    'simple_contest_rules_action_do_lucky_draw' => array(
      'label' => t('Do lucky draw and save'),
      'parameter' => array(
        'cid' => array('type' => 'integer', 'label' => t('Contest Node ID.')),
        'wid' => array('type' => 'integer', 'label' => t('Winner ID. (User ID if contest type is user, Node ID otherwise.)')),
      ),
      'new variables' => array(
        'islucky' => array(
          'type' => 'boolean',
          'label' => t('lucky draw result. TRUE if winner'),
        ),
        'winner_node' => array(
          'type' => 'node',
          'label' => t('Winner node'),
        ),
        'winner_user' => array(
          'type' => 'node',
          'label' => t('Winner user'),
        ),
      ),
      'group' => 'Simple contest',
    ),
    'simple_contest_rules_action_save_winner' => array(
      'label' => t('Save the winner to database'),
      'parameter' => array(
        'cid' => array('type' => 'integer', 'label' => t('Contest Node ID.')),
        'wid' => array('type' => 'integer', 'label' => t('Winner ID. (User ID if contest type is user, Node ID otherwise.)')),
      ),
      'group' => 'Simple contest',
    ),
  );
}

/**
 * Rules action - simple_contest_rules_action_contest_completed.
 */
function simple_contest_rules_action_finalize_results($contest, $settings) {
  
  simple_contest_finalize_results_node($contest);
  
}

/**
 * Rules action - simple_contest_rules_action_contest_completed.
 */
function simple_contest_rules_action_change_status($contest, $settings) {  
  $contest->simple_contest['status'] = $settings['status'];
  node_save($contest);  
}


/**
 * Rules action - simple_contest_rules_action_contest_completed.
 */

function simple_contest_rules_action_send_mail_to_winners($contest, $subject, $from, $message, $settings) {

  global $language;

  $from = ($from) ? str_replace(array("\r", "\n"), '', $from) : NULL;
  
  $winners = simple_contest_get_node_result($contest);
  $settings = simple_contest_get_settings($contest->type);
  
  foreach($winners as $info) {
		
	  $account = user_load($info['uid']);
      if ($info['nid']){
        $participant_node = node_load($info['nid']);
      }else{
        $participant_node = NULL;
      }
      
      $params = array(
        'contest-node' => $contest, 
        'participant-user' => $account, 
        'participant-node' => $participant_node, 
        'participant-rank' => $info['weight'], 
        'settings' => $settings, 
        'subject' => $subject,
        'from' => $from,
        'message' => $message,
      );
      
      drupal_mail('simple_contest_rules', 'rules_action_send_mail_to_winners', $info['mail'], $language, $params, $from);  
    
  }
  
}

/**
 * Rules action - simple_contest_rules_action_contest_completed.
 */
function simple_contest_rules_action_create_new_contest($contest_type, $title, $body, $state_date, $end_date, $node_user, $status) {
  
  $settings = simple_contest_get_settings($contest_type);
  
  $node = new StdClass();
  $date_field = $settings['date_field'];
  $node->type = $contest_type;
  
  node_object_prepare($node);
  
  $node->language = LANGUAGE_NONE;
    
  $node->title = $title;
  
  $node->body[$node->language][0]['value']   = $body;
  $node->body[$node->language][0]['summary'] = text_summary($body);
  $node->body[$node->language][0]['format']  = 'filtered_html';

  $node->is_new = TRUE;
  $node->status = NODE_PUBLISHED;
  
  $node->simple_contest['status'] = $status;
  $node->uid = $node_user->uid;
  
  
  // date fields
  $date_field = $settings['date_field'];
  $node->{$date_field}['und'][0]['value'] = format_date($state_date, 'custom', 'Y-m-d H:i');
  $node->{$date_field}['und'][0]['value2'] = format_date($end_date, 'custom', 'Y-m-d H:i');

  node_save($node); // save the new node
  
  return array('created_contest' => $node);
  
}

function simple_contest_rules_action_do_lucky_draw($cid, $wid) {
  
  $contest = node_load($cid);
  $result = simple_contest_is_instant_winner($contest, $wid);
  
  if ($result) {
    $contest = node_load($cid);
    $settings = simple_contest_get_settings($contest->type);
    
    if ($settings['winner_type'] == 'user') {
      $winner_node = NULL;
      $winner_user = user_load($wid);
    } else {
      $winner_node = node_load($wid);
      $winner_user = user_load($node->uid);
    }
    
  } else {
    $result = FALSE;
    $winner_node = NULL;
    $winner_user = NULL;
  }
  
  return array(
    'islucky' => $result,
    'winner_node' => $winner_node,
    'winner_user' => $winner_user,
  );
}

function simple_contest_rules_action_save_winner($cid, $wid) {
  
  $contest = node_load($cid);
  
  simple_contest_save_contest_winner($contest, $wid);
  
}

/**
 * Rules Conditions
 */

/**
 * Implementation of hook_rules_condition_info().
 */
function simple_contest_rules_rules_condition_info() {
  return array(
    'simple_contest_rules_is_contest' => array(
      'label' => t('Check the node is a contest type'),
      'parameter' => array(
        'node' => array('type' => 'node', 'label' => t('Contest node'))
      ),
      'group' => 'Simple contest',
    ),
    'simple_contest_rules_contest_hasstatus' => array(
      'label' => t('Contest has statuses'),
      'parameter' => array(
        'node' => array('type' => 'node', 'label' => t('Contest node')),
      ),
      'help' => t('Whether the contest has the selected statuses.'),
      'group' => 'Simple contest',
    ),
    'simple_contest_rules_condition_is_lucky_draw_winner' => array(
      'label' => t('Is lucky draw winner'),
      'parameter' => array(
        'node' => array('type' => 'node', 'label' => t('Contest node')),
        'wid' => array('type' => 'integer', 'label' => t('User ID or Node Id')),
      ),
      'help' => t('Do lucky draw and return TRUE if selected as Winner. You must save this winner by calling the "Save the winner to database" action.'),
      'group' => 'Simple contest',
    ),
  );
}

/**
 * Rules Condition is_contest
 */
function simple_contest_rules_is_contest($node) {
  return simple_contest_is_simple_contest($node);
}

/**
 * Condition: condition to check whether contest has status
 */
function simple_contest_rules_contest_hasstatus($contest, $settings) {
  $statuses = $settings['status'];
  foreach($statuses as $status){
    if ($contest->simple_contest['status'] == $status){
      return TRUE;
    }
  }
  return FALSE;
  
}

/**
 * Condition User: form to select contest status
 */
function simple_contest_rules_contest_hasstatus_form($settings, &$form) {
  $form['settings']['status'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Match the contest status against the selected statuses'),
    '#options' => simple_contest_status_array(),
    '#default_value' => isset($settings['status']) ? $settings['status'] : array(SIMPLE_CONTEST_STATUS_ACTIVE),
  );
}

function simple_contest_rules_contest_hasstatus_submit(&$settings, $form, $form_state) {
  $settings['status'] = array_filter(array_keys(array_filter($settings['status'])));
}

/**
 * Condition: is_lucky_draw_winner
 */
function simple_contest_rules_condition_is_lucky_draw_winner($contest, $wid, $settings) {
  return simple_contest_is_instant_winner($contest, $wid);
}

