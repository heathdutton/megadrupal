<?php

/**
 * @file
 * Provides admiinistrative forms and related functions.
 */

/**
 * Page callback for content type mapping forms.
 */
function jobfaucet_mapping_form($form, &$form_state, $content_type) {

  module_load_include('inc', 'jobfaucet', 'jobfaucet.targets');
  _jobfaucet_test_mode_message();

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'jobfaucet') . '/css/admin.css',
  );

  // Pass the content type along to the submit functions.
  $form['content_type'] = array('#type' => 'value', '#value' => $content_type);

  $form['required_mapping']['#tree'] = TRUE;
  $config = jobfaucet_get_mapping($content_type);
  $targets = jobfaucet_get_target_fields();

  foreach ($targets as $key => $target) {
    $form['required_mapping'][$key] = array(
      '#title' => $target['label'],
      '#type' => 'textfield',
      '#required' => $target['required'],
      '#default_value' => isset($config[$key]) ? $config[$key] : '',
      '#attributes' => array('class' => array('jobfaucet-custom-token')),
      '#element_validate' => array('token_element_validate'),
      '#after_build' => array('token_element_validate'),
      '#token_types' => array('node'),
      '#description' => $target['description'],
      // If a fixed value is not allowed, require the field to contain at
      // least one token.
      '#min_tokens' => $target['allow_fixed_value'] ? 0 : 1,
    );
  }

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array('#type' => 'submit', '#value' => t('Save')),
    'reset' => array(
      '#type' => 'submit',
      '#submit' => array('jobfaucet_reset_mapping'),
      '#value' => t('Reset to defaults'),
    ),
  );

  // Adds list of available tokens.
  $form['token_help'] = array(
    '#title' => t('Replacement patterns'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['token_help']['help'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('node'),
  );

  return $form;
}

/**
 * Theme function to display the form as a table.
 */
function theme_jobfaucet_mapping_form($variables) {

  $form = $variables['form'];
  $rows = array();

  foreach (element_children($form['required_mapping']) as $key) {
    $element = $form['required_mapping'][$key];

    // Allow rendering the label in its own table cell.
    $themable_field['element'] = $element;
    $form['required_mapping'][$key]['#title_display'] = 'invisible';

    // Allow rendering the description in its own table cell.
    $description = '';
    if (!empty($element['#description'])) {
      unset($form['required_mapping'][$key]['#description']);
      $description = '<div class="description">' . $element['#description'] . "</div>\n";
    }

    $row = array();
    $row[] = theme_form_element_label($themable_field);
    $row[] = drupal_render($form['required_mapping'][$key]);
    $row[] = array(
      'data' => $description,
      'class' => 'jf-field-description',
    );

    $rows[$key] = $row;
  }

  $header = array(
    t('For this field on job boards...'),
    t('...use this token or static value.'),
    t('Notes'),
  );

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Submit callback for mapping form.
 */
function jobfaucet_mapping_form_submit($form, &$form_state) {
  jobfaucet_set_mapping($form_state['values']['content_type'], $form_state['values']['required_mapping']);
  drupal_set_message(t('The mapping has been saved.'));
}

/**
 * Reset callback for mapping form.
 */
function jobfaucet_reset_mapping($form, &$form_state) {
  jobfaucet_set_default_mapping($form_state['values']['content_type']);
}

/**
 * Page callback for the admin form.
 */
function jobfaucet_admin_settings_form($form, &$form_state) {

  module_load_include('inc', 'jobfaucet', 'jobfaucet.targets');

  // Check account status.
  if (empty($form_state['input'])) {
    $auth_status = jobfaucet_auth_status();
    $auth_message_type = $auth_status['authenticated'] ? 'status' : 'error';

    drupal_set_message($auth_status['message'], $auth_message_type);
  }

  // Get content type info.
  $content_types = field_info_bundles('node');
  $enabled_types = variable_get('jobfaucet_content_type', array('jobfaucet_job'));
  $available_types = array();

  foreach ($content_types as $type => $type_info) {
    $available_types[$type] = $type_info['label'];

    // If this content type is enabled, append a link to the relevant mapping
    // page to it's label.
    if (in_array($type, $enabled_types)) {
      $available_types[$type] .= ' - '
        . l(t('Configure mapping'), 'admin/structure/types/manage/' . $type . '/jobfaucet-mapping');
    }
  }

  // If there are currently any remote jobs associated with this site, users
  // are not allowed to change API keys.
  $remote_job_count = db_query("SELECT remote_id FROM {jobfaucet_jobs}")->rowCount();

  if ($remote_job_count > 0) {
    $auth_description = t('You may not update API keys because your account currently has actively syndicated jobs.');
    $auth_disabled = TRUE;
  }
  else {
    $auth_description = t('To use JobFaucet, enter your JobFaucet public and private keys below.
      If you don\'t yet have an account, you can create one at <a href="http://jobfaucet.com">JobFaucet.com</a>.');
    $auth_disabled = FALSE;
  }

  $form['authentication'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authentication'),
    '#collapsible' => TRUE,
    '#description' => $auth_description,
    // Only show key configuration fields if they are not configured or invalid.
    '#collapsed' => isset($auth_status['authenticated']) ? $auth_status['authenticated'] : FALSE,
    '#disabled' => $auth_disabled,
  );
  $form['authentication']['jobfaucet_public_key'] = array(
    '#title' => t('Public Key'),
    '#type' => 'textfield',
    '#default_value' => variable_get('jobfaucet_public_key'),
  );
  $form['authentication']['jobfaucet_private_key'] = array(
    '#title' => t('Private Key'),
    '#type' => 'textfield',
    '#default_value' => variable_get('jobfaucet_private_key'),
  );

  $form['jobfaucet_content_type'] = array(
    '#title' => t('Job Content Types'),
    '#type' => 'checkboxes',
    '#description' => t('The content types you are using for jobs.  If you select types other than
      <em>JobFaucet Job</em>, you\'ll need to configure the mapping.'),
    '#required' => TRUE,
    '#options' => $available_types,
    '#default_value' => $enabled_types,
  );

  // We also set a warning if jobs aren't being syndicated.
  // See jobfaucet_requirements().
  $form['jobfaucet_syndication_status'] = array(
    '#title' => t('Syndicate Jobs'),
    '#description' => t('Turn syndication on to start syndicating jobs.  This should only be done on your
      production site.  You can also use JobFaucet in "Test Mode", which will allow you to test communication
      with the JobFaucet service without actually posting your jobs.'),
    '#type' => 'radios',
    '#options' => array(
      JOBFAUCET_SYNDICATION_ON => t('Syndication On'),
      JOBFAUCET_SYNDICATION_OFF => t('Syndication Off'),
      JOBFAUCET_TEST_MODE => t('Test Mode'),
    ),
    '#default_value' => variable_get('jobfaucet_syndication_status', JOBFAUCET_SYNDICATION_OFF),
  );

  $form['jobfaucet_feeds_selected'] = array(
    '#title' => t('Syndicate jobs to the following services'),
    '#description' => '',
    '#type' => 'checkboxes',
    '#options' => jobfaucet_get_services(),
    '#default_value' => variable_get('jobfaucet_feeds_selected', array_keys(jobfaucet_get_services())),
  );

  $form = system_settings_form($form);
  array_unshift($form['#submit'], 'jobfaucet_admin_settings_form_submit');

  return $form;
}

/**
 * Submit handler for the admin form.
 */
function jobfaucet_admin_settings_form_submit($form, &$form_state) {

  // Clean up the content type and selected feeds arrays.
  $form_state['values']['jobfaucet_content_type']
    = array_keys(array_filter($form_state['values']['jobfaucet_content_type']));
  $form_state['values']['jobfaucet_feeds_selected']
    = array_keys(array_filter($form_state['values']['jobfaucet_feeds_selected']));

  // Set up default values if an enabled content type has no mapping associated
  // with it.
  foreach ($form_state['values']['jobfaucet_content_type'] as $content_type) {
    $mapping = jobfaucet_get_mapping($content_type);
    if (empty($mapping)) {
      jobfaucet_set_default_mapping($content_type);
    }
  }
}
