<?php


/**
 * @file
 * contains the hook implementation for field API.
 */

/**
 * Implements hook_field_formatter_info().
 */
function og_formatters_field_formatter_info() {
  return array(
    'og_list_delimited' => array(
      'label' => t('Group delimited list'),
      'field types' => array('group'),
      'settings'  => array(
        'links_option' => FALSE,
        'separator_option' => ', ',
        'element_option' => '- None -',
        'wrapper_option' => '- None -',
        'element_class' => '',
        'wrapper_class' => '',
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function og_formatters_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $element = array();
  $element['links_option'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Links'),
    '#description'    => t('When checked groups will be displayed as links'),
    '#default_value'  => $settings['links_option'],
  );
  $element['separator_option'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Separator'),
    '#description'    => t('The separator to use, including leading and trailing spaces'),
    '#default_value'  => $settings['separator_option'],
  );
  $element['element_option'] = array(
    '#type'           => 'select',
    '#title'          => t('Element'),
    '#description'    => t('The HTML element to wrap each group in'),
    '#default_value'  => $settings['element_option'],
    '#options'        => array(
      '- None -'  => '- None -',
      'span'      => 'span',
      'h1'        => 'h1',
      'h2'        => 'h2',
      'h3'        => 'h3',
      'h4'        => 'h4',
      'h5'        => 'h5',
      'strong'    => 'h6',
      'em'        => 'h7',
    ),
  );
  $element['element_class'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Element Class'),
    '#description'    => t('The class assigned to the element'),
    '#default_value'  => $settings['element_class'],
  );
  $element['wrapper_option'] = array(
    '#type'           => 'select',
    '#title'          => t('Wrapper'),
    '#description'    => t('The HTML element to wrap the entire collection in'),
    '#default_value'  => $settings['wrapper_option'],
    '#options'        => array(
      '- None -'  => '- None -',
      'div'       => 'div',
      'span'      => 'span',
      'h1'        => 'h1',
      'h2'        => 'h2',
      'h3'        => 'h3',
      'h4'        => 'h4',
      'h5'        => 'h5',
      'p'         => 'p',
      'strong'    => 'strong',
      'em'        => 'em',
    ),
  );
  $element['wrapper_class'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Wrapper Class'),
    '#description'    => t('The class assigned to the wrapper'),
    '#default_value'  => $settings['wrapper_class'],
  );
  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function og_formatters_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $summary = t('The groups will be displayed separated by "@separator"', array('@separator' => $settings['separator_option']));
  if ($settings['links_option']) {
    $summary .= t('<br />The groups will link to the term pages');
  }
  if ($settings['element_option'] != "- None -") {
    $summary .= t('<br />Elements will be wrapped in a "@element" tag', array('@element' => $settings['element_option']));
    if (!empty($settings['element_class'])) {
      $summary .= t(' with the class of @elemclass', array('@elemclass' => $settings['element_class']));
    }
  }
  if ($settings['wrapper_option'] != "- None -") {
    $summary .= t('<br />The entire list will be wrapped in a "@wrapper" tag', array('@wrapper' => $settings['wrapper_option']));
    if (!empty($settings['wrapper_class'])) {
      $summary .= t(' with the class of @wrapclass', array('@wrapclass' => $settings['wrapper_class']));
    }
  }
  return $summary;
}

/**
 * Implements hook_field_formatter_view().
 */
function og_formatters_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  if (empty($items)) {
    return '';
  }

  $settings = $display['settings'];
  $element = array();
  $separator = check_plain($settings['separator_option']);
  if ($settings['element_option'] != '- None -') {
    $elementwrap[0] = '<' . $settings['element_option'] . ' class="' . check_plain($settings['element_class']) . '">';
    $elementwrap[1] = '</' . $settings['element_option'] . '>';
  }
  else {
    $elementwrap[0] = '';
    $elementwrap[1] = '';
  }
  if ($settings['wrapper_option'] != '- None -') {
    $wrapper[0] = '<' . $settings['wrapper_option'] . ' class="' . check_plain($settings['wrapper_class']) . '">';
    $wrapper[1] = '</' . $settings['wrapper_option'] . '>';
  }
  else {
    $wrapper[0] = '';
    $wrapper[1] = '';
  }
  $formatted = '';

  foreach ($items as $delta => $item) {
    $group_id = $item['gid'];
    $group = entity_load('group', array($group_id));
    $node_id = $group[$group_id]->etid;
    $node = entity_load('node', array($node_id));
    $uri = entity_uri('node', $node[$node_id]);

    if ($settings['links_option']) {
      $formatted .= $elementwrap[0] . l($group[$group_id]->label, $uri['path'], $uri['options']) . $elementwrap[1] . $separator;
    }
    else {
      $formatted .= $elementwrap[0] . check_plain($group[$group_id]->label) . $elementwrap[1] . $separator;
    }
  }
  $length = strlen($separator);
  $formatted = substr($formatted, 0, -($length));
  $formatted = $wrapper[0] . $formatted . $wrapper[1];
  $element[0]['#markup'] = $formatted;
  return $element;
}
