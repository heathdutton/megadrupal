<?php
/**
 * @file
 * Export UI integration.
 */

/**
 * Plugin definition for Ctools Export UI
 */
$plugin = array(
  'schema' => 'nodequeue_populator',
  'access' => 'manipulate all queues',
  'menu' => array(
    'menu prefix' => 'admin/structure/nodequeue',
    'menu item' => 'populator',
    'menu description' => 'Manage Nodequeue Populator',
    'items' => array(
      'list callback' => array(
        'type' => MENU_LOCAL_TASK,
        'weight' => 3,
      ),
      'process' => array(
        'path' => 'process/%ctools_export_ui',
        'title' => 'Process',
        'page callback' => 'drupal_get_form',
        'page arguments' => array('nodequeue_populator_ctools_export_ui_process_page_confirm', 5),
        'load arguments' => array('nodequeue_populator_ctools_export_ui'),
        'access arguments' => array('manipulate all queues'),
        'type' => MENU_CALLBACK,
      ),
    ),
  ),

  'allowed operations' => array(
    'process' => array('title' => t('Process')),
  ),

  'handler' => array(
    'class' => 'nodequeue_populator_ctools_export_ui',
    'parent' => 'ctools_export_ui',
  ),

  'title' => t('Populator'),

  'title singular' => t('populator'),
  'title plural' => t('populators'),
  'title singular proper' => t('Populator'),
  'title plural proper' => t('Populators'),
);

/**
 * Form to edit the settings of an populator.
 */
function nodequeue_populator_ctools_export_ui_form(&$form, &$form_state) {
  $populator = $form_state['item'];

  $form['info']['name'] = array_merge($form['info']['name'], array(
    '#title' => t('Machine-readable name of the populator'),
    '#type' => 'machine_name',
    '#description' => t('The populator name can only consist of lowercase letters, underscores, and numbers.'),
    '#machine_name' => array(
      'exists' => 'nodequeue_populator_ctools_export_ui_form_machine_name_exists',
      'source' => array('title'),
    ),
  ));

  $form['populator_group'] = array(
    '#type' => 'textfield',
    '#title' => t('Group'),
    '#required' => TRUE,
    '#description' => t('If using a cron extension (e.g. Ultimate Cron), a cron job will be created for each group.'),
    '#default_value' => empty($populator->populator_group) ? 'default' : $populator->populator_group,
  );

  $form['npid'] = array(
    '#type'  => 'value',
    '#value' => isset($populator->npid) ? $populator->npid : '',
  );

  $subqueue_value = '';
  if ($populator->sqid) {
    if ($subqueue = subqueue_load($populator->sqid)) {
      $subqueue_value = $subqueue->title . ' [' . $subqueue->sqid . ']';
    }
  }

  $form['subqueue'] = array(
    '#type'          => 'textfield',
    '#maxlength'     => 255,
    '#default_value' => $subqueue_value,
    '#title'         => t('Subqueue'),
    '#description' => t('Select the subqueue you want to populate with nodes.'),
    '#autocomplete_path' => 'nodequeue_populator/subnodequeue_autocomplete',
    '#required'      => TRUE,
  );

  $options = array();
  $processors = nodequeue_populator_get_processors();
  foreach ($processors as $name => $processor) {
    $options[$name] = $processor['title'];
  }

  $processor = !empty($form_state['values']['processor']) ? $form_state['values']['processor'] : $populator->processor;
  $processor = $processor ? $processor : reset(array_keys($options));
  $processor_settings = isset($form_state['values']['processor_settings']) ? $form_state['values']['processor_settings'] : $populator->processor_settings;

  $form['processor'] = array(
    '#type'          => 'select',
    '#options'       => $options,
    '#default_value' => $processor,
    '#title'         => t('Processor'),
    '#required'      => TRUE,
    '#description'   => t('Processor for this populator.'),
    '#ajax' => array(
      'callback' => 'nodequeue_populator_processor_settings_ajax',
      'wrapper' => 'processor-settings',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $form['processor_settings'] = array(
    '#title' => t("processor settings"),
    '#prefix' => '<div id="processor-settings">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
    '#description' => $processors[$processor]['description'],
    '#tree' => TRUE,
    '#visible' => TRUE,
  );

  $callback = nodequeue_populator_prepare_processor_callback($processor, 'settings');
  if ($callback) {
    $form['processor_settings'] = array_merge(
      $form['processor_settings'],
      $callback($processor_settings)
    );
  }
  else {
    $form['processor_settings']['#description'] = t('There are no settings for this processor.');
  }

  return $form;
}

/**
 * Ajax callback for processor settings switching.
 */
function nodequeue_populator_processor_settings_ajax($form, $form_state) {
  return $form['processor_settings'];
}

/**
 * Validate submission of the preset edit form.
 */
function nodequeue_populator_ctools_export_ui_form_validate(&$form, &$form_state) {
  if (preg_match('/.*\[(\d*)\]/', $form_state['values']['subqueue'], $matches)) {
    $form_state['values']['sqid'] = $matches[1];
    if (!$form_state['values']['sqid'] || !subqueue_load($form_state['values']['sqid'])) {
      form_set_error('sqid', t('%sqid is not a valid subqueue id', array('%sqid' => $form_state['values']['sqid'])));
    }
  }
  else {
    form_set_error('subqueue', t('Invalid subqueue entered: %subqueue', array('%subqueue' => $form_state['values']['subqueue'])));
  }

  if (preg_match('/[^a-z0-9_]+/', $form_state['values']['populator_group'])) {
    form_set_error('populator_group', t('The populator group must contain only lowercase letters, numbers, and underscores.'));
  }
}

/**
 * Nodequeue populator name check whether this machine name already exists.
 */
function nodequeue_populator_ctools_export_ui_form_machine_name_exists($value) {
  $result = db_query('SELECT npid FROM {nodequeue_populator} WHERE name = :name', array(':name' => $value))->fetchField();
  return !empty($result);
}

/**
 * Submit handler of the preset edit form.
 *
 * The submit handler is only responsible for moving data from
 * $form_state['values'] to $form_state['item'].
 */
function nodequeue_populator_ctools_export_ui_form_submit(&$form, &$form_state) {
}

/**
 * Process confirmation page.
 */
function nodequeue_populator_ctools_export_ui_process_page_confirm($form, &$form_state, $populator) {
  $form = array();
  $form['populator'] = array(
    '#type' => 'value',
    '#value' => $populator,
  );
  $msg = t('Are you sure you want to process populator %title?', array(
    '%title' => $populator->title,
  ));
  return confirm_form($form, $msg, 'admin/structure/nodequeue/populator');
}

/**
 * Submit handler for populator processing.
 *
 * Process the populator.
 */
function nodequeue_populator_ctools_export_ui_process_page_confirm_submit($form, &$form_state) {
  // Capture watchdog messages.
  $capture = watchdog_messages_capture();

  try {
    $populator = $form_state['values']['populator'];
    nodequeue_populator_process_populator($populator);
  }
  catch (Exception $e) {
    watchdog('nodequeue_populator', 'Error invoking processor for %populator: @error', array(
      '%populator' => $populator->title,
      '@error' => $e->getMessage(),
    ), WATCHDOG_ERROR);
  }

  // Explicitly end capturing.
  unset($capture);

  drupal_goto('admin/structure/nodequeue/populator');
}
