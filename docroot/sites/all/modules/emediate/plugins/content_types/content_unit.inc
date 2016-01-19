<?php
/**
 * @file
 * Content type plugin to allow Views to be exposed as a display type,
 * leaving most of the configuration on the view.
 */

/**
 * Implements hook_ctools_content_types().
 */
function emediate_content_unit_ctools_content_types() {
  return array(
    'single' => TRUE,
    'title' => t('Emediate content unit'),
    'admin settings' => 'emediate_admin_form',
    'category' => 'Emediate',
    'defaults' => array(
      'name' => '',
      'iframe' => FALSE,
      'exclude_campaign' => FALSE
    ),
  );
}

/**
 * Output function for the 'views' content type.
 *
 * Outputs a view based on the module and delta supplied in the configuration.
 */
function emediate_content_unit_content_type_render($subtype, $conf, $panel_args, $context) {
  if (!variable_get('emediate_enabled', EMEDIATE_ENABLED)) {
    return;
  }

  if (module_exists('src_blacklist') && src_blacklist("emediate", "emediate module")) {
    return;
  }

  if (is_array($context)) {
    $context = array_pop($context);
  }

  if (empty($conf['name'])) {
    return;
  }

  $block = new stdClass();
  $block->subject = $conf['override_title'] ? $conf['override_title_text'] : '';
  $block->module = 'emediate';
  $block->delta = $conf['name'];

  $content_unit = emediate_load($conf['name']);
  if ($content_unit) {
    // Get iframe configuration.
    $content_unit->iframe = empty($conf['iframe']) ? FALSE : TRUE;
    $content_unit->exclude_campaign = empty($conf['exclude_campaign']) ? FALSE : TRUE;
    $block->content = _emediate_render_content_unit($content_unit);
  }
  else {
    $block->content = '<!-- Content unit ' . $conf['name'] . ' not found -->';
  }
  return $block;
}

/**
 * Returns an edit form for a block.
 */
function emediate_content_unit_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $content_units = emediate_load_all();
  foreach ($content_units as &$cu) {
    $cu = strtr('@cuid (@name)', array('@cuid' => $cu->cuid, '@name' => $cu->name));
  }

  $form['name'] = array(
    '#type' => 'select',
    '#default_value' => isset($conf['name']) ? $conf['name'] : '',
    '#options' => $content_units,
    '#description' => t('Emediate content unit.'),
    '#title' => t('Emediate content unit'),
  );

  $form['iframe'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Load banner in iframe'),
    '#default_value' => $conf['iframe'],
    '#description'   => t('Check to load banner in an iframe.'),
  );

  $form['exclude_campaign'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Load banner campaign exclusively'),
    '#default_value' => $conf['iframe'],
    '#description'   => t('Check to prevent same campaigns being loaded twice.'),
  );

  return $form;
}

/**
 * Store form values in $conf.
 */
function emediate_content_unit_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Returns the administrative title for a type.
 */
function emediate_content_unit_content_type_admin_info($subtype, $conf, $contexts) {
  $content_unit = emediate_load($conf['name']);

  $block = new stdClass();
  if (empty($content_unit)) {
    $block->title = t('Deleted/missing content unit');
    return;
  }

  $block->content = t('Content unit ID: @cuid', array('@cuid' => $content_unit->cuid));
  $block->content .= '<br/>' . t('Machine name: @name', array('@name' => $content_unit->name));
  $block->title = check_plain($content_unit->label);

  return $block;
}
