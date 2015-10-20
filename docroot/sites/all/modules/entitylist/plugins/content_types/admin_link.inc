<?php

$plugin = array(
  'title' => t('Admin link'),
  'category' => t('Entity list'),
  'description' => t('Displays an admin link for a given list.'),
  'required context' => new ctools_context_required(t('List'), 'entitylist'),
  'defaults' => array(
    'text' => 'Edit list',
    'destination' => TRUE,
  ),
  'admin title' => 'entitylist_ctools_admin_link_admin_title',
  'admin info' => 'entitylist_ctools_admin_link_admin_info',
  'render callback' => 'entitylist_ctools_admin_link_render',
  'edit form' => 'entitylist_ctools_admin_link_edit_form',
);

function entitylist_ctools_admin_link_render($subtype, $conf, $panel_args, $context) {
  // Get the list.
  $list = entitylist_list_load_from_context($context);
  $path = $list->adminPath();

  // This means that the user doesn't have access.
  if ($path === FALSE) {
    return;
  }

  $block = new stdClass();
  $block->module  = 'entitylist';
  $block->content = theme('entitylist_admin_link', array('conf' => $conf, 'path' => $path));
  return $block;
}

function entitylist_ctools_admin_link_admin_title($subtype, $conf, $context) {
  return t('Admin link for "@list"', array('@list' => $context->identifier));
}

function entitylist_ctools_admin_link_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Link text: @text', array('@text' => $conf['text']));
  $block->content = '';
  return $block;
}

function entitylist_ctools_admin_link_edit_form($form, &$form_state) {
  $form['text'] = array(
    '#type' => 'textfield',
    '#title' => t('Link text'),
    '#default_value' => $form_state['conf']['text'],
  );
  $form['destination'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add a destination parameter'),
    '#description' => t('Enabling this option will redirect the user back to the previous page after any form submits.'),
    '#default_value' => $form_state['conf']['destination'],
  );
  return $form;
}

function entitylist_ctools_admin_link_edit_form_submit($form, &$form_state) {
  foreach (array('text', 'destination') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

function theme_entitylist_admin_link($variables) {
  $options = array();
  if ($variables['conf']['destination']) {
    $options['query'] = array(drupal_get_destination());
  }
  return '<div class="entitylist-admin">' . l($variables['conf']['text'], $variables['path'], $options) . '</div>';
}
