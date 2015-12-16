<?php

/**
 * @file
 *   TestSwarm module - admin.
 */

/**
 * Clear tests.
 */

function testswarm_clear_test_details($form, $form_state, $caller) {
  $tests = testswarm_defined_tests();
  if (!empty($tests)) {
    $test = $tests[$caller];
    if ($test) {
      return confirm_form(
        array(
          'caller' => array(
            '#type' => 'value',
            '#value' => $caller,
          ),
        ),
        t('Are you sure you want to remove all test details of %caller?', array('%caller' => $caller)),
        'testswarm-tests',
        t('This action cannot be undone.'),
        t('Remove test details'),
        t('Cancel')
      );
    }
  }
}


/**
 * Form submission handler for testswarm_clear_test_details().
 */
function testswarm_clear_test_details_submit($form, &$form_state) {
  $caller = $form_state['values']['caller'];
  
  $sql = "DELETE FROM {testswarm_test} WHERE caller = :caller";
  db_query($sql, array(
    ':caller' => check_plain($caller),
  ));
  
  $sql = "DELETE FROM {testswarm_test_run} WHERE qt_id NOT IN (SELECT id FROM {testswarm_test})";
  db_query($sql);
  
  $sql = "DELETE FROM {testswarm_test_run_detail} WHERE tri NOT IN (SELECT id FROM {testswarm_test_run})";
  db_query($sql);
  
  $form_state['redirect'] = 'testswarm-tests';
}

/**
 * Form callback for admin settings.
 */
function testswarm_admin_settings_form($form, $form_state) {
  $form['browserstack'] = array(
    '#type' => 'fieldset',
    '#title' => t('Browserstack'),
    '#description' => t('Browserstack settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['browserstack']['testswarm_browserstack_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => variable_get('testswarm_browserstack_username', ''),
    '#description' => t('Your browserstack username'),
  );
  $form['browserstack']['testswarm_browserstack_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => variable_get('testswarm_browserstack_password', ''),
    '#description' => t('Your browserstack password'),
  );
  $form['browserstack']['testswarm_browserstack_api_url'] = array(
    '#type' => 'textfield',
    '#title' => t('API URL'),
    '#default_value' => variable_get('testswarm_browserstack_api_url', 'http://api.browserstack.com/1'),
    '#description' => t('The browserstack url all requests are made to. From the browserstack documentation: "All requests are made to http://api.browserstack.com/VERSION"')
  );
  return system_settings_form($form);
}
 