<?php

/**
 * @file
 * Admin interface for the Ransom Note (ransom_note) module.
 */

/**
 * Implements hook_form().
 *
 * Configuration form for Ransom Note module.
 *
 * @ingroup forms
 */
function ransom_note_configure() {
  $form = array();
  $options = array();
  $fonts = fontyourface_get_fonts('enabled = 1');
  foreach ($fonts as $font) {
    $options[$font->fid] = $font->name;
  }
  $form['ransom_note_fonts'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Fonts to use'),
    '#options'       => $options,
    '#default_value' => variable_get('ransom_note_fonts', array()),
    '#description'   => t('Select which of enabled fonts should be used.'),
  );
  $form['ransom_note_selectors'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Target selectors'),
    '#rows'          => 3,
    '#default_value' => variable_get('ransom_note_selectors', ''),
    '#required'      => TRUE,
    '#description'   => t('A comma-separated list of HTML tag names, CSS classes, IDs and such; for advanced options, see !link.', array('!link' => l(t('jQuery documentation'), 'http://api.jquery.com/category/selectors/'))),
  );
  return system_settings_form($form);
}
