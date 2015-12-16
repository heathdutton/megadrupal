<?php

use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Cache\Data\MapsAttributes as CacheMapsAttributes;
use Drupal\maps_object_criteria\Mapping\ObjectCriteria\ObjectCriteria;

/**
 * Overview.
 */
function maps_object_criteria_object_criteria_mapping_overview($form, &$form_state, Profile $profile, ConverterInterface $converter) {
  drupal_set_title(t('Object criteria mapping for the converter @converter', array('@converter' => $converter->getTitle())));

  $form['#tree'] = TRUE;

  // Retrieve all the object criteria mapping elements.
  foreach (ObjectCriteria::loadAllForConverter($converter) as $mapping) {
    $id = $mapping->getId();

    // Load the maps attribute.
    $attribute = CacheMapsAttributes::getInstance()->loadSingle('attribute:' . $mapping->getAttributeId(), array('profile' => $profile));

    $form['mapping'][$id]['attribute'] = array(
      '#type' => 'item',
      '#title' => check_plain($attribute->getTranslatedTitle()),
    );

    $form['mapping'][$id]['relation_type'] = array(
      '#type' => 'item',
      '#title' => check_plain($mapping->getRelationType()),
    );

    $form['mapping'][$id]['field_name'] = array(
      '#type' => 'item',
      '#title' => check_plain($mapping->getFieldName()),
    );

    $replace = array(
      '%maps_import_profile' => $converter->getProfile()->getName(),
      '%maps_import_converter' => $converter->getCid(),
      '%type' => $converter->getType(),
      '%maps_object_criteria' => $mapping->getId(),
    );
    $form['mapping'][$id]['actions'] = array(
      '#theme' => 'links',
      '#links' => array(
        'delete' => array(
          'href' => strtr('admin/maps-suite/profiles/%maps_import_profile/%type/%maps_import_converter/object_criteria_mapping/delete/%maps_object_criteria', $replace),
          'title' => t('delete'),
        ),
      ),
    );
  }

  return $form;
}


/**
 * Returns HTML for the correspondence table form.
 *
 * @ingroup themeable
 */
function theme_maps_object_criteria_object_criteria_mapping_overview($variables) {
  $form = $variables['form'];
  $rows = array();

  if (isset($form['mapping'])) {
    foreach (element_children($form['mapping']) as $key) {
      $element = &$form['mapping'][$key];

      $row = array();

      $row[] = drupal_render($element['attribute']);
      $row[] = drupal_render($element['relation_type']);
      $row[] = drupal_render($element['field_name']);
      $row[] = drupal_render($element['actions']);

      $rows[] = array(
        'data' => $row,
      );
    }
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('There is no defined mapping yet.'), 'colspan' => 4));
  }

  // New mapping row.
  $form['new']['weight']['#attributes']['class'] = array('mapping-weight');

  $output = theme('table', array(
    'header' => array(
      t('Attribute'),
      t('Relation type'),
      t('Field name'),
      t('Actions'),
    ),
    'rows' => $rows,
    'attributes' => array('id' => 'maps-object-criteria-mapping-overview'),
  ));

  return $output . drupal_render_children($form);
}

/**
 * Form builder.
 */
