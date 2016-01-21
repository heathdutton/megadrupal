<?php
$plugin = array(
  'title' => t('Scald atom'),
  'single' => TRUE,
  'description' => t('Add a scald atom from your site as content.'),
  'category' => t('Scald'),
  'top level' => TRUE,
  'library' => array(array('dnd', 'library')),
  'render callback' => 'scald_pane_render',
  'edit form' => 'scald_pane_edit_form',
  'add form' => 'scald_pane_edit_form',
  'defaults' => array(
    'scald_atom' => '',
    'scald_context' => 'sdl_editor_representation',
  ),

);

/**
 * Output function for the scald atom.
 *
 * Outputs a scald atom based on the module and delta supplied in the configuration.
 */
function scald_pane_render($subtype, $conf, $panel_args) {
  if (!empty($conf['scald_atom'])) {
    $block = new stdClass();
    $block->module = 'scald';
    $block->delta = $conf['scald_atom'];
    $block->content = scald_render($conf['scald_atom'], $conf['scald_context']);

    return $block;
  }
}

/**
 * The form to add or edit a scald atom as content.
 */
function scald_pane_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $types = scald_types();

  $form['scald_atom'] = array(
    '#type' => 'textfield',
    '#title' => t('Scald atom'),
    '#description' => t(''),
    '#default_value' => $conf['scald_atom'],
    '#attached' => array(
      'library' => array(
        array('dnd', 'library'),
        array('atom_reference', 'library'),
      ),
      'js' => array(drupal_get_path('module', 'scald_pane') . '/scald_pane.js'),
      'css' => array(drupal_get_path('module', 'scald_pane') . '/scald_pane.css'),
    ),
    '#attributes' => array(
      'data-types' => implode(',', array_keys($types)),
      'data-dnd-context' => 'sdl_editor_representation',
    ),
    '#preview_context' => 'sdl_editor_representation',
    '#process' => array('atom_reference_field_widget_form_process'),
  );

  $contexts = scald_contexts();
  $display_context = array();
  foreach ($contexts as $name => $context) {
    if (!empty($context['hidden']) || empty($context['parseable'])) {
      continue;
    }

    $display_context[$name] = $context['title'];
  }

  $form['scald_context'] = array(
    '#title' => t('Context'),
    '#type' => 'select',
    '#required' => TRUE,
    '#description' => t('Select a representation.'),
    '#options' => $display_context,
    '#default_value' => $conf['scald_context'],
  );

  if (!empty($conf['scald_atom'])) {
    $atom = scald_atom_load($conf['scald_atom']);

    $form['scald_atom_type'] = array(
      '#type' => 'hidden',
      '#value' => $atom->type,
    );
  }

  return $form;
}

/**
 * Submit the scald atom selection.
 */
function scald_pane_edit_form_submit($form, &$form_state) {
  foreach (array('scald_atom', 'scald_context') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Display the administrative information for a scald atom pane.
 */
function scald_pane_admin_info($subtype, $conf) {
  // Just render the scald atom.
  return scald_panels_dnd_render($subtype, $conf, array());
}
