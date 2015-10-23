<?php

/**
 * @file
 * Sample hooks demonstrating usage in Salesforce Web-to-Lead Webform Data Integration.
 */

/**
 * @defgroup webform_hooks Webform Module Hooks
 * @{
 * sfweb2lead_webform's hooks enable other modules to intercept events, such
 * as the completion of a submission and its subsequent passing of post strings to salesforce. 
 */

/**
 * Alter the processed array of posted fields data coming from webform and about to be passed to salesforce
 * @param array $data 
 *   The posted data array about to be passed to salesforce
 * @param array $context
 *   $context['webform_submission'] is a webform submission object containing
 *   webform nid, uid of the person submitting the form
 *     submission (Object) stdClass
 *       nid (String, 1 characters ) 1
 *       uid (String, 1 characters ) 1
 *       submitted (Integer) 1339531638
 *       remote_addr (String, 9 characters ) 127.0.0.1
 *       is_draft (Integer) 0
 *       data (Array, 1 element)
 *       sid (String, 1 characters ) 1 
 */
function hook_sfweb2lead_webform_posted_data_alter(&$data, $context) {
  // if the webform is node 1, then we are to do the following things to the posted data array
  // before it gets posted
  if ( $context['webform_submission']->nid == 1 ) {
    // consider the webform field name as the company field too!
    if ( !empty($data['name']) ) {
      $data['company'] = $data['name'];
    }
  }
}

/**
 * @}
 */