function maps_object_criteria_add_object_criteria_mapping_form($form, &$form_state, Profile $profile, ConverterInterface $converter) {
  drupal_set_title(t('Add object criteria mapping for converter @converter', array('@converter' => $converter->getTitle())));

  $form = array();
  $form['#profile'] = $profile;
  $form['#converter'] = $converter;

  // Get the list MaPS attributes.
  $attributes = CacheMapsAttributes::getInstance()
    ->load(array('profile' => $profile));

  // Get only the criteria attributes;
  $attributes_select = array();
  foreach ($attributes as $attribute) {
    if (!is_null($attribute->getIdCriteria())) {
      $attributes_select[$attribute->getId()] = $attribute->getTranslatedTitle();
    }
  }

  $form['attribute'] = array(
    '#type' => 'select',
    '#title' => t('MaPS attribute'),
    '#options' => array('' => t('- Select -')) + $attributes_select,
    '#description' => t('The MaPS criteria attribute to map.'),
  );

  // Get all the relation types.
  $relation_types = relation_get_types();
  $relation_types_select = array();
  $fields = array();
  foreach ($relation_types as $id => $relation_type) {
    $relation_types_select[$id] = $relation_type->label;

    // Get the fields of the current relation type.
    $fields[$id] = field_info_instances('relation', $id);

    // Remove the unnecessary endpoints.
    unset($fields[$id]['endpoints']);
  }

  $form['relation_type'] = array(
    '#type' => 'select',
    '#title' => t('Relation types'),
    '#options' => array('' => t('- Select -')) + $relation_types_select,
    '#description' => t('The relation types.'),
  );

  $form['field'] = array(
    '#type' => 'value',
  //  '#value' => $converter->getBundle()
  );
  $form['fields']['#tree'] = TRUE;

  foreach ($relation_types as $id => $relation_type) {
    $form['fields'][$id] = array(
      '#type' => 'select',
      '#title' => t('Field to map'),
      '#description' => t('Select the field to map.'),
      '#options' => array('' => t('- Select -')) + maps_suite_reduce_array($fields[$id], 'label'),
      '#states' => array(
        'visible' => array(
          ':input[name="relation_type"]' => array('value' => $id),
        ),
        'required' => array(
          ':input[name="relation_type"]' => array('value' => $id),
        ),
      ),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form validation handler.
 */
function maps_object_criteria_add_object_criteria_mapping_form_validate($form, &$form_state) {
  if ($relation_type = $form_state['values']['relation_type']) {
    if (empty($form_state['values']['fields'][$relation_type])) {
      form_set_error('fields][' . $relation_type, t('!name field is required.', array('!name' => $form['fields'][$relation_type]['#title'])));
    }
    else {
      form_set_value($form['field'], $form_state['values']['fields'][$relation_type], $form_state);
    }
  }
}

/**
 * Form submit handler.
 */
function maps_object_criteria_add_object_criteria_mapping_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $item = array(
    'converter' => $form['#converter'],
    'attribute_id' => $values['attribute'],
    'relation_type' => $values['relation_type'],
    'field_name' => $values['field'],
  );
  $object_criteria = new ObjectCriteria($item);

  if ($object_criteria->save()) {
    drupal_set_message(t('The mapping has been created.'));
  }
  else {
    drupal_set_message(t('Can\t save the mapping.'), 'error');
  }

  $form_state['redirect'] = array('admin/maps-suite/profiles/' . $form['#profile']->getName() . '/' . $form['#converter']->getType() . '/' . $form['#converter']->getCid() . '/object_criteria_mapping');
}

/**
 * Form builder.
 */
function maps_object_criteria_delete_object_criteria_mapping_form($form, &$form_state, Profile $profile, ConverterInterface $converter, ObjectCriteria $object_criteria) {
  drupal_set_title(t('Delete object criteria mapping @$object_criteria', array('@$object_criteria' => $object_criteria->getId())));

  $form['mode'] = array(
    '#type' => 'radios',
    '#options' => array(
      'unlink' => t('Delete the mapping but keep the created relations in Drupal'),
      'delete' => t('Delete the mapping and all the created relations'),
    ),
    '#description' => t('Caution: if the entities are kept, they will no more be synchronised through MaPS Suite.'),
    '#default_value' => 0,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete the object criteria mapping %id ?', array('%id' => $object_criteria->getId())),
    'admin/maps-suite/profiles/' . $profile->getName() . '/' . $converter->getType()  . '/' . $converter->getCid() . '/object_criteria_mapping',
    '',
    t('Delete'),
    t('Cancel'));
}

/**
 * Form submission handler.
 */
function maps_object_criteria_delete_object_criteria_mapping_form_submit($form, &$form_state) {
  list($profile, $converter, $object_criteria) = $form_state['build_info']['args'];
  $object_criteria->delete($form_state['values']['mode']);

  $form_state['redirect'] = array('admin/maps-suite/profiles/' . $profile->getName() . '/' . $converter->getType()  . '/' . $converter->getCid() . '/object_criteria_mapping');
  drupal_set_message(t('The object criteria mapping @id was successfully deleted!', array('@id' => $object_criteria->getId())));
}
