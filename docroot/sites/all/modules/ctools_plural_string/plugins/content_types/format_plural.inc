<?php

/**
 * @file
 * Plugin provides a panel pane that displays plural string (including count of items string).
 */

/**
 * Plugins are described by creating a $plugin array which will be used by the system that includes this file.
 */
$plugin = array(
  'title' => t('Count of items (plural string)'),
  'single' => TRUE,
  'category' => t('Miscellaneous'),
  'defaults' => array(
    'number' => '',
    'singular' => '',
    'plural' => '',
    'link' => '',
    'show_zero' => FALSE,
    'sanitize' => TRUE,
  ),
  'all contexts' => TRUE,
);

/**
 * Render the content type.
 */
function ctools_plural_string_format_plural_content_type_render($subtype, $conf, $args, $contexts) {
  if (!is_array($contexts)) {
    $contexts = array($contexts);
  }

  $sanitize = !empty($conf['sanitize']);

  $number = NULL;
  $link = NULL;

  if (isset($conf['number'])) {
    $number = trim($conf['number']);
  }

  if (isset($conf['link'])) {
    $link = trim($conf['link']);
  }

  if (!empty($contexts)) {
    $number = trim(ctools_context_keyword_substitute($number, array(), $contexts));
    $link = trim(ctools_context_keyword_substitute($link, array(), $contexts));
  }

  if (!is_numeric($number)) {
    return FALSE;
  }

  $number = intval($number);

  if (empty($conf['show_zero']) && $number === 0) {
    return FALSE;
  }

  $string = format_plural($number, trim($conf['singular']), trim($conf['plural']));

  if (!empty($link)) {
    $url_parts = _ctools_plural_string_parse_url($link);
    $string = l($string, $url_parts['url'], array(
        'query' => isset($url_parts['query']) ? $url_parts['query'] : NULL,
        'fragment' => isset($url_parts['fragment']) ? $url_parts['fragment'] : NULL,
        'html' => !$sanitize,
      )
    );
  }

  $block = new stdClass();
  $block->title = !empty($conf['override_title']) ? check_plain($conf['override_title_text']) : '';
  $block->content = $string;

  return $block;
}

/**
 * Edit form callback for the content type.
 */
function ctools_plural_string_format_plural_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['number'] = array(
    '#type' => 'textfield',
    '#title' => t('Number (count of items)'),
    '#description' => t('Context keywords (see below) evaluating as numbers can be used.'),
    '#required' => TRUE,
    '#size' => 60,
    '#default_value' => isset($conf['number']) ? $conf['number'] : '',
  );

  $form['singular'] = array(
    '#type' => 'textfield',
    '#title' => t('String for the singular case'),
    '#description' => t('Example: 1 new comment'),
    '#required' => TRUE,
    '#size' => 60,
    '#default_value' => isset($conf['singular']) ? $conf['singular'] : '',
  );

  $form['plural'] = array(
    '#type' => 'textfield',
    '#title' => t('String for the plural case'),
    '#description' => t('Example: @count new comments'),
    '#required' => TRUE,
    '#size' => 60,
    '#default_value' => isset($conf['plural']) ? $conf['plural'] : '',
  );

  $form['link'] = array(
    '#type' => 'textfield',
    '#title' => t('Output string as a link'),
    '#description' => t('The Drupal path or absolute URL for this link. The link is optional.'),
    '#required' => FALSE,
    '#size' => 60,
    '#default_value' => isset($conf['link']) ? $conf['link'] : '',
  );

  $form['show_zero'] = array(
    '#type' => 'checkbox',
    '#default_value' => !empty($conf['show_zero']),
    '#title' => t('Display zero value'),
    '#description' => t('When enabled zero values will be shown.'),
  );

  $form['sanitize'] = array(
    '#type' => 'checkbox',
    '#default_value' => !empty($conf['sanitize']),
    '#title' => t('Sanitize'),
    '#description' => t('Use this option to stripped the string from dangerous HTML.'),
  );

  if (!empty($form_state['contexts'])) {
    $form['contexts'] = array(
      '#title' => t('Substitutions'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $rows = array();

    foreach ($form_state['contexts'] as $context) {
      foreach (ctools_context_get_converters('%' . check_plain($context->keyword) . ':', $context) as $keyword => $title) {
        $rows[] = array(
          check_plain($keyword),
          t('@identifier: @title', array('@title' => $title, '@identifier' => $context->identifier)),
        );
      }
    }

    $header = array(t('Keyword'), t('Value'));
    $form['contexts']['context'] = array(
      '#markup' => theme('table', array('header' => $header, 'rows' => $rows)),
    );
  }

  return $form;
}

/**
 * The submit form stores the data in $conf.
 */
function ctools_plural_string_format_plural_content_type_edit_form_submit($form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Provide a summary description.
 */
function ctools_plural_string_format_plural_content_type_summary($conf, $context) {
  return 'format_plural: ' . check_plain($conf['plural']);
}
