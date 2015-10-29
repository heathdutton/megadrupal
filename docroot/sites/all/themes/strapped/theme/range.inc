<?php
function strapped_field_widget_form_alter(&$element, &$form_state, $context) {

  // Ranges shouldnt be in fieldsets.
  if (isset($element['#range_type'])) {
    $element['#type'] = 'item';
    $element['from']['#attributes']['placeholder'] =    $element['from']['#title'];
    $element['from']['#title'] = '';
    $element['to']['#attributes']['placeholder'] =    $element['to']['#title'];
    $element['to']['#title'] = '';

  }
}
