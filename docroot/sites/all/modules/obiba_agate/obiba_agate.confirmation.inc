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
 * The password form.
 */
function obiba_agate_password_form($form, &$form_state) {
  $form['#action'] = '';
  $form['name']['password'] = array(
    '#type' => 'password',
    '#title' => t('Enter your password'),
    '#required' => TRUE,
    '#default_value' => "",
    '#description' => t('Please enter your password'),
    '#size' => 20,
    '#maxlength' => 20,
    '#attributes' => array(
      'id' => 'type-password',
    ),
  );
  $form['name']['repassword'] = array(
    '#type' => 'password',
    '#title' => t('Re-type your password'),
    '#required' => TRUE,
    '#default_value' => "",
    '#description' => t('Please retype your password'),
    '#size' => 20,
    '#maxlength' => 20,
    '#attributes' => array(
      'id' => 'verif-password',
    ),

  );
  $form['name']['confirmed_password'] = array(
    '#type' => 'hidden',
    '#required' => TRUE,
    '#default_value' => "",
    '#attributes' => array(
      'id' => 'password',
    ),
  );
  $form['name']['key'] = array(
    '#type' => 'hidden',
    '#required' => TRUE,
    '#default_value' => "",
    '#attributes' => array(
      'id' => 'key',
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  return $form;
}

/**
 * The password reset/confirmation Form.
 *
 * @return string
 *   The html themed of the password reset form.
 */
function obiba_agate_password_form_process() {
  $module_path_ = drupal_get_path('module', 'obiba_agate');
  drupal_add_js($module_path_ . '/js/obiba-agate-app-confirm-register.js', array(
    'type' => 'file',
    'scope' => 'footer',
  ));
  if (strstr(current_path(), 'confirm')) {
    drupal_set_title('Password registration confirmation');
    return theme('obiba_agate_user-form-password-confirm',
      array(
        'form_password_registration' => drupal_get_form('obiba_agate_password_form'),
      )
      );
  }
  elseif (strstr(current_path(), 'reset')) {
    drupal_set_title('Password Reset');
    return theme('obiba_agate_user-form-password-reset',
      array(
        'form_password_registration' => drupal_get_form('obiba_agate_password_form'),
      ));
  }
}

/**
 * Resource call back to send password to server.
 *
 * @param string $resource
 *   Resource can be reset/confirm.
 */
function obiba_agate_send_password_user($resource = 'confirm') {
  $parameters = drupal_get_query_parameters($_POST);
  if (!empty($parameters)) {
    $agate_client = new ObibaAgateClient();
    $response = $agate_client->sendPassword(
      array(
        'key' => $parameters['key'],
        'password' => $parameters['password'],
      ), $resource
    );
    if (empty($response['code'])) {
      drupal_set_message(t('You can now log in.'));
      drupal_json_output($response);
      die;
    }
    else {
      drupal_add_http_header('Status', '400 Bad Request');
      drupal_set_message(t('Server Error : @message', array('@message' => $response['message'])), 'error');
    }
  }
  drupal_json_output(array());
}
