<?php

/**
 * @file
 * Admin configurations.
 */

/**
 * Overview page of all components for this webform.
 */
function webform_captureplus_page($node, $page_number = 1) {
  $output = drupal_get_form('webform_captureplus_form', $node);

  return array(
    '#theme' => 'webform_captureplus_page',
    '#node' => $node,
    '#form' => $output,
  );
}

/**
 * Theme the output of the main components page.
 *
 * This theming provides a way to toggle between the editing modes if Form
 * Builder module is available.
 */
function theme_webform_captureplus_page($variables) {
  return drupal_render($variables['form']);
}

/**
 * The table-based listing of all components for this webform.
 */
function webform_captureplus_form($form, $form_state, $node) {
  $form = array(
    '#tree' => TRUE,
    '#node' => $node,
    'components' => array(),
  );

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );

  $settings = WebformCapturePlusSettings::createFromNid($node->nid);

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('API Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['settings']['key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#description' => t('API key from <a href="!url">Capture+</a>. Leave it blank to use global key.', array('!url' => 'http://www.postcodeanywhere.co.uk')),
    '#default_value' => isset($node->webform['captureplus']['key']) ?: NULL
  );
  $form['settings']['set_cursor'] = array(
    '#type' => 'checkbox',
    '#title' => t('Replace search box text on focus'),
    '#description' => t('Updates the input field with the current search text.'),
    '#default_value' => $settings->getConfigByName('set_cursor', TRUE),
  );
  $form['settings']['countries'] = array(
    '#type' => 'textfield',
    '#title' => t('Countries'),
    '#description' => t('Countries to allow Capture+ lookup.'),
    '#default_value' => $settings->getConfigByName('countries', 'GBR'),
  );
  $form['settings']['bar_visible'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show country bar'),
    '#description' => t('Country selection bar on Capture+ lookup autocomplete box footer.'),
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => $settings->getConfigByName('bar_visible', FALSE),
  );
  $form['settings']['culture'] = array(
    '#type' => 'textfield',
    '#title' => t('Language'),
    '#description' => t('The address suggestion language.'),
    '#default_value' => $settings->getConfigByName('culture', 'EN-GB'),
    '#size' => 10,
    '#maxlength' => 10,
  );
  $form['settings']['list_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('Width of Capture+ lookup autocomplete box. Leave it blank to adjust the width depending on the result.'),
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' =>  $settings->getConfigByName('list_width', NULL),
  );
  $form['settings']['list_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('Height of Capture+ lookup autocomplete box. Leave it blank to adjust the height depending on the result.'),
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => $settings->getConfigByName('list_height', NULL),
  );

  $options = array();
  $captureplus_fields = variable_get('webform_captureplus_allowed_fields', array());
  $mode_options = WebformCapturePlusSettings::getModeOptions();
  $group_options = WebformCapturePlusSettings::getGroupOptions();

  foreach ($node->webform['components'] as $cid => $component) {
    $options[$cid] = check_plain($component['name']);
    $form['components'][$cid]['cid'] = array(
      '#type' => 'hidden',
      '#default_value' => $component['cid'],
    );
    $form['components'][$cid]['pid'] = array(
      '#type' => 'hidden',
      '#default_value' => $component['pid'],
    );
    $form['components'][$cid]['group'] = array(
      '#type' => 'select',
      '#title' => t('Capture+ group'),
      '#default_value' => $settings->getComponentGroupById($cid),
      '#options' => $group_options,
      '#empty_option' => t('Select'),
      '#empty_value' => NULL,
    );
    $form['components'][$cid]['field'] = array(
      '#type' => 'select',
      '#title' => t('Capture+ field'),
      '#default_value' => $settings->getComponentFieldById($cid),
      '#options' => array_filter($captureplus_fields),
      '#empty_option' => t('Select'),
      '#empty_value' => NULL,
    );
    $form['components'][$cid]['mode'] = array(
      '#type' => 'select',
      '#title' => t('Capture+ mode'),
      '#default_value' => $settings->getComponentModeById($cid),
      '#options' => $mode_options,
      '#empty_option' => t('Select'),
      '#empty_value' => NULL,
    );
  }
  $mode_links = array(
    t('<strong>Default</strong>: Uses the field as search field and populate value of mapped Capture+ field.'),
    t('<strong>Search</strong>: Uses the field ONLY for search.'),
    t('<strong>Populate</strong>: Uses the field ONLY to populate value of mapped Capture+ field.'),
    t('<strong>Preserve</strong>: Keeps the field value, if it is prefilled'),
    t('<strong>None</strong>: No Capture+ field mapping.'),
  );

  $params = array(
    '!doc_link' => url('http://www.postcodeanywhere.co.uk/capture-plus/reference/2.20/', array('external' => TRUE)),
    '!settings' => url('admin/config/content/webform'),
  );
  $links = array(
    t('<strong>Group</strong>: Group allows to create multiple Capture+ look up elements in same form.'),
    t('<strong>Capture+ field</strong>: Capture+ search result provides set of fields. Please check <a href="!doc_link" target="_blank">documentation</a> for all available fields. The options can be changed from <a href="!settings">webform settings</a> page', $params),
    t('<strong>Mode</strong>: Defines how the fields behaviour. Allowed options are:<br />') . theme('item_list', array('items' => $mode_links)),
  );

  $form['components']['notes'] = array(
    '#type' => 'markup',
    '#markup' => theme('item_list', array('items' => $links, 'title' => t('Notes'))),
  );

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 45,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#access' => count($node->webform['components']) > 0,
  );
  $form['warning'] = array(
    '#weight' => -1,
  );

  return $form;
}

