<?php
/*
 * _webform_defaults_component()
 * 
 * Specify the default properties of a component.
 */
function _webform_defaults_wmarketing() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'mandatory' => 0,
    'extra' => array( 
      'url_param' => '',
      'is_hidden' => TRUE,
      'private' => FALSE,
    ),    
  );
}

/*
 * _webform_edit_component()
 * 
 * Generate the form for editing a component.
 */
function _webform_edit_wmarketing($component) {
  $form = array();
  //Help text
  $form['extra']['help'] = array(
    '#markup' => t('Here you can enter parameter name or <b>sitereferrer</b>.<br>'
                 . 'Given parameter will be fetched from url on any site page and stored in cookies.<br/>'
                 . 'Once visitor fill the form, field will be filled with value.<br/><br/>'
                 . 'You can set cookies life time ' . l('here', 'admin/config/webform_marketing') . ''),
  );
  //parameter
  $form['extra']['url_param'] = array(
    '#type' => 'textfield',
    '#title' => t('Parameter name'),
    '#default_value' => $component['extra']['url_param'],
    '#required' => TRUE,
    '#parents' => array('extra', 'url_param'),
    '#description' => t('Write parameter name here. If you want get data about site referrer then you need write <b>sitereferrer</b>. (It is constant of this module)'),
  );
  //is hidden 
  $form['extra']['is_hidden'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hidden'),
    '#default_value' => $component['extra']['is_hidden'],
    '#required' => FALSE,
    '#parents' => array('extra', 'is_hidden'),
    '#description' => t('Check it if you want add hidden field'),
  );  
  return $form;
}

/*
 * _webform_render_component()
 * 
 * Render a Webform component to be part of a form.
 */
function _webform_render_wmarketing($component, $value = NULL, $filter = TRUE) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;
  $url_param_prefix = ($component['extra']['url_param'] == 'sitereferrer' || (!isset($component['extra']['prefix'])) ? '' : $component['extra']['prefix']);
  $element = array(
    '#type' => $component['extra']['is_hidden'] ? 'hidden' : 'textfield',
    '#title' => $component['extra']['is_hidden'] ? '' : t($component['name']),
    '#default_value' => isset($value) ? $value[0] : '',
    '#theme_wrappers' => array('webform_element'),
    '#attributes' => array(
      'url_param' => $url_param_prefix . $component['extra']['url_param'],
      'is_wmarketing' => 1,
    ),
  );
  return $element;
}

/*
 * _webform_display_component()
 * 
 * Display the result of a submission for a component.
 */
function _webform_display_wmarketing($component, $value, $format = 'html') {
  return array(
    '#title' => 'test',
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_wmarketing',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#value' => $value[0],
    '#translatable' => array('title'),
  );
}

/*
 * _webform_table_component()
 * 
 * Return the result of a component value for display in a table.
 */
function _webform_table_wmarketing($component, $value) {
  return check_plain(empty($value[0]) ? '' : $value[0]);
}

/*
 * _webform_csv_headers_component()
 * 
 * Return the header for this component to be displayed in a CSV file.
 */
function _webform_csv_headers_wmarketing($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = $component['extra']['url_param'];  
  return $header;
}

/*
 * _webform_csv_data_component()
 * 
 * Format the submitted data of a component for CSV downloading.
 */
function _webform_csv_data_wmarketing($component, $export_options, $value) {
  return isset($value[0]) ? $value[0] : '';
}

/*
 * _webform_theme_component()
 * 
 * Module specific instance of hook_theme().
 */
function _webform_theme_wmarketing() {
  return array(
    'webform_display_wmarketing' => array(
      'render element' => 'element',
    ),
  );
}

/*
 * Theme realization for wmarketing field
 */
function theme_webform_display_wmarketing($variables) {
  $element = $variables['element'];
  $value = $element['#value'];
  return trim($value) !== '' ? $value : ' ';
}