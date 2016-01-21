<?php
/**
 * @file
 * Rules module integration.
 */

/**
 * Implements hook_rules_action_info().
 */
function pdf_archive_rules_action_info() {
  $actions = array();

  $actions['pdf_archive_create'] = array(
    'label' => t('Create PDF archive'),
    'base' => 'pdf_archive_rules_action_create',
    'group' => t('PDF Archive'),
    'parameter' => array(
      'entity_wrapper' => array(
        'type' => 'entity',
        'label' => t('Entity'),
      ),
      'page_top' => array(
        'type' => 'text',
        'label' => t('Header'),
        'description' => t('Markup to be inserted above the rendered entity.'),
        'optional' => TRUE,
      ),
      'page_bottom' => array(
        'type' => 'text',
        'label' => t('Footer'),
        'description' => t('Markup to be inserted below the rendered entity.'),
        'optional' => TRUE,
      ),
      'view_mode' => array(
        'type' => 'text',
        'label' => t('View mode'),
        'options list' => 'pdf_archive_view_modes',
        'description' => t('The entity view modue to be used when rendering the content.'),
      ),
      'rid' => array(
        'type' => 'integer',
        'label' => t('Role'),
        'options list' => 'pdf_archive_user_roles',
        'description' => t('The role to be used by the PDF rendering, simulated user. This will effect the output of the content if you have any permissions based behaviours.'),
      ),
    ),
    'provides' => array(
      'pdf_archive' => array(
        'type' => 'tcpdf',
        'label' => t('PDF archive'),
      ),
    ),
    'callbacks' => array(
      'form_alter' => 'pdf_archive_rules_action_entity_type_form_alter',
    ),
  );

  $actions['pdf_archive_field_attach'] = array(
    'label' => t('Attach PDF archive to File field'),
    'base' => 'pdf_archive_rules_action_field_attach',
    'group' => t('PDF Archive'),
    'parameter' => array(
      'entity_wrapper' => array(
        'type' => 'entity',
        'label' => t('Entity'),
      ),
      'pdf_archive' => array(
        'type' => 'tcpdf',
        'label' => t('PDF Archive'),
        'description' => t('The PDF Archive object to be used, supplied by the "Create PDF archive" action.'),
      ),
      'field_name' => array(
        'type' => 'text',
        'label' => t('File field'),
        'options list' => 'pdf_archive_file_fields',
      ),
      'filename' => array(
        'type' => 'text',
        'label' => t('Filename'),
      ),
    ),
    'callbacks' => array(
      'form_alter' => 'pdf_archive_rules_action_entity_type_form_alter',
    ),
  );

  $actions['pdf_archive_output'] = array(
    'label' => t('Output PDF archive'),
    'base' => 'pdf_archive_output',
    'group' => t('PDF Archive'),
    'parameter' => array(
      'pdf_archive' => array(
        'type' => 'tcpdf',
        'label' => t('PDF Archive'),
        'description' => t('The PDF Archive object to be used, supplied by the "Create PDF archive" action.'),
      ),
      'mode' => array(
        'type' => 'text',
        'label' => t('Output mode'),
        'options list' => 'pdf_archive_output_modes',
      ),
      'filename' => array(
        'type' => 'text',
        'label' => t('Filename'),
      ),
    ),
  );

  return $actions;
}

/**
 * Rules action callback for 'pdf_archive_create'.
 */
function pdf_archive_rules_action_create($entity_wrapper, $page_top, $page_bottom, $view_mode, $rid) {
  $entity_id = $entity_wrapper->value(array('identifier' => TRUE));
  $entity = $entity_wrapper->raw();
  $entity_type = $entity_wrapper->type();
  $entities = array($entity_id => $entity);
  $pdf = pdf_archive_create($entity_type, $entities, $page_top, $page_bottom, $view_mode, $rid);

  return array(
    'pdf_archive' => $pdf,
  );
}

/**
 * Rules action callback for 'pdf_archive_create'.
 */
function pdf_archive_rules_action_field_attach($entity_wrapper, $pdf, $field_name, $filename) {
  $entity = $entity_wrapper->raw();
  $entity_type = $entity_wrapper->type();
  pdf_archive_field_attach($entity_type, $entity, $pdf, $field_name, $filename);
}

/**
 * Form alter callback for PDF Archive Rules actions.
 */
function pdf_archive_rules_action_entity_type_form_alter(&$form, &$form_state, $options, RulesAbstractPlugin $element) {
  $first_step = empty($element->settings['entity_wrapper:select']);
  $form['reload'] = array(
    '#weight' => 5,
    '#type' => 'submit',
    '#name' => 'reload',
    '#value' => $first_step ? t('Continue') : t('Reload form'),
    '#limit_validation_errors' => array(array('parameter', 'eneity_wrapper')),
    '#submit' => array('pdf_archive_rules_action_entity_type_form_submit_rebuild'),
    '#ajax' => rules_ui_form_default_ajax(),
  );
  // Use ajax and trigger as the reload button.
  $form['parameter']['type']['settings']['type']['#ajax'] = $form['reload']['#ajax'] + array(
    'event' => 'change',
    'trigger_as' => array('name' => 'reload'),
  );

  if ($first_step) {
    // In the first step show only the type select.
    foreach (element_children($form['parameter']) as $key) {
      if ($key != 'entity_wrapper') {
        unset($form['parameter'][$key]);
      }
    }
    unset($form['submit']);
    unset($form['provides']);
    // Disable #ajax for the first step as it has troubles with lazy-loaded JS.
    // @todo: Re-enable once JS lazy-loading is fixed in core.
    unset($form['parameter']['entity_wrapper']['settings']['type']['#ajax']);
    unset($form['reload']['#ajax']);
  }
  else {
    // Hide the reload button in case js is enabled and it's not the first step.
    $form['reload']['#attributes'] = array('class' => array('rules-hide-js'));
  }
}

/**
 * Submit handler for PDF Archive Rules actions Form alter callback.
 */
function pdf_archive_rules_action_entity_type_form_submit_rebuild($form, &$form_state) {
  rules_form_submit_rebuild($form, $form_state);
  // Clear the parameter modes for the parameters, so they get the proper
  // default values based upon the data types on rebuild.
  $form_state['parameter_mode'] = array();
}

