<?php

/**
 * Implements hook_drush_help().
 */
function htacess_drush_help($command) {
  switch ($command) {
    case 'drush:htaccess-deploy':
      return dt('Command to deploy a htaccess profile.');
  }
}

/**
 * Implements hook_drush_command().
 */
function htaccess_drush_command() {
  $items = array();
  $items['htaccess-deploy'] = array(
    'callback' => 'htaccess_htaccess_deploy',
    'drupal dependencies' => array('htaccess'),
    'description' => dt('Deploy a htaccess profile.'),
    'arguments' => array(
      'profile_name'  => dt('Name of the htaccess\' profile.'),
    ),
    'examples' => array(
      'drush htaccess-deploy default' => 'Deploy the default htaccess shipped with Drupal.',
    ),
    'aliases' => array('ht'),
  );
  return $items;
}

/**
 * Callback function for drush htaccess. 
 * @param $profile_name
 */
function htaccess_htaccess_deploy($profile_name = NULL) {
  
  $profile_get = db_select('htaccess', 'h')
    ->fields('h')
    ->condition('name', array(':profile_name' => $profile_name),'IN')
    ->execute()
    ->fetchAssoc();

  if ($profile_get){
    if(drush_confirm(dt('Are you sure you want to deploy the htaccess profile: @profile_name?', array('@profile_name' => $profile_name)), $indent = 0)){

      $processing = htaccess_deploy_profile($profile_name);

      if($processing == true){
        drush_log(dt('Deploying htaccess @profile_name.', array('@profile_name' => $profile_name)), 'ok');

        drush_print(dt('Htaccess '. $profile_name .' has been deployed.', array('@profile_name' => $profile_name)));
      }
      else{
        drush_log(dt('Error during deployment.'), 'error');
      }
    }
    else{
      return;
    }
  }  
  else{
    drush_log(dt('Profile @profile_name not found.', array('@profile_name' => $profile_name)), 'error');
  }
}

function htaccess_deploy_profile($profile_name){
  $root_path = DRUPAL_ROOT;

  $htaccess_path =  $root_path   . '/.htaccess';

  $htaccess_get = db_select('htaccess', 'h')
    ->fields('h')
    ->condition('name', array(':profile_name' => $profile_name),'IN')
    ->execute()
    ->fetchAssoc();

  $htaccess_content = $htaccess_get['htaccess'];

  if (file_put_contents($htaccess_path, $htaccess_content)) {
     // Get the current htaccess deployed
    $htaccess_current = db_select('htaccess', 'h')
    ->fields('h')
    ->condition('deployed', 1, '=')
    ->execute()
    ->fetchAssoc();

    // If any, set the status to 0
    if($htaccess_current){
      db_update('htaccess')->fields(array(
      'deployed' => 0))
      ->condition('id', $htaccess_current['id'], '=')
      ->execute();
    }

     // Set the status to 1
    db_update('htaccess')->fields(array(
    'deployed' => 1))
    ->condition('id', $htaccess_get['id'], '=')
    ->execute();

    drupal_chmod($htaccess_path, 0644);

    return true;
  }
  else{
    return false;
  }
}
