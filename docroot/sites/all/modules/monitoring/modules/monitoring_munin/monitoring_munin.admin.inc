<?php
/**
 * @file
 * Monitoring munin admin pages.
 */

/**
 * Munin graph settings form callback.
 */
function monitoring_munin_graph_settings($form, &$form_state) {

  $form['multigraphs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Munin multigraph definitions'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $options = array();
  foreach (monitoring_munin_multigraphs() as $multigraph) {
    $options[$multigraph['title']] = array('title' => $multigraph['title'], 'vlabel' => $multigraph['vlabel']);
  }

  $form['multigraphs']['delete_multigraphs'] = array(
    '#type' => 'tableselect',
    '#header' => array('title' => t('Title'), 'vlabel' => t('Graph units')),
    '#options' => $options,
  );
  $form['multigraphs']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Multigraph name'),
    '#description' => t('Enter name of a new multigraph.')
  );
  $form['multigraphs']['vlabel'] = array(
    '#type' => 'textfield',
    '#title' => t('Value label'),
    '#description' => t('Enter common value label.')
  );
  $form['multigraphs']['actions']['add_multigraph'] = array(
    '#type' => 'submit',
    '#value' => t('Add multigraph'),
    '#submit' => array('monitoring_munin_multigraph_add_submit')
  );
  $form['multigraphs']['actions']['delete_multigraphs'] = array(
    '#type' => 'submit',
    '#value' => t('Delete selected'),
    '#submit' => array('monitoring_munin_multigraph_delete_submit')
  );

  $options = array();
  foreach (monitoring_munin_multigraphs() as $multigraph) {
    $options[$multigraph['title']] = $multigraph['title'];
  }

  foreach (monitoring_sensor_manager()->getEnabledSensorInfo() as $sensor_name => $sensor_info) {
    $munin_settings = $sensor_info->getSetting('munin', array(
      'multigraphs' => array(),
      'munin_enabled' => FALSE,
      'graph_args' => '',
    ));

    $form[$sensor_name] = array(
      '#type' => 'fieldset',
      '#title' => $sensor_info->getLabel(),
      '#description' => $sensor_info->getDescription(),
      '#tree' => TRUE,
    );
    $form[$sensor_name]['munin_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enabled'),
      '#default_value' => $munin_settings['munin_enabled'],
      '#attributes' => array('id' => $sensor_name . '_enabled'),
    );
    $form[$sensor_name]['multigraphs'] = array(
      '#type' => 'select',
      '#title' => t('Presence in multigraphs'),
      '#multiple' => TRUE,
      '#options' => $options,
      '#default_value' => $munin_settings['multigraphs'],
      '#states' => array(
        'visible' => array(
          ':input[id="' . $sensor_name . '_enabled"]' => array('checked' => TRUE),
        ),
      ),
    );
    // @todo - we need to provide such setting either for single graph or multi
    //   definition.
//    $form[$sensor_name]['graph_args'] = array(
//      '#type' => 'textfield',
//      '#title' => t('Graph arguments'),
//      '#default_value' => $munin_settings['graph_args'],
//    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Munin graph settings submit callback.
 */
function monitoring_munin_graph_settings_submit($form, &$form_state) {
  foreach (monitoring_sensor_manager()->getEnabledSensorInfo() as $sensor_name => $sensor_info) {
    if (!empty($form_state['values'][$sensor_name])) {
      $settings = monitoring_sensor_settings_get($sensor_name);
      $settings['munin'] = $form_state['values'][$sensor_name];
      monitoring_sensor_settings_save($sensor_name, $settings);
    }
  }
}

/**
 * Munin multigraph save submit callback.
 */
function monitoring_munin_multigraph_add_submit($form, &$form_state) {
  if (!empty($form_state['values']['title'])) {
    $title = $form_state['values']['title'];
    monitoring_munin_multigraph_save($title, $form_state['values']['vlabel']);
  }
}

/**
 * Munin multigraph delete submit callback.
 */
function monitoring_munin_multigraph_delete_submit($form, &$form_state) {
  foreach ($form_state['values']['delete_multigraphs'] as $title) {
    monitoring_munin_multigraph_delete($title);
  }
}

/**
 * Configuration page callback.
 */
function monitoring_munin_config_page() {

  $conf_path = explode('/', conf_path());

  return array(
    '#theme' => 'monitoring_config_box',
    '#title' => t('Command definition to collect sensor data'),
    '#description' => t('Create file /etc/munin/plugins/@site_id with following code, make it executable and restart munin-node service.',
      array('@site_id' => str_replace('.', '_', monitoring_host()))),
    '#code' => monitoring_config_code('monitoring_munin', 'command', array(
      '@name' => monitoring_host(),
      '@root' => DRUPAL_ROOT,
      '@uri' => array_pop($conf_path),
    )),
    '#code_height' => '240',
  );
}
