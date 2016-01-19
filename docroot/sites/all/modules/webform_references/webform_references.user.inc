<?php

/**
 * @file
 * Webform module user reference component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_user_reference() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'required'  => 0,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'title_display' => 0,
      'private' => FALSE,
      'attributes' => array(),
      'description' => '',
      'referenceable_types' => array(),
      'widget_type' => '',
      'multiple' => NULL,
      'sort' => array('sort_by' => 'uid', 'order' => 'ASC'),
    ),
  );
}

/**
 * Generate the form for editing a component.
 *
 * Create a set of form elements to be displayed on the form for editing this
 * component. Use care naming the form items, as this correlates directly to the
 * database schema. The component "Name" and "Description" fields are added to
 * every component type and are not necessary to specify here (although they
 * may be overridden if desired).
 *
 * @param array $component
 *   A Webform component array.
 *
 * @return array
 *   An array of form items to be displayed on the edit component page
 */
function _webform_edit_user_reference(array $component) {
  $form = array();
  $form['extra']['referenceable_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('User with roles that can be referenced'),
    '#description' => t('If "authenticated user" is selected then all the users will be displayed.'),
    '#multiple' => TRUE,
    '#default_value' => $component['extra']['referenceable_types'],
    '#options' => user_roles(TRUE),
    '#required' => TRUE,
  );
  $form['extra']['widget_type'] = array(
    '#type' => 'select',
    '#title' => t('Field Type'),
    '#multiple' => FALSE,
    '#default_value' => $component['extra']['widget_type'],
    '#options' => array('select' => t('Select List'), 'checkbox_radio' => t('Checkbox/Radio'), 'autocomplete' => t('Autocomplete')),
    '#required' => TRUE,
  );
  $form['extra']['multiple'] = array(
    '#type' => 'checkbox',
    '#title' => t('Multiple'),
    '#default_value' => $component['extra']['multiple'],
    '#description' => t('Check this option if the user should be allowed to choose multiple values.'),
    '#states' => array(
      // Display this field when the widget_type is select list.
      'invisible' => array(
        ':input[name="extra[widget_type]"]' => array('value' => 'autocomplete'),
      ),
    ),
  );
  $form['extra']['sort'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('container-inline'),
    ),
  );
  $form['extra']['sort']['sort_by'] = array(
    '#type' => 'select',
    '#title' => t('Sort By'),
    '#multiple' => FALSE,
    '#default_value' => $component['extra']['sort']['sort_by'],
    '#options' => array('uid' => t('User Id'), 'name' => t('User Name')),
    '#required' => TRUE,
    '#states' => array(
      // Display this field when the widget_type is select list.
      'invisible' => array(
        ':input[name="extra[widget_type]"]' => array('value' => 'autocomplete'),
      ),
    ),
  );
  $form['extra']['sort']['order'] = array(
    '#type' => 'select',
    '#title' => t('Order'),
    '#multiple' => FALSE,
    '#default_value' => $component['extra']['sort']['order'],
    '#options' => array('ASC' => t('Ascending'), 'DESC' => t('Descending')),
    '#required' => TRUE,
    '#states' => array(
      // Display this field when the widget_type is select list.
      'invisible' => array(
        ':input[name="extra[widget_type]"]' => array('value' => 'autocomplete'),
      ),
    ),
  );
  return $form;
}

/**
 * Render a Webform component to be part of a form.
 *
 * @param array $component
 *   A Webform component array.
 * @param array $value
 *   If editing an existing submission or resuming a draft, this will contain
 *   an array of values to be shown instead of the default in the component
 *   configuration. This value will always be an array, keyed numerically for
 *   each value saved in this field.
 * @param string $filter
 *   Whether or not to filter the contents of descriptions and values when
 *   rendering the component. Values need to be unfiltered to be editable by
 *   Form Builder.
 *
 * @see _webform_client_form_add_component()
 */
