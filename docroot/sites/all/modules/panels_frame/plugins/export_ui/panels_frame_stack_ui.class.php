<?php

class panels_frame_stack_ui extends panels_frame_ui {

  function hook_menu(&$items) {
    $base = array(
      'access arguments' => array('access content'),
      'type' => MENU_CALLBACK,
      'file' => 'stack-admin.inc',
      'file path' => drupal_get_path('module', 'panels_frame') . '/includes',
      'theme callback' => 'ajax_base_page_theme',
    );

    // $op/$cache_key/$name
    $items['panels_frame/stack/frame/ajax/%/%'] = array(
      'page callback' => 'panels_frame_stack_frame_ajax_delegate',
      'page arguments' => array(4, 5),
    ) + $base;

    parent::hook_menu($items);
  }

  function edit_form(&$form, &$form_state) {
    parent::edit_form($form, $form_state);

    ctools_include('stack-admin', 'panels_frame');
    ctools_include('plugins', 'panels');
    ctools_include('ajax');
    ctools_include('modal');
    ctools_modal_add_js();
    ctools_add_css('panels_dnd', 'panels');
    ctools_add_css('panels-frame.ui-stack', 'panels_frame');

    // Set the cache identifier and immediately set an object cache.
    $form_state['cache_key'] = panels_frame_cache_key($form_state['item']->name);
    if (is_object($cache = panels_frame_cache_get('stack', $form_state['cache_key']))) {
      $form_state['item'] = $cache;
    }
    panels_frame_cache_set('stack', $form_state['cache_key'], $form_state['item']);

    $form['info']['#type'] = 'container';
    $form['info']['#attributes']['class'][] = 'stack-admin-info';

    $form['frames'] = panels_frame_stack_edit_form(array(), $form_state);
    $form['frames']['#type'] = 'container';
    $form['frames']['#attributes']['class'][] = 'stack-admin-frames';

    $form['buttons']['#type'] = 'container';
    $form['buttons']['#attributes']['class'][] = 'stack-admin-buttons';

    $form['info']['plugin'] = array(
      '#type' => 'value',
      '#value' => 'stack',
    );
  }

  function edit_form_submit(&$form, &$form_state) {
    parent::edit_form_submit($form, $form_state);

    // Ensure that the Stack layout theme exists in the theme registry,
    // otherwise, rebuild it.
    $theme_hooks = theme_get_registry();
    if (!isset($theme_hooks['panels_frame_stack'])) {
      drupal_theme_rebuild();
    }
  }
}
