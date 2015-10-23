<?php

/**
 * @file
 * Contains the admin forms for the mapping.
 */

use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Mapping\Item\Item;
use Drupal\maps_import\Mapping\Source\MapsSystem as MS;
use Drupal\maps_import\Mapping\Target\Drupal\Field\DefaultField;
use Drupal\maps_import\Mapping\Validator as Validator;
use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Cache\Data\MappingItems as CacheMappingItems;
use Drupal\maps_import\Cache\Data\MapsAttributes as CacheMapsAttributes;

/**
 * Form builder; mapping form.
 *
 * @see maps_import_converter_mapping_form_submit()
 * @ingroup forms
 */
function maps_import_converter_mapping_form($form, &$form_state, Profile $profile, ConverterInterface $converter) {
  drupal_set_title(t('Mapping for the converter @converter', array('@converter' => $converter->getTitle())));

  $form['#attached']['css'][] = drupal_get_path('module', 'maps_import') . '/maps_import.css';
  $form['#tree'] = TRUE;

  // @todo this is only a quick fix.
  if ($converter->getEntitytype() === 'commerce_product') {
    drupal_set_message(t('The SKU field is automatically mapped with the converter UID.'), 'warning');
  }

  if ($converter->getEntitytype() === 'relation') {
    drupal_set_message(t('The source and target fields are automatically mapped with corresponding relation property'));
  }

  $items = $converter->getMapping()->getItems();
  $items += $converter->getMapping()->getItems('delayed');

  $fields = $converter->getMapping()->getTargetFields();

  if (!empty($items)) {
    $replace = array(
      '%maps_import_profile' => $profile->getName(),
      '%maps_import_converter' => $converter->getCid(),
      '%maps_import_mapping_type' => $converter->getType(),
    );

    foreach ($items as $i => $item) {
      $replace['%maps_import_mapping_item'] = $i;
      $options = $item->getOptions();

      // @todo Create an array that lists the already checked properties
      // to avoid duplicate database calls.
      if (!is_null($item->getProperty()) && $item->getProperty()->exists($profile)) {
        $title = $item->getProperty()->getTranslatedTitle();
        if (isset($options['post_save'])) {
          $title = t('@title (Delayed entity)', array('@title' => $title));
        }

        $form['mapping'][$i]['property_id'] = array(
          '#type' => 'item',
          '#title' => check_plain($title),
          '#description' => check_plain($item->getProperty()->getLabel()),
        );
      }
      else {
        $form['mapping'][$i]['property_id'] = array(
          '#type' => 'item',
          '#title' => t('The MaPS System® property does not exist anymore.'),
        );
      }

      if ((!is_null($item->getField()) && array_key_exists($item->getField()->getId(), $fields)) || isset($options['post_save'])) {

        // Criteria mapping.
        // @todo move this is the related submodule.
        $title = $item->getField()->getLabel();

        if (isset($options['idcriteria'])) {
          $title = t('Criteria');
        }

        $form['mapping'][$i]['field_name'] = array(
          '#type' => 'item',
          '#title' => check_plain($title),
          '#description' => check_plain($item->getField()->getDescription(
            array(
              'id',
              'type',
              'translatable',
              'multiple',
            )
          )),
        );
      }
      else {
        $form['mapping'][$i]['field_name'] = array(
          '#type' => 'item',
          '#title' => t('The Drupal field does not exist anymore.'),
        );
      }

      $form['mapping'][$i]['weight'] = array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#delta' => 50,
        '#title_display' => 'invisible',
        '#default_value' => $item->getWeight(),
      );

      $form['mapping'][$i]['required'] = array(
        '#type' => 'checkbox',
        '#title' => t('Required'),
        '#title_display' => 'invisible',
        '#default_value' => (int) $item->isRequired(),
      );

      $form['mapping'][$i]['links'] = array(
        '#theme' => 'links',
      );

      $form['mapping'][$i]['links']['#links']['mapping-delete'] = array(
        'href' => strtr('admin/maps-suite/profiles/%maps_import_profile/%maps_import_mapping_type/%maps_import_converter/mapping/%maps_import_mapping_item/delete', $replace),
        'title' => t('delete'),
      );

      if ($item->getProperty() instanceof MS\RelatedEntity\RelatedEntity) {
        $replace['%maps_import_parent_converter'] = $item->getProperty()->getParentId();

        $form['mapping'][$i]['links']['#links']['mapping'] = array(
          'href' => strtr('admin/maps-suite/profiles/%maps_import_profile/%maps_import_mapping_type/%maps_import_parent_converter/mapping', $replace),
          'title' => t('mapping'),
        );
      }

      // Add "edit" link for object/media mapping.
      if (isset($options['media_type'])) {
        $form['mapping'][$i]['links']['#links']['edit'] = array(
          'href' => strtr('admin/maps-suite/profiles/%maps_import_profile/%maps_import_mapping_type/%maps_import_converter/mapping/%maps_import_mapping_item/edit_object_media', $replace),
          'title' => t('edit'),
        );
      }

      if ($item->hasOptions()) {
        $form['mapping'][$i]['links']['#links']['options'] = array(
          'href' => strtr('admin/maps-suite/profiles/%maps_import_profile/%maps_import_mapping_type/%maps_import_converter/mapping/%maps_import_mapping_item/options', $replace),
          'title' => t('options'),
        );
      }

      // @todo: Add an option link to manage options
      // for localisable and multiple property.
    }
  }

  switch ($converter->getType()) {
    case 'object':
      $properties = array(t('Object properties') => $converter->getMapping()->getSourceProperties());
      break;

    case 'media':
      $properties = array(t('Media properties') => $converter->getMapping()->getSourceProperties());
      // @todo move this.
    case 'link':
      $properties = array(t('Link properties') => $converter->getMapping()->getSourceProperties());
      break;
  }
  $properties[t('Attributes')] = $converter->getMapping()->getSourceAttributes();

  // Add new mapping.
  $form['new']['property_id'] = array(
    '#title' => t('Map new attribute'),
    '#type' => 'select',
    '#options' => array('' => t('- Select -')) + maps_import_properties_select($properties),
    '#description' => t('MaPS System® object property or attribute'),
  );

  $form['new']['field_name'] = array(
    '#type' => 'select',
    '#title' => t('Field'),
    '#title_display' => 'invisible',
    '#options' => array('' => t('- Select -')) + maps_import_fields_select($converter, $fields),
    '#description' => t('Drupal field'),
  );

  $form['new']['required'] = array(
    '#type' => 'checkbox',
    '#title' => t('Required'),
    '#title_display' => 'invisible',
  );

  $form['new']['weight'] = array(
    '#type' => 'weight',
    '#attributes' => array('class' => array('field-weight')),
    '#prefix' => '<div class="add-new-mapping">&nbsp;</div>',
    '#description' => t('Weight for new mapping'),
  );

  $rows[] = array(
    'data' => array(l(t('Add a fixed field.'), '')),
    'colspan' => 5,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Returns HTML for the correspondence table form.
 *
 * @ingroup themeable
 */
function theme_maps_import_converter_mapping_form($variables) {
  $form = $variables['form'];
  $rows = array();

  if (isset($form['mapping'])) {
    foreach (element_children($form['mapping']) as $key) {
      $element = &$form['mapping'][$key];
      $element['weight']['#attributes']['class'] = array('mapping-weight');

      $row = array();
      $classes = array('draggable');

      if (!empty($element['#errors'])) {
        $classes[] = 'error';
      }

      $row[] = drupal_render($element['property_id']);
      $row[] = drupal_render($element['field_name']);
      $row[] = drupal_render($element['required']);
      $row[] = drupal_render($element['weight']);
      $row[] = drupal_render($element['links']);

      $rows[] = array(
        'data' => $row,
        'class' => $classes,
      );
    }
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('There is no defined mapping yet.'), 'colspan' => 4));
  }

  // New mapping row.
  $form['new']['weight']['#attributes']['class'] = array('mapping-weight');

  $rows[] = array(
    'data' => array(
      drupal_render($form['new']['property_id']),
      drupal_render($form['new']['field_name']),
      drupal_render($form['new']['required']),
      drupal_render($form['new']['weight']),
      '&nbsp;',
    ),
    'class' => array('draggable'),
  );

  drupal_add_tabledrag(
    'maps-import-mapping-overview',
    'order',
    'sibling',
    'mapping-weight'
  );

  $output = theme('table', array(
    'header' => array(
      t('Property'),
      t('Field'),
      t('Required'),
      t('Weight'),
      t('Actions'),
    ),
    'rows' => $rows,
    'attributes' => array('id' => 'maps-import-mapping-overview'),
  ));

  return $output . drupal_render_children($form);
}

