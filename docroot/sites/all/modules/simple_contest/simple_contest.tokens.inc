<?php
// $Id: simple_contest.token.inc,v 1.1.2.1 2010/07/09 04:36:13 vinoth.3v Exp $
/**
 * @file
 *
 * simple_contest.token.inc
 */

/**
 * Implementation of hook_token_list().
 */
function simple_contest_token_info() {

  // token types
  
  $info['types']['contest-node'] = array(
    'name' => t('Contest node'),
    'description' => t('Tokens related to contest node.'),
    'type' => 'node',
    'needs-data' => 'node',
  );
  
  $info['types']['participant-user'] = array(
    'name' => t('participant user or Winner user'),
    'description' => t("Tokens related to the participant or winner user."),
    'type' => 'user',
    'needs-data' => 'user',
  );
  
  $info['types']['participant-node'] = array(
    'name' => t('participant node or Winner node'),
    'description' => t("Tokens related to the participant or winner node."),
    'type' => 'node',
    'needs-data' => 'node',
  );

  
  // tokens
  
  $info['tokens']['contest-node']['max-winners'] = array(
    'name' => t('Max winners'),
    'description' => t("The max winners of this contest."),
  );
  
  $info['tokens']['contest-node']['winners-count'] = array(
    'name' => t('Winners count'),
    'description' => t("The total number of winners selected for this contest."),
  );
  
  $info['tokens']['contest-node']['winner-type'] = array(
    'name' => t('Winner type'),
    'description' => t("The winner type [node, user]."),
  );
  
  $info['tokens']['contest-node']['contest-status'] = array(
    'name' => t('Contest status'),
    'description' => t("The status of this contest."),
  );
  
  $info['tokens']['participant-user']['participant-rank'] = array(
    'name' => t('participant or winner user rank'),
    'description' => t("The rank for this participant or winner user."),
  );
  
  $info['tokens']['participant-node']['participant-rank'] = array(
    'name' => t('participant or winner node rank'),
    'description' => t("The rank for this participant or winner node."),
  );
  
  return $info;
  
}


/**
 * Implementation of hook_token_values().
 */
function simple_contest_tokens($type, $tokens, array $data = array(), array $options = array()) {

  $replacements = array();
  $sanitize = !empty($options['sanitize']);
  
  switch ($type){
    
    case 'contest-node':
      if ( !empty($data['contest-node']) ) {
        $contest = $data['contest-node'];
        $settings = simple_contest_get_settings($contest->type);
        foreach ($tokens as $name => $original) {
          switch ($name) {
            case 'max-winners':
              $replacements[$original] = $settings['max_winners'];
              break;
            case 'winner-type':
              $replacements[$original] = $settings['winner_type'];
              break;
            case 'contest-status':
              $statuses = simple_contest_status_array();
              $replacements[$original] = $statuses[$contest['simple_contest']['status']];
              break;
            case 'winners-count':
              $replacements[$original] = $contest['simple_contest']['winners_count'];
              break;
          }
        }
      }
      break;
    case 'participant-user': //
    case 'participant-node':
      if ( !empty($data['participant-rank']) ) {
        foreach ($tokens as $name => $original) {
          switch ($name) {
            case 'participant-rank':
              $replacements[$original] = $data['participant-rank'];
              break;
          }
        }
      }
      break;    
    }
    
  return $replacements;

}

