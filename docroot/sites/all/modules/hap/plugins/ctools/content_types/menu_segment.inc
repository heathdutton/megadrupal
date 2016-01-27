<?php

/**
 * Content-type plugin to render a segment of a menu tree.
 */
$plugin = array(
  'title' => t('★ Menu Segment'),
  'description' => t('Render a segment of a menu tree'),
  'category' => t('Miscellaneous'),
  'defaults' => array(
    'menu' => 'main-menu',
    'link_path' => '',
    'include_self' => TRUE,
  ),
);

/**
 * Settings form callback.
 */
function hap_menu_segment_content_type_edit_form($form, &$form_state) {
  $form['menu'] = array(
    '#type' => 'select',
    '#title' => t('Menu'),
    '#options' => module_exists('menu') ? menu_get_menus() : menu_list_system_menus(),
    '#default_value' => $form_state['conf']['menu'],
  );

  $form['link_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Link path'),
    '#description' => t('Type the menu link path that should be the parent of the render menu.'),
    '#default_value' => $form_state['conf']['link_path'],
  );

  $form['include_self'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include self'),
    '#description' => t('If checked, will render the menu link itself as a single item in an unordered list.'),
    '#default_value' => $form_state['conf']['include_self'],
  );

  return $form;
}

/**
 * Submission handler for settings callback.
 */
function hap_menu_segment_content_type_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Admin title callback.
 */
function hap_menu_segment_content_type_admin_title($subtype, $conf, $context) {
  $plugin = ctools_get_content_type('menu_segment');
  $replacements = array(
    '!title' => $plugin['title'],
    '!path' => empty($conf['link_path']) ? '/' : $conf['link_path'],
    '!extra' => (isset($conf['include_self']) && $conf['include_self']) ? t('(includes self)') : '',
  );

  return t('!title: !path !extra', $replacements);
}

/**
 * Admin info callback.
 */
function hap_menu_segment_content_type_admin_info($subtype, $conf) {
  $menus = module_exists('menu') ? menu_get_menus() : menu_list_system_menus();
  $replacements = array(
    '%menu' => $menus[$conf['menu']],
  );

  $element = new stdClass();
  $element->title = t('From %menu', $replacements);
  $element->content= 'ヽ(´ｰ｀ )ﾉ';

  return $element;
}

/**
 * Render callback.
 */
function hap_menu_segment_content_type_render($subtype, $conf, $panel_args, $context) {
  hap_include('menu');
  $segment = hap_menu_tree_segment($conf['menu'], $conf['link_path'], $conf['include_self']);

  $element = new stdClass();
  $element->module = 'hap';
  $element->content = $segment;

  return $element;
}
