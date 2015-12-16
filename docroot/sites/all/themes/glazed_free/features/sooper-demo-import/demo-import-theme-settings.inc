<?php

$form['glazed_settings']['demo_import'] = array(
  '#title' => t('Demo Import'),
  '#type' => 'fieldset',
  '#description' => t("Demo content packages will help learn to use Glazed elements and modules. If you are on very poor hosting import packages one by one to avoid timeout issues. If you disable a demo content package it won't delete any content! Imported content can only be removed manually."),
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
   '#weight' => 4,
);
if (module_exists('uuid_features')) {
  module_load_include('module', 'uuid');
  module_load_include('inc', 'features', 'features.admin');
  $demo_content_modules = array_filter(_features_get_features_list(), "_glazed_is_demo_content");

  usort($demo_content_modules, function($a, $b) {
    return (count($a->info['features']['uuid_node']) < count($b->info['features']['uuid_node'])) ? 1 : -1;
  });

  foreach ($demo_content_modules as $module) {
    if (isset($module->info['features']) && isset($module->info['features']['uuid_node'])) {
      $node_sample = $module->info['features']['uuid_node'][0];
      if (entity_get_id_by_uuid('node', array($node_sample))) {
        $checkbox = 1;
      }
      else {
        $checkbox = 0;
      }

      $form['glazed_settings']['demo_import'][$module->name] = array(
        '#type' => 'checkbox',
        '#title' => $module->info['name'],
        '#description' => count($module->info['features']['uuid_node']) . ' ' . t('Pages'),
        '#default_value' => $checkbox,
      );
    }
  }
}
else {
  $form['glazed_settings']['demo_import']['#prefix'] = '<p><em>' . 'Please enable the UUID Features module to enable demo import' . '</em></p>';
}
