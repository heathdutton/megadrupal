<?php
/**
 * @file
 *  wildfire_messages.template.admin.inc
 *
 * Forms for template management
 *
 * @author Craig Jones <craig@tiger-fish.com>
 *
 * Code derived from Wildfire 1:
 * @author Chris Cohen <chris.cohen@tiger-fish.com>
 */

/**
 * Form handler to list all templates and allow them to be turned on or off.
 */
function wildfire_template_list_form($form, &$form_state) {

  module_load_include('inc', 'wildfire_messages', 'wildfire_messages.template.api');

  $templates = wildfire_templates_refresh();
  $form = array();
  $form['#tree'] = TRUE;

  foreach ($templates as $name => $template) {
    $screenshot = '';

    // Is there a screenshot for this template? If so, show it. Otherwise, show
    // the 'no screenshot' image.
    if ($template['screenshot']) {
      $screenshot = theme(
        'image',
        array(
          'path' => $template['screenshot'],
          'alt' => t('Screenshot for template @template', array('@template' => $template['title'])),
          'title' => t('Screenshot for template @template', array('@template' => $template['title'])),
        )
      );
    }
    else {
      $screenshot = theme(
        'image',
        array(
          'path' => drupal_get_path('module', 'wildfire') . '/images/no-screenshot.png',
          'alt' => t('No screenshot is available for this template'),
          'title' => t('No screenshot is available for this template'),
        )
      );
    }

    $form['screenshot'][$name] = array(
      '#markup' => $screenshot,
    );
    $form['template_title'][$name] = array(
      '#type' => 'item',
      '#markup' => $template['title'],
    );
    $form['description'][$name] = array(
      '#type' => 'item',
      '#markup' => theme('wildfire_template_regions', array('template' => $template)),
    );
    $form['status'][$name] = array(
      '#type' => 'checkbox',
      '#default_value' => $template['status'],
    );
    $form['settings'][$name] = array(
      '#type' => 'markup',
      '#markup' => l(
        t('settings'),
        'admin/wildfire/templates/' . $name . '/settings'
      )
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for wildfire_template_list_form().
 */
function wildfire_template_list_form_submit($form, &$form_state) {

  module_load_include('inc', 'wildfire_messages', 'wildfire_messages.template.api');

  foreach ($form_state['values']['status'] as $name => $value) {
    if ($value) {
      wildfire_template_enable($name);
    }
    else {
      wildfire_template_disable($name);
    }
  }

  drupal_set_message(t('Changes have been saved.'));
}

/**
 * Form handler to store template settings
 *
 * @param $form
 * @param $form_state
 * @param $template
 *  Machine name for the template
 *
 * @return array
 *  A valid Drupal FAPI array
 */
function wildfire_template_settings_form($form, &$form_state, $template) {

  module_load_include('inc', 'wildfire_messages', 'wildfire_messages.template.api');

  $form = array();

  $settings = wildfire_templates_get_settings($template, 0);

  $form['settings'] = array(
    '#type' => 'container',
  );

  if (!empty($settings)) {

    foreach ($settings as $skey => $svalue) {

      $form['settings'][$skey] = array(
        '#type' => 'fieldset',
        '#title' => filter_xss(t(drupal_ucfirst($skey))),
      );

      foreach ($svalue as $key => $value) {

        $form['settings'][$skey]['settings_' . $skey . '_' . $key] = array(
          '#type' => 'textfield',
          '#title' => filter_xss($key),
          '#default_value' => $value,
        );

        if ($skey == 'files') {
          // Also add a file upload field
          $form['settings'][$skey]['settings_' . $skey . '_' . $key . '_upload'] = array(
            '#type' => 'file',
            '#title' => filter_xss($key . ' - upload'),
          );
        }

      }

    }

    $form_state['template'] = $template;
    $form_state['delta'] = 0;

    $form['delta'] = array(
      '#type' => 'hidden',
      '#value' => 0,
    );

    $form['actions'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'id' => 'edit-actions',
        'class' => array('form-actions'),
      )
    );

    $form['actions']['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#attributes' => array(
        'class' => array('button-yes'),
      )
    );


  }
  else {

    $form['settings']['no-configuration'] = array(
      '#type' => 'markup',
      '#markup' => t(
        'The "!template" template has no settings that can be configured',
        array(
          '!template' => $template
        )
      )
    );

  }

  return $form;

}

/**
 * Submit handler to store template settings
 */
function wildfire_template_settings_form_submit($form, &$form_state) {

  module_load_include('inc', 'wildfire_messages', 'wildfire_messages.template.api');

  $settings = array();

  if (is_array($form_state['values'])) {

    // Go through the submission, processing any file uploads.
    foreach ($form_state['values'] as $key => $value) {
      if (isset($form_state['values'][$key . '_upload'])) {

        $file = file_save_upload($key . '_upload', array(), 'public://wildfire_template_assets/', FILE_EXISTS_RENAME);

        if (!empty($file)) {
          $file->status = FILE_STATUS_PERMANENT;
          file_save($file);
          $form_state['values'][$key] = $file->filename;
        }

        unset($form_state['values'][$key . '_upload']);
      }
    }

    // Create the settings to store. We only store variables that have a '_' in
    // them, as a nested array.
    foreach ($form_state['values'] as $key => $value) {
      $pc = explode('_', $key);
      if (is_array($pc) && $pc[0] == 'settings') {
        $settings[$pc[1]][$pc[2]] = $value;
      }
    }

  }

  if (wildfire_templates_set_settings(
    $form_state['template'],
    $form_state['delta'],
    $settings
  )) {

    drupal_set_message(
      t(
        'Settings for "!template" saved',
        array(
          '!template' => $form_state['template']
        )
      ),
      'status'
    );

    drupal_goto('admin/wildfire/templates');

  }
  else {

    drupal_set_message(
      t(
        'Settings for "!template" could not be saved',
        array(
          '!template' => $form_state['template']
        )
      ),
      'error'
    );

  }

}
