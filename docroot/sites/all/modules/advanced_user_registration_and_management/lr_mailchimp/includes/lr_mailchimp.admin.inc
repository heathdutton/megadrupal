<?php
/**
 * @file
 * Admin page callback file for the lr_mailchimp module.
 */
module_load_include('inc', 'lr_social_login', 'includes/lr_social_login.admin');
/**
 * Function that build an abstract representation of a HTML form for Mailchimp settings.
 *
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form.
 * @return array A renderable form array for the respective request.
 */
function lr_mailchimp_settings_form($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'lr_mailchimp') . '/css/lr_mailchimp.css');
  $mailchimp_group = 'mailchimp_integration';
  $form[$mailchimp_group] = array(
    '#type' => 'fieldset',
    '#title' => t('MAILCHIMP INTEGRATION'),
    '#group' => 'settings',
  );
  $form[$mailchimp_group]['lr_mailchimp_enable'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want to automatically subscribe users to mailchimp List when they register through social login?'),
    '#prefix' => '<div id="mailchimp-enable-error"></div>',
    '#default_value' => variable_get('lr_mailchimp_enable', 0),
    '#options' => array(
      1 => t('YES'),
      0 => t('NO'),
    ),
  );
  $form[$mailchimp_group]['lr_mailchimp_apikey'] = array(
    '#type' => 'textfield',
    '#title' => t('MailChimp API Key'),
    '#prefix' => '<div id="mailchimp-api-error"></div>',
    '#default_value' => variable_get('lr_mailchimp_apikey', ''),
    '#description' => t('Enter your Mailchimp API Key (After entering your Mailchimp API Key, hit the "Save" button below).'),
  );
  $form[$mailchimp_group]['mailchimp_save'] = array(
    '#type' => 'button',
    '#id' => 'edit-mailchimp-save',
    '#value' => t('Save'),
    '#ajax' => array(
      'callback' => 'lr_mailchimp_save',
      'wrapper' => 'mailchimp-display-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  $api_key = variable_get('lr_mailchimp_apikey', '');
  $value = (!empty($form_state['values']['lr_mailchimp_apikey']) ? trim($form_state['values']['lr_mailchimp_apikey']) : (!empty($api_key) ? trim($api_key) : ''));
  $mailchimp_list_result = advance_get_mailchimp_list($value);
  $mailchimp_list = !empty($mailchimp_list_result) ? $mailchimp_list_result : array('0' => '--Select--');
  $form[$mailchimp_group]['lr_mailchimp_list'] = array(
    '#type' => 'select',
    '#title' => t('MailChimp List'),
    '#options' => $mailchimp_list,
    '#ajax' => array(
      'callback' => 'lr_mailchimp_savefields',
      'wrapper' => 'mailchimp-fields-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#default_value' => variable_get('lr_mailchimp_list', ''),
    '#prefix' => '<div id="mailchimp-display-div">',
    '#suffix' => '</div>',
    '#description' => t('Select the list, you want to subscribe users to.'),
  );
  $value_list = variable_get('lr_mailchimp_list');

  if (!empty($value) && !empty($value_list)) {
    $option_mailchimp_merge_var = lr_mailchimp_merge_vars($value, $value_list);
  }
  else {
    $option_mailchimp_merge_var = array('0' => '--Select--');
  }

  if (count($option_mailchimp_merge_var) > 1) {

    $mappingfields = lr_mailchimp_mapping_fields();
    $form['mailchimp_integration']['mapping_fields'] = array(
      '#type' => 'textbox',
      '#prefix' => '<div><strong>MailChimp Mapping fields</strong></div><div id="mailchimp-fields-div">',
      '#suffix' => '</div><div class="description">Map your Mailchimp list merge fields to the social login profile data</div>',
    );

    foreach ($option_mailchimp_merge_var as $option_mailchimp_merge) {
      $option_mailchimp_mapping_field = 'mailchimp_merge_var_' . $option_mailchimp_merge['tag'];

      if ($option_mailchimp_mapping_field != 'mailchimp_merge_var_EMAIL') {
        $form['mailchimp_integration']['mapping_fields'][$option_mailchimp_mapping_field] = array(
          '#type' => 'select',
          '#title' => check_plain(t($option_mailchimp_merge['name'])),
          '#options' => $mappingfields,
          '#default_value' => variable_get($option_mailchimp_mapping_field, ''),
        );
      }
    }
  }
  else {
    $form[$mailchimp_group]['lr_mailchimp_mappingfields'] = array(
      '#type' => 'select',
      '#options' => !empty($option_mailchimp_merge_var) ? $option_mailchimp_merge_var : array('0' => '--Select--'),
      '#prefix' => '<div><strong>MailChimp mapping fields</strong></div><div id="mailchimp-fields-div">',
      '#suffix' => '</div><div class="description">Map your Mailchimp list merge fields to the social login profile data</div>',
      '#default_value' => variable_get('lr_mailchimp_mappingfields', 0),
    );
  }
  $form['#submit'][] = 'lr_mailchimp_settings_form_submit';
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Submit mailchimp settings.
 *
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form
 * @return bool
 */
function lr_mailchimp_settings_form_submit($form, &$form_state) {
  foreach ($form_state['input'] as $key => $value) {
    if (strpos($key, 'mailchimp_merge_var_') !== FALSE) {
      variable_set($key, $value);
    }
  }
  if (!lr_social_login_save_settings($form_state)) {
    return FALSE;
  }
}


/**
 * Get all mapping fields for mailchimp.
 *
 * @return array Collect all mapping field with extended field mapping
 */
function lr_mailchimp_mapping_fields() {
  $mappingfields = array(
    '0' => '--Select--',
    'User ID' => 'User ID',
    'Username' => 'Username',
  );
  $field_properities = lr_field_mapping_user_properties();
  $property_options = array();

  foreach ($field_properities as $property => $property_info) {
    if (isset($property_info['label'])) {
      $property_options[$property] = $property_info['label'];
    }
  }

  $property_options['lr_Addresses'] = 'Addresses';

  $mappingfields = array_merge($mappingfields, $property_options);
  //Advanced module LR Code Hook Start
  if (count(module_implements('lr_add_mapping_fields')) > 0) {
    // Call all modules that implement the hook, and let them make changes to $variables.
    $result = module_invoke_all('lr_add_mapping_fields', $mappingfields);
    if (is_array($result) && count($result) > 0) {
      $mappingfields = $result;
    }
  }
  //Advanced module LR Code Hook End
  return $mappingfields;
}

/**
 * Get mailchimp list from mailchimp account.
 *
 * @param type $mailchimpapikey Mailchimp api key that you got from your mailchimp account
 * @return array mailchimp list that stored in your mailchimp account if empty list then return empty string
 */
function advance_get_mailchimp_list($mailchimpapikey) {
  if (!empty($mailchimpapikey)) {
    if (!class_exists('MCAPI')) {
      require drupal_get_path('module', 'lr_mailchimp') . '/integrations/mailchimp/MCAPI.class.php';
    }

    $loginradiusmailchimp = new MCAPI($mailchimpapikey);

    if (!$loginradiusmailchimp->errorCode) {
      $names[] = '--Select--';
      $count = 0;

      do {

        $lists = $loginradiusmailchimp->lists(array(), $count * 100, 100);

        if (is_array($lists)) {
          foreach ($lists['data'] as $list) {
            $names[$list['id']] = $list['name'];
          }

          $count++;
        }
        else {
          return;
        }
      } while (count($lists['data']) > 0);

      return $names;
    }
  }
  else {
    $names = array('0' => '--Select--');
    return $names;
  }
}

/**
 * Get mailchimp merge vars from mailchimp account.
 *
 * @param string $mailchimpapikey mailchimp api key that you got from your mailchimp account
 * @param string $mailchimplistid Get mailchimp list id that strored in your mailchimp account
 * @return array Merge vars that attached with the mailchimp list
 */
function lr_mailchimp_merge_vars($mailchimpapikey, $mailchimplistid) {
  if (!class_exists('MCAPI')) {
    require drupal_get_path('module', 'lr_mailchimp') . '/integrations/mailchimp/MCAPI.class.php';
  }

  $loginradiusmailchimp = new MCAPI($mailchimpapikey);
  $mergevars = $loginradiusmailchimp->listMergeVars($mailchimplistid);

  if (!$loginradiusmailchimp->errorCode) {
    return $mergevars;
  }
}

/**
 * Handle Ajax callback when mailchimp api key save button clicked.
 *
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form.
 * @return array if mailchimp is valid or not empty then return mailchimp list else return error which is mailchimp is not valid
 */
function lr_mailchimp_save($form, &$form_state) {
  $mailchimpapikey = trim($form_state['values']['lr_mailchimp_apikey']);
  $mailchimpenable = trim($form_state['values']['lr_mailchimp_enable']);

  $mailchimp_list = array('0' => '--Select--');

  if (!empty($mailchimpapikey)) {
    $mailchimp_list = advance_get_mailchimp_list($mailchimpapikey);
  }
  if (empty($mailchimpenable)) {
    drupal_set_message(t('Please enable Mailchimp first'), 'error');
    $commands = array();
    $commands[] = ajax_command_replace('#mailchimp-enable-error', '<div id="mailchimp-enable-error">' . theme('status_messages') . '</div>');
    return array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
  } // If everything is fine show lists in a dropdown.
  elseif (empty($mailchimpapikey) || empty($mailchimp_list)) {
    drupal_set_message(t('Please enter a valid Mailchimp API Key'), 'error');
    $commands = array();
    $commands[] = ajax_command_replace('#mailchimp-api-error', '<div id="mailchimp-api-error">' . theme('status_messages') . '</div>');
    return array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
  }
  else {
    $form['lr_mailchimp_list'] = array(
      '#type' => 'select',
      '#id' => 'edit-lr-mailchimp-list',
      '#name' => 'lr_mailchimp_list',
      '#title' => t('MailChimp List'),
      '#options' => $mailchimp_list,
      '#default_value' => variable_get('lr_mailchimp_list', ''),
      '#ajax' => array(
        'callback' => 'lr_mailchimp_savefields',
        'wrapper' => 'mailchimp-fields-div',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#attributes' => array('onclick' => 'jQuery("#mailchimp-api-error, #mailchimp-enable-error").hide();'),
      '#description' => t('Select the list, you want to subscribe users to.'),
    );
    $form_state['rebulid'] = TRUE;
    return $form['lr_mailchimp_list'];
  }
}

/**
 * Handle Ajax callback for mailchimp list.
 *
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form.
 * @return array mapping field woth merge vars in mailchimp integration
 */
function lr_mailchimp_savefields($form, &$form_state) {
  $mailchimpapikey = trim($form_state['values']['lr_mailchimp_apikey']);
  $mailchimplistid = trim($form_state['values']['lr_mailchimp_list']);
  $option_mailchimp_merge_var = lr_mailchimp_merge_vars($mailchimpapikey, $mailchimplistid);

  if (count($option_mailchimp_merge_var) > 1) {
    $form['mailchimp_integration']['mapping_fields'] = array(
      '#type' => 'textbox',
      '#prefix' => '<div id="mailchimp-fields-div">',
      '#suffix' => '</div>',
    );

    $mappingfields = lr_mailchimp_mapping_fields();

    foreach ($option_mailchimp_merge_var as $option_mailchimp_merge) {
      if ($option_mailchimp_merge['tag'] != 'EMAIL') {
        $option_mailchimp_mapping_field = 'mailchimp_merge_var_' . $option_mailchimp_merge['tag'];
        $form['mailchimp_integration']['mapping_fields'][$option_mailchimp_mapping_field] = array(
          '#type' => 'select',
          '#id' => 'edit-' . strtolower(str_replace('_', '-', $option_mailchimp_mapping_field)),
          '#name' => $option_mailchimp_mapping_field,
          '#title' => t($option_mailchimp_merge['name']),
          '#options' => $mappingfields,
        );

        $form['mailchimp_integration']['mapping_fields'][$option_mailchimp_mapping_field]['#value'] = variable_get($option_mailchimp_mapping_field);
      }
    }
    return $form['mailchimp_integration']['mapping_fields'];
  }
}
