<?php

/**
 * Bundle display settings cloning form.
 */
function bundle_clone_admin_display($form, &$form_state, $entity_type) {
  bundle_clone_config($form, $entity_type);

  $info = entity_get_info($entity_type);
  $options = array();
  foreach ($info['view modes'] as $mode => $definition) {
    $options[$mode] = $definition['label'];
  }
  $form['view_mode'] = array(
    '#type' => 'select',
    '#title' => t('View mode'),
    '#options' => $options,
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Update display'),
  );
    
  return $form;
}

function bundle_clone_admin_display_validate(&$form, &$form_state) {
  bundle_clone_validate_selection($form, $form_state);
}

function bundle_clone_admin_display_submit(&$form, &$form_state) {
  $config = (object) $form_state['values']['config'];
  foreach ($config->targets as $target) {
    $config->target = $target;
    $source = $config->source;
    $bundles = $config->bundles;
    $view_mode = $form_state['values']['view_mode'];
    $holders = array(
      '!source' => l($bundles[$source]['label'], $bundles[$source]['admin']['real path']),
      '!target' => l($bundles[$target]['label'], $bundles[$target]['admin']['real path']),
      '@view_mode' => $form['view_mode']['#options'][$view_mode],
    );
    drupal_set_message(t('Updating display @view_mode !source into !target', $holders));
    bundle_clone_update_display($config, $view_mode);
  }
}

function bundle_clone_update_display($config, $view_mode) {
  $instances = field_info_instances($config->entity_type, $config->source);
  $target_instances = field_info_instances($config->entity_type, $config->target);
  foreach ($target_instances as $instance => $target) {
    if (!isset($instances[$instance])) {
      drupal_set_message(t('Skipping @instance', array('@instance' => $instance)));
      continue;
    }
    drupal_set_message(t('Updating @instance', array('@instance' => $instance)));
    if (!isset($target['display'][$view_mode])) {
      $view_mode = 'default';
    }
    $target['display'][$view_mode] = 
      $instances[$instance]['display'][$view_mode] + $target['display'][$view_mode];
    field_update_instance($target);
  }
}