function _webform_render_user_reference(array $component, array $value = NULL, $filter = TRUE) {
  switch ($component['extra']['widget_type']) {
    case 'autocomplete':
      $form_item = array(
        '#type' => 'textfield',
        '#autocomplete_path' => 'webform_references/user/autocomplete/' . implode('+', array_filter($component['extra']['referenceable_types'])),
        '#description' => $filter ? _webform_filter_descriptions($component['extra']['description']) : $component['extra']['description'],
        '#required'         => $component['required'] || $component['mandatory'],
        // Either one being true will could as required...
        // because webform changed in 4.x-alpha8
        '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
        '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
        '#weight' => $component['weight'],
        '#element_validate' => array('webform_references_validate_user_reference'),
        '#theme_wrappers' => array('webform_element'),
      );
      if (isset($value)) {
        $selected_user = user_load($value[0]);
        $form_item['#default_value'] = ($selected_user) ? $selected_user->name . " [id:$selected_user->uid]" : '';
      }
      break;

    case 'checkbox_radio':
      $user_list = _webform_references_get_user_list($component['extra']['referenceable_types'], NULL, $component['extra']['sort']['sort_by'], $component['extra']['sort']['order']);
      $form_item = array(
        '#type' => ($component['extra']['multiple']) ? 'checkboxes' : 'radios',
        '#description' => $filter ? _webform_filter_descriptions($component['extra']['description']) : $component['extra']['description'],
        '#required'         => $component['required'] || $component['mandatory'],
        // Either one being true will could as required...
        // because webform changed in 4.x-alpha8.
        '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
        '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
        '#options' => $user_list,
        '#weight' => $component['weight'],
        '#multiple' => $component['extra']['multiple'],
        '#pre_render' => array(),
      );
      $form_item['#theme_wrappers'] = array($form_item['#type'], 'webform_element');
      if (isset($value)) {
        if ($form_item['#type'] == 'checkboxes') {
          $form_item['#default_value'] = $value;
        }
        else {
          $form_item['#default_value'] = $value[0];
        }
      }
      break;

    default:
      $user_list = _webform_references_get_user_list($component['extra']['referenceable_types'], NULL, $component['extra']['sort']['sort_by'], $component['extra']['sort']['order']);
      $form_item = array(
        '#type' => 'select',
        '#description' => $filter ? _webform_filter_descriptions($component['extra']['description']) : $component['extra']['description'],
        '#required'         => $component['required'] || $component['mandatory'],
        // Either one being true will could as required...
        // because webform changed in 4.x-alpha8.
        '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
        '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
        '#options' => $user_list,
        '#empty_option' => t('No Value'),
        '#weight' => $component['weight'],
        '#multiple' => $component['extra']['multiple'],
        '#theme_wrappers' => array('webform_element'),
      );
      if (isset($value)) {
        $form_item['#default_value'] = $value;
      }
      break;
  }
  return $form_item;
}

/**
 * Validation Callback for user reference field.
 */
function webform_references_validate_user_reference($element, $form_state) {
  $value = $element['#value'];
  $uid = NULL;
  if (!empty($value)) {
    preg_match('/^(?:\s*|(.*) )?\[\s*id\s*:\s*(\d+)\s*\]$/', $value, $matches);
    if (!empty($matches)) {
      list(, $name, $uid) = $matches;
      if (!empty($name)) {
        $real_name = db_select('users', 'u')
          ->fields('u', array('name'))
          ->condition('u.uid', $uid)
          ->execute()
          ->fetchField();
        if (trim($name) != trim($real_name)) {
          form_error($element, t('Please check your selection.'));
        }
      }
    }
    else {
      form_error($element, t('Please check your selection.'));
    }
  }
}

/**
 * A hook for changing the input values before saving to the database.
 *
 * Note that Webform will save the result of this function directly into the
 * database.
 *
 * @param array $component
 *   A Webform component array.
 * @param string $value
 *   The POST data associated with the user input.
 *
 * @return string
 *   A string of value to be saved into the database.
 */
