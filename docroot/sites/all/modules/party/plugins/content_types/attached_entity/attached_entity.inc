<?php

/**
 * @file
 * Plugin to handle attached entity content types
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Attached Entity'),
  'defaults' => array('view_mode' => 'full', 'delta' => -1),
  'content type' => 'party_attached_entity_content_type_content_type',
);

/**
 * Just one subtype.
 *
 * Ordinarily this function is meant to get just one subtype. However, we are
 * using it to deal with the fact that we have changed the subtype names. This
 * lets us translate the name properly.
 */
function party_attached_entity_content_type_content_type($subtype) {
  $types = party_attached_entity_content_type_content_types();
  if (isset($types[$subtype])) {
    return $types[$subtype];
  }
}

/**
 * Return all attached entity content types available
 */
function party_attached_entity_content_type_content_types() {
  $types = &drupal_static(__FUNCTION__, array());
  if (!empty($types)) {
    return $types;
  }

  // This will hold all the individual field content types.
  $data_sets = party_get_data_set_info();

  foreach ($data_sets as $data_set_name => $data_set) {
    if (!isset($types[$data_set_name])) {
      $types[$data_set_name] = array(
        'category' => t('Attached Entities'),
        'icon' => 'icon_attached_entity.png',
        'title' => t('Attached Entity: @data_set_label (@data_set_name)', array(
          '@data_set_label' => $data_set['label'],
          '@data_set_name' => $data_set_name,
        )),
        'required context' => new ctools_context_required(t('Party'), 'party'),
        'description' => t('Attached Entity from Party'),
        'edit form' => 'party_attached_entity_content_type_options',
      );
    }
  }

  return $types;
}

/**
* Render the custom content type.
*/
function party_attached_entity_content_type_render($subtype, $conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    return;
  }

  // Get a shortcut to the party.
  $party = $context->data;
  $data_set_name = $subtype;
  $data_set = party_get_data_set_info($data_set_name);
  $empty = FALSE;

  if (empty($conf['bypass_access']) && !party_access('view', $party, $data_set)) {
    return;
  }

  // Get the attached entities in this data set
  $attached_entities = $party->getDataSetController($data_set_name)->getEntities();

  // If there are none, do not render anything
  if (empty($attached_entities)) {
    $empty = TRUE;
  }

  // Remove all unneccesary attached entities if we're not showing them all
  if ($conf['delta'] != -1) {
    $attached_entity_list = array_values($attached_entities);
    // If one with the right delta exists...
    if (isset($attached_entity_list[$conf['delta']])) {
      // Replace the array with an array just containing the one attached entity
      $attached_entity = $attached_entity_list[$conf['delta']];
      $attached_entities = array(
        $conf['delta'] => $attached_entity,
      );
    }
    // ... otherwise don't render anything
    else {
      $empty = TRUE;
    }
  }

  // $attached_entities should now be alist of all the ones we want to show so lets convert this into a list of entities
  $entities = array();
  foreach ($attached_entities as $attached_entity) {
    $entities[] = $attached_entity;
  }

  if (empty($entities)) {
    $empty = TRUE;
  }

  if ($empty && empty($conf['show_empty'])) {
    return FALSE;
  }
  else if ($empty) {
    $content = $conf['empty_message'];
  }
  else {
    $content = entity_view($data_set['entity type'], $entities, $conf['view_mode']);
  }

  $block_delta = $data_set_name;
  if ($conf['delta'] != -1) {
    $block_delta .= '_' . $conf['delta'];
  }

  // Build the content type block.
  $block = new stdClass();
  $block->module  = 'attached_entity';
  $block->title = $data_set['label'];
  $block->content = $content;
  $block->delta   = $block_delta;

  return $block;
}

/**
 * Returns an edit form for custom type settings.
 */
function party_attached_entity_content_type_options($form, &$form_state) {
  $conf = $form_state['conf'];
  $subtype = $form_state['subtype_name'];
  $data_set_name = $subtype;
  $data_set = party_get_data_set_info($data_set_name);

  $delta_options = array();
  $delta_options['-1'] = 'Show All';
  for ($i = 0; $i < 20; $i++) {
    $delta_options["$i"] = "$i";
  }
  $form['delta'] = array(
    '#title' => t('Attached Entity Delta'),
    '#description' => t('If more than one attached entity of this type is present, which one should we pick?'),
    '#type' => 'select',
    '#options' => $delta_options,
    '#default_value' => $conf['delta'],
  );

  $entity = entity_get_info($data_set['entity type']);
  $view_mode_options = array();
  foreach ($entity['view modes'] as $mode => $option) {
    $view_mode_options[$mode] = $option['label'];
  }
  $form['view_mode'] = array(
    '#title' => t('View mode'),
    '#type' => 'select',
    '#description' => t('Select a view mode for this attached entity.'),
    '#options' => $view_mode_options,
    '#default_value' => $conf['view_mode'],
  );

  $form['bypass_access'] = array(
    '#title' => t('Bypass Access Control'),
    '#description' => t('Bypass standard access control to view this attached
      entity'),
    '#type' => 'checkbox',
    '#default_value' => !empty($conf['bypass_access']),
  );

  $form['show_empty'] = array(
    '#title' => t('Show a Message if the Data Set is empty'),
    '#description' => t('Show a Message even if there are no attached entities
      in this data set. This message could be used to add a link to add the
      data set.'),
    '#type' => 'checkbox',
    '#default_value' => !empty($conf['show_empty']),
  );

  $form['empty_message'] = array(
    '#title' => t('Empty Message'),
    '#description' => t('Message to show if the are no attached entities in the
      data set'),
    '#type' => 'textarea',
    '#default_value' => empty($conf['empty_message']) ? '' : $conf['empty_message'],
    '#states' => array(
      'visible' => array(
        'input[name="show_empty"]' => array('checked' => TRUE),
      ),
    ),
  );

  return $form;
}

function party_attached_entity_content_type_options_submit($form, &$form_state) {
  $form_state['conf']['delta'] = $form_state['values']['delta'];
  $form_state['conf']['view_mode'] = $form_state['values']['view_mode'];
  $form_state['conf']['show_empty'] = $form_state['values']['show_empty'];
  $form_state['conf']['bypass_access'] = $form_state['values']['bypass_access'];
  $form_state['conf']['empty_message'] = $form_state['values']['empty_message'];
}

/**
 * Returns the administrative title for a type.
 */
function party_attached_entity_content_type_admin_title($subtype, $conf, $context) {
  $data_set_name = $subtype;
  $data_set = party_get_data_set_info($data_set_name);
  ctools_include('fields');
  return t('"@s" @data_set_label', array('@s' => $context->identifier, '@data_set_label' => $data_set['label']));
}
