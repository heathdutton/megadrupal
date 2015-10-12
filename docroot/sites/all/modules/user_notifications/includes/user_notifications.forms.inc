<?php

/**
 * Form callback wrapper: create or edit a notification.
 *
 * @param UserNotification $user_notification
 *   The user notification object being edited by this form.
 *
 * @see user_notifications_notification_form()
 */
function user_notifications_notify_form_wrapper(UserNotification $user_notification) {
  // Include the forms file from the Product module.
  module_load_include('inc', 'user_notifications', 'includes/user_notifications.forms');
  return drupal_get_form('user_notifications_notification_form', $user_notification);
}

/**
 *
 */
function user_notifications_notification_form($form, &$form_state, UserNotification $user_notification) {
  $wrapper = entity_metadata_wrapper('user_notifications', $user_notification);
  $state = $wrapper->state->value();

  $form_state['user_notifications'] = $user_notification;
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'user_notifications') . '/includes/user_notifications.forms.inc';

  $default_state = user_notifications_get_default_state();
  $states = user_notifications_get_states_options(array('show_on_form' => TRUE));

  $form['state'] = array(
    '#type' => 'select',
    '#title' => t('State'),
    '#description' => t('State of this notification'),
    '#default_value' => ($state ? $state->identifier() : $default_state->identifier()),
    '#options' => $states,
    '#access' => !empty($states) && user_access('edit ' . $user_notification->type . ' user notification state'),
  );

  // Add the field related form elements.
  $form_state['user_notification'] = $user_notification;

  $langcode = entity_language('user_notifications', $user_notification);

  if (empty($langcode)) {
    $langcode = LANGUAGE_NONE;
  }

  field_attach_form('user_notifications', $user_notification, $form, $form_state, $langcode);

  // Load the product type to get the default revision setting.
  //$product_type = commerce_product_type_load($product->type);

  #// When updating a product, do not collapse the Change History fieldset if the
  #// product type is configured to create a new revision by default.
  #$form['change_history'] = array(
  #  '#type' => 'fieldset',
  #  '#title' => t('Change history'),
  #  '#collapsible' => TRUE,
  #  '#collapsed' => empty($product->product_id) || empty($product_type['revision']),
  #  '#weight' => 50,
  #);
  #if (!empty($product->product_id)) {
  #  $form['change_history']['revision'] = array(
  #    '#type' => 'checkbox',
  #    '#title' => t('Create new revision on update'),
  #    '#description' => t('If an update log message is entered, a revision will be created even if this is unchecked.'),
  #    '#default_value' => $product_type['revision'],
  #    '#access' => user_access('administer commerce_product entities'),
  #  );
  #}
  #$form['change_history']['log'] = array(
  #  '#type' => 'textarea',
  #  '#title' => !empty($product->product_id) ? t('Update log message') : t('Creation log message'),
  #  '#rows' => 4,
  #  '#description' => t('Provide an explanation of the changes you are making. This will provide a meaningful history of changes to this product.'),
  #);

  #$form['actions'] = array(
  #  '#type' => 'actions',
  #  '#weight' => 400,
  #);

  #// Simply use default language
  #$form['language'] = array(
  #  '#type' => 'value',
  #  '#value' => $langcode,
  #);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Notification'),
  );

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  //$form['#validate'][] = 'commerce_product_product_form_validate';

  return $form;
}

/**
 * Submit callback for user_notifications_notification_form().
 */
function user_notifications_notification_form_submit($form, &$form_state) {
  $user_notification = &$form_state['user_notifications'];

  if (!empty($form_state['values']['state'])) {
    $user_notification->state = $form_state['values']['state'];
  }

  //TODO: Copied from commerce_product and to be implemented time permitting...
  #// Trigger a new revision if the checkbox was enabled or a log message supplied.
  #if ((user_access('administer commerce_product entities') && !empty($form_state['values']['revision'])) ||
  #  (!user_access('administer commerce_product entities') && !empty($form['change_history']['revision']['#default_value'])) ||
  #  !empty($form_state['values']['log'])) {
  #  $product->revision = TRUE;
  #  $product->log = $form_state['values']['log'];
  #}

  // Notify field widgets.
  field_attach_submit('user_notifications', $user_notification, $form, $form_state);

  // Save the product.
  user_notifications_save($user_notification);

  // Redirect based on the button clicked.
  drupal_set_message(t('Notification saved.'));
}

/**
 * User notification states setting form
 */
