<?php

/**
 * @file
 * Administrative UI for languages management.
 */

use Drupal\maps_import\Profile\Profile;

/**
 * Form builder; languages mapping form.
 *
 * @see maps_import_settings_languages_form_validate()
 * @ingroup forms
 */
function maps_import_settings_languages_form($form, &$form_state, Profile $profile) {
  drupal_set_title(t('Languages for profile @profile', array('@profile' => $profile->getTitle())));

  $form['#tree'] = TRUE;
  $form['#theme'] = 'maps_import_settings_languages_form';
  $form['#profile'] = $profile;

  // Source.
  $source = array();

  foreach ($profile->getMapsConfigurationLanguages() as $id => $language) {
    $source[$id] = check_plain($language['title']);

    if ($id == Profile::MAPS_SUITE_MAPS_SYSTEM_LANGUAGE_DEFAULT_ID) {
      $source[$id] .= ' - ' . t('Default language');
    }
  }

  // Target.
  $target = array('' => t('Ignored'));
  $languages = language_list('enabled');
  $language_default = language_default('language');

  foreach ($languages[1] as $langcode => $language) {
    $target[$langcode] = t('@title', array('@title' => $language->name));

    if ($language_default === $langcode) {
      $target[$langcode] .= ' (' . t('Default language') . ')';
    }
  }

  $default = $profile->getLanguages() + array_fill_keys(array_keys($source), '');

  $element = &$form[$profile->getLanguagesVariableKey()];
  $element = array();

  foreach ($source as $id => $name) {
    $form['titles'][$id] = array('#markup' => $name);
    $element[$id] = array(
      '#type' => 'select',
      '#options' => $target,
      '#default_value' => $default[$id],
    );
  }

  return $source ? system_settings_form($form) : $form;
}

/**
 * Form validation handler; languages mapping form.
 *
 * @see maps_import_settings_languages_form()
 */
function maps_import_settings_languages_form_validate($form, &$form_state) {
  $vkey = $form['#profile']->getLanguagesVariableKey();

  if ($values = array_filter($form_state['values'][$vkey])) {
    $unique = array_unique($values);

    if ($diff = array_diff_key($values, $unique)) {
      // Highlight all fields that contain errors in one operation.
      // The message is added manually after.
      array_map('form_error', array_intersect_key($form[$vkey], $diff));
      drupal_set_message(t('Each Drupal language should only be mapped once.'), 'error');
    }
  }
}

/**
 * Returns HTML for the language overview form.
 *
 * @ingroup themeable
 */
function theme_maps_import_settings_languages_form($variables) {
  $form = $variables['form'];

  $vkey = $form['#profile']->getLanguagesVariableKey();
  $variables = array(
    'header' => array(t('MaPS language'), t('Drupal language')),
    'rows' => array(),
    'empty' => t('No language was found in the MaPS System® configuration.'),
  );

  foreach (element_children($form[$vkey]) as $key) {
    $row = array();
    $row[] = array('data' => drupal_render($form['titles'][$key]), 'header' => TRUE);

    // Add error class manually, because _form_set_class() does not handle
    // empty messages. Issue here: http://drupal.org/node/289452, where comment
    // #64 is taken from current module.
    // @see maps_settings_languages_form_validate()
    if (isset($form[$vkey][$key]['#parents']) && NULL !== form_get_error($form[$vkey][$key])) {
      $form[$vkey][$key]['#attributes']['class'][] = 'error';
    }

    $row[] = drupal_render($form[$vkey][$key]);
    $variables['rows'][] = $row;
  }

  $form[$vkey]['#printed'] = TRUE;

  return theme('table', $variables) . drupal_render_children($form);
}
