<?php

/**
 * @file
 * Provides field formatters for full xml and tokenized display.
 */
 
/**
 * Implements hook_field_formatter_info().
 */
function nlmfield_field_formatter_info() {
  $formatters = array(
    'nlmfield_xml' => array(
      'label' => t('Full XML'),
      'field types' => array('nlmfield_contributor'),
    ),
    'nlmfield_token_preset' => array(
      'label' => t('Preset Tokenized Display'),
      'field types' => array('nlmfield_contributor'),
      'settings' => array('token_preset' => 'fullname'),
    ),
    'nlmfield_token_custom' => array(
      'label' => t('Custom Tokenized Display'),
      'field types' => array('nlmfield_contributor'),
      'settings' => array('token_custom' => ''),
    ),
  );
  return $formatters;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function nlmfield_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  
  // Preset formatter
  if ($display['type'] == 'nlmfield_token_preset') {
    $presets = nlmfield_presets('nlmfield_contributor');
    $preset_options = array();
    foreach ($presets as $preset) {
      $preset_options[$preset['name']] = $preset['title'];
    }
    
    $element['token_preset'] = array(
      '#title' => t('Preset Token Formatter'),
      '#type' => 'select',
      '#default_value' => $settings['token_preset'] ? $settings['token_preset'] : '',
      '#required' => TRUE,
      '#options' => $preset_options,
    );
  }
  
  // Custom formatter
  if ($display['type'] == 'nlmfield_token_custom') {
    
    $desc =  "Patterns are processed according to the following rules:
                <ul>
                  <li>All bit-tokens are replaced with their extracted values</li>
                  <li>If a empty bit-token is followed by a space, the following space is ignored</li>
                  <li>You may use a ?<bit-token> to indicate that the following character should only be included if the bit is not empty</li>
                  <li>You may escape characters using \</li>
                </ul>";
    
    $bits = nlmfield_bits('nlmfield_contributor');
    
    $desc .= "<b>Bit Tokens</b><ul>";
    foreach ($bits as $bit) {
      $desc .= "<li><b>" . $bit['char'] . "</b> - " . $bit['title'] . "</li>";
    }
    $desc .= "</ul>";
    
    $element['token_custom'] = array(
      '#title' => t('Custom Token Formatter'),
      '#type' => 'textfield',
      '#default_value' => $settings['token_custom'] ? $settings['token_custom'] : '',
      '#required' => TRUE,
      '#description' => filter_xss($desc),
    );
  }
  
  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function nlmfield_field_formatter_settings_summary($field, $instance, $view_mode) {
  $formatter = $instance['display'][$view_mode]['type'];
  
  if ($formatter == 'nlmfield_token_preset') {
    $presets = nlmfield_presets($field['type']);
    $preset_name = $instance['display'][$view_mode]['settings']['token_preset'];
    return 'Using ' . $presets[$preset_name]['title'];
  }
  if ($formatter == 'nlmfield_token_custom') {
    return 'Using custom pattern';
  }
}

/**
 * Implements hook_field_formatter_view().
 */
function nlmfield_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  
  switch ($display['type']) {
    case 'nlmfield_xml':
      foreach ($items as $delta => $item) {
        $element[$delta] = array('#markup' => '<pre>' . htmlspecialchars($item['xml']) . '</pre>');
      }
      return $element;
    case 'nlmfield_token_preset':
      foreach ($items as $delta => $item) {
        $element[$delta] = array('#markup' => nlmfield_process_preset($field['type'], $display['settings']['token_preset'], $item['xml']));
      }
      return $element;
    case 'nlmfield_token_custom':
      foreach ($items as $delta => $item) {
        $element[$delta] = array('#markup' => nlmfield_process_pattern($field['type'], $display['settings']['token_custom'], $item['xml']));
      }
      return $element;
  }

  return $element;
}