<?php

/**
 * @file
 * AdInsight Clarity administration UI.
 */

/**
 * Implements hook_admin_settings().
 */
function adinsight_clarity_admin_settings_form($form_state) {

  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => 'Account Settings',
  );

  $form['account']['adinsight_clarity_account'] = array(
    '#type' => 'textfield',
    '#title' => t('AdInsight Clarity Account'),
    '#default_value' => variable_get('adinsight_clarity_account', ''),
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE,
    '#description' => t('Enter your <a href="@adinsight">AdInsight Clarity</a> account number', array('@adinsight' => 'http://www.adinsight.com/')),
  );

  $form['account']['adinsight_clarity_base_phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Base telephone number'),
    '#default_value' => variable_get('adinsight_clarity_base_phone', ''),
    '#size' => 20,
    '#maxlength' => 20,
    '#required' => TRUE,
    '#description' => t('<p>The base number should match the dymanic number format (numbers and spaces)</p><p>The base phone number is inserted into the page and replaced with the dynamicly generated number when the AdInsight script executes. The base phone number will also be seen by users who have JavaScript disabled.</p>'),
  );

  $form['account']['adinsight_clarity_pool'] = array(
    '#type' => 'textfield',
    '#title' => t('Dynamic Number Pool ID'),
    '#default_value' => variable_get('adinsight_clarity_pool', ''),
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE,
    '#description' => t('Defines the number pool that the AdInsight script draws dynamic numbers from.'),
  );

  $form['usage'] = array(
    '#type' => 'fieldset',
    '#title' => t('Usage'),
  );

  // Usage instructions can only be shown if the number pool and base phone
  // number fields are already set, so we swap out the usage instructions for
  // a warning until they are set.
  $base = variable_get('adinsight_clarity_base_phone', '');
  $pool = variable_get('adinsight_clarity_pool', '');
  if (!empty($pool) && !empty($base)) {

    $tag = _adinsight_clarity_build_tag();

    $form['usage']['method-filter'] = array(
      '#type' => 'item',
      '#title' => t('Filter'),
      '#description' => t('This module enables a custom filter which will automatically replace the special <code>&lt;adinsight /&gt;</code> tag with the AdInsight span tag: <code><strong>@tag</strong></code>', array('@tag' => $tag)),
    );
    $form['usage']['method-block'] = array(
      '#type' => 'item',
      '#title' => t('Block'),
      '#description' => t('This module exposes a block called <em>AdInsight Clarity Telephone Number</em> containing the AdInsight span tag.'),
    );

    if (module_exists('token')) {
      $form['usage']['method-token'] = array(
        '#type' => 'item',
        '#title' => t('Token'),
        '#description' => t('This module exposes tokens for each of the settings and a token for the HTML span.'),
      );
    }
    else {
      $form['usage']['method-token'] = array(
        '#type' => 'item',
        '#title' => t('Token'),
        '#description' => t('<p>The Token module is not enabled.</p><p>This module exposes tokens for each of the settings and a token for the HTML span.</p>'),
      );
    }

    $form['usage']['method-tag'] = array(
      '#type' => 'item',
      '#title' => t('Manual HTML'),
      '#description' => t('<p>You can insert the following span tag at each point that you want your phone number to appear:</p><code><strong>@tag</strong></code><p><strong>Note:</strong> If you change the Number Pool or Base Number settings you will need to replace any existing spans in your theme or content.</p>', array('@tag' => $tag)),
    );
  }
  else {
    $form['usage']['info'] = array(
      '#type' => 'item',
      '#title' => '',
      '#description' => t('Please complete the account settings section and save your changes.'),
    );
  }

  $form['adinsight_clarity_js_scope'] = array(
    '#type' => 'radios',
    '#title' => t('JavaScript Scope'),
    '#options' => array(
      'header' => t('Header'),
      'footer' => t('Footer (default)'),
    ),
    '#default_value' => variable_get('adinsight_clarity_js_scope', 'footer'),
  );

  return system_settings_form($form);
}

/**
 * Implements hook_form_validate().
 */
function adinsight_clarity_admin_settings_form_validate($form, &$form_state) {

  // Check the fields for errors.
  $has_errors = FALSE;

  // Account should be a number.
  if (!preg_match('/^[0-9]{1,}$/', $form_state['values']['adinsight_clarity_account'])) {
    form_set_error('adinsight_clarity_account', t('Your AdInsight Clarity account must be numeric'));
    $has_errors = TRUE;
  }

  // Base number should match substitution format (numbers and spaces).
  if (!preg_match('/^[0-9 ]{1,}$/', $form_state['values']['adinsight_clarity_base_phone'])) {
    form_set_error('adinsight_clarity_base_phone', t('The base telephone number must match the dynamic substitution format (numbers and spaces)'));
    $has_errors = TRUE;
  }

  // Pool should be a number.
  if (!preg_match('/^[0-9]{1,}$/', $form_state['values']['adinsight_clarity_pool'])) {
    form_set_error('adinsight_clarity_pool', t('Your AdInsight Clarity pool ID must be numeric'));
    $has_errors = TRUE;
  }

  // If there are no errors reset the tag builder static.
  if (!$has_errors) {
    drupal_static_reset('_adinsight_clarity_build_tag');
  }
}
