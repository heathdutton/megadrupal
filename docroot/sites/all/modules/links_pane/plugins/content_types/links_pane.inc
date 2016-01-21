<?php

/**
 * @file
 * Links pane plugin.
 */

$plugin = array(
  'title' => t('Links pane'),
  'single' => TRUE,
  'description' => t('A pane with custom links.'),
  'category' => t('Miscellaneous'),
  'all contexts' => TRUE,
  'defaults' => array(
    'admin_title' => '',
    'links' => array(),
    'list_type' => 'ul',
    'list_separator' => '',
  ),
);

function links_pane_links_pane_content_type_render($subtype, $conf, $args, $context) {
  $block = new stdClass();
  $block->module = 'links_pane';
  $block->title = !empty($conf['override_title']) ? check_plain($conf['override_title_text']) : '';
  $block->content = links_pane_links_pane_content_type_render_links($conf, $context);

  return $block;
}

function links_pane_links_pane_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  ctools_include('dependent');

  $form['admin_title'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($conf['admin_title']) ? $conf['admin_title'] : '',
    '#title' => t('Administrative title'),
    '#description' => t('This title will be used administratively to identify this pane. If blank, the regular title will be used.'),
  );

  $form['list_type'] = array(
    '#type' => 'radios',
    '#title' => t('List type'),
    '#options' => array(
      'ul' => t('Unordered list'),
      'ol' => t('Ordered list'),
      'separator' => t('Simple separator'),
    ),
    '#default_value' => $conf['list_type'],
  );

  $form['list_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Separator'),
    '#default_value' => $conf['list_separator'],
    '#dependency' => array('radio:list_type' => array('separator')),
  );

  $form['links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Links'),
    '#tree' => TRUE,
  );

  $form['links']['help'] = array(
    '#markup' => t('To remove the link, set its path and title as empty strings. To output just the title without a link, set path as empty string.'),
  );
  $form['links']['table'] = array(
    '#theme' => 'links_pane_config_form_table',
  );

  // Add new link form elements.
  $conf['links'][] = array(
    'title' => '',
    'path' => '',
    'class' => '',
    'rel' => '',
    'wrapper' => '',
    'wrapper_class' => '',
    'html_title' => FALSE,
    'translate_title' => FALSE,
  );

  $delta = 0;

  foreach ($conf['links'] as $link) {
    $delta++;
    $form['links']['table'][$delta] = array();

    $form['links']['table'][$delta]['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#size' => 15,
      '#default_value' => $link['title'],
    );

    $form['links']['table'][$delta]['path'] = array(
      '#type' => 'textfield',
      '#title' => t('Drupal path'),
      '#size' => 15,
      '#default_value' => $link['path'],
    );

    $form['links']['table'][$delta]['class'] = array(
      '#type' => 'textfield',
      '#title' => t('Link class'),
      '#size' => 10,
      '#default_value' => $link['class'],
    );

    $form['links']['table'][$delta]['rel'] = array(
      '#type' => 'textfield',
      '#title' => t('Rel Text'),
      '#size' => 10,
      '#default_value' => $link['rel'],
    );

    $form['links']['table'][$delta]['wrapper'] = array(
      '#type' => 'select',
      '#title' => t('Link wrapper'),
      '#options' => array(
        '' => t('- None -'),
        'div' => 'DIV',
        'span' => 'SPAN',
        'h2' => 'H2',
        'h3' => 'H3',
        'h4' => 'H4',
        'h5' => 'H5',
        'h6' => 'H6',
        'p' => 'P',
        'strong' => 'STRONG',
        'em' => 'EM',
      ),
      '#default_value' => $link['wrapper'],
    );

    $form['links']['table'][$delta]['wrapper_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Wrapper class'),
      '#size' => 10,
      '#default_value' => $link['wrapper_class'],
    );

    $form['links']['table'][$delta]['html_title'] = array(
      '#type' => 'checkbox',
      '#title' => t('Title contains HTML'),
      '#default_value' => !empty($link['html_title']),
    );

    $form['links']['table'][$delta]['translate_title'] = array(
      '#type' => 'checkbox',
      '#title' => t('Translate title'),
      '#default_value' => !empty($link['translate_title']),
    );

    $form['links']['table'][$delta]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#default_value' => $delta,
      '#delta' => 10,
      '#attributes' => array('class' => array('link-weight')),
    );
  }

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

  return $form;
}

function links_pane_links_pane_content_type_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
  if (isset($form_state['conf']['links'])) {
    $data = isset($form_state['conf']['links']['table']) ? $form_state['conf']['links']['table'] : array();
    $links = array();
    foreach ($data as $link) {
      if ((empty($link['path'])) && ((!isset($link['title'])) || (strlen($link['title']) === 0))) {
        continue;
      }
      $weight = $link['weight'];
      unset($link['weight']);
      $links[$weight] = $link;
    }
    ksort($links, SORT_NUMERIC);
    $form_state['conf']['links'] = array_values($links);
  }
}

function links_pane_links_pane_content_type_admin_title($subtype, $conf) {
  return !empty($conf['admin_title']) ? $conf['admin_title'] : t('Custom links');
}

function links_pane_links_pane_content_type_render_links($conf, $contexts) {
  if (!is_array($contexts)) {
    $contexts = array($contexts);
  }

  $result = '';

  $links = $conf['links'];
  if (!is_array($links)) {
    return $result;
  }

  $link_list = array();

  foreach ($links as $link) {
    if (!isset($link['title']) && !isset($link['path'])) {
      continue;
    }
    $title = isset($link['title']) ? $link['title'] : '';
    if (!empty($link['translate_title'])) {
      $title = t($title);
    }
    if (!empty($contexts)) {
      $title = ctools_context_keyword_substitute($title, array(), $contexts);
    }
    $title = empty($link['html_title']) ? check_plain($title) : filter_xss_admin($title);
    $path = isset($link['path']) ? $link['path'] : '';
    if (!empty($contexts)) {
      $path = ctools_context_keyword_substitute($path, array(), $contexts);
    }
    $options = array('html' => TRUE);
    if (!empty($link['class'])) {
      $options['attributes']['class'] = explode(' ', trim($link['class']));
    }
    if (!empty($link['rel'])) {
      $options['attributes']['rel'] = check_plain($link['rel']);
    }

    $link_element = !empty($path) ? l($title, $path, $options) : theme('html_tag', array('element' => array('#tag' => 'span', '#value' => $title)));

    if (!empty($link['wrapper'])) {
      $wrapper = array('#tag' => check_plain($link['wrapper']), '#value' => $link_element);
      if (!empty($link['wrapper_class'])) {
        $wrapper['#attributes']['class'] = explode(' ', trim($link['wrapper_class']));
      }
      $link_element = theme('html_tag', array('element' => $wrapper));
    }

    $link_list[] = $link_element;
  }

  if ($conf['list_type'] === 'separator') {
    $result = implode(check_plain($conf['list_separator']), $link_list);
  }
  else {
    $result = theme('item_list', array(
      'items' => $link_list,
      'title' => NULL,
      'type' => $conf['list_type'],
    ));
  }

  return $result;
}
