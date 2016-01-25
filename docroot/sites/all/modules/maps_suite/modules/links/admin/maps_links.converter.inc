<?php

/**
 * @file
 * Administrative UI for link converters management.
 */

use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_links\Converter\Link as LinkConverter;

/**
 * Form builder; correspondence table form.
 *
 * @see maps_links_converters_overview_form_submit()
 * @ingroup forms
 */
function maps_links_converters_overview_form($form, &$form_state, Profile $profile) {
  drupal_set_title(t('Links converter settings for the profile @profile', array('@profile' => $profile->getTitle())));

  $form['converters'] = array('#tree' => TRUE);
  $form['#profile'] = $profile;

  foreach (maps_links_get_converters($profile->getPid()) as $cid => $converter) {
    $form['converters'][$cid]['#converter'] = $converter;
    $form['converters'][$cid]['cid'] = array('#type' => 'value', '#value' => $converter->getCid());

    $form['converters'][$cid]['weight'] = array(
      '#type' => 'weight',
      '#delta' => 50,
      '#title_display' => 'invisible',
      '#title' => t('Converter weight'),
      '#default_value' => $converter->getWeight(),
    );

    $replace = array('%maps_import_profile' => $profile->getName(), '%maps_links_converter' => $converter->getCid());
    $form['converters'][$cid]['links'] = array(
      '#theme' => 'links',
      '#links' => array(
        'converter-edit' => array(
          'href' => strtr('admin/maps-suite/profiles/%maps_import_profile/link/%maps_links_converter/edit', $replace),
          'title' => t('edit'),
        ),
        'converter-delete' => array(
          'href' => strtr('admin/maps-suite/profiles/%maps_import_profile/link/%maps_links_converter/delete', $replace),
          'title' => t('delete'),
        ),
        'converter-mapping' => array(
          'href' => strtr('admin/maps-suite/profiles/%maps_import_profile/link/%maps_links_converter/mapping', $replace),
          'title' => t('mapping'),
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
 * Form submission handler; correspondence table form.
 *
 * @see maps_links_converters_overview_form()
 */
function maps_links_converters_overview_form_submit($form, &$form_state) {
  $values = $form_state['values']['converters'];

  foreach ($values as $cid => $value) {
    if ($value['weight'] != $form['converters'][$cid]['#converter']->getWeight()) {
      drupal_write_record('maps_links_converter', $value, array('cid'));
    }
  }

  drupal_set_message(t('The changes have been saved.'));
}

/**
 * Menu callback; add a new converter.
 *
 * @param Profile $profile
 *   The profile instance.
 */
function maps_links_converter_add(Profile $profile) {
  $converter = new LinkConverter($profile);
  drupal_set_title(maps_import_profile_title($profile, 'Add a converter for the profile @profile'));
  menu_set_active_item('admin/maps-suite/profiles/' . $profile->getName() . '/link');
  $router_item = menu_get_item();
  $router_item['fake'] = TRUE;
  menu_set_item($_GET['q'], $router_item);

  return drupal_get_form('maps_links_converter_edit_form', $profile, $converter);
}

/**
 * Form builder; converter create/edit form.
 *
 * @see maps_links_converter_edit_form_validate()
 * @see maps_links_converter_edit_form_submit()
 * @ingroup forms
 */
function maps_links_converter_edit_form($form, &$form_state, Profile $profile, ConverterInterface $converter) {
  $options = $converter->getOptions();

  if ($converter->getTitle()) {
    drupal_set_title(t('Edit converter @converter', array('@converter' => $converter->getTitle())));
  }
  else {
    drupal_set_title(t('Add converter for profile @profile', array('@profile' => $profile->getTitle())));
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('A natural name for this converter. You can always change this name later. Maximum number of characters allowed: @count.', array('@count' => 32)),
    '#default_value' => $converter->getTitle(),
    '#required' => TRUE,
    '#maxlength' => 32,
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => $converter->getDescription(),
    '#description' => t('A description of this converter.'),
    '#maxlength' => 255,
  );

  if (!is_null($converter->getLinkType()) && !is_null($converter->getBundle())) {
    $entity_info = $converter->entityInfo();

    $form['_bundle'] = array(
      '#title' => t('Bundle'),
      '#value' => $entity_info[$converter->getBundle()]['label'],
      '#disabled' => TRUE,
      '#type' => 'textfield',
    );
    $form['bundle'] = array('#type' => 'hidden', '#value' => $converter->getBundle());

    $link_types = $profile->getConfigurationTypes('link_type', $profile->getDefaultLanguage());

    $form['_link_type'] = array(
      '#title' => t('Link type'),
      '#value' => $link_types[$converter->getLinkType()]['title'],
      '#disabled' => TRUE,
      '#type' => 'textfield',
    );
    $form['link_type'] = array('#type' => 'hidden', '#value' => $converter->getLinkType());
  }
  else {
    $form['bundle'] = array(
      '#type' => 'select',
      '#title' => t('Bundle'),
      '#default_value' => $converter->getBundle(),
      '#description' => t('Select the bundle.'),
      '#options' => array('' => t('- Select -')) + maps_suite_reduce_array($converter->entityInfo(), 'label'),
      '#required' => TRUE,
    );

    $form['link_type'] = array(
      '#type' => 'select',
      '#title' => t('Link type'),
      '#default_value' => $converter->getLinkType(),
      '#description' => t('Select the link type.'),
      '#options' => array('' => t('- Select -')) + maps_suite_reduce_array($profile->getConfigurationTypes('link_type', $profile->getDefaultLanguage()), 'title'),
      '#required' => TRUE,
    );
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
 * @see maps_links_converter_form()
 * @see maps_links_converter_form_submit()
 */
function maps_links_converter_edit_form_validate($form, &$form_state) {
  if (empty($form_state['values']['bundle']) || empty($form_state['values']['link_type'])) {
    form_set_error('Bundle and link type are required');
  }
}

/**
 * Form submission handler; converter edit form.
 *
 * @see maps_links_converter_edit_form()
 * @see maps_links_converter_edit_form_validate()
 */
function maps_links_converter_edit_form_submit($form, &$form_state) {
  $profile = $form_state['build_info']['args'][0];
  $converter = $form_state['build_info']['args'][1];

  $form_state['redirect'] = array('admin/maps-suite/profiles/' . $profile->getName() . '/link');

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
    'options' => array(),
    'mapping' => array(),
    'pid' => $profile->getPid(),
    'cid' => $converter->getCid(),
  );

  if ($save_state !== drupal_write_record('maps_links_converter', $form_state['values'], $update)) {
    watchdog('maps_suite', 'An error occured while creating/updating a MaPS link converter.<hr />Values: !values<hr/>Converter: !converter.', array('!values' => maps_suite_highlight_php($form_state['values']), '!converter' => maps_suite_highlight_php($converter)), WATCHDOG_ERROR);
    drupal_set_message(t('An error occured while saving the converter.'), 'error');
    $form_state['redirect'] = array('admin/maps-suite/profiles/' . $profile->getName() . '/link/converters');
  }
}

/**
 * Returns HTML for the correspondence table form.
 *
 * @ingroup themeable
 */
function theme_maps_links_converters_overview_form($variables) {
  $form = $variables['form'];
  $rows = array();

  drupal_add_tabledrag('maps-links-converters-overview', 'order', 'sibling', 'converter-weight');

  foreach (element_children($form['converters']) as $cid) {
    $form['converters'][$cid]['weight']['#attributes']['class'] = array('converter-weight');

    $link_types = $form['#profile']->getConfigurationTypes('link_type', $form['#profile']->getDefaultLanguage());

    $row = array();
    $row[] = $form['converters'][$cid]['#converter']->getTitle();
    $row[] = $form['converters'][$cid]['#converter']->getCid();
    $row[] = $form['converters'][$cid]['#converter']->getDescription();
    $row[] = $link_types[$form['converters'][$cid]['#converter']->getLinkType()]['title'];
    $row[] = $form['converters'][$cid]['#converter']->getBundle();
    $row[] = drupal_render($form['converters'][$cid]['weight']);
    $row[] = drupal_render($form['converters'][$cid]['links']);

    $rows[] = array(
      'data' => $row,
      'class' => array('draggable'),
    );
  }

  $url = url('admin/maps-suite/profiles/' . $form['#profile']->getName() . '/add/link');
  $output = theme('table', array(
    'header' => array(
      t('Title'),
      t('Machine name'),
      t('Description'),
      t('Link type'),
      t('Bundle'),
      t('Weight'),
      t('Actions'),
    ),
    'rows' => $rows,
    'attributes' => array('id' => 'maps-links-converters-overview'),
    'empty' => t('There is no defined converter yet. You can <a href="@url">add a new converter there</a>.', array('@url' => $url)),
  ));

  return $output . drupal_render_children($form);
}


/**
 * Form builder; converter deletion confirmation form.
 *
 * @see maps_links_converter_delete_confirm_form_submit()
 * @ingroup forms
 */
function maps_links_converter_delete_confirm_form($form, &$form_state, Profile $profile, ConverterInterface $converter) {
  drupal_set_title(t('Delete converter @converter', array('@converter' => $converter->getTitle())));

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
 * @see maps_links_converter_delete_confirm_form()
 */
function maps_links_converter_delete_confirm_form_submit($form, &$form_state) {
  $profile = $form_state['build_info']['args'][0];
  $converter = $form_state['build_info']['args'][1];
  $converter->delete();

  $form_state['redirect'] = array('admin/maps-suite/profiles/' . $profile->getName() . '/link');
  drupal_set_message(t('The converter @title was successfully deleted!', array('@title' => $converter->getTitle())));
}
