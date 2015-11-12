<?php
/**
 * @file
 * Contains a custom form for Admin
 */

/**
 * Implements a custom form for admin.
 */
function webform_confirmation_form() {

  $query = new EntityFieldQuery();
  $entities = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'webform')
    ->propertyCondition('status', 1)
    ->execute();
  $nodes = entity_load('node', array_keys($entities['node']));
  foreach ($nodes as $key => $node) {
    $options['webform_client_form_' . $key] = $node->title;
  }
  $form['webform_confirmation_config'] = array(
    '#type' => 'select',
    '#title' => t('Select the webform for which we want the email configuration'),
    '#default_value' => variable_get('webform_confirmation_config'),
    '#multiple' => TRUE,
    '#options' => $options,
  );
  $form['webform_confirmation_redirect'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('webform_confirmation_redirect', ''),
    '#title' => t('Enter the redirection link'),
    '#description' => t('Redirection link something like <strong>redirect/test</strong> if you wish the user to go to the link <strong>example.com/redirect/test</strong>.'),
  );
  $form['webform_confirmation_no_verfication_link'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('webform_confirmation_no_verfication_link', 0),
    '#title' => t('Donot send a link in mail'),
    '#description' => t('Selecting this option will only send a welcome mail without confirmation link.'),
  );
  $form['webform_confirmation_shorten_url'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('webform_confirmation_shorten_url', 0),
    '#title' => t('Shorten the URL'),
    '#description' => t('Please check this if you want to shorten the url being sent. For username and api key visit !url.',
        array('!url' => l(t('Bitly Site'), 'https://bitly.com'))),
    '#states' => array(
      'checked' => array(
        ':input[name="webform_confirmation_login"]' => array('filled' => TRUE),
        ':input[name="webform_confirmation_api_key"]' => array('filled' => TRUE),
      ),
    ),
  );
  $form['webform_confirmation_bit_ly_information'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bitly Information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#states' => array(
      'expanded' => array(
        ':input[name="webform_confirmation_shorten_url"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['webform_confirmation_bit_ly_information']['webform_confirmation_login'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('webform_confirmation_login', ''),
    '#title' => t('Enter the bitly Username'),
    '#description' => t('Please enter your bitly username from !url.',
        array('!url' => l(t('here'), 'https://bitly.com/a/your_api_key'))),
  );
  $form['webform_confirmation_bit_ly_information']['webform_confirmation_api_key'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('webform_confirmation_api_key', ''),
    '#title' => t('Enter the bitly Api Key'),
    '#description' => t('Please enter your bitly Api Key from !url.',
        array('!url' => l(t('here'), 'https://bitly.com/a/your_api_key'))),
  );
  return system_settings_form($form);
}

/**
 * Implements a template for the content of mail.
 */
function webform_confirmation_form_template() {
  $form = array();
  $form['webform_confirmation_email_address'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('webform_confirmation_email_address', ''),
    '#title' => t('Enter the Email Address'),
    '#description' => t('Please enter the email Address from which you want the mail to go from'),
  );
  $form['webform_confirmation_subject_name'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('webform_confirmation_subject_name', ''),
    '#title' => t('Enter the Subject of the mail'),
    '#description' => t('Please enter the subject of the mail'),
  );
  $form['webform_confirmation_body_start'] = array(
    '#type' => 'textarea',
    '#default_value' => variable_get('webform_confirmation_body_start', ''),
    '#title' => t('Enter the starting body'),
    '#description' => t('Please enter the starting body of the mail'),
  );
  $form['webform_confirmation_body_end'] = array(
    '#type' => 'textarea',
    '#default_value' => variable_get('webform_confirmation_body_end', ''),
    '#title' => t('Enter the ending body'),
    '#description' => t('Please enter the ending body of the mail'),
  );
  return system_settings_form($form);
}

/**
 * Implementing a custom search page.
 */
function webform_confirmation_search() {
  $view = views_get_view('list_of_submissions');
  $view->set_display('list_of_submissions-block');
  $view->pre_execute();
  $view->execute();
  return $view->render();
}
