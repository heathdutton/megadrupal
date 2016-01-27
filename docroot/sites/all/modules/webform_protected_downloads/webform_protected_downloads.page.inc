<?php
// $Id: webform_protected_downloads.page.inc,v 1.1.2.4 2010/11/17 21:56:37 berliner Exp $

/**
 * Displays the download page
 *
 * It is important that the first thing to do is the session check and
 * registration, because the function
 *    webform_protected_downloads_file_user_has_access($nid, $fid)
 * uses information from the session to determine the access state of the
 * current user.
 *
 * @param string $hash 
 * @return void
 */

function webform_protected_downloads_download_page($node, $hash = NULL) {
  global $conf;
  
  // don't cache access allowed or denied
  $conf['cache'] = 0;
  
  $admin_access = FALSE;
  $access = FALSE;
  if ($hash === NULL) {
    global $user;
    $admin_access = user_access('administer webform protected downloads');
    $nid = $node->nid;
  }
  else {
    // check if the hash is registered
    $nid = webform_protected_downloads_get_node_from_hash($hash);
  
    // check if access should be granted if a node is found for this hash
    $access = webform_protected_downloads_hash_is_valid($hash) || webform_protected_downloads_session_is_valid($hash);
  }
  
  drupal_set_title(check_plain($node->title));
  $configuration = webform_protected_downloads_get_configuration($node->nid);
  
  // check if the hash is valid for this node and has not yet expired
  if ($nid && $node->nid == $nid && ($access || $admin_access)) {
    
    $hash_details = webform_protected_downloads_get_hash_details($hash);
    
    if (!$admin_access) {
      
      // create or write the access information into the session
      webform_protected_downloads_session_update($node, $hash);
    
      // update the timestamp of the "used" column so that we know when this
      // hash has been last used, this is important for one-time access
      db_update("wpd_access_hashes")->fields(array('used' => time()))->condition('hash', $hash)->execute();
      
      // get the submission, including all it's data
      module_load_include('inc', 'webform', 'includes/webform.submissions');
      $submission = webform_get_submission($node->nid, $hash_details->sid);
      
    }
    
    // set all protected files listed
    $files = array();
    foreach ($node->wpd['private_files'] as $key => $file) {
      if (webform_protected_downloads_file_user_has_access($node->nid, $key) || $admin_access) {
        $files[] = (array) $file;
      }
    }
    
    // render the text above the file listing
    $output = _webform_protected_downloads_token_replace($configuration->text_download_access, $node, $hash);
    
    if (!$admin_access) {
      
      // we need to sanitize the user entered data before we let webform do
      // the token replacement, because we can't set strict to true and so must
      // care for this on our own
      foreach ($submission->data as $cid => $data_value) {
        
        // webform 7.x-3.x
        if (isset($submission->data[$cid]['value'])) {
          foreach ($submission->data[$cid]['value'] as $key => $value) {
            $submission->data[$cid]['value'][$key] = _webform_filter_xss($value);
          }
        }
        else {
        // webform 7.x-4.x
          foreach ($submission->data[$cid] as $key => $value) {
            $submission->data[$cid][$key] = _webform_filter_xss($value);
          }
        }
      }
      
      // now replace the placeholders with the submitted user data
      // we need to set $allow_anonymous to TRUE, otherwise the replacement won't
      // work, this should be save, since the hash is already a security measure
      // strict is set to false so that filter_xss will not get called after the
      // value replacement
      $output = _webform_filter_values($output, $node, $submission, $email = NULL, $strict = FALSE, $allow_anonymous = TRUE);
    }
    
    // filter the output according to the nodes filter format, don't check if
    // the current user has access to this format
    $output = check_markup($output, $configuration->text_download_access_format, $node->language);
    
    if (count($files)) {
      
      // render the protected files
      $output .= theme('file_formatter_table', array('items' => $files));
    
    }
    else {
      
      // no files found
      $output .= '<p>' . t('No files have been found.') . '</p>';
      
    }
    
    return $output;
  }
  
  $output = _webform_protected_downloads_token_replace($configuration->text_download_noaccess, $node, $hash);
  $output = check_markup($output, $configuration->text_download_noaccess_format, $node->language);
  return $output;
}

/**
 * Check if the given hash is valid
 *
 * @param string $hash 
 * @return void
 */

function webform_protected_downloads_hash_is_valid($hash) {
  
  $sql = "SELECT    expires, access_type, used
          FROM      {wpd_access_hashes}
          LEFT JOIN {webform_submissions} USING(sid)
          LEFT JOIN {wpd_node_configuration} USING(nid)
          WHERE     hash = :hash";
  
  $access = db_query($sql, array(':hash' => $hash))->fetchObject();
  
  // no hash found, so access is denied
  if (!$access) {
    return FALSE;
  }
  
  // for access type "single" we deny access if the hash has already been used
  if ($access->access_type == WEBFORM_PROTECTED_DOWNLOADS_ACCESS_TYPE_SINGLE) {
    return $access->used == 0;
  }
  else {
    // otherwise we grant access if the hash has not yet expired
    return ($access->expires == 0) || $access->expires > time();
  }
  
  return FALSE;
}

/**
 * Check if there is a valid session for the given hash
 *
 * @param string $hash 
 * @return boolean
 */

function webform_protected_downloads_session_is_valid($hash) {
  
  return isset($_SESSION[WEBFORM_PROTECTED_DOWNLOADS_SESSION_KEY][$hash]) && $_SESSION[WEBFORM_PROTECTED_DOWNLOADS_SESSION_KEY][$hash]['expires'] > time();
  
}

/**
 * Create or update the session info used to grant access and keep track of
 * access expiration
 *
 * @param object $node 
 * @param string $hash 
 */

function webform_protected_downloads_session_update($node, $hash) {
  
  // check that the session variable is correctly initialized
  if (!isset($_SESSION[WEBFORM_PROTECTED_DOWNLOADS_SESSION_KEY])) {
    $_SESSION[WEBFORM_PROTECTED_DOWNLOADS_SESSION_KEY] = array();
  }
  
  // if this is the first time, we need to add the hash to the session
  if (!isset($_SESSION[WEBFORM_PROTECTED_DOWNLOADS_SESSION_KEY][$hash])) {
    
    // get the expiration in seconds for the session
    $session_expires = webform_protected_downloads_get_configuration(
      $node->nid,
      'expiration_session',
      WEBFORM_PROTECTED_DOWNLOADS_DEFAULT_EXPIRATION_SESSION
    );

    // register this hash in the session
    $_SESSION[WEBFORM_PROTECTED_DOWNLOADS_SESSION_KEY][$hash] = array(
      'expires' => $session_expires > 0 ? time() + $session_expires : 0,
      'nid' => $node->nid,
    );
    
  }
  
}