<?php
/**
 * @file
 * Admin page callbacks for recipient configuration.
 */

/**
 * Form function for recipient views.
 *
 * @ingroup forms
 */
function manymail_views_targets_views_form($form, &$form_state) {
  $options = array();
  $views = _manymail_views_valid_views();

  $header = array(
    'human_name' => t('Name'),
    'description' => t('Description'),
  );

  foreach ($views as $name => $view) {
    $options[$name] = array(
      'human_name' => check_plain($view->human_name),
      'description' => check_plain($view->description),
    );
  }

  $form['allowed_views'] = array(
    '#type' => 'fieldset',
    '#title' => t('Allowed recipient views'),
    '#description' => t('Select which views can be used as a recipient view.<br />Note: to be selectable here, a view has to be enabled and return a UID.'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['allowed_views']['manymail_views_allowed_views'] = array(
    '#type' => 'tableselect',
    '#default_value' => variable_get('manymail_views_allowed_views', array()),
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No views available.'),
    '#js_select' => FALSE,
  );

  $form['allowed_views']['manymail_views_allowed_views_override'] = array(
    '#type' => 'checkbox',
    '#title' => t('Certain users may ignore this restriction'),
    '#default_value' => variable_get('manymail_views_allowed_views_override', 1),
    '#description' => t('When checked, users with the :permission permission can target any !what regardless of what has been configured above.', array(
      ':permission' => 'Override preset recipient views',
      '!what' => 'view',
    )),
  );

  return system_settings_form($form);
}
