<?php
/**
 * @file
 * Administration-related functions for Addemar Subscription module.
 */

/**
 * Administration page form callback for the Addemar subscription module.
 *
 * @return array
 *   The admin form array.
 *
 * @ingroup forms
 */
function addemar_subscription_admin($form, &$form_state) {
  $form = array();

  if (!extension_loaded('soap')) {
    drupal_set_message(t('SOAP has to be enabled to use this module.'), 'error');
    $form['error'] = array(
      '#markup' => t('Please enable SOAP in order to configure this module.'),
    );
  }
  else {
    drupal_add_css(drupal_get_path('module', 'addemar_subscription') . '/assets/styles/addemar_subscription.css');

    $form['addemar_subscription'] = array(
      '#type' => 'vertical_tabs',
    );

    $form['addemar_subscription_soap'] = array(
      '#type'         => 'fieldset',
      '#title'        => t('SOAP settings'),
      '#collapsible'  => TRUE,
      '#collapsed'    => FALSE,
      '#group'        => 'addemar_subscription',
    );
    $form['addemar_subscription_soap']['addemar_subscription_soap_wsdl'] = array(
      '#type'             => 'textfield',
      '#title'            => t('URL for SOAP wsdl'),
      '#default_value'    => variable_get('addemar_subscription_soap_wsdl', ADDEMAR_SUBSCRIPTION_SOAP_WSDL),
      '#description'      => t('Enter the URL for the Addemar SOAP WSDL (e.g: https://ws-mailing.presscenter.org/soap/wsdl/?token=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx&version=x.x)'),
      '#required'         => TRUE,
      '#maxlength'        => 255,
    );
    $form['addemar_subscription_settings'] = array(
      '#type'         => 'fieldset',
      '#title'        => t('General Settings'),
      '#collapsible'  => TRUE,
      '#collapsed'    => FALSE,
      '#group'        => 'addemar_subscription',
    );

    // Add a wrapper for the groups and add more button.
    $form['addemar_subscription_settings']['groups_wrapper'] = array(
      '#tree' => FALSE,
      '#weight' => -4,
      '#prefix' => '<div class="clearfix" id="addemar-group-wrapper">',
      '#suffix' => '</div>',
    );

    // Container for just the groups and their descriptions.
    $form['addemar_subscription_settings']['groups_wrapper']['addemar_subscription_groups'] = array(
      '#type' => 'item',
      '#prefix' => '<div id="addemar-groups">',
      '#title'  => t('Available newsletter options'),
      '#description' => t('The above are the newsletters that will be available as a selectable option in the newsletter subscription form. To remove an option, remove the description and unselect all contact groups for that option.'),
      '#suffix' => '</div>',
    );

    $groups = variable_get('addemar_subscription_groups', array());

    if (empty($form_state['group_count'])) {
      $form_state['group_count'] = count($groups);
    }
    $group_count = $form_state['group_count'];

    $form['addemar_subscription_settings']['groups_wrapper']['addemar_subscription_settings_group_count'] = array(
      '#type' => 'hidden',
      '#value' => $group_count,
    );

    $delta = $group_count - count($groups);
    $i = 1;

    // Display existing groups.
    foreach ($groups as $group) {
      $key = 'addemar_subscription_group:' . $i;
      $form['addemar_subscription_settings']['groups_wrapper']['addemar_subscription_groups'][$key] = _addemar_subscription_add_more_newsletter_form($key, $i, $group['group_id'], $group['description']);
      $i++;
    }

    // Display initial group ($groups is empty) or add a new one (delta).
    if (empty($groups) || $delta) {

      for ($delta; $delta > 0 || $i == 1; $delta--) {
        $key = 'addemar_subscription_group:' . $i;
        $form['addemar_subscription_settings']['groups_wrapper']['addemar_subscription_groups'][$key] = _addemar_subscription_add_more_newsletter_form($key, $i);
        $i++;
      }
    }

    $form['addemar_subscription_settings']['groups_wrapper']['addemar_subscription_add_more'] = array(
      '#type' => 'submit',
      '#value' => t('Add more'),
      '#attributes' => array(
        'title' => t("If the amount of boxes above isn't enough, click here to add more choices."),
      ),
      '#weight' => 1,
      '#limit_validation_errors' => array(array('addemar_subscription_groups')),
      '#submit' => array('addemar_subscription_add_more_submit'),
      '#ajax' => array(
        'callback' => '_addemar_subscription_add_more_newsletter',
        'wrapper' => 'addemar-groups',
        'effect' => 'fade',
      ),
    );

    $form['addemar_subscription_settings']['addemar_subscription_language_code'] = array(
        '#type'             => 'textfield',
        '#title'            => t('Language field code'),
        '#default_value'    => variable_get('addemar_subscription_language_code', ADDEMAR_SUBSCRIPTION_LANGUAGE_CODE),
        '#description'      => t('Enter the code of the field used to store the contact\'s language (without <code>&lt;? ?&gt;</code>). If you do not need to store the contact\'s language, you can leave this empty.'),
        '#required'         => FALSE,
    );
    $form['addemar_subscription_settings']['addemar_subscription_source_message'] = array(
      '#type'             => 'textfield',
      '#title'            => t('Source'),
      '#default_value'    => variable_get('addemar_subscription_source_message', ADDEMAR_SUBSCRIPTION_SOURCE_MESSAGE),
      '#description'      => t('Insert the source you want to mention with all contacts as a value.<br/> This source will be visible in the history of every contact subscribed via this form.'),
      '#required'         => FALSE,
    );
    $form['addemar_subscription_settings']['addemar_subscription_subscription_type'] = array(
      '#type'             => 'radios',
      '#title'            => t('Type of subscription'),
      '#options'          => array(
        0 => t('Simple'),
        1 => t('Confirmed'),
      ),
      '#default_value'    => variable_get('addemar_subscription_subscription_type', ADDEMAR_SUBSCRIPTION_SUBSCRIPTION_TYPE),
      '#description'      => t('Insert the type of subscription scenario you want.<br/>
        You can set the value to: <br/>
        Simple No e-mail will be sent. Only the confirmation page you set in "ret" will be shown.<br/>
        Confirmed The subscriber will receive a message confirming he has subscribed.'),
      '#required'         => TRUE,
    );
    $form['addemar_subscription_form'] = array(
      '#type'         => 'fieldset',
      '#title'        => t('Form Settings'),
      '#collapsible'  => TRUE,
      '#collapsed'    => FALSE,
      '#group'        => 'addemar_subscription',
    );

    $options = variable_get_value('addemar_subscription_addemar_fields', array('default' => ''));
    $example = '';

    if ($s_client = _addemar_subscription_get_soap_client()) {
      try {
        $language_code = variable_get('addemar_subscription_language_code', ADDEMAR_SUBSCRIPTION_LANGUAGE_CODE);
        $fields = $s_client->getContactStructure();
        $default_language = language_default('language');

        foreach ($fields->fields as $field) {
          // Do not show language as a text field
          if ($field->name == $language_code) {
            $form_state['language_code_exists'] = TRUE;
            continue;
          }
          // Get the field id's to get labels for.
          $field_ids = $s_client->getFieldId($field->name);
          // This gets all results by that name, assume the first result is
          // the right one.
          if (isset($field_ids[0])) {
            $labels = $s_client->getFieldLabels($field_ids[0]);
            if (!empty($labels)) {
              foreach ($labels as $label) {
                if (substr($label->name, 0, 2) === $default_language && !empty($label->value)) {
                  $field_label = $label->value;
                  break;
                }
              }
            }
          }
          if (!isset($field_label)) {
            $field_label = str_replace(' ', '_', ucfirst($field->name));
          }
          $example .= $field->name . '|' . $field_label . "\n";
          unset($field_label);
        }
        $example = '<br/><br/>' . t('The following is a live example from the current Addemar subscription WSDL:') . '<blockquote><em>' . nl2br($example) . '</em></blockquote></p>' . t('This example can be copy-pasted into the above text box as is.');
      }
      catch (SoapFault $e) {
        $msg = t('The Addemar subscription WSDL is not properly configured. If you are using https, check that php_openssl is enabled.');
        drupal_set_message(check_plain($msg), 'error');
        watchdog('error', $msg);
      }
    }

    $form['addemar_subscription_form']['addemar_subscription_addemar_fields'] = array(
      '#type'             => 'textarea',
      '#title'            => t('Addemar Fields'),
      '#default_value'    => $options,
      '#description'      => t('List all fields declared in Addemar here. Enter one value per line, in the format <em>key|label|required</em>.<br/><br/>The <strong>key</strong> is the field code defined in Addemar without <code>&lt;? ?&gt;</code>. The <strong>label</strong> will be a translatable %language string which is displayed in front of the field. The <strong>required</strong> is optional and will be used to mark form fields as required/mandatory. e.g.: <em>email|Email|required</em> or <em>name|Name</em>', array('%language' => language_default('name'))) . $example,
      '#required'         => FALSE,
    );
    $form['addemar_subscription_form']['addemar_subscription_page_url'] = array(
      '#type'             => 'textfield',
      '#title'            => t('Page URL'),
      '#default_value'    => variable_get('addemar_subscription_page_url', ADDEMAR_SUBSCRIPTION_PAGE_URL),
      '#description'      => t('Enter the URL of the page where the form must be displayed.'),
      '#required'         => TRUE,
    );
    $form['addemar_subscription_form']['addemar_subscription_submit_button'] = array(
      '#type'             => 'textfield',
      '#title'            => t('Submit value'),
      '#default_value'    => variable_get_value('addemar_subscription_submit_button'),
      '#description'      => t('Enter the text value that should appear on the submit button.'),
      '#required'         => TRUE,
    );

    $form['addemar_subscription_message'] = array(
      '#type'         => 'fieldset',
      '#title'        => t('Message Settings'),
      '#collapsible'  => TRUE,
      '#collapsed'    => FALSE,
      '#group'        => 'addemar_subscription',
    );
    $form['addemar_subscription_message']['addemar_subscription_message_success'] = array(
      '#type'             => 'textfield',
      '#title'            => t('Success'),
      '#default_value'    => variable_get_value('addemar_subscription_message_success'),
      '#description'      => t('Enter the message that will be displayed if everything went well.'),
      '#required'         => TRUE,
    );
    $form['addemar_subscription_message']['addemar_subscription_message_error'] = array(
      '#type'             => 'textfield',
      '#title'            => t('Error'),
      '#default_value'    => variable_get_value('addemar_subscription_message_error'),
      '#description'      => t('Enter the message that will be displayed if an error is returned by Addemar.'),
      '#required'         => TRUE,
    );

    $form['#validate'][] = 'addemar_subscription_admin_validate';
    $form['#submit'][] = 'addemar_subscription_admin_submit';

    return system_settings_form($form);
  }

  return $form;
}

/**
 * Custom form validation to set the error when a SoapFault is catched.
 *
 * @see addemar_subscription_admin_submit()
 */
function addemar_subscription_admin_validate($form, &$form_state) {
  $soap_wsdl = $form_state['values']['addemar_subscription_soap_wsdl'];

  $language_code = $form_state['values']['addemar_subscription_language_code'];

  if ($s_client = _addemar_subscription_get_soap_client($soap_wsdl)) {
    $language_code_exists = FALSE;
    $fields = $s_client->getContactStructure();
    foreach ($fields->fields as $field) {
      if ($field->name == $language_code) {
        $language_code_exists = TRUE;
      }
    }
    if (!$language_code_exists) {
      form_set_error('addemar_subscription_language_code', t('The field code <code>%language_code</code> does not currently exist in Addemar. Please enter the name of an existing language field so that a language can be saved for users submitting the newsletter form. To see the existing fields, log in to Addemar and go to Segment &gt; Fields.', array('%language_code' => check_plain($language_code))));
    }
  }
  else {
    // The message is already set by _addemar_subscription_get_soap_client.
    form_set_error('addemar_subscription_soap_wsdl');
  }
}

/**
 * Custom submit function
 *
 * @see addemar_subscription_admin_validate().
 */
function addemar_subscription_admin_submit($form, &$form_state) {

  // Do not submit empty group id.
  foreach ($form_state['values']['addemar_subscription_groups'] as $key => $values) {

    if (empty($values['group_id']) || empty($values['description'])) {
      unset($form_state['values']['addemar_subscription_groups'][$key]);
    }
  }

  // Flush menu cache if the path has been changed.
  if (variable_get('addemar_subscription_page_url', '') != $form_state['values']['addemar_subscription_page_url']) {
    // Set the variable prior to flushing the cache otherwise, the path is still
    // unknown.
    variable_set('addemar_subscription_page_url', $form_state['values']['addemar_subscription_page_url']);
    menu_rebuild();
  }
}

/**
 * Custom submit function for the add more button in order to rebuild the form.
 */
function addemar_subscription_add_more_submit($form, &$form_state) {
  $form_state['group_count']++;
  $form_state['rebuild'] = TRUE;
}
