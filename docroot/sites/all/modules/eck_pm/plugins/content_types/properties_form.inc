<?php

/**
 * @file
 * Handle rendering entity properties form as panes.
 */

$plugin = array(
  'title' => t('Properties'),
  'defaults' => array('label' => 'title'),
  'content type' => 'eck_pm_properties_form_content_type_content_type',
);


function eck_pm_properties_form_content_type_content_type($subtype) {
  $types = eck_pm_properties_form_content_type_content_types();
  if (isset($types[$subtype])) {
    return $types[$subtype];
  }
}

function eck_pm_properties_form_content_type_content_types() {
  $types = &drupal_static(__FUNCTION__, array());
  if (!empty($types)) {
    return $types;
  }

  // This will hold all the individual field content types.
  $context_types = array();
  $entities = EntityType::loadAll();
  $entity_types = array();
  $description = t('Properties on the referenced entity.');

  foreach ($entities as $entity_type => $entity_object) {
    $entity_types[$entity_type] = $entity_type;
    $property_info = entity_get_property_info($entity_type);
    $entity_create = entity_create($entity_type, array());
    $vars = array('entity' => $entity_create);
    $vars += $property_info;
    $widgets = eck_property_behavior_invoke_plugin($entity_object, 'default_widget', $vars);
    if (!empty($widgets)) {
      foreach ($widgets as $property => $widget) {
        $types[$entity_type . ':' . $property] = array(
          'category' => t('Form'),
          //'icon' => 'icon_field.png',
          'title' => t('Property form: @widget_label', array(
            '@widget_label' => isset($widget['#title']) ? $widget['#title'] : ucfirst($property),
          )),
          'description' => t('Property on the referenced entity.'),
        );
      }
    }
  }

  // Create the required context for each field related to the bundle types.
  foreach ($types as $key => $property_content_type) {
    list($entity_type, $property) = explode(':', $key, 2);
    $types[$key]['required context'] = new ctools_context_required(t('Form'), $entity_type, array(
      'form' => array('form'),
    ));
  }

  $types['buttons'] = array(
    'single' => TRUE,
    'title' => t('ECK form submit buttons'),
    'description' => t('Submit buttons for the eck form.'),
    'required context' => new ctools_context_required(t('Form'), $entity_types, array('form' => array('form'))),
    'category' => t('Form'),
  );

  return $types;
}



/**
* Render the custom content type.
*/
function eck_pm_properties_form_content_type_render($subtype, $conf, $panel_args, $context) {

  if (empty($context) || empty($context->data)) {
    return;
  }
  $block = new stdClass();
  $block->content = array();
  // Get a shortcut to the entity.
  $entity = $context->data;

  if ($subtype == 'buttons') {
    foreach (array('submit', 'form_token', 'form_build_id', 'form_id', 'entity', 'actions') as $element) {
      $block->content[$element] = isset($context->form[$element]) ? $context->form[$element] : NULL;
      unset($context->form[$element]);
    }
  }
  else {
    list($entity_type, $property) = explode(':', $subtype, 2);
    if (empty($property)) {
      return;
    }

    if (isset($context->form)) {

      $block->content[$property] = $context->form[$property];
      unset($context->form[$property]);
    }
  }

  return $block;
}

/**
* Returns the administrative title for a type.
*/
function eck_pm_properties_form_content_type_admin_title($subtype, $conf, $context) {
  if ($subtype == 'buttons') {
    return t('ECK Buttons.');
  }
  list($entity_type, $property) = explode(':', $subtype, 2);

  $field = array('label' => $property);

  return t('"@s" @field form', array('@s' => $context->identifier, '@field' => $field['label']));
}

function eck_pm_properties_form_content_type_edit_form($form, &$form_state) {
  // provide a blank form so we have a place to have context setting.
  return $form;
}
