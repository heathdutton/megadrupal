<?php

/**
 * @file
 * Breadcrumb administration and module settings UI.
 *
 */
 
/**
 * Displays the breadcrumb admin overview page.
 */
function breadcrumb2_overview_breadcrumbs() {
  $view = views_get_view('breadcrumbs', 'default');
  $view->override_url = $_GET['q'];
  return $view->preview();
}

/**
 * Returns a breadcrumb submission form.
 */
function breadcrumb2_add() {
  $breadcrumb = entity_get_controller('breadcrumb2')->create();

  if (isset($_GET['path'])) {
    $breadcrumb->path = $_GET['path'];
  }

  $output = drupal_get_form('breadcrumb2_form', $breadcrumb);

  return $output;
}

/**
 * The breadcrumb edit form.
 */
function breadcrumb2_form($form, &$form_state, $breadcrumb) {
  // Save the breadcrumb for later, in case we need it.
  $form['#breadcrumb'] = $breadcrumb;
  $form_state['breadcrumb'] = $breadcrumb;
  
  $form['bid'] = array(
    '#type' => 'value',
    '#value' => isset($breadcrumb->bid) ? $breadcrumb->bid : NULL,
  );

  // Add the field related form elements.
  field_attach_form('breadcrumb2', $breadcrumb, $form, $form_state);

  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#maxlength' => 60,
    '#default_value' => !empty($breadcrumb->path) ? $breadcrumb->path : '',
    '#weight' => -10,
  );
  if (!empty($breadcrumb->path)) {
    $form['path']['#disabled'] = TRUE;
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );
  if (!empty($breadcrumb->bid)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete breadcrumb'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('breadcrumb2_form_submit_delete')
    );
  }
  $form['#validate'][] = 'breadcrumb2_form_validate';
  $form['#submit'][] = 'breadcrumb2_form_submit';
  return $form;
}

function breadcrumb2_form_validate($form, &$form_state) {
  $breadcrumb = $form_state['breadcrumb'];
  $breadcrumb->path = $form_state['values']['path'];
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('breadcrumb2', $breadcrumb, $form, $form_state);
}

/**
 * Breadcrumb form submit handler.
 */
function breadcrumb2_form_submit($form, &$form_state) {

  $breadcrumb = &$form_state['breadcrumb'];
  
  $breadcrumb->bid = $form_state['values']['bid'];
  $breadcrumb->path = $form_state['values']['path'];
  
  // Notify field widgets.
  field_attach_submit('breadcrumb2', $breadcrumb, $form, $form_state);
  
  // Save the breadcrumb
  breadcrumb2_save($breadcrumb);

  drupal_set_message(t('Breadcrumb saved.'));
  $form_state['redirect'] = 'breadcrumb/' . $breadcrumb->bid;

}

/**
 * Breadcrumb view page.
 */
function breadcrumb2_page_view($breadcrumb, $view_mode = 'full') {
  $build = $breadcrumb->view($view_mode);
  if ($view_mode = 'full') {
    $build['breadcrumb2'][$breadcrumb->bid]['path'] = array(
      '#markup' => filter_xss($breadcrumb->path),
      'weight' => -5,
    );
  }
  return $build;
}

/**
 * Breadcrumb edit page.
 */
function breadcrumb2_page_edit($breadcrumb) {
  return drupal_get_form('breadcrumb2_form', $breadcrumb);
}

/**
 * Breadcrumb form submit handler for the delete button.
 */
function breadcrumb2_form_submit_delete($form, &$form_state) {
  $breadcrumb = $form_state['breadcrumb'];
  $form_state['redirect'] = 'breadcrumb/' . $breadcrumb->bid . '/delete';
}

/**
 * Confirm form for deleting a profile.
 */
function breadcrumb2_delete_confirm_form($form, &$form_state, $breadcrumb) {
  $form_state += array('breadcrumb' => $breadcrumb);
  $confirm_question = t('Are you sure you want to delete breadcrumb for path %path?', array('%path' => $breadcrumb->path));
  return confirm_form($form, $confirm_question, 'breadcrumb/' . $breadcrumb->bid);
}

function breadcrumb2_delete_confirm_form_submit($form, &$form_state) {
  $breadcrumb = $form_state['breadcrumb'];
  $breadcrumb->delete();
  drupal_set_message(t('Deleted breadcrumb for path %path.', array('%path' => $breadcrumb->path)));
  $form_state['redirect'] = 'admin/structure/breadcrumbs';
}

/**
 * Breadcrumbs settings form.
 */
function breadcrumb2_admin_settings($form, &$form_state) {
  $form['breadcrumb2_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Prefix'),
    '#default_value' => variable_get('breadcrumb2_prefix', t('You are here:')),
    '#description' => t('Optional custom prefix.'),
  );
  $form['breadcrumb2_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Separator'),
    '#default_value' => variable_get('breadcrumb2_separator', '»'),
    '#size' => 3,
    '#description' => t('Symbol that separates breadcrumbs.'),
  );
  $form['breadcrumb2_append_page_title_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page Title'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['breadcrumb2_append_page_title_fieldset']['breadcrumb2_append_page_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Append page title'),
    '#default_value' => variable_get('breadcrumb2_append_page_title', TRUE),
    '#description' => t('Append the page title at the end of every breadcrumb.'),
  );

  $form['breadcrumb2_append_page_title_fieldset']['breadcrumb2_append_page_title_no_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use plain text page title crumb'),
    '#default_value' => variable_get('breadcrumb2_append_page_title_no_link', TRUE),
    '#description' => t('Enable this option to remove the link from the appended page title crumb.'),
  );  
  $form['breadcrumb2_hide_breadcrumb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide breadcrumb'),
    '#default_value' => variable_get('breadcrumb2_hide_breadcrumb', TRUE),
    '#description' => t('Hide breadcrumb if it only contains "Home".'),
  );
  
  $form['breadcrumb2_home'] = array(
    '#type' => 'textfield',
    '#title' => t('Home breadcrumb text'),
    '#default_value' => variable_get('breadcrumb2_home', t('Home')),
    '#description' => t('This text will be displayed as the first item of the breadcrumb trail. Typically Home or your site name. Leave blank to have no home breadcrumb.'),
  );
  return system_settings_form($form);
}