/**
 * Recursive function for nesting components into a table.
 *
 * @see preprocess_webform_captureplus_form()
 */
function _webform_captureplus_form_rows($node, $cid, $component, $level, &$form, &$rows) {
  // Create presentable values.
  if (drupal_strlen($component['value']) > 30) {
    $component['value'] = drupal_substr($component['value'], 0, 30);
    $component['value'] .= '...';
  }
  $component['value'] = check_plain($component['value']);

  // Remove individual titles from the required and weight fields.
  unset($form['components'][$cid]['required']['#title']);
  unset($form['components'][$cid]['pid']['#title']);
  unset($form['components'][$cid]['group']['#title']);
  unset($form['components'][$cid]['field']['#title']);
  unset($form['components'][$cid]['mode']['#title']);

  // Add special classes for weight and parent fields.
  $form['components'][$cid]['cid']['#attributes']['class'] = array('webform-cid');
  $form['components'][$cid]['pid']['#attributes']['class'] = array('webform-pid');

  // Build indentation for this row.
  $indents = '';
  for ($n = 1; $n <= $level; $n++) {
    $indents .= '<div class="indentation">&nbsp;</div>';
  }
  $row_data = array(
    array('data' => $indents . filter_xss($component['name']), 'class' => array('webform-component-name')),
  );
  $default_excludes = array('fieldset', 'date', 'time', 'grid', 'checkboxes', 'email' );
  $exclude_field_types =  variable_get('webform_captureplus_exclude_webform_component_types', $default_excludes);
  if (in_array($component['type'], $exclude_field_types)) {
    unset($form['components'][$cid]['group']);
    unset($form['components'][$cid]['field']);
    unset($form['components'][$cid]['mode']);
    $row_data[] = array('data' => '');
    $row_data[] = array('data' => '');
    $row_data[] = array('data' => '');
  }
  else {
    // Add each component to a table row.
    $row_data[] = array('data' => drupal_render($form['components'][$cid]['group']), 'class' => array('select'));
    $row_data[] = array('data' => drupal_render($form['components'][$cid]['field']), 'class' => array('select'));
    $row_data[] = array('data' => drupal_render($form['components'][$cid]['mode']), 'class' => array('select'));
  }
  $rows[] = array('data' => $row_data, 'class' => array(), 'data-cid' => $cid);
  if (isset($component['children']) && is_array($component['children'])) {
    foreach ($component['children'] as $cid => $component) {
      _webform_captureplus_form_rows($node, $cid, $component, $level + 1, $form, $rows);
    }
  }

}

