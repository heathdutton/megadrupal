<?php
/**
 * @file
 * Obiba Agate Mdule.
 *
 * Copyright (c) 2015 OBiBa. All rights reserved.
 * This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Register action (Ajax resource).
 */
function obiba_agate_register_user() {

  $form_attributes = obiba_agate_schema_form();

  // Secure the request by verify if the set of POST parameters are trusted by
  // the server.
  $response_captcha = $_POST['reCaptchaResponse'];
  unset($_POST['reCaptchaResponse']);

  // Perform the register request.
  $_POST['reCaptchaResponse'] = $response_captcha;
  $agate = new ObibaAgateClient();
  $response = $agate->addAgateUser($_POST);
  if (!empty($response['code']) && $response['code'] == 200) {
    drupal_json_output($response);
  }
  else {
    if (!empty($response['codeError'])) {
      $message_serv_array = json_decode($response['message']);
      obiba_agate_register_user_response_error($response['codeError'],
        json_encode(array("message" => $message_serv_array->message)),
        $form_attributes);
    }
  }
}

/**
 * Profile User (Ajax resource).
 */
function obiba_agate_user_profile() {
  switch ($_SERVER['REQUEST_METHOD']) {
    case 'GET':
      $agate = new ObibaAgateUserProfile();
      $ticket = ObibaAgateClient::hasTicket();
      if (!empty($ticket)) {
        $profile = $agate->getUserProfile();
        global $user;
        $user_profile = user_view(user_load($user->uid));
        $user_view = drupal_render($user_profile);
        drupal_json_output(array(
          'userProfile' => $profile,
          'drupalUserDisplay' => $user_view,
        ));
      }
      break;

    case 'PUT':
      $agate = new ObibaAgateUserProfile();
      $ticket = ObibaAgateClient::hasTicket();
      if (!empty($ticket)) {
        $profile = $agate->updateUserProfile(file_get_contents('php://input'));
        if (empty($profile['errorServer'])) {
          drupal_json_output(array('locationRedirection' => 'view'));
        }
        else {
          drupal_json_output(array_merge(array('locationRedirection' => 'edit'), $profile));
        }
      }
      break;
  }
}

/**
 * Function call Back to send the new password to server.
 */
function obiba_agate_user_profile_update_password() {
  switch ($_SERVER['REQUEST_METHOD']) {
    case 'PUT':
      $agate = new ObibaAgateUserProfile();
      $user_password = drupal_json_decode(file_get_contents('php://input'));
      $response = $agate->updateUserPassword($user_password['currentPassword'], $user_password['newPassword']);
      if (!empty($response['errorServer'])) {
        drupal_json_output($response);
      }
      break;
  }
}
