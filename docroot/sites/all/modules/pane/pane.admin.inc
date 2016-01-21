<?php
/**
 * List Panes form callback.
 */
function pane_panes_list($form, &$form_state) {
  $header = array(
    'title' => array('data' => t('Title')),
    'plugin' => array('data' => t('Type')),
    'description' => array('data' => t('Description')),
    'operations' => array('data' => t('Operations')),
  );

  $plugins = pane_registry();

  $per_page = 10;
  $panes = pane_get_containers();
  $total = count($panes);
  $current_page = pager_default_initialize($total, $per_page, $element = 0);
  $chunks = array_chunk($panes, $per_page, TRUE);

  $rows = array();
  if (!empty($chunks)) {
    foreach ($chunks[$current_page] as $pane) {
      $rows[$pane->name] = array(
        'title' => filter_xss($pane->title),
        'plugin' => $plugins[$pane->plugin]['title'],
        'description' => check_plain($pane->description),
        'operations' => l(t('configure'), "admin/structure/pane/$pane->name/configure") . ' | ' . l(t('edit content'), "admin/structure/pane/$pane->name/edit"),
      );
    }
  }

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('There are no panes.'),
  );

  $form['pager'] = array(
    '#theme' => 'pager',
    '#quantity' => $total,
  );

  return $form;
}

/**
 * Gets the configuration form for the pane.
 */
function pane_panes_configure($form, $form_state, $pane = NULL) {
  $form['#tree'] = TRUE;

  if (!$pane) {
    // Load a temporary pane to get configuration.
    $plugin = !empty($form_state['values']['plugin']) ? $form_state['values']['plugin'] : 'text';
    $class = _pane_get_class($plugin);
    $pane =  pane_instance($plugin);
  }
  elseif (!empty($form_state['values']['plugin'])) {
    $class = _pane_get_class($form_state['values']['plugin']);
  }
  else {
    $class = _pane_get_class($pane->plugin);
  }

  $plugins = array();
  foreach (pane_registry() as $type => $pane_type) {
    $plugins[$type] = $pane_type['title'];
  }

  $container_name = $pane->container;
  ctools_include('export');
  $container = ctools_export_load_object('pane_container', 'names', array($container_name));
  $container = isset($container[$container_name]) ? $container[$container_name] : new stdClass();
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => isset($container->title) ? $container->title : '',
    '#description' => t('The title of the pane as shown to the user.'),
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine Name'),
    '#description' => t('machine-friendly name.'),
    '#size' => 32,
    '#maxlength' => 32,
    '#default_value' => $container_name,
    '#disabled' => !empty($container_name),
    '#machine_name' => array(
      // The name of the source field that we will Take the User Friendly name
      // from and convert it to Machine Friendly name.
      'source' => array('title'),
      'exists' => 'pane_machine_name_exists',
    ),
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => $pane->description,
    '#description' => t('A brief description of your pane. Used as title on administration pages.'),
    '#required' => TRUE,
  );

  $form['plugin'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#default_value' => !empty($container_name) ? $pane->plugin : NULL,
    '#options' => $plugins,
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'pane_plugin_change_ajax',
      'wrapper' => 'pane-config',
    ),
  );

  if (user_access('administer panes')) {
    $form['remove'] = array(
      '#type' => 'checkbox',
      '#title' => t('Remove this pane'),
      '#description' => t('NOTE: Be sure this pane is no longer in use on your site before removing'),
      '#default_value' => FALSE,
    );
  }

  $form['configuration'] = array(
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
    '#title' => t('Configuration'),
    '#prefix' => '<div id="pane-config">',
    '#suffix' => '</div>',
  );

  $container = new $class();
  $container->setContainer(!empty($container_name) ? $container_name : 'new');
  $container->setConfiguration($pane->configuration);
  $container->container_edit_form_submit($form_state);
  $edit_form = $container->container_edit_form($form_state);
  if (!empty($edit_form)) {
    foreach ($container->container_edit_form($form_state) as $id => $form_item) {
      $form['configuration'][$id] = $form_item;
    }
  }
  else {
    $form['configuration'] = array(
      '#markup' => '<div id="pane-config"></div>',
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => 'Cancel',
    '#submit' => array('pane_panes_goto'),
    '#limit_validation_errors' => array(),
  );

  return $form;
}

/**
 * Submit handler for pane configuration form.
 */
function pane_panes_configure_submit($form, &$form_state) {
  $container = pane_instance($form_state['values']['plugin']);
  $pane_id = $form_state['values']['name'];
  $container->setContainer($pane_id)->container_edit_form_submit($form_state);

  $pane = new stdClass();
  $pane->name = $form_state['values']['name'];
  $pane->title = $form_state['values']['title'];
  $pane->description = $form_state['values']['description'];
  $pane->plugin = $form_state['values']['plugin'];
  $pane->configuration = $container->configuration;

  if ($form_state['values']['remove']) {
    pane_delete_container($pane);
  }
  else {
    pane_save_container($pane);
  }

  pane_panes_goto($form, $form_state);
}

/**
 * Gets the content edit form for the pane.
 */
function pane_panes_edit($form, $form_state, $pane) {
  drupal_set_title("Panes: $pane->description");

  global $language;

  $lang = LANGUAGE_NONE;

  if (drupal_multilingual()) {
    $lang = (TRUE === isset($form_state['values']['language'])) ? $form_state['values']['language'] : $language->language;
  }

  $container = pane_load_container($pane->container, $lang);
  $form = $container->data_edit_form_wrapper($form, $form_state);
  $form['pane_container'] = array(
    '#type' => 'hidden',
    '#value' => $pane->container,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Content'),
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => 'Cancel',
    '#submit' => array('pane_panes_goto'),
    '#limit_validation_errors' => array(),
  );

  return $form;
}

/**
 * Validation handler for pane content editing.
 */
function pane_panes_edit_validate(&$form, &$form_state) {
  $container = pane_load_container($form_state['values']['pane_container']);
  $container->data_edit_form_validate($form, $form_state);
}

/**
 * Submit handler for pane content editing.
 */
function pane_panes_edit_submit(&$form, &$form_state) {
  $container = pane_load_container($form_state['values']['pane_container']);
  $container->data_edit_form_wrapper_submit($form, $form_state);
  $container->save_data();

  pane_panes_goto($form, $form_state);
}

/**
 * Check to see if a Pane exists with the machine name
 *
 * @param string $value
 * @return boolean
 */
function pane_machine_name_exists($value) {
  $container = pane_load_container($value);

  return (FALSE != $container);
}

function pane_plugin_change_ajax($form, &$form_state) {
  return $form['configuration'];
}

function pane_panes_goto($form, &$form_state) {
  if (!empty($_GET['destination'])) {
    $destination = $_GET['destination'];
  }
  else {
    $destination = 'admin/structure/pane';
  }
  drupal_goto($destination);
}
