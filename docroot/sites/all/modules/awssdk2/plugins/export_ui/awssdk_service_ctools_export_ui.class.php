<?php

class awssdk_service_ctools_export_ui extends ctools_export_ui {

  function edit_form(&$form, &$form_state) {
    parent::edit_form($form, $form_state);
    if ($form_state['step'] == 'config') {
      $form['info']['name']['#disabled'] = TRUE;
    }
    if (!empty($form['info']['name']['#value'])) {
      return;
    }
    ctools_include('plugins');
    $plugins = ctools_get_plugins('awssdk2', 'service');
    $options = array();
    foreach ($plugins as $plugin) {
      $plugin = (object) $plugin;
      $options[$plugin->name] = $plugin->title;
    }
    $form['info']['name']['#type'] = 'select';
    $form['info']['name']['#options'] = $options;
  }
}