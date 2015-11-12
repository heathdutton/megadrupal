<?php

/**
 * @file
 * Contains the callback for an ultra-simplified real CRM, which can be accessed
 * by REST, that is, POSTing data, including a pseudo-password, to the path at
 * mockable_crm_example_simple_crm, of which the callback is defined here.
 *
 * Notes:
 * (1) This is not meant to be enabled on a production server.
 * (2) The example module mockable_crm_example is provided as an example of how
 *     to use Mockable to test integration of Drupal with a real external CRM.
 *     Because no actual real external CRM exists (it's only an example), this
 *     simplified real CRM is provided simply for a proof of concept. If you use
 *     this module as a basis to create your own CRM integration module, you
 *     can safely delete this file and the corresponding menu hook entirely, as
 *     the functionality it provides will be in whatever real external system
 *     you are integrating with.
 * (3) The code below is *not* tested, because it's provided for convenience,
 *     and adding tests for this code (which will not be part of a real-world
 *     module) might be confusing; as the real tests we want to showcase test
 *     the mock object and not this real CRM.
 */

/**
 * Callback for the path mockable_crm_example_simple_crm
 */
function mockable_crm_example() {
  // We are storing our entire peusdo-database in a Drupal variable. Not
  // robust, but this implementation is meant to be super-simplified.
  $database = variable_get('mockable_crm_example_simple_real_db');

  // Even though this function resides within Drupal, it is meant to
  // demonstrate how to interact with Drupal via REST, that is calling
  // an URL with POST in this case. So the way to use this code is to
  // make a call via HTTP. The POST variable "data" will contain a json-
  // encoded array with the information necessary to interact with the
  // CRM.
  $data = drupal_json_decode($_POST['data']);

  $return = array();

  // First check if the password is correct.
  if (isset($data['password']) && $data['password'] == variable_get('mockable_crm_example_real_password', rand('1000000', '9999999'))) {
    // If so, make sure "action" is set to "get" or "add", then simply
    // modify the variable accordingly and return a valid response.
    if (isset($data['action'])) {
      switch ($data['action']) {
        case 'get':
          if (isset($data['record_id']) && $data['record_id']) {
            if (isset($database[$data['record_id']])) {
              $return['status'] = 'ok';
              $return['data'] = $database[$data['record_id']];
            }
            else {
              $return['status'] = 'error';
              $return['error'] = 'the request record_id does not exist in our database.';
            }
          }
          else {
            $return['status'] = 'error';
            $return['error'] = 'when using get, you must provide a record_id';
          }
          break;
        case 'add':
          if (isset($data['name']) && $data['name'] && isset($data['email']) && $data['email']) {
            $pseudo_unique_id = REQUEST_TIME . rand(1000000, 9999999);
            $database[$pseudo_unique_id] = array(
              'name' => $data['name'],
              'email' => $data['email'],
            );
            variable_set('mockable_crm_example_simple_real_db', $database);
            $return['status'] = 'ok';
            $return['data'] = $pseudo_unique_id;
          }
          else {
            $return['status'] = 'error';
            $return['error'] = 'when using add, you must provide a valid name and email';
          }
          break;
        default:
          $return['status'] = 'error';
          $return['error'] = 'you must provide get or add as your action';
          break;
      }
    }
    else {
      $return['status'] = 'error';
      $return['error'] = 'you must provide an action';
    }
  }
  else {
    $return['status'] = 'error';
    $return['error'] = 'the password you provided is invalid (' . variable_get('mockable_crm_example_real_password', rand('1000000', '9999999')) . ' ' . serialize($_POST) . ')';
  }

  die (drupal_json_encode($return));
}
