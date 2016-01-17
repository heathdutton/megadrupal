<?php

/**
 * @file
 * Provides field widget hooks for nlmfield module.
 */

/**
 * Implements hook_field_widget_info().
 */
function nlmfield_field_widget_info() {
  $widgets = array();
  $widgets['nlmfield_xml'] = array(
    'label' => t('XML'),
    'field types' => array('nlmfield_contributor'),
  );
  $widgets['nlmfield_text'] = array(
    'label' => t('TEXT'),
    'field types' => array('nlmfield_contributor'),
  );
  return $widgets;
}

/**
 * Implements hook_field_widget_form().
 */
function nlmfield_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $widget = $instance['widget'];
  $settings = $field['settings'];

  $element += array(
    '#type' => 'fieldset',
    '#title' => t('NLM Field'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => true
  );

  $element['xml']['#title'] = 'XML';
  $element['xml']['#type'] = 'textarea';
  $element['xml']['#default_value'] = !empty($items[$delta]['xml']) ? $items[$delta]['xml'] : '';

  $xml = $element['xml']['#default_value'];

  switch ($widget['type']) {

    case 'nlmfield_xml':
    // This case is not being used for now, just leaving in place, in case needed in future.
    break;

    case 'nlmfield_text':

      $element['xml']['#type'] = 'hidden';
      $element['xml']['#delta'] = $delta;
      $element['xml']['#field_name'] = $element['#field_name'];
      $element['xml']['#element_validate'] = array('nlmfield_field_widget_text_to_xml');
      $element['xml']['#nlmfield_namespace'] = $settings['nlmfield_namespace'] ? $settings['nlmfield_namespace'] : 'nlm';
      $element['xml']['#nlmfield_namespace_def_url'] = $settings['nlmfield_namespace_def_url'] ? $settings['nlmfield_namespace_def_url'] : 'http://www.ncbi.nlm.nih.gov';
      $element['xml']['#additional_namespaces'] = $settings['additional_namespaces'] ? $settings['additional_namespaces'] : '';


      $element['string_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name String'),
        '#default_value' => drupal_html_to_text(nlmfield_process_bit('nlmfield_contributor', 'n', $xml)),
      );

      $element['prefix'] = array(
        '#type' => 'textfield',
        '#title' => t('Name Prefix'),
        '#default_value' => drupal_html_to_text(nlmfield_process_bit('nlmfield_contributor', 'p', $xml)),
      );

      $element['given_names'] = array(
        '#type' => 'textfield',
        '#title' => t('First name and middle name'),
        '#default_value' => drupal_html_to_text(nlmfield_process_bit('nlmfield_contributor', 'g', $xml)),
        '#description' => t("First or given name, e.g. 'Peter'."),
      );

      $element['surname'] = array(
        '#type' => 'textfield',
        '#title' => t('Last Name'),
        '#default_value' => drupal_html_to_text(nlmfield_process_bit('nlmfield_contributor', 's', $xml)),
        '#description' => t("Your last, or family, name, e.g. 'MacMoody'."),
      );

      $element['suffix'] = array(
        '#type' => 'textfield',
        '#title' => t('Name Suffix'),
        '#default_value' => drupal_html_to_text(nlmfield_process_bit('nlmfield_contributor', 'x', $xml)),
      );

      $element['degrees'] = array(
        '#type' => 'textfield',
        '#title' => t('Degrees'),
        '#default_value' => drupal_html_to_text(nlmfield_process_bit('nlmfield_contributor', 'd', $xml)),
      );

      $element['email'] = array(
        '#type' => 'textfield',
        '#title' => t('Email'),
        '#default_value' => drupal_html_to_text(nlmfield_process_bit('nlmfield_contributor', 'e', $xml)),
        '#description' => t("Your email address, e.g. higgs-boson@gmail.com"),
      );

      $element['contributor_type'] = array(
        '#type' => 'textfield',
        '#title' => t('Contributor type'),
        '#default_value' => drupal_html_to_text(nlmfield_process_bit('nlmfield_contributor', 't', $xml)),
        '#description' => t("Specifies contribution type, typical values are 'author', 'editor' etc"),
      );

      $element['role'] = array(
        '#type' => 'textfield',
        '#title' => t('Role / Occupation'),
        '#default_value' => drupal_html_to_text(nlmfield_process_bit('nlmfield_contributor', 'r', $xml)),
        '#description' => t("Your role and/or occupation, e.g. 'Orthopedic Surgeon'."),
      );

      $element['aff'] = array(
        '#type' => 'textfield',
        '#title' => t('Affiliation'),
        '#default_value' => drupal_html_to_text(nlmfield_process_bit('nlmfield_contributor', 'a', $xml)),
        '#description' => t("Your organization or institution (if applicable), e.g. 'Royal Free Hospital'.")
      );

      $element['address'] = array(
        '#type' => 'textfield',
        '#title' => t('Address'),
        '#default_value' => drupal_html_to_text(nlmfield_process_bit('nlmfield_contributor', 'A', $xml)),
        '#description' => t("Your organization, institution's or residential address."),
      );

    break;
  }

  return $element;
}