function _webform_submit_user_reference(array $component, $value) {
  if ($component['extra']['widget_type'] == 'autocomplete') {
    preg_match('/^(?:\s*|(.*) )?\[\s*id\s*:\s*(\d+)\s*\]$/', $value, $matches);
    if (!empty($matches)) {
      list(, , $uid) = $matches;
      $value = $uid;
    }
  }
  return $value;
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_user_reference() {
  return array('webform_display_user_reference_field' => array('render element' => 'element'));
}

/**
 * Display the result of a submission for a component.
 *
 * The output of this function will be displayed under the "Results" tab then
 * "Submissions". This should output the saved data in some reasonable manner.
 *
 * @param array $component
 *   A Webform component array.
 * @param array $value
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database table schema.
 *   or NULL if no value submitted.
 * @param string $format
 *   Either 'html' or 'text'. Defines the format that the content should be
 *   returned as. Make sure that returned content is run through check_plain()
 *   or other filtering functions when returning HTML.
 *
 * @return array
 *   A renderable element containing at the very least these properties:
 *    - #title
 *    - #weight
 *    - #component
 *    - #format
 *    - #value
 *   Webform also uses #theme_wrappers to output the end result to the user,
 *   which will properly format the label and content for use within an e-mail
 *   (such as wrapping the text) or as HTML (ensuring consistent output).
 */
function _webform_display_user_reference(array $component, $value, $format = 'html') {
  $value = (array) $value;
  return array(
    '#title'          => $component['name'],
    '#weight'         => $component['weight'],
    '#theme'          => 'webform_display_user_reference_field',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#post_render'    => array('webform_element_wrapper'),
    '#component'      => $component,
    '#format'         => $format,
    '#value'          => array_filter($value),
  );
}

/**
 * Format the output of data for this component.
 */
function theme_webform_display_user_reference_field($variables) {
  $values = (array) $variables['element']['#value'];
  $items = array();
  foreach (array_filter($values) as $value) {
    $uid = check_plain($value);
    $items[] = _webform_references_get_user_name($uid, TRUE);
  }
  $output = count($items) > 1 ? theme('item_list', array('items' => $items)) : (isset($items[0]) ? $items[0] : '');
  return $output;
}

/**
 * Helper function to get name of user.
 *
 * @param int $uid
 *   User Id.
 * @param bool $link
 *   FALSE for plain text and TRUE for linked text.
 *
 * @return string
 *   Name of the user (Linked if $link parameter is set to TRUE).
 */
function _webform_references_get_user_name($uid, $link = FALSE) {
  $user_items = db_select('users', 'u')
    ->fields('u', array('name'))
    ->condition('uid', $uid)
    ->execute()
    ->fetchAssoc();
  $name = ($link == TRUE) ? l($user_items['name'], 'user/' . $uid) : check_plain($user_items['name']);
  return $name;
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_user_reference(array $component, $value) {
  $value = (array) $value;
  $items = array();

  foreach (array_filter($value) as $uid) {
    $items[] = _webform_references_get_user_name($uid);
  }
  return (!empty($items)) ? implode('<br />', $items) : '';
}

/**
 * Return the header for this component to be displayed in a CSV file.
 *
 * The output of this function will be displayed under the "Results" tab then
 * "Download".
 *
 * @param array $component
 *   A Webform component array.
 * @param array $export_options
 *   An array of options that may configure export of this field.
 *
 * @return array
 *   An array of data to be displayed in the first three rows of a CSV file, not
 *   including either prefixed or trailing commas.
 */
function _webform_csv_headers_user_reference(array $component, array $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $component['name'];
  return $header;
}

/**
 * Format the submitted data of a component for CSV downloading.
 *
 * The output of this function will be displayed under the "Results" tab then
 * "Download".
 *
 * @param array $component
 *   A Webform component array.
 * @param array $export_options
 *   An array of options that may configure export of this field.
 * @param array $value
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema.
 *   or NULL if no value submitted.
 *
 * @return string
 *   A string of item to be added to the CSV file.
 */
function _webform_csv_data_user_reference(array $component, array $export_options, $value) {
  $return = array();
  $value = (array) $value;
  foreach (array_filter($value) as $uid) {
    $return[] = _webform_references_get_user_name($uid);
  }
  return !empty($return) ? implode(', ', $return) : '';
}

/**
 * Menu callback for the autocomplete results.
 */
function webform_references_user_autocomplete($bundles, $string = '') {
  $options = array(
    'string' => $string,
    'limit' => 10,
  );
  $roles = explode('+', $bundles);
  if (!count($roles)) {
    return array();
  }
  $roles = array_combine($roles, $roles);
  $references = _webform_references_get_user_list($roles, $string);
  $matches = array();
  foreach ($references as $id => $name) {
    $matches[$name . " [id:$id]"] = $name;
  }
  drupal_json_output($matches);
}

/**
 * Helper function to get user list.
 */
function _webform_references_get_user_list($roles, $string = '', $sort_by = 'uid', $order = 'ASC') {
  $roles = array_filter($roles);
  $query = db_select('users', 'u');
  $query->fields('u', array('uid', 'name'));
  if (!empty($roles) && !isset($roles[2])) {
    $query->join('users_roles', 'ur', 'ur.uid = u.uid');
    $query->condition('ur.rid', $roles, 'IN');
  }
  if ($string != '') {
    $query->condition('u.name', '%' . $string . '%', 'LIKE');
    $query->range(0, 10);
  }
  else {
    $query->orderBy($sort_by, $order);
  }
  $query->condition('u.status', 1);
  $results = $query->execute()->fetchAll();
  $user_list = array();
  foreach ($results as $result) {
    $user_list[$result->uid] = $result->name;
  }
  return $user_list;
}