function user_notifications_state_overview_form($form, &$form_state) {
  $user_notifications_states = user_notifications_states();

  // Form elements for existing states .
  $form['state']['#tree'] = TRUE;
  if (!empty($user_notifications_states)) {
    foreach ($user_notifications_states as $sid => $state) {
      $form['state'][$sid]['sid'] = array(
        '#type' => 'hidden',
        '#default_value' => $sid,
      );
      $form['state'][$sid]['label'] = array(
        '#title' => t('Label'),
        '#type' => 'textfield',
        '#default_value' => $state->label,
        '#maxlength' => 128,
        '#size' => 20,
        '#required' => TRUE,
      );
      $form['state'][$sid]['default_state'] = array(
        '#type' => 'radio',
        '#name' => 'default_state',
        '#return_value' => $sid,
        '#tree' => FALSE,
        '#default_value' => ($state->default_state) ? $sid : FALSE,
      );
      $form['state'][$sid]['weight'] = array(
        '#title' => t('Weight'),
        '#type' => 'weight',
        '#default_value' => $state->weight,
        '#delta' => 15,
        '#attributes' => array('class' => array('user-notifications-state-weight')),
      );
      $form['state'][$sid]['description'] = array(
        '#title' => t('Description'),
        '#type' => 'textfield',
        '#default_value' => $state->description,
        '#maxlength' => 128,
        '#size' => 50,
      );
      $form['state'][$sid]['active'] = array(
        '#title' => t('Active'),
        '#type' => 'checkbox',
        '#default_value' => $state->active,
      );
      $form['state'][$sid]['show_on_form'] = array(
        '#title' => t('Show on form'),
        '#type' => 'checkbox',
        '#default_value' => $state->show_on_form,
      );
      $form['state'][$sid]['delete'] = array(
        '#type' => 'item',
        '#markup' => l(t('delete'), 'admin/structure/user-notifications/user-notifications-states/manage/' . $state->identifier() . '/delete', array('query' => drupal_get_destination())),
      );
      $form['state'][$sid]['export'] = array(
        '#type' => 'item',
        '#markup' => l(t('export'), 'admin/structure/user-notifications/user-notifications-states/manage/' . $state->identifier() . '/export'),
      );
    }
  }

  $form['help'] = array(
    '#type' => 'item',
    '#description' => t("This table defines the user notification states available on this site. A default is required and will be used if no states are marked as 'Show on Form'."),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Submit handler for the user notifications states form
 */
function user_notifications_state_overview_form_submit($form, &$form_state) {

  $default_state = $form_state['values']['default_state'];
  foreach ($form_state['input']['state'] as $state) {
    $user_notifications_state = entity_load_single('user_notifications_state', $state['sid']);
    $user_notifications_state->label = $state['label'];
    $user_notifications_state->default_state = $state['sid'] == $default_state;
    $user_notifications_state->active = isset($state['active']) ? $state['active'] : 0;
    $user_notifications_state->show_on_form = isset($state['show_on_form']) ? $state['show_on_form'] : 0;
    $user_notifications_state->weight = $state['weight'];
    $user_notifications_state->description = $state['description'];

    $user_notifications_state->save();
  }

  drupal_set_message(t('Your user notification states have been saved.'));
}

/**
 * Theme handler for user notifications states form
 */
function theme_user_notifications_state_overview_form($variables) {
  $form = $variables['form'];

  drupal_add_tabledrag('user-notifications-state-admin-settings-table', 'order', 'self', 'user-notifications-state-weight');
  $header = array(
    array('data' => t('Label !required', array('!required' => '<span class="form-required" title="' . t('This field is required.') . '">*</span>'))),
    array('data' => t('Description')),
    array('data' => t('Default')),
    array('data' => t('Active')),
    array('data' => t('Show on form')),
    array('data' => t('Weight')),
    array('data' => t('Operations'), 'colspan' => 2)
  );

  foreach (element_children($form['state']) as $key) {
    $row = array();
    foreach (array(
        'label',
        'description',
        'default_state',
        'active',
        'show_on_form',
        'weight',
        'delete',
        'export'
      ) as $element) {
      // Since we're rendering these in a table, remove any #title attributes.
      if (!empty($form['state'][$key][$element]['#title'])) {
        unset($form['state'][$key][$element]['#title']);
      }
      $row[] = drupal_render($form['state'][$key][$element]);
    }

    $rows[] = array(
      'class' => array('draggable'),
      'data' => $row,
    );
  }

  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'user-notifications-state-admin-settings-table')
  ));

  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Generates the User Notification state editing form.
 */
function user_notifications_state_form($form, &$form_state, $user_notifications_state, $op = 'edit') {

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $user_notifications_state->label,
    '#description' => t('The human-readable name of this user notification state.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $user_notifications_state->identifier(),
    '#maxlength' => 32,
    //'#disabled' => $registration_state->locked && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'user_notifications_get_states',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for
    this user notification state. It must only contain lowercase letters,
     numbers, and underscores.'),
  );
  $form['default_state'] = array(
    '#title' => t('Default'),
    '#type' => 'checkbox',
    '#default_value' => isset($user_notifications_state->default_state) ?
      $user_notifications_state->default_state : 0,
    '#attributes' => array('class' => array('reg-default')),
  );
  $form['weight'] = array(
    '#title' => t('Weight'),
    '#type' => 'weight',
    '#default_value' => isset($user_notifications_state->weight) ?
      $user_notifications_state->weight : 0,
    '#delta' => 15,
    '#attributes' => array('class' => array('user-notifications-state-weight')),
  );
  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textfield',
    '#default_value' => isset($user_notifications_state->description) ?
      $user_notifications_state->description : '',
    '#maxlength' => 128,
    '#size' => 50,
  );
  $form['active'] = array(
    '#title' => t('Active'),
    '#type' => 'checkbox',
    '#default_value' => isset($user_notifications_state->active) ?
      $user_notifications_state->active : 0,
  );
  $form['show_on_form'] = array(
    '#title' => t('Show on form'),
    '#type' => 'checkbox',
    '#default_value' => isset($user_notifications_state->show_on_form) ?
      $user_notifications_state->show_on_form : 0,
  );


  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save User Notification state'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function user_notifications_state_form_submit(&$form, &$form_state) {
  $user_notifications_state = entity_ui_form_submit_build_entity($form, $form_state);
  $user_notifications_state->save();
  $form_state['redirect'] = 'admin/structure/user-notifications/user-notifications-states';
}
