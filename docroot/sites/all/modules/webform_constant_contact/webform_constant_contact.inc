<?php
// $Id$

/**
 * @file: Webform components to integrate the Constant Contact module and Webform
 * So you can subscribe to a Constant Contact mailing list using a Webform-generated form
 */

/**
 * Create a default cc_email component.
 */
function _webform_defaults_cc_email() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'user_email' => 1,
    'extra' => array(
      'options' => '',
      'questions' => '',
      'optrand' => 0,
      'qrand' => 0,
      'description' => '',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_cc_email($component) {
  if (!module_exists('constant_contact')) {
    $link = l(t('"Constant Contact" module'), 'http://drupal.org/project/constant_contact', array('attributes' => array('target' => '_blank')));
    form_set_error('', t('This form uses the "Constant Contact Email" webform component which requires the !link to be installed and enabled!', array('!link' => $link)));
  }

  // get the constant contact contact lists
  $lists = array();

  // if we have an object get the users contact lists
  $cc = constant_contact_create_object();

  if (is_object($cc)) :
    $lists = constant_contact_get_lists($cc);
  endif;

  $form['user_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Prefill Email Field if User Logged in'),
    '#default_value' => $component['value'] == '%useremail' ? 1 : 0,
    '#description' => t('If a user is logged in the email field will be prefilled with their email address.'),
    '#weight' => 0,
    '#element_validate' => array('_webform_edit_cc_email_validate'),
  );
  $form['extra']['action'] = array(
    '#type' => 'radios',
    '#title' => t('Form Action'),
    '#default_value' => trim($component['extra']['action']) == '' ? 'subscribe' : $component['extra']['action'],
    '#description' => t('Choose the action to be taken on submit.'),
    '#required' => TRUE,
    '#options' => array(
      'subscribe' => t('Subscription'),
      'unsubscribe' => t('Unsubscription'),
    ),
  );
  $form['extra']['field_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Size of the Email Field'),
    '#default_value' => isset($component['extra']['field_size']) ? $component['extra']['field_size'] : '',
    '#description' => t('Specify the size of the email field, numbers only.'),
  );

  $form['extra']['subscribe_checkbox'] = array(
    '#type' => 'radios',
    '#title' => t('Display Explicit Subscribe Checkbox'),
    '#default_value' => trim($component['extra']['subscribe_checkbox']) == '' ? 1 : $component['extra']['subscribe_checkbox'],
    '#description' => t('If you would like to display a subscribe checkbox set this option to yes. if you do not display a subscribe checkbox users will have to select a contact list to subscribe, if your not displaying contact lists then whether or not the email field is required will determine if the user is subscribed, i.e. if email field is not required and user enters no email their not subscribed and if they do enter one they are subscribed.'),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    ),
  );
  $form['extra']['subscribe_checkbox_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Subscribe Checkbox Title'),
    '#default_value' => isset($component['extra']['subscribe_checkbox_title']) ? $component['extra']['subscribe_checkbox_title'] : '',
    '#description' => t('A title for the subscribe checkbox.'),
  );
  $form['extra']['default_optin'] = array(
    '#type' => 'radios',
    '#title' => t('Optin Users by Default'),
    '#default_value' => trim($component['extra']['default_optin']) == '' ? 1 : $component['extra']['default_optin'],
    '#description' => t('This determines if the subscribe checkbox is checked by default.'),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    ),
  );
  $form['extra']['show_contact_list_selection'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show a Contact List Selection to Users'),
    '#default_value' => isset($component['extra']['show_contact_list_selection']) ? $component['extra']['show_contact_list_selection'] : '',
    '#description' => t('If this box is checked a contact list selection will be shown to users.'),
  );
  $form['extra']['contact_list_selection_mandatory'] = array(
    '#type' => 'checkbox',
    '#title' => t('Contact List Selection Mandatory'),
    '#default_value' => isset($component['extra']['contact_list_selection_mandatory']) ? $component['extra']['contact_list_selection_mandatory'] : '',
    '#description' => t('Check this option if the user must choose at least one contact list.'),
  );
  $form['extra']['list_selection_format'] = array(
    '#type' => 'radios',
    '#title' => t('Contact List Selection Format'),
    '#default_value' => trim($component['extra']['list_selection_format']) == '' ? 'checkboxes' : $component['extra']['list_selection_format'],
    '#description' => t('Choose what format of contact list selection the user will be presented with.'),
    '#required' => TRUE,
    '#options' => array(
      'checkboxes' => t('Checkboxes'),
      'select' => t('Multi-Select List'),
    ),
  );
  $form['extra']['contact_list_selection_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Contact List Selection Title'),
    '#default_value' => isset($component['extra']['contact_list_selection_title']) ? $component['extra']['contact_list_selection_title'] : '',
    '#description' => t('An optional title for the contact list selection field, this is required if setting the "Contact list selection mandatory" option on. Set to a blank string to remove it.'),
  );
  $form['extra']['lists'] = array(
    '#type' => 'select',
    '#title' => t("Contact Lists"),
    '#default_value' => is_array($component['extra']['lists']) ? $component['extra']['lists'] : array(),
    '#description' => t('If you show the contact list selection, choose which contact lists will be available to select, if you do not show the contact list selection choose which lists their automatically added to.<br />If you do not select any lists here all lists will be used for both situations.'),
    '#options' => $lists,
    '#multiple' => TRUE,
    '#size' => 10,
  );
  return $form;
}

