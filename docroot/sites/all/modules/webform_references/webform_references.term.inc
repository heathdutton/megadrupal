<?php

/**
 * @file
 * Webform module term reference component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_term_reference() {
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
      'sort' => array('sort_by' => 'name', 'order' => 'ASC'),
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
function _webform_edit_term_reference(array $component) {
  $form = array();
  $voc_keys = array();
  foreach (taxonomy_get_vocabularies() as $vid => $vocab) {
    $voc_keys[$vid] = $vocab->name;
  }
  $form['extra']['referenceable_types'] = array(
    '#type' => 'select',
    '#title' => t('Select Vocabularies that can be referenced'),
    '#multiple' => FALSE,
    '#default_value' => $component['extra']['referenceable_types'],
    '#options' => $voc_keys,
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
    '#options' => array('tid' => t('Term Id'), 'name' => t('Term Name'), 'weight' => t('Term Weight')),
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
function _webform_render_term_reference(array $component, array $value = NULL, $filter = TRUE) {
  switch ($component['extra']['widget_type']) {
    case 'autocomplete':
      $form_item = array(
        '#type' => 'textfield',
        '#autocomplete_path' => 'webform_references/term/autocomplete/' . $component['extra']['referenceable_types'],
        '#description' => $filter ? _webform_filter_descriptions($component['extra']['description']) : $component['extra']['description'],
        '#required'         => $component['required'] || $component['mandatory'],
        // Either one being true will could as required...
        // because webform changed in 4.x-alpha8
        '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
        '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
        '#weight' => $component['weight'],
        '#element_validate' => array('webform_references_validate_term_reference'),
        '#theme_wrappers' => array('webform_element'),
      );
      if (isset($value)) {
        $selected_term = taxonomy_term_load($value[0]);
        $form_item['#default_value'] = ($selected_term) ? $selected_term->name : '';
      }
      break;

    case 'checkbox_radio':
      $term_list = _webform_references_get_term_list($component['extra']['referenceable_types'], $component['extra']['sort']['sort_by'], $component['extra']['sort']['order']);
      $form_item = array(
        '#type' => ($component['extra']['multiple']) ? 'checkboxes' : 'radios',
        '#description' => $filter ? _webform_filter_descriptions($component['extra']['description']) : $component['extra']['description'],
        '#required'         => $component['required'] || $component['mandatory'],
        // Either one being true will could as required...
        // because webform changed in 4.x-alpha8.
        '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
        '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
        '#options' => $term_list,
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
      $term_list = _webform_references_get_term_list($component['extra']['referenceable_types'], $component['extra']['sort']['sort_by'], $component['extra']['sort']['order']);
      $form_item = array(
        '#type' => 'select',
        '#description' => $filter ? _webform_filter_descriptions($component['extra']['description']) : $component['extra']['description'],
        '#required'         => $component['required'] || $component['mandatory'],
        // Either one being true will could as required...
        // because webform changed in 4.x-alpha8.
        '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
        '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
        '#options' => $term_list,
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
 * Validation Callback for term reference field.
 */
