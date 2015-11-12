<?php

class entitylist_ui_lists extends ctools_export_ui {

  function edit_form(&$form, &$form_state) {
    $item = $form_state['item'];

    // Basics.
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => $item->title,
      '#required' => TRUE,
    );
    $form['name'] = array(
      '#type' => 'machine_name',
      '#title' => t('Machine-readable name'),
      '#default_value' => $item->name,
      '#required' => TRUE,
      '#machine_name' => array(
        'exists' => 'entitylist_list_load',
        'source' => array('title'),
      ),
    );
    $form['description'] = array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#default_value' => $item->description,
    );

    $form['content_type'] = array(
      '#type' => 'checkbox',
      '#title' => t('Create CTools content type plugins for each entity in this list'),
      '#description' => t('This will create a CTools content type plugin for each entity in this panel that you freely can place in Panels. %warning', array('%warning' => t('Warning: Only check this if you know what you are doing.'))),
      '#default_value' => $item->content_type,
    );

    // Handlers.
    $handlers = entitylist_get_handler_plugins();
    $options = array();
    foreach ($handlers as $key => $handler) {
      $options[$key] = array(
        'name' => $handler['name'],
        'description' => $handler['description'],
      );
    }
    $form['handler_plugin'] = array(
      '#prefix' => '<label>' . t('Handler') . '</label>',
      '#type' => 'tableselect',
      '#required' => TRUE,
      '#multiple' => FALSE,
      '#header' => array(
        'name' => t('Name'),
        'description' => t('Description'),
      ),
      '#options' => $options,
      '#default_value' => $item->handler_plugin,
    );
  }

  function edit_form_submit(&$form, &$form_state) {
    $item = $form_state['item'];

    $item->name = $form_state['values']['name'];
    $item->title = $form_state['values']['title'];
    $item->description = $form_state['values']['description'];
    $item->handler_plugin = $form_state['values']['handler_plugin'];
    $item->content_type = $form_state['values']['content_type'];
  }

  function edit_form_settings(&$form, &$form_state) {
    $item = $form_state['item'];
    $handler = new $item->handler_plugin((array)$item->handler_config);
    $form['handler_config'] = $handler->configForm($form_state);
    if (!empty($form['handler_config'])) {
      $form['handler_config']['#tree'] = TRUE;
    }
  }

  function edit_form_settings_submit(&$form, &$form_state) {
    $item = $form_state['item'];
    if (!empty($form_state['values']['handler_config'])) {
      $item->handler_config = $form_state['values']['handler_config'];
    }
    else {
      $item->handler_config = array();
    }
  }

  function edit_form_preview(&$form, &$form_state) {

  }

  function edit_form_preview_submit(&$form, &$form_state) {

  }

}
