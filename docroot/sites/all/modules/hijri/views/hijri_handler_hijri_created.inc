<?php

/**
 * @file
 * Definition of hijri handler created.
 */

/**
 * Hijri Created field displays the the node Created in hijri format.
 */
class hijri_handler_hijri_created extends views_handler_field_date {
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['date_format']['#options']['hijri_full'] = t('Hijri full format: @date', array('@date' => hijri_format_date(time(), 'full')));
    $form['date_format']['#options']['hijri_long'] = t('Hijri long format: @date', array('@date' => hijri_format_date(time(), 'long')));
    $form['date_format']['#options']['hijri_medium'] = t('Hijri medium format: @date', array('@date' => hijri_format_date(time(), 'medium')));
    $form['date_format']['#options']['hijri_short'] = t('Hijri short format: @date', array('@date' => hijri_format_date(time(), 'short')));
  }

  function render($values) {
    $format = $this->options['date_format'];
    $node = node_load($values->nid);
    $correction = $node->field_hijri_correction[$node->language][0]['value'];
    $value = $values->{$this->field_alias};
    switch ($format) {
      case 'hijri_full':
        return format_string(hijri_format_date($value, 'full', NULL, $correction));

      case 'hijri_long':
        return format_string(hijri_format_date($value, 'long', NULL, $correction));

      case 'hijri_medium':
        return format_string(hijri_format_date($value, 'medium', NULL, $correction));

      case 'hijri_short':
        return format_string(hijri_format_date($value, 'short', NULL, $correction));

      default:
        return parent::render($values);
    }
  }
}

