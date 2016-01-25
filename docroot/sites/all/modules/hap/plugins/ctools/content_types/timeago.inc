<?php

// node context required
// get time field
// convert to seconds (unix timestamp)
// pass through format_interval()

$plugin = array(
  'title' => t('★ Time Ago..'),
  'description' => t('Formats a time interval with requested granularity.'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'category' => t('Node'),
  'defaults' => array(
    'granularity' => 2,
    'field' => 'created',
    'js_render' => 0,
  ),
  'js' => ctools_add_js('timeago', 'hap'),
);

/**
 * Settings callback.
 */
function hap_timeago_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['granularity'] = array(
    '#type' => 'textfield',
    '#title' => t('Granularity'),
    '#description' => t('How many different units to display in the string.'),
    '#default_value' => $conf['granularity'],
    '#element_validate' => array('element_validate_integer_positive'),
    '#maxlength' => 1,
    '#size' => 1,
  );

  $form['field'] = array(
    '#type' => 'select',
    '#title' => t('Field'),
    '#description' => t('Specify the field on the node that represents the timestamp.'),
    '#options' => array(
      'created' => t('Created'),
      'changed' => t('Changed'),
    ),
    '#default_value' => $conf['field'],
  );

  $form['js_render'] = array(
    '#type' => 'checkbox',
    '#title' => t('Render using javascript'),
    '#description' => t('If checked, will render the from js. Using this to compensate heavy caching like a reverse proxy.'),
    '#default_value' => $conf['js_render'],
  );

  return $form;
}

/**
 * Submission handler for settings callback.
 */
function hap_timeago_content_type_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Render callback.
 */
function hap_timeago_content_type_render($subtype, $conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    return;
  }

  $node = $context->data;
  $time = $node->{$conf['field']};

  // If hap_publication_date is enabled we use the custom field.
  if (module_exists('hap_publication_date') && !empty($node->published)) {
    $time = $node->published;
  }

  $class = '';
  if ($conf['js_render']) {
    $nid = $node->nid;
    drupal_add_js(array('hap_timeago' => array('node-' . $nid => $time)), 'setting');
    $class = 'time-ago-node-' . $nid;
  }

  $format_interval = format_interval(REQUEST_TIME - $time, $conf['granularity']);
  $element = new stdClass();
  $element->module = 'hap';
  $element->content = '<span class="time-ago ' . $class . '">' . t('!time ago', array('!time' => $format_interval)) . '</span>';

  return $element;
}

