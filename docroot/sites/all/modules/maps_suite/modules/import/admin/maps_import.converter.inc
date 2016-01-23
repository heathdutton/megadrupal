<?php

/**
 * @file
 * Administrative UI for converters management.
 */

use Drupal\maps_import\Cache\Object\Converter as CacheConverter;
use Drupal\maps_import\Cache\Object\Profile as CacheProfile;
use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Profile\Profile;

/**
 * Form builder; correspondence table form.
 *
 * @see maps_import_converters_overview_form_submit()
 * @ingroup forms
 */
function maps_import_converters_overview_form($form, &$form_state, Profile $profile, $type) {
  drupal_set_title(t('@type converter settings for the profile @profile', array('@type' => ucfirst($type), '@profile' => $profile->getTitle())));

  $form['converters'] = array('#tree' => TRUE);
  $form['#profile'] = $profile;
  $form['#converter_type'] = $type;

  foreach (maps_import_get_converters($profile->getPid()) as $cid => $converter) {
    // Filter by type.
    if (get_class($converter) !== 'Drupal\\maps_import\\Converter\\' . ucfirst($type)) {
      continue;
    }

    $form['converters'][$cid]['#converter'] = $converter;
    $form['converters'][$cid]['cid'] = array('#type' => 'value', '#value' => $converter->getCid());

    $form['converters'][$cid]['weight'] = array(
      '#type' => 'weight',
      '#delta' => 50,
      '#title_display' => 'invisible',
      '#title' => t('Converter weight'),
      '#default_value' => $converter->getWeight(),
    );

    $links = module_invoke_all('maps_import_converter_actions', $converter);
    $form['converters'][$cid]['links'] = array(
      '#theme' => 'links',
      '#links' => $links,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form submission handler; correspondence table form.
 *
 * @see maps_import_converters_overview_form()
 */
function maps_import_converters_overview_form_submit($form, &$form_state) {
  $values = $form_state['values']['converters'];

  foreach ($values as $cid => $value) {
    if ($value['weight'] != $form['converters'][$cid]['#converter']->getWeight()) {
      drupal_write_record('maps_import_converter', $value, array('cid'));
    }
  }

  drupal_set_message(t('The changes have been saved.'));
}

/**
 * Returns HTML for the correspondence table form.
 *
 * @ingroup themeable
 */
function theme_maps_import_converters_overview_form($variables) {
  $form = $variables['form'];
  $rows = array();

  drupal_add_tabledrag('maps-import-converters-overview', 'order', 'sibling', 'converter-weight');

  foreach (element_children($form['converters']) as $cid) {
    $form['converters'][$cid]['weight']['#attributes']['class'] = array('converter-weight');

    $row = array();
    $row[] = $form['converters'][$cid]['#converter']->getTitle();

    $row[] = $form['converters'][$cid]['#converter']->getDescription();
    $row[] = $form['converters'][$cid]['#converter']->getEntityType();
    $row[] = $form['converters'][$cid]['#converter']->getBundle();
    $row[] = drupal_render($form['converters'][$cid]['weight']);
    $row[] = drupal_render($form['converters'][$cid]['links']);

    $rows[] = array(
      'data' => $row,
      'class' => array('draggable'),
    );
  }

  $output = theme('table', array(
    'header' => array(t('Title'), t('Description'),
      t('Entity type'), t('Bundle'), t('Weight'), t('Actions')),
    'rows' => $rows,
    'attributes' => array('id' => 'maps-import-converters-overview'),
    'empty' => t('There is no defined converter yet.'),
  ));

  return $output . drupal_render_children($form);
}

/**
 * Menu callback; add a new converter.
 *
 * @param Profile $profile
 *   The profile instance.
 * @param string $type
 *   The converter type.
 */
function maps_import_converter_add(Profile $profile, $type) {
  if ($converter = maps_import_converter_load('Drupal\\maps_import\\Converter\\' . maps_suite_drupal2camelcase($type), $profile->getPid())) {
    drupal_set_title(maps_import_profile_title($profile, 'Add a converter for the profile @profile'));
    menu_set_active_item('admin/maps-suite/profiles/' . $profile->getName() . '/' . $type);
    $router_item = menu_get_item();
    $router_item['fake'] = TRUE;
    menu_set_item($_GET['q'], $router_item);
    return drupal_get_form('maps_import_converter_edit_form', $profile, $converter);
  }

  drupal_not_found();
  drupal_exit();
}

/**
 * Form builder; converter create/edit form.
 *
 * @see maps_import_converter_edit_form_validate()
 * @see maps_import_converter_edit_form_submit()
 * @ingroup forms
 */
function maps_import_converter_edit_form($form, &$form_state, Profile $profile, ConverterInterface $converter) {
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('A natural name for this converter. You can always change this name later. Maximum number of characters allowed: @count.', array('@count' => 32)),
    '#default_value' => $converter->getTitle(),
    '#required' => TRUE,
    '#maxlength' => 32,
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#default_value' => $converter->getName(),
    '#maxlength' => 255,
    '#description' => t("The profile's unique name."),
    '#machine_name' => array(
      'exists' => 'maps_import_converter_name_exists',
      'source' => array('title'),
      'replace_pattern' => '[^a-z0-9_]+',
      'replace' => '_',
      'pid' => $profile->getPid(),
    ),
  );

  $form['cid'] = array(
    '#type' => 'hidden',
    '#default_value' => $converter->getCid(),
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => $converter->getDescription(),
    '#description' => t('A description of this converter.'),
    '#maxlength' => 255,
  );

  $form['class'] = array(
    '#type' => 'hidden',
    '#default_value' => get_class($converter),
  );

  $uids = array();
  foreach ($converter->getMapping()->getSourceProperties() as $key => $value) {
    $uids[$key] = $value->getTranslatedTitle();
  }

  $form['uid'] = array(
    '#title' => t('Unique identifier'),
    '#type' => 'select',
    '#options' => $uids,
    '#default_value' => $converter->getUid(),
    '#description' => t('Select an object property or attribute as unique identifier.'),
    '#required' => TRUE,
  );

  $form['uid_scope'] = array(
    '#title' => t('Scope for the unique identifier'),
    '#type' => 'select',
    '#options' => array(
      ConverterInterface::SCOPE_GLOBAL => t('Global'),
      ConverterInterface::SCOPE_PROFILE => t('Profile'),
    ),
    '#default_value' => $converter->getUidScope(),
    '#description' => t('Global scope means that objects from other converters or profiles may be overriden or replace fetched ones. Profile or converter scope means that only objects fetched through the current profile or converter may override each others.'),
    '#required' => TRUE,
  );

  if ($converter->hasAdditionalOptions()) {
    $form['options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Options'),
    );
    $form['options'] += $converter->optionsForm($form, $form_state);
  }

  // Edit converter.
  if (!is_null($converter->getEntityType())
      && !is_null($converter->getBundle())) {

    $entity_info = $converter->entityInfo();

    $form['_entity_type_'] = array(
      '#title' => t('Entity type'),
      '#value' => $entity_info[$converter->getEntityType()]['label'],
      '#disabled' => TRUE,
      '#type' => 'textfield',
    );

    $form['_bundle'] = array(
      '#title' => t('Bundle'),
      '#value' => $entity_info[$converter->getEntityType()]['bundles'][$converter->getBundle()]['label'],
      '#disabled' => TRUE,
      '#type' => 'textfield',
    );

    $form['entity_type'] = array(
      '#type' => 'hidden',
      '#value' => $converter->getEntityType(),
    );

    $form['bundle'] = array(
      '#type' => 'hidden',
      '#value' => $converter->getBundle(),
    );

    $form['bundles']['#tree'] = TRUE;
    $form['bundles'][$converter->getEntityType()] = array(
      '#type' => 'hidden',
      '#value' => $converter->getBundle(),
    );
  }
  else {
    // Add entity_type and bundle options.
    $form['entity_type'] = array(
      '#type' => 'select',
      '#title' => t('Entity type'),
      '#description' => t('Select the entity type.'),
      '#options' => array('' => '- None -'),
      '#required' => TRUE,
    );

    $form['bundle'] = array('#type' => 'value', '#value' => $converter->getBundle());
    $form['bundles']['#tree'] = TRUE;

    $presets = maps_import_image_presets($converter->getProfile());

    foreach ($converter->entityInfo() as $entity_type => $plugin) {
      $form['entity_type']['#options'][$entity_type] = $plugin['label'];

      // Remove image bundle if there is no preset defined.
      if ($entity_type == 1 && empty($presets)) {
        unset($plugin['bundles']['image']);
      }

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

      if ($converter->getEntityType() === $entity_type) {
        $form['bundles'][$entity_type]['#default_value'] = $converter->getBundle();
      }
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form validation handler; converter form.
 *
 * @see maps_import_converter_form()
 * @see maps_import_converter_form_submit()
 */
function maps_import_converter_edit_form_validate($form, &$form_state) {
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
 * Form submission handler; converter edit form.
 *
 * @see maps_import_converter_edit_form()
 * @see maps_import_converter_edit_form_validate()
 */
function maps_import_converter_edit_form_submit($form, &$form_state) {
  $profile = $form_state['build_info']['args'][0];
  $converter = $form_state['build_info']['args'][1];
  $form_state['redirect'] = array('admin/maps-suite/profiles/' . $profile->getName() . '/' . $converter->getType() . '/');

  if ((int) $converter->getCid() > 0) {
    $update = array('cid');
    $save_state = SAVED_UPDATED;
  }
  else {
    $update = array();
    $save_state = SAVED_NEW;
  }

  // Add some default values.
  $form_state['values'] += array(
    'pid' => $profile->getPid(),
    'filters' => array(),
    'mapping' => array(),
  );

  if (isset($form['options'])) {
    foreach ($form['options'] as $option_name => $option) {
      if (substr($option_name, 0, 1) !== '#' && isset($form_state['values'][$option_name])) {
        $form_state['values']['options'][$option_name] = $form_state['values'][$option_name];
      }
    }
  }

  if ($save_state !== drupal_write_record('maps_import_converter', $form_state['values'], $update)) {
    watchdog('maps_suite', 'An error occured while creating/updating a MaPS Import converter.<hr />Values: !values<hr/>Converter: !converter.', array('!values' => maps_suite_highlight_php($form_state['values']), '!converter' => maps_suite_highlight_php($converter)), WATCHDOG_ERROR);
    drupal_set_message(t('An error occured while saving the converter.'), 'error');
    $form_state['redirect'] = array('admin/maps-suite/profiles/' . $profile->getName() . '/converters');
  }
  else {
    if ($save_state == SAVED_UPDATED) {
      // Get children converters if any.
      $list = db_select('maps_import_converter')
        ->fields('maps_import_converter', array('cid'))
        ->condition('parent_id', $form_state['values']['cid'])
        ->execute()
        ->fetchAllAssoc('cid');

      if ($list) {
        db_update('maps_import_converter')
          ->fields(array('title' => $form_state['values']['title']))
          ->condition('cid', array_keys($list))
          ->execute();
      }
    }
  }

}


/**
 * Form builder; converter options form.
 *
 * @see maps_import_converter_options_form_validate()
 * @see maps_import_converter_options_form_submit()
 * @ingroup forms
 */
function maps_import_converter_options_form($form, &$form_state, Profile $profile, ConverterInterface $converter) {
  drupal_set_title(t('Options for converter @converter', array('@converter' => $converter->getTitle())));

  $form['options'] = array('#tree' => TRUE) + $converter->getMapping()->optionsForm($form, $form_state);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form validation handler; converter options form.
 *
 * @see maps_import_converter_options_form()
 * @see maps_import_converter_options_form_submit()
 */
function maps_import_converter_options_form_validate($form, &$form_state) {
  $converter = $form_state['build_info']['args'][1];
  $converter->getMapping()->optionsFormValidate($form, $form_state);
}

/**
 * Form submission handler; converter options form.
 *
 * @see maps_import_converter_options_form()
 * @see maps_import_converter_options_form_validate()
 */
function maps_import_converter_options_form_submit($form, &$form_state) {
  list($profile, $converter) = $form_state['build_info']['args'];
  $converter->getMapping()->optionsFormSubmit($form, $form_state);

  db_update('maps_import_converter')
    ->fields(array('options' => serialize($form_state['values']['options'])))
    ->condition('pid', $profile->getPid())
    ->condition('cid', $converter->getCid())
    ->execute();

  drupal_set_message(t('The changes have been saved.'));
}

/**
 * Form builder; converter deletion confirmation form.
 *
 * @see maps_import_converter_delete_confirm_form_submit()
 * @ingroup forms
 */
function maps_import_converter_delete_confirm_form($form, &$form_state, Profile $profile, ConverterInterface $converter) {
  drupal_set_title(t('Delete converter @converter', array('@converter' => $converter->getTitle())));

  $form['mode'] = array(
    '#type' => 'radios',
    '#options' => array(
      'unlink' => t('Delete the converter but keep the converted entities in Drupal'),
      'delete' => t('Delete the converter and all the converted entities'),
    ),
    '#description' => t('Caution: if the entities are kept, they will no more be synchronised through MaPS Suite.'),
    '#default_value' => 0,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete the converter %title ?', array('%title' => $converter->getTitle())),
    'admin/maps-suite/profiles/' . $profile->getName() . '/' . $converter->getType(),
    '',
    t('Delete'),
    t('Cancel'));
}

/**
 * Form submission handler; delete the converter.
 *
 * @see maps_import_converter_delete_confirm_form()
 */
function maps_import_converter_delete_confirm_form_submit($form, &$form_state) {
  list($profile, $converter) = $form_state['build_info']['args'];
  $converter->delete(array('mode' => $form_state['values']['mode']));

  $form_state['redirect'] = array('admin/maps-suite/profiles/' . $profile->getName() . '/' . $converter->getType());
  drupal_set_message(t('The converter @title was successfully deleted!', array('@title' => $converter->getTitle())));
  CacheConverter::getInstance()->clearBinCache();
}

/**
 * Form builder; media types correspondance table.
 *
 * @see maps_import_converter_media_settings_form_validate()
 * @see maps_import_converter_media_settings_form_submit()
 */
function maps_import_converter_media_settings_form($form, &$form_state, Profile $profile) {
  drupal_set_title(t('Media settings for profile @profile', array('@profile' => $profile->getTitle())));

  $media_types = maps_import_get_maps_media_types($profile);
  $form['#tree'] = TRUE;
  $form['#profile'] = $profile;

  $options = $profile->getOptions();

  foreach ($media_types as $key => $title) {
    $form['media_type'][$key]['title'] = array('#type' => 'item', '#markup' => $title);

    $form['media_type'][$key]['path'] = array(
      '#type' => 'textfield',
      '#default_value' => isset($options['media_settings'][$key]['path']) ? $options['media_settings'][$key]['path'] : '',
    );

    $select_options = array(t('None'));
    // Only media type 1 (image) has a preset for the moment.
    if ($key == 1) {
      $select_options += maps_suite_reduce_array($profile->getConfigurationTypes('image_preset'), 'title');
    }

    $form['media_type'][$key]['preset'] = array(
      '#type' => 'select',
      '#options' => $select_options,
      '#default_value' => isset($options['media_settings'][$key]['preset']) ? $options['media_settings'][$key]['preset'] : '',
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form validation handler; sanitize the values.
 *
 * @see maps_import_converter_media_settings_form_validate()
 * @see maps_import_converter_media_settings_form_submit()
 */
function maps_import_converter_media_settings_form_validate($form, &$form_state) {
  foreach ($form_state['values']['media_type'] as $key => $media_type) {
    form_set_value($form['media_type'][$key]['path'], trim($media_type['path'], '\\/'), $form_state);
  }
}

/**
 * Form submission handler; save the settings.
 *
 * @see maps_import_converter_media_settings_form_validate()
 * @see maps_import_converter_media_settings_form_submit()
 */
function maps_import_converter_media_settings_form_submit($form, &$form_state) {
  $media_types = array();
  $values = $form_state['values']['media_type'];
  $profile = $form['#profile'];

  foreach ($values as $key => $value) {
    // Create the folder.
    $dir = $profile->getMediaAccessibility() . '://' . $profile->getMediaDirectory() . '/' . $value['path'];
    if (!is_dir($dir)) {
      mkdir($dir);
    }

    $media_types[$key]['path'] = $value['path'];
    $media_types[$key]['preset'] = $value['preset'];
  }

  $save = array(
    'pid' => $profile->getPid(),
    'options' => array_merge($profile->getOptions(), array('media_settings' => $media_types)),
  );

  drupal_write_record('maps_import_profile', $save, array('pid'));
  CacheProfile::getInstance()->clearBinCache();
}

/**
 * Returns HTML for the media settings form.
 *
 * @ingroup themeable
 */
function theme_maps_import_converter_media_settings_form($variables) {
  $form = $variables['form'];
  $rows = array();

  if (!empty($form['media_type'])) {
    foreach (element_children($form['media_type'], TRUE) as $key) {
      $element = &$form['media_type'][$key];

      $row = array();
      $row[] = drupal_render($element['title']);
      $row[] = drupal_render($element['path']);
      $row[] = drupal_render($element['preset']);
      $rows[] = array(
        'data' => $row,
      );
    }
  }

  $output = theme('table', array(
    'header' => array(t('MaPS media type'), t('Directory path'), t('Preset')),
    'rows' => $rows,
    'empty' => t('There is no media types defined.'),
  ));

  return $output . drupal_render_children($form);
}
