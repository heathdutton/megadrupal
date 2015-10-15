<?php

/**
 * @file
 * Definitions for the Export UI plugin and forms.
 */

/**
 * Define the Export UI plugin.
 */
$plugin = array(
  'schema' => 'commons_bw_ui',

  'access' => 'administer commons bw ui',

  'menu' => array(
    'menu prefix' => 'admin/config/user-interface',
    'menu item' => 'commons-browsing-widget-user-interface',
    'menu title' => 'Commons browsing widget user interface',
    'menu description' => 'Configure the tabbed browsing widget.',
  ),

  'title singular' => t('browsing widget tab'),
  'title plural' => t('browsing widget tabs'),
  'title singular proper' => t('Browsing widget tab'),
  'title plural proper' => t('Browsing widget tabs'),

  'handler' => array(
    'class' => 'commons_bw_ui',
    'parent' => 'ctools_export_ui',
  ),

  'form' => array(
    'settings' => 'commons_bw_ui_ctools_export_ui_form',
    'submit' => 'commons_bw_ui_ctools_export_ui_form_submit',
  ),
);

/**
 * Define the preset add/edit form.
 */
function commons_bw_ui_ctools_export_ui_form(&$form, &$form_state) {
  $tab = $form_state['item'];

  // Attach the CSS/JS responsible for hiding/displaying the extra form elements
  // required for users with JavaScript disabled.
  $form['#attached'] = array(
    'css' => array(
      drupal_get_path('module', 'commons_bw_ui') . '/commons_bw_ui.css',
    ),
    'js' => array(
      drupal_get_path('module', 'commons_bw_ui') . '/commons_bw_ui.js',
    ),
  );

  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => array(
      'view' => t('View'),
    ),
    '#default_value' => $tab->type,
    '#description' => t('Select the type of content to display inside of the browsing widget tab. Currently only %view is supported.', array('%view' => 'view')),
    '#required' => TRUE,
  );

  // Get the list of view name options.
  $view_names = _commons_bw_ui_get_view_name_options();

  // Get the default value for the view name dropdown.
  $selected = isset($form_state['values']['name']) ? $form_state['values']['name'] : key($view_names);

  $form['view_name'] = array(
    '#type' => 'fieldset',
    '#title' => t('View name'),
  );
  $form['view_name']['name'] = array(
    '#type' => 'select',
    '#title' => t('Name'),
    '#options' => $view_names,
    '#default_value' => !empty($tab->name) ? $tab->name : NULL,
    '#description' => t('Select a view to display inside the browsing widget tab.'),
    '#required' => TRUE,
    '#attributes' => array(
      'class' => array('enabled-for-ajax'),
    ),
    '#ajax' => array(
      'callback' => 'commons_bw_ui_dependent_dropdown_degrades_first_callback',
      'wrapper' => 'dropdown-second-replace',
    ),
  );

  // Since we don't know if the user has js or not, we always need to output
  // this element, then hide it with with css if javascript is enabled.
  $form['view_name']['name_selection'] = array(
    '#type' => 'submit',
    '#value' => t('Choose'),
    '#attributes' => array(
      'class' => array('view-name-selection-button'),
    ),
  );

  // If a view name exists in $form_state['values'] use it as a parameter to
  // pass to the function that retrieves the view displays.
  $selected = isset($form_state['values']['name']) ? $form_state['values']['name'] : key($view_names);

  $form['view_display'] = array(
    '#type' => 'fieldset',
    '#title' => t('View display'),
  );
  $form['view_display']['display'] = array(
    '#type' => 'select',
    '#title' => t('Display'),
    '#options' => _commons_bw_ui_get_view_display_options($selected),
    '#default_value' => !empty($tab->display) ? $tab->display : NULL,
    '#description' => t('Select a display to use to render the view.'),
    '#prefix' => '<div id="dropdown-second-replace">',
    '#required' => TRUE,
    '#suffix' => '</div>',
    '#attributes' => array('class' => array('enabled-for-ajax')),
  );

  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $tab->weight,
    '#delta' => 10,
    '#required' => TRUE,
  );

  $form['front'] = array(
    '#type' => 'checkbox',
    '#title' => t('Front'),
    '#default_value' => $tab->front,
  );

  $form['bundle'] = array(
    '#type' => 'select',
    '#title' => t('Bundle'),
    '#options' => node_type_get_names(),
    '#default_value' => $tab->bundle,
    '#description' => t('The bundle of the entity that is displayed in the tab.'),
    '#required' => TRUE,
  );

  if (empty($form_state['values']['name'])) {
    // Disable view display selection if a view name has not been chosen.
    $form['view_display']['display']['#disabled'] = TRUE;
    $form['view_display']['display']['#description'] = t('Select a view to determine its available displays.');
  }
  else {
    $form['view_display']['display']['#title'] = t('@view display', array('@view' => $view_names[$selected]));
  }
}

/**
 * Submit function for commons_bw_ui_ctools_export_ui_form().
 */
function commons_bw_ui_ctools_export_ui_form_submit($form, &$form_state) {
  // Handle the ctools save button.
  // Only submit will result in actual submission, all others rebuild.
  switch ($form_state['triggering_element']['#value']) {
    case t('Save'):
      return;
  }

  // 'Choose' or anything else will cause rebuild of the form and present
  // it again.
  $form_state['rebuild'] = TRUE;

  // As Commons Browsing Widget User Interface exposes tab content types to a
  // variety of Commons alternations, some of which occur as the result of
  // altering system information, commons_groups and commons_trusted_contacts
  // need to be reverted, otherwise the group field will not be added to the
  // content type.
  $revert = array();

  foreach (array('commons_groups', 'commons_trusted_contacts') as $module) {
    if (module_exists($module)) {
      $revert[$module] = array('field_instance');
    }
  }

  if (!empty($revert)) {
    features_revert($revert);
  }
}

/**
 * Selects just the display dropdown to be returned for re-rendering.
 *
 * @return
 *   Renderable array (the display dropdown).
 */
function commons_bw_ui_dependent_dropdown_degrades_first_callback($form, $form_state) {
  return $form['view_display']['display'];
}