function webform_references_validate_term_reference($element, $form_state) {
  $term_name = $element['#value'];
  if ($term_name != '') {
    $vid = $element['#webform_component']['extra']['referenceable_types'];
    $tid = _webform_references_get_term_tid($term_name, $vid);
    if (!$tid) {
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
function _webform_submit_term_reference(array $component, $value) {
  if ($component['extra']['widget_type'] == 'autocomplete') {
    $vid = $component['extra']['referenceable_types'];
    $value = _webform_references_get_term_tid($value, $vid);
  }
  return $value;
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_term_reference() {
  return array('webform_display_term_reference_field' => array('render element' => 'element'));
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
function _webform_display_term_reference(array $component, $value, $format = 'html') {
  $value = (array) $value;
  return array(
    '#title'          => $component['name'],
    '#weight'         => $component['weight'],
    '#theme'          => 'webform_display_term_reference_field',
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
function theme_webform_display_term_reference_field($variables) {
  $values = (array) $variables['element']['#value'];
  $items = array();
  foreach (array_filter($values) as $value) {
    $tid = check_plain($value);
    if ($tid) {
      $items[] = _webform_references_get_term_name($tid, TRUE);
    }
  }
  $output = count($items) > 1 ? theme('item_list', array('items' => $items)) : (isset($items[0]) ? $items[0] : '');
  return $output;
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_term_reference(array $component, $value) {
  $value = (array) $value;
  $items = array();

  foreach (array_filter($value) as $tid) {
    if ($tid) {
      $items[] = _webform_references_get_term_name($tid, TRUE);
    }
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
function _webform_csv_headers_term_reference(array $component, array $export_options) {
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
function _webform_csv_data_term_reference(array $component, array $export_options, $value) {
  $return = array();
  $value = (array) $value;
  foreach (array_filter($value) as $tid) {
    if ($tid) {
      $return[] = _webform_references_get_term_name($tid);
    }
  }
  return !empty($return) ? implode(', ', $return) : '';
}

/**
 * Menu callback for the autocomplete results.
 */
function webform_references_term_autocomplete($vid, $string = '') {
  $options = array(
    'string' => $string,
    'limit' => 10,
  );
  
  $query = db_select('taxonomy_term_data', 't');
  $query->addField('t', 'tid');
  $term_title_alias = $query->addField('t', 'name', 'term_name');
  $query->addTag('translatable')
    ->addTag('term_access');
  $query->condition('t.vid', $vid);
  
  if ($options['string'] !== '') {
    $query->condition('t.name', '%' . $options['string'] . '%', 'LIKE');
  }

  if ($options['limit']) {
    $query->range(0, $options['limit']);
  }

  $query->orderBy($term_title_alias);
  $result = $query->execute()->fetchAll();
  $references = array();
  foreach ($result as $term) {
    $references[] = array(
      'name' => $term->term_name,
    );
  }

  $matches = array();
  foreach ($references as $row) {
    // Add a class wrapper for a few required CSS overrides.
    $name = check_plain($row['name']);
    $matches[$name] = check_plain($name);
  }
  drupal_json_output($matches);
}

/**
 * Helper function to get term list.
 */
function _webform_references_get_term_list($vid, $sort_by = 'name', $order = 'ASC') {
  $result = db_select('taxonomy_term_data', 't')
    ->fields('t', array('tid', 'name'))
    ->condition('t.vid', $vid)
    ->orderBy($sort_by, $order)
    ->addTag('translatable')
    ->addTag('term_access')
    ->execute()
    ->fetchAll();
  $term_list = array();
  foreach ($result as $term) {
    $term_list[$term->tid] = $term->name;
  }
  return $term_list;
}

/**
 * Helper function to get title of term.
 *
 * @param int $tid
 *   Term Id.
 * @param bool $link
 *   FALSE for plain text and TRUE for linked text.
 *
 * @return string
 *   Name of the term (Linked if $link parameter is set to TRUE).
 */
function _webform_references_get_term_name($tid, $link = FALSE) {
  $term = taxonomy_term_load($tid);
  $name = ($link == TRUE) ? l($term->name, 'taxonomy/term/' . $tid) : check_plain($term->name);
  return $name;
}

/**
 * Helper function to get tid of term.
 *
 * @param string $term_name
 *   Term Name.
 * @param bool $link
 *   FALSE for plain text and TRUE for linked text.
 *
 * @return int
 *   Tid of the term.
 */
function _webform_references_get_term_tid($term_name, $vid) {
  $tid = db_select('taxonomy_term_data', 't')
    ->fields('t', array('tid'))
    ->condition('t.name', $term_name)
    ->condition('t.vid', $vid)
    ->execute()
    ->fetchField();
  return $tid;
}
