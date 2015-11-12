<?php

/**
 * Settings form.
 */
function maestro_initiator_settings_form($form, &$form_state) {
  $form['maestro_initiator_maestro_version'] = array(
    '#type' => 'textfield',
    '#title' => t('Maestro Engine Version'),
    '#default_value' => maestro_initiator_get_version(),
  );
  $result = module_invoke_all('maestro_enabled_processes');

  if (!empty($result)) {
    $cnt = 0;
    //find all the available workflow templates
    $templates = maestro_template_names();
    $templates[0] = t('None');
    foreach ($result as $module) {
      $form['fs-' . $cnt] = array(
        '#type' => 'fieldset',
        '#title' => $module['title'],
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      foreach ($module['processes'] as $key => $specs) {
        $form['fs-' . $cnt]['maestro_initiator_' . $key] = array(
          '#type' => 'select',
          '#title' => $specs['title'],
          '#options' => $templates,
          '#empty_value' => 0,
          '#default_value' => variable_get('maestro_initiator_' . $key, 0),
        );
      }
      $cnt++;
    }
  }
  else {
    $form['mrkup'] = array(
      '#type' => 'markup',
      '#markup' => t('No configurable processes found.'),
    );
  }
  return system_settings_form($form);
}