/**
 * Form validation handler; mapping form.
 *
 * @see maps_import_converter_mapping_form()
 * @see maps_import_converter_mapping_form_submit()
 */
function maps_import_converter_mapping_form_validate($form, &$form_state) {
  $converter = $form_state['build_info']['args'][1];

  // @todo: Think about the mapping management
  // for multiple or localisable property.
  $values = $form_state['values'];

  if (empty($values['new']['property_id']) xor empty($values['new']['field_name'])) {
    $error_element = empty($values['new']['property_id']) ? 'new][property_id' : 'new][field_name';
    form_set_error($error_element, t('All required fields need to be filled in order to add a new mapping!'));
  }

  if (!empty($values['new']['property_id'])) {
    $fields = $converter->getMapping()->getTargetFields();

    $property = MS\PropertyWrapper::load($converter, $values['new']['property_id']);
    $field = $fields[$values['new']['field_name']];

    $property_wrapper_type = is_subclass_of($property, 'Maps\\Import\\Mapping\\Source\\MapsSystem\\Attribute') ? 'attribute' : 'property';

    // First validation: Type.
    $type_validator = new Validator\Type($property, $field);
    $type_validator_result = $type_validator->validate();
    if ($type_validator_result == Validator\Validator::VALIDATOR_WARNING) {
      drupal_set_message(t('The compatibility between the MaPS System® %maps_type %maps_title and the Drupal field %drupal_name could not be verified. Please make sure there is no mistake.',
        array(
          '%maps_type' => $property_wrapper_type,
          '%maps_title' => $property->getDescription(array('title')),
          '%drupal_name' => $field->getName(),
        )
      ), 'warning');
    }
    elseif ($type_validator_result == Validator\Validator::VALIDATOR_ERROR) {
      form_set_error('new][property_id', t('The MaPS System® %maps_type %maps_title can not be mapped with the Drupal field %drupal_name.',
        array(
          '%maps_type' => $property_wrapper_type,
          '%maps_title' => $property->getDescription(array('title')),
          '%drupal_name' => $field->getName(),
        )
      ));
    }

    // Second validation: translatable.
    $translate_validator = new Validator\Translate($property, $field);
    if ($translate_validator->validate() == Validator\Validator::VALIDATOR_WARNING) {
      drupal_set_message(t('The translation state of the newly created mapping element does not match. Please ensure there is no mistake there.'), 'warning');
    }

    // Third validation: multiple.
    $multiple_validator = new Validator\Multiple($property, $field);
    $multiple_validator_result = $multiple_validator->validate();
    if ($multiple_validator_result == Validator\Multiple::MULTIPLE_VALIDATOR_DRUPAL_NOT_MULTIPLE) {
      drupal_set_message(t('The multiple status of the two linked fields does not match. The mapping should succeed, but you may loose some data.'), 'warning');
    }
    elseif ($multiple_validator_result == Validator\Multiple::MULTIPLE_VALIDATOR_DRUPAL_NOT_INFINITE) {
      drupal_set_message(t('The Drupal field may accept at most %count values. Please ensure this will not result in data loss.', array('%count' => $field->getCardinality())), 'warning');
    }
  }
}

