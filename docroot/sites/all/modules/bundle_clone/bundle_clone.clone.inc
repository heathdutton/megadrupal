<?php
/**
 * Bundle field cloning form.
 *
 * Adds ajax to basic form which updates fields list according to chosen source bunlde.
 */
function bundle_clone_admin($form, &$form_state, $entity_type) {
  bundle_clone_config($form, $entity_type);

  $form['config']['source']['#ajax'] = array(
    'callback' => 'bundle_clone_present_fields',
    'wrapper' => 'bundle-clone-fields',
  );

  $form['wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields'),
    '#description' => t('Missing fields will be created and existing will be updated'),
    '#prefix' => '<div id="bundle-clone-fields">',
    '#suffix' => '</div>',
  );

  if (isset($form_state['values']['config']['source'])) {
    $instances = field_info_instances($entity_type, $form_state['values']['config']['source']);
    $options = array('all' => t('All'));
    foreach ($instances as $instance => $info) {
      $options[$instance] =  $info['label'];
    }
    $form['wrapper']['fields'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#multiple' => TRUE,
      '#default_value' => 'all',
      '#required' => TRUE,
    );
  }

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Clone'),
  );
    
  return $form;
}

function bundle_clone_present_fields($form, $form_state) {
  return $form['wrapper'];
}

function bundle_clone_admin_validate(&$form, &$form_state) {
  bundle_clone_validate_selection($form, $form_state);
}

function bundle_clone_admin_submit(&$form, &$form_state) {
  $config = (object) $form_state['values']['config'];
  foreach ($config->targets as $target => $ignore) {
    $config->target = $target;
    $source = $config->source;
    $bundles = $config->bundles;
    $holders = array(
      '!source' => l($bundles[$source]['label'], $bundles[$source]['admin']['real path']),
      '!target' => l($bundles[$target]['label'], $bundles[$target]['admin']['real path']),
    );
    drupal_set_message(t('Cloning !source into !target', $holders));
    unset($form_state['values']['fields']['all']);
    $fields = array_keys($form_state['values']['fields']);
    bundle_clone_clone($config, $fields);
  }
}

function bundle_clone_clone($config, $fields) {
  $instances = field_info_instances($config->entity_type, $config->source);
  if (!$fields) {
    $fields = array_keys($instances);
  }
  
  $target_instances = field_info_instances($config->entity_type, $config->target);
  foreach ($fields as $field) {
    $source = $instances[$field];
    if (isset($target_instances[$field])) {
      $target = $target_instances[$field];
      drupal_set_message(t('Updating @instance', array('@instance' => $field)));
      foreach (array('widget', 'settings', 'display') as $component) {
        $target[$component] = $source[$component] + $target[$component];
      }
      field_update_instance($target);
    }
    else {
      drupal_set_message(t('Creating @instance', array('@instance' => $field)));
      unset($source['id']);
      $source['bundle'] = $config->target;
      field_create_instance($source);
    }

  }
}

