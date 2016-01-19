<?php
// $Id: simple_contest.main.inc,v 1.1.2.1 2010/07/09 04:36:13 vinoth.3v Exp $

/**
 * @file
 *
 * simple_contest.main.inc
 * 
 */

/**
 * Implementation of hook_mail().
 */
function _simple_contest_mail($key, &$message, $params) {

  $settings = $params['settings'];
  
  // lets others to change the params
  $result = module_invoke_all('simple_contest', 'mail', $key, $params);
  
  $params += $result;
  
  if ( $key == 'winners_mail' ) {

      $subject = $settings['email_to_winner_subject'];
      $body = $settings['email_to_winner_body'];
       
      $message['subject'] = token_replace($subject, $params);
      $message['body'][] = token_replace($body, $params);

      return;
  }
  
  if ( $key == 'admin_winners_mail' ) {
    
      $subject = $settings['email_to_admin_subject'];
      $body = $settings['email_to_admin_body'];
      
      $message['subject'] = token_replace($subject, $params);
      $message['body'][] = token_replace($body, $params);
      
      return;
  }
  
  if ( $key == 'email_to_participants' ) {
    
      $subject = $settings['email_to_participants_subject'];
      $body = $settings['email_to_participants_body'];
      
      $message['subject'] = token_replace($subject, $params);
      $message['body'][] = token_replace($body, $params);
      
      return;
  }

}
