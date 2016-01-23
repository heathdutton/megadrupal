<?php
/**
 * @file
 * Admin page(s) for the classified module.
 *
 * @copyright (c) 2010 Ouest Systemes Informatiques (OSInet)
 *
 * @author Frederic G. MARAND <fgm@osinet.fr>
 *
 * @license General Public License version 2 or later
 *
 * New code implementing a feature set derived from the ed_classified module:
 *
 * ---- Information about ed_classified ----
 *
 * Michael Curry, Exodus Development, Inc.
 * exodusdev@gmail.com
 * for more information, please visit:
 *   http://exodusdev.com/drupal/modules/ed_classified.module
 *
 * Copyright (c) 2006, 2007 Exodus Development, Inc.  All Rights Reserved.
 *
 * Licensed under the terms of the GNU Public License (GPL) version 2.  Please
 * see LICENSE.txt for license terms.  Possession and use of this code signifies
 * acceptance of license terms.
 *
 * ---- /ed-classified ----
 */

/**
 * Implements hook_settings().
 *
 * @return array
 *   A form array. Keys will be automagically saved as variables.
 */
function classified_admin_settings($form, &$form_state) {
  $vid = _classified_get('vid');
  $vocabulary = taxonomy_vocabulary_load($vid, TRUE);
  if (!$vocabulary) {
    $form['error'] = array(
      '#markup' => t('Module vocabulary has gone missing. Please reinstall the module.'),
    );
    return $form;
  }

  // Ads configuration section.
  $form['ads-wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Ads configuration'),
    '#collapsible' => TRUE,
  );

  $form['ads-wrapper']['classified-max-length'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum ad length'),
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => _classified_get('max-length'),
    '#description' => t('The maximum number of characters in your Classified Ad. Set to 0 to disable the max length check.'),
  );

  $instances = field_info_instances('node', 'classified');
  if (empty($instances['body'])) {
    drupal_set_message(t('Field %body not found on %type node type. Please <a href="@link">recreate it</a>.', array(
      '%body' => 'body',
      '%type' => 'classified',
      '@link' => url('admin/structure/types/manage/classified/fields'),
    )), 'error');
  }
  $form['ads-wrapper']['classified-list-body'] = array(
    '#type' => 'radios',
    '#title' => t('List display'),
    '#options' => array(
      'empty' => t('Empty'),
      'body' => t('Node teaser'),
      'node' => t('Node view in "Ad list" <a href="@link">build mode</a>', array(
        '@link' => url('admin/structure/types/manage/classified/display/classified'),
      )),
    ),
    '#description' => t('How should Classified Ads be displayed in lists.'),
    '#default_value' => _classified_get('list-body'),
  );

  $form['ads-wrapper']['classified-edit-modr8'] = array(
    '#type' => 'checkbox',
    '#title' => t('Modr8 edits - Modr8 7.x-1.x integration exists but is not tested!'),
    '#description' => t('If modr8 is available, editing a node sends it back to moderation for non-privileged users'),
    '#default_value' => _classified_get('edit-modr8'),
  );

  // Lifetimes and grace section.
  $form['lifetimes-wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Ad Lifetimes'),
    '#theme' => 'classified_admin_lifetimes',
    // '#tree'        => TRUE,
    '#collapsible' => TRUE,
    '#description' => t('Lifetime for each classified ad category. Categories themselves are configured <a href="@here">here</a> and the vocabulary containing them is configured <a href="@there">there</a>. All durations are in days.', array(
      '@here' => url('admin/structure/taxonomy/' . $vid),
      '@there' => url('admin/structure/taxonomy/edit/vocabulary/' . $vid),
    )),
  );

  $categories = taxonomy_get_tree($vid);
  $lifetimes = _classified_get('lifetimes');
  $lifetime_default = reset($lifetimes);

  // Empty placeholder needs to be in form to allow setting in validate handler.
  $form['classified-lifetimes'] = array(
    '#type' => 'value',
    '#value' => NULL,
  );

  $form['lifetimes-wrapper']['classified-lifetime-default'] = array(
    '#type' => 'textfield',
    '#title' => t('Default lifetime'),
    '#default_value' => $lifetime_default,
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t('Will be used for every term not having a specific lifetime below.'),
  );

  foreach ($categories as $term) {
    $form['lifetimes-wrapper']['classified-lifetime-' . $term->tid] = array(
      '#type' => 'textfield',
      '#title' => theme('indentation', array('size' => $term->depth)) . check_plain($term->name),
      '#default_value' => isset($lifetimes[$term->tid]) ? $lifetimes[$term->tid] : NULL,
      '#size' => 3,
      '#maxlength' => 3,
    );
  }

  $form['lifetimes-wrapper']['classified-grace'] = array(
    '#type' => 'textfield',
    '#title' => t('Grace delay for expired ads'),
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t('Once expired, ads are kept unpublished for that delay before being deleted. Set to -1 for infinite. Setting to 0 means immediate deletion upon expiration.'),
    '#default_value' => _classified_get('grace'),
  );

  $form['#validate'][] = 'classified_admin_settings_validate';
  $ret = system_settings_form($form);
  return $ret;
}

/**
 * Validate handler for settings form.
 */
function classified_admin_settings_validate($form, &$form_state) {
  $instances = field_info_instances('node', 'classified');
  if (empty($instances['body'])) {
    form_set_error('', t('Invalid configuration: missing %body field', array(
      '%body' => 'body',
    )));
  }

  $lifetimes = array();
  $base_name = 'classified-lifetime-';
  $base_length = drupal_strlen($base_name);

  foreach (element_children($form_state['values']) as $name) {
    if (strpos($name, $base_name) !== 0) {
      continue;
    }
    elseif ($name == $base_name . 'default') {
      $lifetimes[0] = $form_state['values'][$name];
      unset($form_state['values'][$name]);
    }
    else {
      $lifetimes[drupal_substr($name, $base_length)] = $form_state['values'][$name];
      unset($form_state['values'][$name]);
    }
  }

  $form_state['values']['classified-lifetimes'] = $lifetimes;
}

/**
 * Render the ad lifetimes as a table.
 *
 * @see classified_admin_settings()
 *
 * @param array $variables
 *   A theme arguments array. Single key: form.
 *
 * @return array
 *   A render array.
 */
function theme_classified_admin_lifetimes($variables) {
  $form = $variables['form'];
  $header = array(
    t('Category'),
    t('Days'),
  );
  $rows = array();

  foreach (element_children($form) as $key) {
    $element = &$form[$key];
    if ($key == 'classified-lifetime-default') {
      $title = $element['#title']
        . '<div class="description">' . $element['#description'] . '</div>';
      $element['#title'] = $element['#description'] = NULL;
      $rows[] = array(
        $title,
        drupal_render($element),
      );
    }
    elseif ($key == 'classified-grace') {
      $title = $element['#title']
        . '<div class="description">' . $element['#description'] . '</div>';
      $element['#title'] = $element['#description'] = NULL;
      $rows[] = array(
        $title,
        drupal_render($element),
      );
    }
    elseif (strpos($key, 'classified-lifetime-') !== 0) {
      continue;
    }
    else {
      $title = $element['#title'];
      $element['#title'] = NULL;
      $rows[] = array(
        $title,
        drupal_render($element),
      );
    }
  }

  $ret = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'classified-admin-settings-lifetimes')));
  $ret .= drupal_render_children($form);
  return $ret;
}
