<?php
/**
 * @file
 * Fieldset related theme functions.
 */


/**
 * Implements hook_preprocess_bootstrap_panel().
 */
function strapped_preprocess_bootstrap_panel(&$variables) {

  // Apply a fix for webofmr fieldset missing a class.
  if (isset($variables['element']['#webform_component'])) {
    $variables['attributes']['class'][] = 'webform-component';
  }


  // Pass form type to the template
  if (isset($variables['element']['#form_type'])) {
    // used in the template
    $variables['form_type'] = $variables['element']['#form_type'];
  }



  if (isset($variables['element']['#title_tooltip']) && !empty($variables['element']['#title_tooltip']['title'])) {
    if (theme_get_setting('bootstrap_tooltip_enabled')) {
      $variables['title_tooltip'] = !empty($variables['element']['#title_tooltip']) ? ' <span class="tip glyphicon glyphicon-question-sign" data-toggle="tooltip" data-html="true" data-placement="top" title="' . $variables['element']['#title_tooltip']['title'] . '"></span>' : '';
    }
  }
  else {
    $variables['title_tooltip'] = '';
  }

  if (isset($variables['element']['#title_popover']) && !empty($variables['element']['#title_popover']['title'])) {
      $variables['title_popover'] = !empty($variables['element']['#title_popover']) ? ' <a class="tip glyphicon glyphicon-question-sign" data-trigger="focus" role="button" tabindex="0" data-toggle="popover" data-html="true" data-placement="top" title="' . $variables['element']['#title_popover']['title'] . '" data-content="' . $variables['element']['#title_popover']['description'] . '"></a>' : '';
  }
  else {
    $variables['title_popover'] = '';
  }


  $variables['inline'] = 0;
  if (isset($variables['element']['#title_display']) && $variables['element']['#title_display'] == 'inline') {
    // used in the template
    $variables['inline'] = 1;
  }

  // Set a template variable based on the title_display property
  if (isset($variables['element']['#title_display'])) {
    $variables['title_display'] = $variables['element']['#title_display'];
  }
  else {
    $variables['title_display'] = 'before';
  }

  // Set a template variable based on the title_width property
  if (isset($variables['element']['#title_align'])) {
    $variables['title_align'] = $variables['element']['#title_align'];
  }
  else {
    $variables['title_align'] = 'right';
  }



  // If collapsible is true and there is no ID then we had better add one.

  //
  if (!isset($variables['element']['#id']) && isset($variables['element']['#title'])) {

    // @todo would prefer to use group_name as the ID but cant find it right now and am in a hurry

    $variables['element']['#id'] = strtolower(drupal_clean_css_identifier('fieldset-' . $variables['element']['#title']));
    $variables['id'] = $variables['element']['#id'];
    // For fieldsets without an ID we need one to make vertical tabs work proper. Quiz module had this issue.
    $variables['attributes']['id'] =  $variables['id'];
  }

  // Add a class to indicate label option
  $variables['label_class'] = 'label-' . $variables['title_display'];

  // If #group is set then this is going into vertical tabs so in the template file we shouldnt mess too much with the markup of fieldsets.
  $variables['legend'] = isset($variables['element']['#group']) ? TRUE : FALSE;

  // Add support for the#value parameter
  if (isset($variables['element']['#value'])) {
    $variables['content'] .= $variables['element']['#value'];
  }


}

/**
 * Overrides theme_fieldset().
 */
function strapped_fieldset($variables) {

  // Perform some magical manipulation to allow form element to be used to render fieldsets.
  $fieldset['element'] = array();


  $parameters = array(
    '#form_type',
    '#title',
    '#title_display',
      '#element_grid',
      '#title_grid',
      '#grid',
    '#form_type',
    '#form_type_wrapper',
    '#form_group_wrapper',
    '#form_field_wrapper',
    '#title_tooltip',
    '#title_popover',
      '#value'
  );

  $attributes = array();
  // Deal with webforms.
  _strapped_merge_webform_properties( $variables['element'],$attributes);


  foreach ($parameters as $key => $value) {
    if (isset($variables['element'][$value])) {
      $fieldset['element'][$value] = $variables['element'][$value];
    }
  }


  // If a fieldset has a #from_type of horizontal then kill off the panel heading
  // inside the fieldset so it doesnt double up.
  if (isset($variables['element']['#form_type']) && $variables['element']['#form_type'] == 'horizontal') {
    $variables['element']['#title'] = NULL;
  }
  else {
    $fieldset['element']['#title'] = NULL;
  }

  // Remove the form group wrapper as it is a pain
 // $fieldset['element']['#form_group_wrapper'] = FALSE;


  // If this is a group fieldset then make sure we dont and the form wrappers as it breaks vertical tabs.
  if (isset($variables['element']['#group'])) {
    $fieldset['element']['#form_group_wrapper'] = FALSE;
    $fieldset['element']['#form_type_wrapper'] = FALSE;
    $fieldset['element']['#form_field_wrapper'] = FALSE;
  }



  // Add the children
  $fieldset['element']['#children'] = theme('bootstrap_panel', $variables);

  return theme('form_element', $fieldset);
}