/**
 * Validation function for the email edit form.
 */
function _webform_edit_cc_email_validate($element, &$form_state) {
  if ($form_state['values']['user_email']) {
    $form_state['values']['value'] = '%useremail';
  }
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_cc_email($component, $value = NULL, $filter = TRUE) {
  if ($value[0]) {
    $default_value = $value[0];
  }
  else {
    $default_value = _webform_filter_values($component['value']);
  }
  $element = array(
    '#type' => 'webform_cc_email',
    '#weight' => $component['weight'],
    '#prefix' => '<div class="webform-component-' . $component['type'] . '" id="webform-component-' . $component['form_key'] . '">',
    '#suffix' => '</div>',
    '#webform_component' => $component,
    '#theme' => 'webform_cc_email',
    '#element_validate' => array('_webform_validate_cc_email'),
  );
  $element['cc_email_address'] = array(
    '#type' => 'textfield',
    '#title' => htmlspecialchars($component['name'], ENT_QUOTES),
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#default_value' => $default_value,
    '#required' => isset($component['required']) ? $component['required'] : $component['mandatory'],
    '#field_prefix' => empty($component['extra']['field_prefix']) ? NULL : check_plain($component['extra']['field_prefix']),
    '#field_suffix' => empty($component['extra']['field_suffix']) ? NULL : check_plain($component['extra']['field_suffix']),
    '#description' => htmlspecialchars($component['extra']['description'], ENT_QUOTES),
    '#attributes' => empty($component['extra']['attributes']) ? NULL : $component['extra']['attributes'],
    '#theme_wrappers' => array('webform_element'),
    '#webform_component' => array('type' => 'webform_cc_email'),
  );

  if (isset($component['extra']['attributes'])) :
    $element['cc_email_address']['#attributes'] = $component['extra']['attributes'];
  endif;

  if (isset($component['extra']['field_size'])) {
    if (trim($component['extra']['field_size']) != '' AND is_numeric($component['extra']['field_size'])) :
      $element['cc_email_address']['#size'] = $component['extra']['field_size'];
    endif;
  }

  // build the contact lists based on the options they have selected    // get the constant contact contact lists
  $lists = array();

  // if we have an object get the users contact lists
  $cc = constant_contact_create_object();

  if (is_object($cc)) :
    $lists = constant_contact_get_lists($cc);
  endif;
  $selected_lists = (is_array($component['extra']['lists'])) ? $component['extra']['lists'] : array();

  if ($component['extra']['action'] == 'subscribe' AND $component['extra']['subscribe_checkbox']) :
    $element['subscribe'] = array(
      '#title' => (!empty($component['extra']['subscribe_checkbox_title'])) ? $component['extra']['subscribe_checkbox_title'] : '',
      '#type' => 'checkbox',
      '#default_value' => $component['extra']['default_optin'],
    );
  endif;

  if ($component['extra']['show_contact_list_selection']) {
    $options = array();

    if (count($selected_lists) > 0) {
      foreach ($selected_lists as $list) {
        if ($list > 0 AND isset($lists[$list])) {
          $options[$list] = $lists[$list];
        }
      }

      if (!count($options)) {
        $options = $lists;
      }

      $element['list_selection'] = array(
        '#title' => (!empty($component['extra']['contact_list_selection_title'])) ? $component['extra']['contact_list_selection_title'] : '',
        '#prefix' => '',
        '#type' => $component['extra']['list_selection_format'],
        '#default_value' => array(),
        '#options' => $options,
        '#required' => $component['extra']['contact_list_selection_mandatory'],
      );

      if ($component['extra']['list_selection_format'] == 'select') {
        $element['list_selection']['#multiple'] = TRUE;
        $element['list_selection']['#size'] = 10;
      }
    }
  }
  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_cc_email($component, $value, $format = 'html') {
  $values = array(
    array(
      '#title' => $component['name'],
      '#weight' => $component['weight'],
      '#theme' => 'webform_display_cc_email',
      '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
      '#post_render' => array('webform_element_wrapper'),
      '#webform_component' => $component,
      '#format' => $format,
      '#value' => isset($value[0]) ? $value[0] : '',
    ),
  );

  // add the action
  if (isset($value[1])) {
    $values[] = array(
      '#title' => 'Action',
      '#weight' => 998,
      '#theme' => 'webform_display_cc_email',
      '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
      '#post_render' => array('webform_element_wrapper'),
      '#webform_component' => $component,
      '#format' => $format,
      '#value' => isset($value[1]) ? $value[1] : '',
    );
  }

  // add the contact lists
  if (isset($value[2])) {
    $values[] = array(
      '#title' => 'Contact Lists',
      '#weight' => 999,
      '#theme' => 'webform_display_cc_email',
      '#theme_wrappers' => array('webform_element'),
      '#post_render' => array('webform_element_wrapper'),
      '#webform_component' => $component,
      '#format' => 'html',
      '#value' => isset($value[2]) ? implode(', ', array_values(unserialize($value[2]))) : '',
      '#parents' => array(), // Prevents error with theme_webform_element when it's building CSS classes.
    );
  }

  return $values;
}

/**
 * A Drupal Forms API Validation function. Validates the entered values from
 * email components on the client-side form.
 * @param $form_element
 *   The e-mail form element.
 * @param $form_state
 *   The full form state for the webform.
 * @return
 *   None. Calls a form_set_error if the e-mail is not valid.
 */
function _webform_validate_cc_email($form_element, $form_state) {
  if (!module_exists('constant_contact')) {
    $link = l(t('"Constant Contact" module'), 'http://drupal.org/project/constant_contact', array('attributes' => array('target' => '_blank')));
    form_set_error('', t('This form uses the "Constant Contact Email" webform component which requires the !link to be installed and enabled!', array('!link' => $link)));
  }

  $component = $form_element['#webform_component'];
  if (!empty($form_element['cc_email_address']['#value']) && !valid_email_address($form_element['cc_email_address']['#value'])) {
    form_error($form_element, t('%value is not a valid email address.', array('%value' => $form_element['cc_email_address']['#value'])));
  }
  elseif (empty($form_element['cc_email_address']['#value']) && $form_element['#required']) {
    form_error($form_element, t('E-mail address is required.', array('%name' => $component['name'])));
  }
}

/**
 * Implements _webform_submit_component().
 */
function _webform_submit_cc_email($component, $value) {
  global $user;
  $mail = $value['cc_email_address'];
  $return_val = array(0 => $mail);
  $fields = array(); // A place to put additional fields of information that may come from the form.

  // Let's try grabbing the other posted variables and using those as additional fields.

  // Look at the site's mapped CC fields from the variables table.
  $cc_extra_field_mappings = explode(', ', variable_get('cc_extra_field_mappings'));
  $mappings = array();
  foreach ($cc_extra_field_mappings as $key => $value) {
    $mapping = explode(':', $value);
    $mappings[$mapping[0]] = $mapping[1];
  }

  // Look at the submitted form data and try to match it up and send it over along with the email address.
  foreach($_POST['submitted'] as $key => $value) {
    if (in_array($key, $mappings)) { // If the name of the webform field matches one of the mapped fieldnames, let's send it over to Constant Contact as data. An example would be a webform field that was created with the machine name of "field_first_name" that was mapped in the settings to correspond to the "FirstName" CC field.
      $matched_key = array_search($key, $mappings);
      $fields[$matched_key] = $value;
    }
    elseif (array_key_exists($key, $mappings)) { // Alternatively, if the name of the webform field matches one of the basic API fieldnames (the keys), let's send it over to Constant Contact as data. An example would be a webform field that was created with the machine name of "FirstName".
      $fields[$key] = $value;
    }
  }

  // Get the name/title of the associated webform.
  $result = db_select('node', 'n')
  ->fields('n', array('title'))
  ->condition('nid', $component['nid'])
  ->execute()
  ->fetchField();
  // Add a note about the info in Constant Contact being added via Webform.
  if (empty($fields['Note'])) {
    $fields['Note'] = t('This contact was added via the "!title" webform within Drupal. Please review the record within the webform results in order to get the full text that the user may have entered when filling out the form.', array('!title' => $result));
  }

  $fields['EmailType'] = 'HTML'; // Default to HTML emails for the user.
  
  // Make sure we have a valid email address before continuing
  if (!valid_email_address($mail)) {
    return FALSE;
  }
  elseif (isset($value['subscribe']) AND !intval($value['subscribe'])) {
    return $return_val;
  }

  // get CC object
  $cc = constant_contact_create_object();
  if (!is_object($cc)) {
    return FALSE;
  }

  $lists = constant_contact_get_lists($cc);
  $contact_lists = array();

  if ($component['extra']['show_contact_list_selection']) {
    if (is_array($value['list_selection']) AND count($value['list_selection']) > 0) {
      foreach ($value['list_selection'] as $list) {
        if ($list > 0 AND isset($lists[$list])) {
          $contact_lists[$list] = $lists[$list];
        }
      }
      $contact_lists = array_keys($contact_lists);
    }
  }
  else {
    $contact_lists = (is_array($component['extra']['lists'])) ? array_keys($component['extra']['lists']) : array();
  }
  if (count($contact_lists) > 0) {
    $cc->set_action_type('contact'); /* important, tell CC that the contact made this action */
    $contact = $cc->query_contacts($mail);
    $return_val[2] = serialize($contact_lists);
    if ($component['extra']['action'] == 'subscribe') {
      $return_val[1] = 'Subscribe';

      // subscribe a user
      if ($contact) {
        // get the contacts full details
        $contact = $cc->get_contact($contact['id']);

        $contact_lists = (is_array($contact['lists'])) ? array_merge($contact['lists'], array_keys($contact_lists)) : array_keys($contact_lists);

        // contact exists so update their subscribed lists
        $status = $cc->update_contact($contact['id'], $mail, $contact_lists);
      }
      else {
        // contact does not exist so subscribe them
        $status = $cc->create_contact($mail, $contact_lists, $fields);
        $array_keys = array_keys($contact_lists);
      }
    }
    else {
      $return_val[1] = 'Unsubscribe';

      // unsubscribe a user
      if ($contact) {
        $contact = $cc->get_contact($contact['id']);

        // remove them from the contact lists they have chosen
        foreach ($contact_lists as $list_id => $list_name) {
          if (isset($contact['lists'][$list_name])) {
            unset($contact['lists'][$list_name]);
          }
        }

        // contact exists so update their subscribed lists
        $status = $cc->update_contact($contact['id'], $mail, $contact['lists']);
      }
    }
  }
  return $return_val;
}

/**
 * Module specific instance of hook_help().
 *
 * This allows each Webform component to add information into hook_help().
 */
function _webform_help_component($section) {
  switch ($section) {
    case 'admin/config/content/webform#cc_email':
      return t('Allows creation of an email field and contact list selection when you have the constant contact module installed.');
  }
}

/**
 * Format the text output for this component.
 */
function theme_webform_display_cc_email($variables) {
  $element = $variables['element'];
  $element['#value'] = empty($element['#value']) ? ' ' : $element['#value'];
  return $element['#format'] == 'html' ? check_plain($element['#value']) : $element['#value'];
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_cc_email() {
  return array(
    'webform_display_cc_email' => array(
      'render element' => 'element',
    ),
  );
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_cc_email($component) {
  $result = db_select('webform_submitted_data', 'wsd')
  ->fields('wsd', array('data'))
  ->condition('nid', $component['nid'])
  ->condition('cid', $component['cid'])
  ->condition('no', 0)
  ->execute()
  ->fetchAll(PDO::FETCH_ASSOC);
  $nonblanks = 0;
  $submissions = 0;
  $wordcount = 0;
  // Loop through the array of results.
  foreach ($result as $data) {
    if (strlen(trim($data['data'])) > 0 ) {
      $nonblanks++;
      $wordcount += str_word_count(trim($data['data']));
    }
    $submissions++;
  }
  $rows[0] = array(t('Submissions'), $submissions);
  return $rows;
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_cc_email($component, $value) {
  return check_plain(empty($value[0]) ? '' : $value[0]);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_cc_email($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $component['name'];
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_cc_email($component, $export_options, $value) {
  return empty($value[0]) ? '' : $value[0];
}
