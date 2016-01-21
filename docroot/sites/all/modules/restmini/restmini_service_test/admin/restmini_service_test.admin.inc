<?php
/**
 * @file
 * restmini: Admin layer.
 */


/**
 * RESTmini Service Test test execution form.
 *
 * @param array $form
 * @param array &$form_state
 *
 * @return array
 */
function restmini_service_test_execute_form($form, &$form_state) {
  // This class must always be included when invoking hook_restmini_service_test().
  module_load_include('inc', 'restmini_service_test', 'inc/RestMiniServiceTestVar');

  drupal_add_css(
    drupal_get_path('module', 'restmini') . '/admin/restmini.admin.css',
    array('type' => 'file', 'group' => CSS_DEFAULT, 'preprocess' => FALSE, 'every_page' => FALSE)
  );
  drupal_add_css(
    drupal_get_path('module', 'restmini_service_test') . '/admin/restmini_service_test.admin.css',
    array('type' => 'file', 'group' => CSS_DEFAULT, 'preprocess' => FALSE, 'every_page' => FALSE)
  );
  drupal_add_js(
    drupal_get_path('module', 'restmini_service_test') . '/admin/restmini_service_test.admin.js',
    array('type' => 'file', 'group' => JS_DEFAULT, 'preprocess' => FALSE, 'every_page' => FALSE)
  );

  $form['#attributes']['autocomplete'] = 'off';

  $form['tests_available'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tests available'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  $modules = module_implements('restmini_service_test');
  if (!$modules) {
    drupal_set_message(
      t('No RESTmini Service tests available, no enabled module implements hook_restmini_service_test().',
        array(), array('context' => 'module:restmini_service_test')),
      'notice'
    );
    return $form;
  }

  $modules_info = system_rebuild_module_data();

  foreach ($modules as $module) {
    $form['tests_available'][$module] = array(
      '#type' => 'checkbox',
      '#title' => !empty($modules_info[$module]->info['name']) ? ($modules_info[$module]->info['name'] . ' (' . $module . ')') : $module,
      '#default_value' => 0,
      '#suffix' => '<div class="restmini-service-test-tests">',
      '#attributes' => array(
        'onclick' => 'restminiServiceTestCascadeDown(this);'
      ),
    );

    $function = $module . '_restmini_service_test';
    $tests = array_keys($function());

    foreach ($tests as $name) {
      $form['tests_available'][$module . ':' . $name] = array(
        '#type' => 'checkbox',
        '#title' => $name,
        '#default_value' => 0,
        '#attributes' => array(
          'onclick' => 'restminiServiceTestCascadeUp(this);'
        ),
      );
    }
    $form['tests_available'][] = array(
      '#type' => 'markup',
      '#markup' => '</div>',
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit', array(), array('context' => 'module:restmini_service_test')),
  );

  return $form;
}

/**
 * @param array $form
 * @param array @$form_state
 */
function restmini_service_test_execute_form_submit($form, &$form_state) {
  $values =& $form_state['values']['tests_available'];

  /*foreach ($var as $val) {
    //
  }*/

  drupal_set_message(
    t('GUI based test execution is not implemented yet, use drush restmini-service-test \'http://ser.ver\' \'test_module\' instead.',
      array(), array('context' => 'module:restmini_service_test')),
    'notice'
  );
}
