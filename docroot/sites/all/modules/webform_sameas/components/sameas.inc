<?php
/**
 * @file
 * Webform module sameas component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_sameas() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'mandatory' => FALSE,
    'extra' => array(
      'size' => '',
      'maxlength' => '25',
      'disabled' => 0,
      'unique' => TRUE,
      'title_display' => 0,
      'description' => NULL,
      'attributes' => array(
        'class' => array(
          'webform-sameas',
          'webform-sameas-controller',
        ),
      ),
      'private' => FALSE,
      'display_mode' => 'none',
      'src_field' => '',
      'target_field' => '',
      'field_prefix' => '',
      'field_suffix' => '',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_sameas($component) {
  $form = array();
  $form['value'] = array(
    '#type' => 'checkbox',
    '#title' => t('Default value'),
    '#default_value' => $component['value'],
    '#description' => t('The default value of the field.'),
  );
  $form['display']['display_mode'] = array(
    '#type' => 'select',
    '#options' => array(
      'none' => t('Do not modify the display of the target fields.'),
      'invisible' => t('Hide target fields'),
      'disabled' => t('Disable target fields'),
    ),
    '#default_value' => $component['extra']['display_mode'],
    '#parents' => array('extra', 'display_mode'),
    '#title' => t('Display mode'),
  );
  $form['mapping']['src_fields'] = array(
    '#type' => 'select',
    '#options' => _webform_sameas_get_other_components($component['nid'], $component['form_key']),
    '#parents' => array('extra', 'src_field'),
    '#default_value' => $component['extra']['src_field'],
    '#title' => t('Source Field'),
  );
  $form['mapping']['target_fields'] = array(
    '#type' => 'select',
    '#options' => _webform_sameas_get_other_components($component['nid'], $component['form_key']),
    '#parents' => array('extra', 'target_field'),
    '#default_value' => $component['extra']['target_field'],
    '#title' => t('Target Field'),
  );
  return $form;
}

/**
 * Render a Webform component to be part of a form.
 */
function _webform_render_sameas($component, $value = NULL, $filter = TRUE) {
  $element = array(
    '#type' => 'checkbox',
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#default_value' => $filter ? _webform_filter_values($component['value'], NULL, NULL, NULL, FALSE) : $component['value'],
    '#weight' => $component['weight'],
    '#description' => $filter ? _webform_filter_descriptions($component['extra']['description']) : $component['extra']['description'],
    '#attributes' => $component['extra']['attributes'],
    '#theme_wrappers' => array('webform_element'),
    '#webform_component' => $component,
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'webform_sameas') . '/js/webform_sameas.js',
      ),
    ),
  );
  if (isset($value)) {
    $element['#default_value'] = $value[0];
  }
  $element['#attributes']['class'][] = $component['form_key'];
  $element['#attributes']['class'][] = 'webform-sameas--' . $component['form_key'];
  $element['#attributes']['class'][] = 'webform-sameas-cid--' . $component['cid'];

  $settings = array(
    'webform_sameas' => array(
      'controllers' => array(
        'webform-sameas--' . $component['form_key'] => array(
          'field_map' => array(
            $component['extra']['src_field'] => $component['extra']['target_field'],
          ),
          'display_mode' => $component['extra']['display_mode'],
        ),
      ),
    ),
  );
  drupal_add_js($settings, 'setting');

  return $element;
}

/**
 * Display the result of a submission for a component.
 *
 * The output of this function will be displayed under the "Results" tab then
 * "Submissions". This should output the saved data in some reasonable manner.
 *
 * @param $component
 *   A Webform component array.
 * @param $value
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database table schema.
 * @param $format
 *   Either 'html' or 'text'. Defines the format that the content should be
 *   returned as. Make sure that returned content is run through check_plain()
 *   or other filtering functions when returning HTML.
 * @return
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
function _webform_display_sameas($component, $value, $format = 'html') {
  $value_text = webform_sameas_get_field_value_as_text($value);
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_textfield',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#post_render' => array('webform_element_wrapper'),
    '#field_prefix' => $component['extra']['field_prefix'],
    '#field_suffix' => $component['extra']['field_suffix'],
    '#component' => $component,
    '#format' => $format,
    '#value' => $value_text,
  );
}



/**
 * Return the result of a component value for display in a table.
 *
 * The output of this function will be displayed under the "Results" tab then
 * "Table".
 *
 * @param $component
 *   A Webform component array.
 * @param $value
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema.
 * @return
 *   Textual output formatted for human reading.
 */
function _webform_table_sameas($component, $value) {
  $value_text = webform_sameas_get_field_value_as_text($value);

  return $value_text;
}

/**
 * Return the header for this component to be displayed in a CSV file.
 *
 * The output of this function will be displayed under the "Results" tab then
 * "Download".
 *
 * @param $component
 *   A Webform component array.
 * @param $export_options
 *   An array of options that may configure export of this field.
 * @return
 *   An array of data to be displayed in the first three rows of a CSV file, not
 *   including either prefixed or trailing commas.
 */
function _webform_csv_headers_sameas($component, $export_options) {
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
 * @param $component
 *   A Webform component array.
 * @param $export_options
 *   An array of options that may configure export of this field.
 * @param $value
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema.
 * @return
 *   An array of items to be added to the CSV file. Each value within the array
 *   will be another column within the file. This function is called once for
 *   every row of data.
 */
function _webform_csv_data_sameas($component, $export_options, $value) {
  $value_text = webform_sameas_get_field_value_as_text($value);

  return $value_text;
}

/**
 * Calculate and returns statistics about results for this component.
 *
 * This takes into account all submissions to this webform. The output of this
 * function will be displayed under the "Results" tab then "Analysis".
 *
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @param $sids
 *   An optional array of submission IDs (sid). If supplied, the analysis will
 *   be limited to these sids.
 * @param $single
 *   Boolean flag determining if the details about a single component are being
 *   shown. May be used to provided detailed information about a single
 *   component's analysis, such as showing "Other" options within a select list.
 * @return
 *   An array of data rows, each containing a statistic for this component's
 *   submissions.
 */
function _webform_analysis_sameas($component, $sids = array()) {
  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->fields('wsd', array('data'))
    ->condition('nid', $component['nid'])
    ->condition('cid', $component['cid']);

  if (count($sids)) {
    $query->condition('sid', $sids, 'IN');
  }

  $nonblanks = 0;
  $submissions = 0;
  $wordcount = 0;

  $result = $query->execute();
  $yes_count = 0;
  $no_count = 0;
  foreach ($result as $data) {
    if ($data) {
      $yes_count++;
    }
    else {
      $no_count++;
    }
    $submissions++;
  }

  $rows = array();
  $rows[0] = array(t('Yes'), $yes_count);
  $rows[1] = array(t('No'), $no_count);

  return $rows;
}