/**
 * Validate handler for webform_captureplus_form().
 */
function webform_captureplus_form_validate($form, &$form_state) {
  $group_modes = array();

  if (isset($form_state['values']['components'])) {
    foreach ($form_state['values']['components'] as $cid => $component) {
      // If an element is an address field component, all 3 values should be selected.
      $address_fields = array_filter(array($component['group'], $component['field'], $component['mode']));
      if (count($address_fields) && count($address_fields) < 3) {
        if (empty($component['group'])) {
          form_error($form['components'][$cid]['group'], t('Please select a group'));
        }
        if (empty($component['field'])) {
          form_error($form['components'][$cid]['field'], t('Please select Capture+ field for '));
        }
        if (empty($component['mode'])) {
          form_error($form['components'][$cid]['mode'], t('Please select mode'));
        }
      }
      elseif (!empty($component['group']) && !empty($component['mode'])) {
        $group_modes[$component['group']][] = $component['mode'];
      }
    }
  }
  // Each group should have at least one DEFAULT/SEARCH mode field.
  foreach ($group_modes as $group_id => $modes) {
    if (!in_array(3, $modes) && !in_array(1, $modes)) {
      form_set_error('', t('Group !group_id does not have a field with Search or Default mode.', array('!group_id' => $group_id)));
    }
  }
}

/**
 * Submit handler for webform_captureplus_form() to save component order.
 */
function webform_captureplus_form_submit($form, &$form_state) {
  $node = node_load($form_state['values']['nid']);
  $fields = array();
  foreach ($form_state['values']['components'] as $cid => $component) {
    if (!empty($component['field']) && !empty($component['mode'])) {
      $fields[$component['cid']] = array(
        'field' => $component['field'],
        'mode' => $component['mode'],
        'group' => $component['group'],
      );
    }
  }
  $settings['fields'] = $fields;
  $settings['config'] = array_filter($form_state['values']['settings']);
  $storage_key = 'webform_captureplus_settings_' . $node->nid;

  // Clean existing settings.
  variable_del($storage_key);
  if (!empty($settings)) {
    variable_set($storage_key, $settings);
  }

  drupal_set_message(t('The settings have been updated.'));
}

/**
 * Fields mapping.
 *
 * @return array
 */
function webform_captureplus_get_field_mappings() {
  return array(
    'Id',
    'DomesticId',
    'Language',
    'LanguageAlternatives',
    'Department',
    'Company',
    'SubBuilding',
    'BuildingNumber',
    'BuildingName',
    'SecondaryStreet',
    'Street',
    'Block',
    'Neighbourhood',
    'District',
    'City',
    'Line1',
    'Line2',
    'Line3',
    'Line4',
    'Line5',
    'AdminAreaName',
    'AdminAreaCode',
    'Province',
    'ProvinceName',
    'ProvinceCode',
    'PostalCode',
    'CountryName',
    'CountryIso2',
    'CountryIso3',
    'CountryIsoNumber',
    'SortingNumber1',
    'SortingNumber2',
    'Barcode',
    'POBoxNumber',
    'Label',
    'Type',
    'DataLevel',
    'Field1',
    'Field2',
    'Field3',
    'Field4',
    'Field5',
    'Field6',
    'Field7',
    'Field8',
    'Field9',
    'Field10',
    'Field11',
    'Field12',
    'Field13',
    'Field14',
    'Field15',
    'Field16',
    'Field17',
    'Field18',
    'Field19',
    'Field20',
  );
}
