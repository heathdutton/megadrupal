<?php

/**
 * @file
 * Admin page.
 */

/**
 * Feedbackify settings form.
 */
function feedbackify_settings_form() {
  $form = array();
  $form['feedbackify_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Feedbackify form ID'),
    '#default_value' => variable_get('feedbackify_id', NULL),
    '#required' => TRUE,
  );

  $form['feedbackify_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Button color'),
    '#default_value' => variable_get('feedbackify_color', '#237BAB'),
    '#description' => t('Please specify a hexadecimal color value like %color,
      or leave blank for transparent.', array('%color' => '#237BAB')),
  );

  $form['feedbackify_pos'] = array(
    '#type' => 'select',
    '#title' => t('Button Position'),
    '#default_value' => variable_get('feedbackify_pos', 'right'),
    '#options' => array(
      'left' => t('Left'),
      'right' => t('Right'),
    ),
  );

  $options = array(
    t('On every page except the listed pages.'),
    t('On the listed pages only.'),
  );
  $description = t("Enter one page per line as Drupal paths. The '*' character
        is a wildcard. Example paths are %blog for the blog page and %blog-wildcard
        for every personal blog. %front is the front page.", array(
          '%blog' => 'blog',
          '%blog-wildcard' => 'blog/*',
          '%front' => '<front>',
        )
  );
  $pages = variable_get('feedbackify_pages', '');
  $visibility = variable_get('feedbackify_visibility', 0);

  $form['feedbackify_visibility_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page specific settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['feedbackify_visibility_settings']['feedbackify_visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Display Feedbackify button'),
    '#options' => $options,
    '#default_value' => $visibility,
  );
  $form['feedbackify_visibility_settings']['feedbackify_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#default_value' => $pages,
    '#description' => $description,
    '#wysiwyg' => FALSE,
  );
  return system_settings_form($form);
}

/**
 * Settings form validate handler to ensure a hexadecimal color value.
 */
function feedbackify_settings_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['feedbackify_color'])) {
    if (!preg_match('/^#[0-9a-fA-F]{3}([0-9a-fA-F]{3})?$/', $form_state['values']['feedbackify_color'])) {
      form_error($form, t('Button color must be a hexadecimal color value like %color, or left blank for transparent.', array('%color' => '#237BAB')));
    }
  }
}
