<?php
/**
 * @file
 * comstack.forms.inc
 */

/**
 * Form building function which provides the add/edit form for
 * the comstack_message_type entity.
 */
function comstack_message_type_form($form, &$form_state, $comstack_message_type, $op = 'edit', $entity_type = NULL) {
  $form['comstack_message_type'] = array(
    '#type' => 'value',
    '#value' => $comstack_message_type,
  );

  // Prevent any comstack messages from being created if no message module
  // message types exists.
  $comstack_message_types = comstack_get_message_types_flat();
  $message_types = message_type_load();
  $available_message_types = array();

  if (empty($message_types)) {
    return array(
      'form_text' => array(
        '#type' => 'markup',
        '#value' => '<p>' . t('Comstack builds off of the Message module Message types, currently none exist. To use this module first <a href="!url">create a message type</a>.', array('!url' => url('admin/structure/messages/add'))) . '</p>',
      ),
    );
  }

  // Build a list of available message types.
  if ($op === 'add') {
    // Do it!
    foreach ($message_types as $name => $type) {
      if (!in_array($name, $comstack_message_types)) {
        $available_message_types[$name] = check_plain($type->description);
      }
    }

    // If there aren't any message types available bail out.
    if (empty($available_message_types)) {
      return array(
        'form_text' => array(
          '#type' => 'markup',
          '#value' => '<p>' . t('Comstack builds off of the Message module Message types, all of the available types have been paired with. To add more <a href="!url">create click here</a>.', array('!url' => url('admin/structure/messages/add'))) . '</p>',
        ),
      );
    }
  }

  if ($op === 'clone') {
    $comstack_message_type->label .= ' (cloned)';
    $comstack_message_type->type = $entity_type . '_clone';
  }

  // The forms tabs container.
  $form['tabs'] = array(
    '#type' => 'vertical_tabs',
  );

  // Basic settings fieldset.
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#group' => 'tabs',
  );

  // The machine name and label are inherited from the Message type.
  if ($op === 'add') {
    $form['general']['type'] = array(
      '#type' => 'select',
      '#title' => t('Message type to pair to'),
      '#description' => t('Comstack pairs with a message module type and extends it. Select the message type you want to extend.'),
      '#required' => TRUE,
      '#options' => $available_message_types,
    );

    // Store the message types for the form submit handler so we can populate
    // the label property.
    $form['message_types'] = array(
      '#type' => 'value',
      '#value' => $message_types,
    );
  }
  else {
    $message_types_flat = array();
    foreach ($message_types as $k => $v) {
      $message_types_flat[$k] = check_plain($v->description);
    }

    $form['general']['type'] = array(
      '#type' => 'select',
      '#title' => t('Message type to pair to'),
      '#options' => $message_types_flat,
      '#default_value' => $comstack_message_type->type,
      '#disabled' => TRUE,
    );
    $form['label'] = array(
      '#type' => 'value',
      '#value' => $comstack_message_type->label,
    );
  }

  $form['general']['subject'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Subjects'),
    '#default_value' => isset($comstack_message_type->subject) ? $comstack_message_type->subject : 0,
    '#description' => t('Check if the messages in this type has a subject that a user can populate.'),
  );

  $form['general']['template'] = array(
    '#type' => 'checkbox',
    '#title' => t('Messages of this type should follow a template'),
    '#default_value' => isset($comstack_message_type->template) ? $comstack_message_type->template : 0,
    '#description' => t('Check if a structured text template should be used, left unchecked the messages shall be free-form/text.'),
  );

  $form['general']['template_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Template text'),
    '#default_value' => isset($comstack_message_type->template_text) ? $comstack_message_type->template_text : '',
    '#description' => t('Template follows Drupal t() conventions... should be tokens really'),
    '#states' => array(
      'visible' => array(
        ':input[id="edit-template"]' => array('checked' => TRUE),
      ),
    ),
  );

  // User preferences.
  $form['user_preferences'] = array(
    '#type' => 'fieldset',
    '#title' => t('User preference settings'),
    '#collapsible' => TRUE,
    '#group' => 'tabs',
  );

  $form['user_preferences']['user_preferences'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow user preferences for this message type'),
    '#default_value' => isset($comstack_message_type->user_preferences) ? $comstack_message_type->user_preferences : 0,
    '#description' => t('This option will allow users to select which delivery methods they get messages of this type by (from the chosen set in this form).'),
  );

  // Flood control.
  $form['flood'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flood control'),
    '#collapsible' => TRUE,
    '#group' => 'tabs',
  );

  $form['flood']['flood_control'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable flood control'),
    '#default_value' => isset($comstack_message_type->flood_control) ? $comstack_message_type->flood_control : 0,
    '#description' => t('Enable flood control to rate limit the number of messages of this type that can be sent per user.'),
  );

  $form['flood']['flood_control_interval'] = array(
    '#title' => t('Time interval'),
    '#type' => 'select',
    '#options' => drupal_map_assoc(array(60, 180, 300, 600, 900, 1800, 2700, 3600, 10800, 21600, 32400, 43200, 86400), 'format_interval'),
    '#default_value' => isset($comstack_message_type->flood_control_interval) ? $comstack_message_type->flood_control_interval : 3600,
    '#states' => array(
      'visible' => array(
        ':input[id="edit-flood-control"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['flood']['flood_control_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum number of messages that can be sent within the interval'),
    '#description' => t('Note that this limit will apply to individual users and API calls made by that user.'),
    '#required' => TRUE,
    '#default_value' => isset($comstack_message_type->flood_control_limit) ? $comstack_message_type->flood_control_limit : 50,
    '#element_validate' => array(
      'element_validate_integer_positive',
    ),
    '#states' => array(
      'visible' => array(
        ':input[id="edit-flood-control"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Delivery methods.
  $form['delivery'] = array(
    '#type' => 'fieldset',
    '#title' => t('Delivery methods &amp; settings'),
    '#collapsible' => TRUE,
    '#group' => 'tabs',
  );

  //$delivery_methods = comstack_get_delivery_methods_flat();
  $delivery_methods = array(
    'email' => t('Email'),
  );

  $form['delivery']['delivery_methods'] = array(
    '#title' => t('Delivery methods to allow'),
    '#type' => 'checkboxes',
    '#required' => TRUE,
    '#options' => $delivery_methods,
    '#default_value' => isset($comstack_message_type->delivery_methods) ? $comstack_message_type->delivery_methods : array(),
    '#tree' => TRUE,
  );

  // Message meta settings.
  if (module_exists('comstack_read_receipts')) {
    $form['read'] = array(
      '#type' => 'fieldset',
      '#title' => t('Last Read &amp; Read receipts'),
      '#collapsible' => TRUE,
      '#group' => 'tabs',
    );

    $form['read']['store_last_read_time'] = array(
      '#type' => 'checkbox',
      '#title' => t('Store last read time'),
      '#default_value' => isset($comstack_message_type->store_last_read_time) ? $comstack_message_type->store_last_read_time : 0,
      '#description' => t('If checked message metadata will be updated storing read information.'),
    );

    $form['read']['record_read_receipts'] = array(
      '#type' => 'checkbox',
      '#title' => t('Record read receipts'),
      '#default_value' => isset($comstack_message_type->record_read_receipts) ? $comstack_message_type->record_read_receipts : 0,
      '#description' => t('If checked detailed records of who has read a message and when will be kept. This allows for "Read at xx:xx" inline information.'),
    );

    $form['read']['record_read_receipts_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Grouping settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#states' => array(
        'visible' => array(
          ':input[id="edit-record-read-receipts"]' => array('checked' => TRUE),
        ),
      ),
    );

    $form['read']['record_read_receipts_perm'] = array(
      '#type' => 'checkbox',
      '#title' => t('Provide read receipts permissions'),
      '#default_value' => isset($comstack_message_type->record_read_receipts_perm) ? $comstack_message_type->record_read_receipts_perm : 0,
      '#description' => t('Provide a new permission that will allow individual users to opt out of sending read receipts.'),
    );

    $form['read']['record_read_receipts_default'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable read receipts by default?'),
      '#default_value' => isset($comstack_message_type->record_read_receipts_default) ? $comstack_message_type->record_read_receipts_default : 1,
      '#description' => t('Provide a new permission that will allow individual users to opt out of sending read receipts.'),
    );
}

  $form['grouping'] = array(
    '#type' => 'checkbox',
    '#title' => t('Group messages of this type'),
    '#default_value' => isset($comstack_message_type->grouping) ? $comstack_message_type->grouping : 0,
    '#description' => t('Check if the messages in this type can be grouped together, e.g. Notifications type with the groups: Photo, Note, Comment reply.'),
  );

  $form['grouping_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Grouping settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[id="edit-grouping"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['grouping_fieldset']['groups'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Groups'),
    '#options' => array(),
    '#default_value' => isset($comstack_message_type->groups) ? $comstack_message_type->groups : array(),
    '#tree' => TRUE,
  );


  $form['actions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable actions'),
    '#default_value' => isset($comstack_message_type->actions) ? $comstack_message_type->actions : 0,
    '#description' => t('When enabled actions will be requested via a hook per message, e.g. Dimiss, Reply.'),
  );


  // Grouping fieldset.
  $form['destination'] = array(
    '#type' => 'fieldset',
    '#title' => t('Message Grouping'),
    '#collapsible' => TRUE,
    '#group' => 'tabs',
  );


  // Form actions, named to avoid the "actions" message type entity property.
  $form['form_actions'] = array('#type' => 'actions');
  $form['form_actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save message type'),
    '#weight' => 40,
  );

  if ($op != 'add' && $op != 'clone') {
    $form['form_actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete message type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('comstack_message_type_form_submit_delete')
    );
  }

  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function comstack_message_type_form_submit(&$form, &$form_state) {
  $comstack_message_type = entity_ui_form_submit_build_entity($form, $form_state);

  // If adding a new comstack message then prepopulate the label property.
  if (!isset($comstack_message_type->id)) {
    $comstack_message_type->label = $form_state['values']['message_types'][$form_state['values']['type']]->description;
  }

  // Save the entity!
  entity_save('comstack_message_type', $comstack_message_type);

  // Redirect the user.
  $form_state['redirect'] = 'admin/structure/comstack/types';
}

/**
 * Form API submit callback for the delete button.
 */
function comstack_message_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/comstack/types/manage/' . $form_state['comstack_message_type']->type . '/delete';
}

/**
 * Admin settings form callback.
 */
function comstack_admin_settings_form() {
  $form = array();

  return system_settings_form($form);
}
