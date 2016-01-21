<?php

/**
 * @file
 * Administrative GUI for Mockable.
 */

/**
 * Form callback for the admin page.
 */
function mockable_admin($form, &$form_state) {
  $form = array();

  $rules = mockable_get();

  $form['mockable_use'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use mock objects.'),
    '#description' => t('See the Mockable module\'s README.txt file and example modules for how to use mock objects.'),
    '#default_value' => count($rules)?1:0,
  );

  $form['mockable_warn_every_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Warn administrators on every page when mock objects are being used. Users with insufficient privileges will not see the warning.'),
    '#default_value' => variable_get('mockable_warn_every_page', TRUE),
    '#states' => array(
      'visible' => array(
        ':input[name="mockable_use"]' => array('checked' => TRUE),
      ),
    ),
  );

  // third party module settings.
  foreach (module_invoke_all('mockable_info') as $key => $info) {
    $form[$key . '_main'] = array(
      '#type' => 'checkbox',
      '#title' => $info['title'],
      // The default value corresponds to whether a pattern exists
      // for this function, or if no pattern exists at all, because
      // we want all rules to become one when the checkbox "Use mock
      // objects" becomes on.
      '#default_value' => (!count($rules)) || mockable_get($key),
      '#states' => array(
        'visible' => array(
          ':input[name="mockable_use"]' => array('checked' => TRUE),
        ),
      ),
    );

    $form[$key . '_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => $info['title'] . ' — ' . t('settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#states' => array(
        'visible' => array(
          ':input[name="' . $key . '_main"]' => array('checked' => TRUE),
          ':input[name="mockable_use"]' => array('checked' => TRUE),
        ),
      ),
    );
    
    foreach ($info['settings'] as $variable => $field) {
      $form[$key . '_fieldset'][$variable] = $field;
    }
  }

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="mockable_use"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['advanced']['mock_use_specific'] = array(
    '#type' => 'textarea',
    '#title' => t('Advanced: edit patterns used to match function names'),
    '#default_value' => implode(PHP_EOL, mockable_get()),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );

  return $form;
}

/**
 * Return a simplified state array based on $form_state
 *
 * @param $form_state
 *   A form state array as used internally in Drupal
 *
 * @return
 *   An associative array with only the information needed by
 *   Mockable.
 */
function mockable_admin_form_state($form_state) {
  $return = array(
    'patterns' => array(),
    'advanced' => array(),
    'advanced-new' => array(),
    'patterns-on' => array(),
    'patterns-off' => array(),
  );
  $return['use'] = $form_state['complete form']['mockable_use']['#value'];
  $return['warn'] = $form_state['complete form']['mockable_warn_every_page']['#value'];
  foreach (module_invoke_all('mockable_info') as $pattern => $info) {
    $return['patterns'][$pattern] = $form_state['complete form'][$pattern . '_main']['#value'];
    if ($return['patterns'][$pattern]) {
      $return['patterns-on'][] = $pattern;
    }
    else {
      $return['patterns-off'][] = $pattern;
    }
  }

  $default_patterns = explode(PHP_EOL, $form_state['complete form']['advanced']['mock_use_specific']['#default_value']);

  $patterns = explode(PHP_EOL, $form_state['complete form']['advanced']['mock_use_specific']['#value']);
  foreach ($patterns as $pattern) {
    $trimmed = trim($pattern);
    if ($trimmed) {
      if (!in_array($trimmed, $default_patterns)) {
        $return['advanced-new'][] = $trimmed;
      }
      $return['advanced'][] = $trimmed;
    }
  }

  return $return;
}

/**
 * Validation handler for Mockable's administrative form
 */
function mockable_admin_validate($form, &$form_state) {
  $state = mockable_admin_form_state($form_state);
  if ($state['use'] && !count($state['patterns-on']) && count($state['patterns-off']) && !count($state['advanced'])) {
    form_set_error('mockable_use', t('Mock objects cannot be on unless at least one pattern is set to use mock objects'));
  }

  if ($state['use']) {
    foreach (module_implements('mockable_info_validate') as $module) {
      $function = $module . '_mockable_info_validate';
      if (function_exists($function)) {
        $function($form, $form_state);
      }
    }
  }
}

/**
 * Submit handler for Mockable's administrative form.
 */
function mockable_admin_submit($form, &$form_state) {
  $state = mockable_admin_form_state($form_state);
  variable_set('mockable_warn_every_page', $state['warn']);
  if ($form_state['complete form']['mockable_use']['#value']) {
    foreach (module_implements('mockable_info_submit') as $module) {
      $function = $module . '_mockable_info_submit';
      if (function_exists($function)) {
        $function($form, $form_state);
      }
    }

    // We know we want to to use mock objects, but we are not sure for
    // which patterns.
    $available_patterns = module_invoke_all('mockable_info');
    $selected_patterns = array();
    foreach ($available_patterns as $key => $info) {
      if ($form_state['complete form'][$key . '_main']['#value']) {
        $selected_patterns[] = $key;
      }
    }
    // remove old patterns
    mockable_unset();
    if (count($selected_patterns) == count($available_patterns)) {
      mockable_set();
    }
    else {
      foreach ($selected_patterns as $pattern) {
        try {
          mockable_set($pattern);
        }
        catch (Exception $e) {
          drupal_set_message(t('Some patterns could not be set'), 'error', FALSE);
        }
      }

      $errors = array();
      $patterns = explode(PHP_EOL, $form_state['complete form']['advanced']['mock_use_specific']['#value']);
      foreach ($patterns as $pattern) {
        $pattern = trim($pattern);
        if ($pattern) {
          // we can't blindly apply this rule, because if something
          // has been unchecked and this rule would apply to it,
          // it should not appear here.
          $add_rule = TRUE;
          foreach ($available_patterns as $function => $functioninfo) {
            try {
              if (mockable_rule_applies($function, $pattern) && !in_array($function, $selected_patterns)) {
                $add_rule = FALSE;
                break;
              }
            }
            catch (Exception $e) {
              $errors[] = $pattern;
              $add_rule = FALSE;
              break;
            }
          }
          if ($add_rule) {
            mockable_set($pattern);
          }
          else {
            if (in_array($pattern, $state['advanced-new'])) {
              drupal_set_message(t('The pattern @p could not be added because the associated checkbox is not selected', array('@p' => $pattern)), 'error', FALSE);
            }
          }
        }
      }

      foreach ($errors as $error) {
        drupal_set_message(t('We could not determine whether some patterns (@patterns) apply to unselected functions, so they were removed from the list', array('@patterns' => implode(', ', $errors))), 'error', FALSE);
      }
    }
  }
  else {
    mockable_unset();
  }
  drupal_set_message(mockable_get()?t('Mock objects are being used.'):t('Mock objects are not being used.'), mockable_get()?'warning':'status');
}
