<?php

$plugin = array(
  'title' => t('★ Horizontal Rule'),
  'description' => t('Renders a horizontal rule (<hr>) html element.'),
  'category' => t('Miscellaneous'),
  'defaults' => array(
    'class' => '',
  ),
);

/**
 * Render callback.
 */
function hap_hr_content_type_render($subtype, $conf, $panel_args, $context) {
  $output = '<hr ';
  $output .= !empty($conf['class']) ? 'class="' . $conf['class'] . '" ' : '';
  $output .= '/>';

  $element = new stdClass();
  $element->module = 'hap';
  $element->content = $output;

  return $element;

}

/**
 * Settings form callback.
 */
function hap_hr_content_type_edit_form($form, &$form_state) {
  $form['class'] = array(
    '#type' => 'textfield',
    '#title' => t('Class'),
    '#description' => t('Optionally add a class to your horizontal rule.'),
    '#default_value' => $form_state['conf']['class'],
  );

  return $form;
}

/**
 * Submission handler for settings callback.
 */
function hap_hr_content_type_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Admin title callback.
 */
function hap_hr_content_type_admin_title($subtype, $conf, $context) {
  $plugin = ctools_get_content_type('hr');
  $replacements = array(
    '!title' => $plugin['title'],
  );

  return t('!title: <------------------------------------->', $replacements);
}

/**
 * Admin info callback.
 */
function hap_hr_content_type_admin_info($subtype, $conf) {
  $output = !empty($conf['class']) ? t('Class of "%class"', array('%class' => $conf['class'])) : t('No class');
  $element = new stdClass();
  $element->title = $output;
  $element->content= 'ヽ(´ｰ｀ )ﾉ';

  return $element;
}