function nlmfield_field_widget_text_to_xml($element, &$form_state) {

  $delta = $element['#delta'];
  $field_name = $element['#field_name'];
  $ns_url = $element['#nlmfield_namespace_def_url'];
  $ns = $element['#nlmfield_namespace'];

  $add_ns = array();
  if ($add_ns_string = $element['#additional_namespaces']) {
    $additional_namespaces = explode("\n", $add_ns_string);
    $additional_namespaces = array_map('trim', $additional_namespaces);
    $additional_namespaces = array_filter($additional_namespaces, 'strlen');

    foreach ($additional_namespaces as $part) {
      $pair = explode('|', $part);
      $add_ns[$pair[0]] = $pair[1];
    }
  }
  $val = $form_state['values'][$field_name]['und'][$delta];

  nlmfield_load_library();
  $dom = new BetterDOMDocument("<contrib xmlns='$ns_url' />", $ns);

  // Set additional namespaces
  foreach ($add_ns as $prefix => $url) {
    $dom->documentElement->setAttribute('xmlns:' . $prefix, $url);
    $dom->registerNamespace($prefix, $url);
  }

  if (!empty($val['contributor_type'])) {
    $dom->documentElement->setAttribute("contrib-type", $val['contributor_type']);
  }

  // Array to map the values of element with its xml-tag and parent tag.
  // e.g. if we have values for surname we should have xml like
  // <name><surname>Surname</surname></name> OR
  // if we have only string_name with us then we can have xml like
  // <string-name> String Name </string-name>
  $mappings = array();
  $mappings['string_name'] = array('tag' => 'string-name');
  $mappings['surname'] = array('tag' => 'surname', 'group' => 'name');
  $mappings['given_names'] = array('tag' => 'given-names', 'group' => 'name');
  $mappings['prefix'] = array('tag' => 'prefix', 'group' => 'name');
  $mappings['suffix'] = array('tag' => 'suffix', 'group' => 'name');
  $mappings['email'] = array('tag' => 'email');
  $mappings['role'] = array('tag' => 'role');
  $mappings['aff'] = array('tag' => 'aff');
  $mappings['address'] = array('tag' => 'address');
  $mappings['degrees'] = array('tag' => 'degrees');

  // Initialize empty name variable.
  $name = '';

  // Flag to determine whether to set the values or not.
  $flag = FALSE;

  // Create the name element and sub-elements
  foreach ($mappings as $key => $value) {
    if(!empty($val[$key])) {
      $flag = TRUE;
      $tag = $value['tag'];
      $inner_text = drupal_html_to_text($val[$key]);
      if (isset($value['group']) && $value['group'] == 'name') {
        $name = !empty($name) ? $name : $dom->append("<name />");
        $dom->append("<{$tag}>{$inner_text}</{$tag}>", $name);
      }
      else {
        $dom->append("<{$tag}>{$inner_text}</{$tag}>");
      }
    }
  }

  $xml = NULL;
  if ($flag === TRUE) {
    $xml = $dom->out();
  }
  form_set_value($element, $xml, $form_state);
}
