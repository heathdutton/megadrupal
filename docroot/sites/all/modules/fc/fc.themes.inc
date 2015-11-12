<?php
/**
 * @file
 * Field Complete - Provides field-based completeness for any entity - themes.
 */

function theme_form_complete_marker($variables) {
  // This is may be used in the installer, pre-database setup.
  $t = get_t();
  $attributes = array(
    'class' => 'form-entity-complete',
    'title' => $t(variable_get('fc_tooltip', 'This field can be left blank but adds to the completeness of this item.')),
  );
  return '<span' . drupal_attributes($attributes) . '>&#8224;</span>';
}