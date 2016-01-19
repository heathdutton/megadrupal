<?php
/**
 * @file
 * Defines the Accordion Container webform component.
 */


/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_accordion_grp() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'title_display' => 0,
      'collapsible' => 0,
      'collapsed' => 0,
      'description' => '',
      'private' => FALSE,
      'webform_accordion_settings' => array(
        'webform_accordion_grp_active' => '0',
        'webform_accordion_grp_autoheight' => TRUE,
        'webform_accordion_grp_clearstyle' => FALSE,
        'webform_accordion_grp_collapsible' => FALSE,
      ),
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_accordion_grp($component) {
  $form = array();

  // Empty placeholder so that users don't get a warning about
  // the component not having an edit function defined.
  $form['webform_accordion_grp_placeholder'] = array(
    '#type' => 'markup',
    '#value' => '',
  );
  $form['extra']['webform_accordion_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Accordion Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['extra']['webform_accordion_settings']['webform_accordion_grp_autoheight'] = array(
    '#type' => 'checkbox',
    '#title' => t('Autoheight'),
    '#description' => t('Check to have all panels be the same height.  Uncheck to have a variable panel height.'),
    '#default_value' => $component['extra']['webform_accordion_settings']['webform_accordion_grp_autoheight'],
  );
  $form['extra']['webform_accordion_settings']['webform_accordion_grp_clearstyle'] = array(
    '#type' => 'checkbox',
    '#title' => t('Clear Style'),
    '#description' => t('Check to clear height and overflow styles after finishing animations. This enabled accordions to work with dynamic content. Requires the autoHeight option to be unchecked.'),
    '#default_value' => $component['extra']['webform_accordion_settings']['webform_accordion_grp_clearstyle'],
  );
  $form['extra']['webform_accordion_settings']['webform_accordion_grp_collapsible'] = array(
    '#type' => 'checkbox',
    '#title' => t('collapsible'),
    '#description' => t('Whether all the sections can be closed at once. When checked, allows collapsing the active section.'),
    '#default_value' => $component['extra']['webform_accordion_settings']['webform_accordion_grp_collapsible'],
  );
  $form['extra']['webform_accordion_settings']['webform_accordion_grp_active'] = array(
    '#type' => 'textfield',
    '#title' => t('Active tab'),
    '#description' => t('Enter the index (0 based) of the tab to initially open. Set to "false" to have all tabs start collapsed (Requires setting Collapsible to be checked).'),
    '#default_value' => $component['extra']['webform_accordion_settings']['webform_accordion_grp_active'],
  );
  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_accordion_grp($component, $value = NULL, $filter = TRUE) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  $active_tab = $component['extra']['webform_accordion_settings']['webform_accordion_grp_active'];
  if ($active_tab == 'false') {
    $active_tab = FALSE;
  }
  elseif (is_numeric($active_tab)) {
    $active_tab = (int) $active_tab;
  }
  else {
    $active_tab = 0;
  }

  $settings = array(
    'webform_accordion' => array(
      $component['form_key'] => array(
        'active' => $active_tab,
        'autoHeight' => (bool) $component['extra']['webform_accordion_settings']['webform_accordion_grp_autoheight'],
        'clearStyle' => (bool) $component['extra']['webform_accordion_settings']['webform_accordion_grp_clearstyle'],
        'collapsible' => (bool) $component['extra']['webform_accordion_settings']['webform_accordion_grp_collapsible'],
      ),
    ),
  );
  drupal_add_js($settings, 'setting');

  $element = array(
    '#type' => 'markup',
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : NULL,
    '#weight' => $component['weight'],
    '#description' => $filter ? _webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#attributes' => array('class' => array('webform-component-accordion-grp'), 'id' => 'webform-component-' . $component['form_key']),
    '#pre_render' => array('webform_accordion_grp_prerender', 'webform_element_title_display'),
    '#translatable' => array('title', 'description'),
    '#prefix' => '<div class="accordion-container" id="' . $component['form_key'] . '">',
    '#suffix' => '</div>',
    '#markup' => '',
  );
  return $element;
}

/**
 * Pre-render function to set a fieldset ID.
 */
function webform_accordion_grp_prerender($element) {
  $element['#attributes']['id'] = 'webform-component-' . str_replace('_', '-', implode('--', array_slice($element['#parents'], 1)));
  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_accordion_grp($component, $value, $format = 'html') {
  if ($format == 'text') {
    $element = array(
      '#title' => $component['name'],
      '#weight' => $component['weight'],
      '#theme_wrappers' => array('webform_element_text'),
      '#translatable' => array('title'),
    );
  }
  else {
    $element = _webform_render_accordion_grp($component, $value);
  }

  $element['#format'] = $format;

  return $element;
}

