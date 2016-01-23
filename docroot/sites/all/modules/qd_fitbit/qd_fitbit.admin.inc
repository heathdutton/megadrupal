 <?php
/**
 * @file
 * Admin functions
 */


/**
 * Implements hook_admin().
 */
function qd_fitbit_admin($form, &$form_state) {

  $form['qd_fitbit_intro'] = array(
    '#type' => 'markup',
    '#markup' => t('You must first register an app with Fitbit here: https://dev.fitbit.com/apps/new'),
  );

  $form['qd_fitbit_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Client (consumer) key'),
    '#default_value' => variable_get('qd_fitbit_key'),
    '#size' => 50,
    '#required' => TRUE,
  );

  $form['qd_fitbit_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Client (consumer) secret'),
    '#default_value' => variable_get('qd_fitbit_secret'), '#size' => 50,
    '#required' => TRUE,
  );

  $form['qd_fitbit_features'] = array(
    '#type' => 'checkboxes',
    '#options' => array(
      QD_FITBIT_FEATURE_WEIGHT => t('Weight, BMI, Body fat'),
      QD_FITBIT_FEATURE_STEPS => t('Steps'),
      QD_FITBIT_FEATURE_DISTANCE => t('Distance'),
      QD_FITBIT_FEATURE_ELEVATION => t('Elevation'),
      QD_FITBIT_FEATURE_SEDENTARY => t('Sedentary time vs. Active time'),
    ),
    '#title' => t('Enable download for the following data:'),
    '#default_value' => variable_get('qd_fitbit_features'),
  );

  return system_settings_form($form);
}


/**
 * Implements hook_form_FORMID_alter().
 */
function qd_fitbit_form_user_profile_form_alter(&$form, &$form_state, $form_id) {
  global $user;

  $key = variable_get('qd_fitbit_key');
  $secret = variable_get('qd_fitbit_secret');
  $consumer = DrupalOAuthConsumer::load($key, FALSE);

  $form['qd_fitbit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fitbit settings'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  if (isset($key) && isset($secret)) {
    if (qd_fitbit_get_access_client($user->uid, $key)) {
      // All connected (oauth), build the settings.
      qd_fitbit_build_settings_form($form, $user);
      qd_fitbit_build_disconnect_form($form);
    }
    else {
      // Need to have the user oauth to fitbit.
      qd_fitbit_build_connect_form($form);
    }
  }
  else {
    $form['qd_fitbit']['fitbit_settings_message'] = array(
      '#type' => 'markup',
      '#markup' => t('Fitbit integration has not yet been configured on this site yet.'),
    );
  }
}


/**
 * Build the portion of the form to OAuth to Fitbit.
 *
 * @param array $form
 *   The user profile form array.
 */
function qd_fitbit_build_connect_form(&$form) {
  $form['qd_fitbit']['fitbit_connect'] = array(
    '#type' => 'submit',
    '#value' => t('Connect with Fitbit'),
    '#submit' => array('qd_fitbit_connect_submit'),
  );
}


/**
 * Build the portion of the form to delete Fitbit OAuth tokens.
 *
 * @param array $form
 *   The user profile form array.
 */
function qd_fitbit_build_disconnect_form(&$form) {
  $form['qd_fitbit']['fitbit_disconnect'] = array(
    '#type' => 'submit',
    '#value' => t('Disconnect from Fitbit'),
    '#submit' => array('qd_fitbit_disconnect_submit'),
  );
}


/**
 * Build the portion of the form to display QD Fitbit settings.
 *
 * @param array $form
 *   The user profile form array.
 * @param object $user
 *   The user object.
 */
function qd_fitbit_build_settings_form(&$form, $user) {
  // Load default values.
  $defaults = array();
  if (isset($user->data['qd_fitbit_settings'])) {
    $defaults = $user->data['qd_fitbit_settings'];
  }

  // $form['qd_fitbit']['qd_fitbit_settings'] = array(
  //   '#type' => 'checkboxes',
  //   '#options' => array(
  //     QD_FITBIT_WEIGHT => t('Weight'),
  //     QD_FITBIT_BMI => t('BMI (Weight must be turned on)'),
  //     QD_FITBIT_BODYFAT => t('Body fat'),
  //     QD_FITBIT_STEPS => t('Steps'),
  //   ),
  //   '#title' => t('What data would you like to sync from Fitbit?'),
  //   '#default_value' => $defaults,
  // );
}


/**
 * Implements hok_user_presave().
 */
function qd_fitbit_user_presave(&$edit, $account, $category) {
  // Store the fitbit settings in the data column of the user.
  // if (isset($edit['qd_fitbit_settings'])) {
  //   $edit['data']['qd_fitbit_settings'] = $edit['qd_fitbit_settings'];

  //   // Enable any required modules.
  //   $module_list = array();

  //   foreach ($edit['qd_fitbit_settings'] as $item) {
  //     if ($item === QD_FITBIT_WEIGHT) {
  //       $module_list[] = 'qd_core_weight';
  //     }

  //     if ($item === QD_FITBIT_BMI) {
  //       $module_list[] = 'qd_core_bmi';
  //     }

  //     if ($item === QD_FITBIT_BODYFAT) {
  //       $module_list[] = 'qd_core_bodyfat';
  //     }

  //     if ($item == QD_FITBIT_STEPS) {
  //       $module_list[] = 'qd_core_activity';
  //     }
  //   }

  //   module_enable($module_list, TRUE);
  //}
}
