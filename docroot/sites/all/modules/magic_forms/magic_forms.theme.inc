<?php
/**
 * @file
 * Magic forms theme file.
 */


/**
 * Theme the form element.
 *
 * @param array $variables
 *   An array of variables.
 *
 * @return string
 *   The output HTML.
 */
function theme_magic_forms_form_element($variables) {
  $element = $variables['element'];
  $magic_form = &drupal_static('magic_forms_form__' . $element['#magic-forms']['form_build_id']);

  $output = '';

  if (magic_forms_config_property(MAGIC_FORMS_FIELD_ERRORS_WRAPPER, $magic_form)) {
    $output .= "<div class=\"" . magic_forms_config_property(MAGIC_FORMS_FIELD_ERRORS_WRAPPER_CLASS, $magic_form, MAGIC_FORMS_VARS_FIELD_ERRORS_WRAPPER_CLASS_DEFAULT) . "\">";
  }

  if (magic_forms_config_property(MAGIC_FORMS_GROUP_ERRORS, $magic_form) && magic_forms_config_property(MAGIC_FORMS_GROUP_ERRORS_ANCHOR, $magic_form)) {
    $output .= "<a id=\"field-anchor-{$element['#id']}\"></a>";
  }

  if (isset($element['#magic-forms']['prefix'])) {
    $output .= $element['#magic-forms']['prefix'];
  }

  $output .= theme($element['#type'], $element);
  if (isset($element['#magic-forms']['suffix'])) {
    $output .= $element['#magic-forms']['suffix'];
  }

  if (magic_forms_config_property(MAGIC_FORMS_FIELD_ERRORS_WRAPPER, $magic_form)) {
    $output .= '</div>';
  }

  return $output;
}