/**
 * Form submission handler; mapping form.
 *
 * @see maps_import_converter_mapping_form()
 */
function maps_import_converter_mapping_form_submit($form, &$form_state) {
  $converter = $form_state['build_info']['args'][1];
  $values = $form_state['values'];

  // Update existing mapping if necessary.
  $items = $converter->getMapping()->getItems();
  if (!empty($items)) {
    foreach ($items as $id => $_item) {
      $current = $values['mapping'][$id];
      $current['required'] = (int) !empty($values['mapping'][$id]['required']);

      if ($current['weight'] != $_item->getWeight() || $current['required'] != $_item->isRequired()) {
        $record = array(
          'id' => $id,
          'type' => $_item->getType(),
          'weight' => $current['weight'],
          'required' => $current['required'],
        );

        drupal_write_record('maps_import_mapping_item', $record, array('id', 'type'));
      }
    }
  }

  // Save new mapping.
  if (!empty($values['new']['property_id'])) {
    $property = MS\PropertyWrapper::load($converter, $values['new']['property_id']);
    $field = new DefaultField(array('id' => $values['new']['field_name']));

    $item = new Item(
      array(
        'converter' => $converter,
        'property' => $property,
        'field' => $field,
        'required' => $values['new']['required'],
        'weight' => $values['new']['weight'],
      )
    );

    if ($item->save()) {
      drupal_set_message(t('The new mapping has been created.'));
    }
    else {
      drupal_set_message(t('An error occured while creating the new mapping.'));
    }
  }
  else {
    drupal_set_message(t('The changes have been saved.'));
  }

  // Clear cache.
  CacheMappingItems::getInstance()->clearBinCache();
}

/**
 * Form builder; mapping deletion form.
 *
 * @see maps_import_converter_mapping_delete_form_submit()
 * @ingroup forms
 */
function maps_import_converter_mapping_delete_form($form, &$form_state, Profile $profile, ConverterInterface $converter, Item $item) {
  $options = $item->getOptions();

  if (isset($options['id'])) {
    $form['mode'] = array(
      '#type' => 'radios',
      '#title' => t('Converted entities desynchronization mode'),
      '#options' => array(
        'unlink' => t('Keep the created entities in Drupal, but desynchronize them from MaPS Suite'),
        'delete' => t('Remove all the created entities'),
      ),
      '#default_value' => 'unlink',
      '#required' => TRUE,
    );
  }

  return confirm_form(
    $form,
    t('Delete mapping @item for converter @converter', array('@converter' => $converter->getTitle(), '@item' => $item->getId())),
    'admin/maps-suite/profiles/' . $profile->getName() . '/' . $converter->getType() . '/' . $converter->getCid() . '/mapping',
    t('Are you sure you want to delete the mapping %field for the property %name?', array('%field' => $item->getField()->getLabel(), '%name' => $item->getProperty()->getTranslatedTitle())),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Form submission handler; mapping form.
 *
 * @see maps_import_converter_mapping_delete_form()
 */
function maps_import_converter_mapping_delete_form_submit($form, &$form_state) {
  list($profile, $converter, $item) = $form_state['build_info']['args'];

  $item->delete();
  $options = $item->getOptions();

  if (isset($options['id']) && $related_converter = maps_import_converter_load($options['id'])) {
    $related_converter->delete(array('mode' => $form_state['values']['mode']));
  }

  drupal_set_message(t('The mapping was successfully deleted!'));

  $form_state['redirect'] = array(
    'admin/maps-suite/profiles/' . $profile->getName() . '/' . $converter->getType() . '/' . $converter->getCid() . '/mapping',
  );
}

/**
 * Form builder; Add an object media relation.
 */
function maps_import_converter_mapping_add_object_media_form(Profile $profile, ConverterInterface $converter) {
  $item = new Item();
  menu_set_active_item('admin/maps-suite/profiles/' . $profile->getName() . '/' . $converter->getType() . '/' . $converter->getCid() . '/mapping/add_object_media');
  $router_item = menu_get_item();
  $router_item['fake'] = TRUE;
  menu_set_item($_GET['q'], $router_item);
  return drupal_get_form('maps_import_converter_mapping_edit_object_media_form', $profile, $converter, $item);
}

/**
 * Form validate; Add an object media relation.
 */
function maps_import_converter_mapping_edit_object_media_form($form, &$form_state, Profile $profile, ConverterInterface $converter, Item $item) {
  drupal_set_title(t('Add an object media mapping for converter @converter', array('@converter' => $converter->getTitle())));

  $options = $item->getOptions();

  $form_state['converter'] = $converter;

  $fields = $converter->getMapping()->getTargetFields();

  if (!empty($options)) {
    $form['id'] = array(
      '#type' => 'hidden',
      '#value' => $item->getId(),
    );
  }

  $form['media_type'] = array(
    '#title' => t('Media type'),
    '#type' => 'select',
    '#options' => maps_import_get_maps_media_types($profile),
    '#required' => TRUE,
    '#description' => t('The MaPS System® media type.'),
  );

  if (!empty($options['media_type'])) {
    $form['media_type']['#default_value'] = $options['media_type'];
  }

  $form['start'] = array(
    '#title' => t('Start range'),
    '#type' => 'textfield',
    '#default_value' => isset($options['media_start_range']) ? $options['media_start_range'] : 1,
    '#required' => TRUE,
    '#size' => 2,
  );

  $form['limit'] = array(
    '#title' => t('Limit range'),
    '#type' => 'textfield',
    '#default_value' => isset($options['media_limit_range']) ? $options['media_limit_range'] : 5,
    '#required' => TRUE,
    '#size' => 2,
  );

  $form['weight'] = array(
    '#type' => 'hidden',
    '#default_value' => 50,
    '#required' => TRUE,
  );

  $form['field_name'] = array(
    '#type' => 'select',
    '#title' => t('Field'),
    '#options' => array('' => t('- Select -')) + maps_import_fields_select($converter, $fields, FALSE),
    '#required' => TRUE,
    '#description' => t('Drupal field'),
  );
  if (!empty($options)) {
    $form['field_name']['#default_value'] = $item->getField()->getId();
  }

  $form['required'] = array(
    '#type' => 'checkbox',
    '#title' => t('Required'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form validate; Add an object media relation.
 */
function maps_import_converter_mapping_edit_object_media_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  if (!is_numeric($values['start']) || !is_numeric($values['limit'])) {
    $error_element = !is_numeric($values['start']) ? 'start' : 'limit';
    form_set_error($error_element, t('Range values must be integer.'));
  }
}

/**
 * Form submit; Add an object media relation.
 *
 * @see maps_import_converter_mapping_add_object_media_form
 * @see maps_import_converter_mapping_add_object_media_form_validate
 */
function maps_import_converter_mapping_edit_object_media_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  // Save new mapping.
  $media = MS\Media\Media::createMediaFromMediaType($values['media_type'], array('id' => $values['media_type']));
  $field = new DefaultField(array('id' => $values['field_name']));

  $item = new Item(
    array(
      'converter' => $form_state['converter'],
      'property' => $media,
      'field' => $field,
      'required' => $values['required'],
      'weight' => $values['weight'],
      'options' => array(
        'media_type' => $values['media_type'],
        'media_start_range' => $values['start'],
        'media_limit_range' => $values['limit'],
      ),
    )
  );

  if (isset($values['id'])) {
    $item->setId($values['id']);
  }

  if ($item->save()) {
    drupal_set_message(t('The new mapping has been created.'));
  }
  else {
    drupal_set_message(t('An error occured while creating the new mapping.'));
  }

  $form_state['redirect'] = array(
    'admin/maps-suite/profiles/' . $form_state['converter']->getProfile()->getName() . '/object/' . $form_state['converter']->getCid() . '/mapping',
  );
}

/**
 * Form validate; Add a related entity.
 *
 * @see maps_import_converter_mapping_add_related_entity_form_submit()
 * @see maps_import_converter_mapping_add_related_entity_form_validate()
 */
function maps_import_converter_mapping_add_related_entity_form($form, &$form_state, Profile $profile, ConverterInterface $converter) {
  drupal_set_title(t('Add a related entity mapping for converter @converter', array('@converter' => $converter->getTitle())));

  $child_converter_class = 'Drupal\\maps_import\\Converter\\Child\\' . ucfirst($converter->getType());
  $child_converter = new $child_converter_class($profile);

  $form_state['child_converter'] = $child_converter;

  // Add entity_type and bundle options.
  $form['entity_type'] = array(
    '#type' => 'select',
    '#title' => t('Entity type'),
    '#description' => t('Select the entity type.'),
    '#options' => array('' => '- None -'),
    '#required' => TRUE,
  );

  $form['bundle'] = array('#type' => 'value');
  $form['bundles']['#tree'] = TRUE;

  foreach ($child_converter->entityInfo() as $entity_type => $plugin) {
    $form['entity_type']['#options'][$entity_type] = $plugin['label'];

    $form['bundles'][$entity_type] = array(
      '#type' => 'select',
      '#title' => t('Bundle type for entity %name', array('%name' => $plugin['label'])),
      '#description' => t('Select the bundle.'),
      '#options' => array('' => t('- Select -')) + maps_suite_reduce_array($plugin['bundles'], 'label'),
      '#states' => array(
        'visible' => array(
          ':input[name="entity_type"]' => array('value' => $entity_type),
        ),
        'required' => array(
          ':input[name="entity_type"]' => array('value' => $entity_type),
        ),
      ),
    );
  }

  $fields = $converter->getMapping()->getTargetFields();

  $form['field_name'] = array(
    '#type' => 'select',
    '#title' => t('Drupal Field'),
    '#options' => array('' => t('- Select -')) + maps_import_fields_select($child_converter, $fields),
    '#description' => t('Select the Drupal field.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form validation handler; add related entity form.
 *
 * @see maps_import_converter_mapping_add_related_entity_form()
 * @see maps_import_converter_mapping_add_related_entity_form_submit()
 */
function maps_import_converter_mapping_add_related_entity_form_validate($form, &$form_state) {
  if ($entity_type = $form_state['values']['entity_type']) {
    if (empty($form_state['values']['bundles'][$entity_type])) {
      form_set_error('bundles][' . $entity_type, t('!name field is required.', array('!name' => $form['bundles'][$entity_type]['#title'])));
    }
    else {
      form_set_value($form['bundle'], $form_state['values']['bundles'][$entity_type], $form_state);
    }
  }
}

/**
 * Form submission handler.
 *
 * @see maps_import_converter_edit_form()
 * @see maps_import_converter_edit_form_validate()
 */
function maps_import_converter_mapping_add_related_entity_form_submit($form, &$form_state) {
  list($profile, $converter) = $form_state['build_info']['args'];
  $child_converter = $form_state['child_converter'];

  $form_state['redirect'] = array(
    'admin/maps-suite/profiles/' . $profile->getName() . '/' . $converter->getType() . '/' . $converter->getCid() . '/mapping',
  );

  // Create a random name.
  $name = $converter->getName() . time();

  // Add some default values.
  $values = array(
    'pid' => $converter->getPid(),
    'title' => $converter->getTitle(),
    'name' => $name,
    'uid_scope' => $converter->getUidScope(),
    'entity_type' => $form_state['values']['entity_type'],
    'bundle' => $form_state['values']['bundle'],
    'class' => get_class($child_converter),
    'parent_id' => $converter->getCid(),
  );

  if ($id = db_insert('maps_import_converter')->fields($values)->execute()) {
    // Save the new mapping.
    $item = array(
      'cid' => $converter->getCid(),
      'property_id' => $converter->getUid() . ':' . $id,
      'field_name' => $form_state['values']['field_name'],
      'options' => array(
        'id' => $id,
        'parent_id' => $converter->getCid(),
      ),
    );

    if (SAVED_NEW === drupal_write_record('maps_import_mapping_item', $item, array())) {
      drupal_set_message(t('Related entity saved.'));
    }

    // Clear cache.
    CacheMappingItems::getInstance()->clearBinCache();
  }
  else {
    watchdog('maps_suite', 'An error occured while creating/updating a MaPS Import converter.<hr />Values: !values<hr/>Converter: !converter.', array('!values' => maps_suite_highlight_php($form_state['values']), '!converter' => maps_suite_highlight_php($form['#converter'])), WATCHDOG_ERROR);
    drupal_set_message(t('An error occured while saving the converter.'), 'error');
  }
}

/**
 * Form builder; Add a delayed entity.
 *
 * @see maps_import_converter_mapping_add_delayed_entity_form_submit()
 * @see maps_import_converter_mapping_add_delayed_entity_form_validate()
 */
function maps_import_converter_mapping_add_delayed_entity_form($form, &$form_state, Profile $profile, ConverterInterface $converter) {
  drupal_set_title(t('Add delayed entity mapping for converter @converter', array('@converter' => $converter->getTitle())));

  $form_state['converter'] = $converter;

  // @todo: used in maps_import_converter_mapping_form().
  // Move this in a function ?
  switch ($converter->getType()) {
    case 'object':
      $properties = array(t('Object properties') => $converter->getMapping()->getSourceProperties());
      break;

    case 'media':
      $properties = array(t('Media properties') => $converter->getMapping()->getSourceProperties());
      break;
  }
  $properties[t('Attributes')] = $converter->getMapping()->getSourceAttributes();

  $form['property_id'] = array(
    '#title' => t('Source property'),
    '#type' => 'select',
    '#options' => array('' => t('- Select -')) + maps_import_properties_select($properties),
    '#description' => t("MaPS System® object property or attribute that contain the parent object's id to update"),
    '#required' => TRUE,
  );

  // Add entity_type and bundle options.
  $form['entity_type'] = array(
    '#type' => 'select',
    '#title' => t('Entity type'),
    '#description' => t('Select the entity type.'),
    '#options' => array('' => '- None -'),
    '#required' => TRUE,
  );

  $form['bundle'] = array('#type' => 'value');
  $form['bundles']['#tree'] = TRUE;
  $form['field_name'] = array('#type' => 'value');
  $form['field_names']['#tree'] = TRUE;

  foreach ($converter->entityInfo() as $entity_type => $plugin) {
    $form['entity_type']['#options'][$entity_type] = $plugin['label'];

    $form['bundles'][$entity_type] = array(
      '#type' => 'select',
      '#title' => t('Bundle type for entity %name', array('%name' => $plugin['label'])),
      '#description' => t('Select the bundle.'),
      '#options' => array('' => t('- Select -')) + maps_suite_reduce_array($plugin['bundles'], 'label'),
      '#states' => array(
        'visible' => array(
          ':input[name="entity_type"]' => array('value' => $entity_type),
        ),
        'required' => array(
          ':input[name="entity_type"]' => array('value' => $entity_type),
        ),
      ),
    );

    foreach ($plugin['bundles'] as $bundle => $bundle_info) {
      $dummy_class = get_class($converter);
      $dummy_converter = new $dummy_class($converter->getProfile());
      $dummy_converter->setEntityType($entity_type);
      $dummy_converter->setBundle($bundle);

      $form['field_names'][$bundle] = array(
        '#type' => 'select',
        '#title' => t('Target field'),
        '#options' => array('' => t('- Select -')) + maps_import_fields_select($dummy_converter, $dummy_converter->getMapping()->getTargetFields()),
        '#description' => t('Drupal field'),
        '#states' => array(
          'visible' => array(
            ':input[name="bundles[' . $entity_type . ']"]' => array('value' => $bundle),
            ':input[name="entity_type"]' => array('value' => $entity_type),
          ),
          'required' => array(
            ':input[name="bundles[' . $entity_type . ']"]' => array('value' => $bundle),
            ':input[name="entity_type"]' => array('value' => $entity_type),
          ),
        ),
      );
    }
  }

  $form['update_mode'] = array(
    '#type' => 'select',
    '#title' => t('Update mode'),
    '#options' => array(
      '' => t('- Select -'),
      'add' => t('Add'),
      'replace' => t('Replace'),
    ),
    '#description' => t('In the target field, if the data must be added or override existing values'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}


/**
 * Form validation handler; add delayed entity form.
 *
 * @see maps_import_converter_mapping_add_delayed_entity_form_submit()
 * @see maps_import_converter_mapping_add_delayed_entity_form()
 */
function maps_import_converter_mapping_add_delayed_entity_form_validate($form, &$form_state) {
  if ($entity_type = $form_state['values']['entity_type']) {
    if (empty($form_state['values']['bundles'][$entity_type])) {
      form_set_error('bundles][' . $entity_type, t('!name field is required.', array('!name' => $form['bundles'][$entity_type]['#title'])));
    }
    else {
      $bundle = $form_state['values']['bundles'][$entity_type];
      form_set_value($form['bundle'], $bundle, $form_state);

      if (empty($form_state['values']['field_names'][$bundle])) {
        form_set_error('field_names][' . $form_state['bundle'], t('!name field is required.', array('!name' => $form['field_names'][$bundle]['#title'])));
      }
      else {
        form_set_value($form['field_name'], $form_state['values']['field_names'][$bundle], $form_state);
      }
    }
  }
}

/**
 * Form submission handler; add delayed entity form.
 *
 * @see maps_import_converter_mapping_add_delayed_entity_form()
 * @see maps_import_converter_mapping_add_delayed_entity_form_validate()
 */
function maps_import_converter_mapping_add_delayed_entity_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $item = array(
    'cid' => $form_state['converter']->getCid(),
    'property_id' => $values['property_id'],
    'field_name' => $values['field_name'],
    'options' => array(
      'post_save' => TRUE,
      'entity_type' => $values['entity_type'],
      'bundle' => $values['bundle'],
      'update_mode' => $values['update_mode'],
    ),
  );

  if (SAVED_NEW === drupal_write_record('maps_import_mapping_item', $item, array())) {
    // Clear cache.
    CacheMappingItems::getInstance()->clearBinCache();

    drupal_set_message(t('Delayed entity saved.'));
  }
}

/**
 * Form builder handler; manage item options.
 *
 * @see maps_import_converter_mapping_options_form_validate()
 * @see maps_import_converter_mapping_options_form_submit()
 */
function maps_import_converter_mapping_options_form($form, &$form_state, Profile $profile, ConverterInterface $converter, Item $item) {
  drupal_set_title(t('Mapping options for item @item', array('@item' => $item->getId())));

  $form_state['item'] = $item;
  $form_state['converter'] = $converter;

  $form += $item->optionsForm($form, $form_state);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form validation handler.
 *
 * @see maps_import_converter_mapping_options_form()
 * @see maps_import_converter_mapping_options_form_submit()
 */
function maps_import_converter_mapping_options_form_validate($form, &$form_state) {
  $form_state['item']->optionsFormValidate($form, $form_state);
}

/**
 * Form submit handler.
 *
 * @see maps_import_converter_mapping_options_form()
 * @see maps_import_converter_mapping_options_form_validate()
 */
function maps_import_converter_mapping_options_form_submit($form, &$form_state) {
  $result = $form_state['item']->optionsFormSubmit($form, $form_state);

  if ($result) {
    drupal_set_message(t('Options updated.'));
  }

  // Clear cache.
  CacheMappingItems::getInstance()->clearBinCache();

  $form_state['redirect'] = array(
    'admin/maps-suite/profiles/' . $form_state['converter']->getProfile()->getName() . '/' . $form_state['converter']->getType() . '/' . $form_state['converter']->getCid() . '/mapping',
  );
}

/**
 * Form builder.
 */
function maps_import_converter_mapping_add_object_criteria_form($form, &$form_state, Profile $profile, ConverterInterface $converter) {
  drupal_set_title(t('Add object criteria mapping for converter @converter', array('@converter' => $converter->getTitle())));

  $form = array();

  // Get the list MaPS attributes.
  $attributes = CacheMapsAttributes::getInstance()->load(array('profile' => $profile));

  // Get only the criteria attributes;
  $select = array();
  foreach($attributes as $attribute) {
    if (!is_null($attribute->getIdCriteria())) {
      $select[$attribute->getId()] = $attribute->getTranslatedTitle();
    }
  }

  $form['attribute'] = array(
    '#type' => 'select',
    '#title' => t('MaPS attribute'),
    '#options' => $select,
    '#description' => t('The MaPS criteria attribute to map.'),
  );

  // Get all the relation types.


  return $form;
}