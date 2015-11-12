<?php

/**
 * @file
 * Menu callbacks to administrative screens for node payments.
 */

/**
 * Menu callback for pay_node admin screen.
 */
function pay_node_admin_settings($form, &$form_state) {
  $form = array();

  // Available node payment models.
  $pay_node_models = variable_get('pay_node_models', array());
  $form['pay_node_models'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Available node payment models'),
  );
  $form['pay_node_models']['form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Node creation'),
    '#default_value' => $pay_node_models['form'],
    '#description' => t('Payment information will be requested on node creation forms. This is useful for paid directory listings or other account information stored as nodes.'),
  );
  $form['pay_node_models']['view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Node display'),
    '#default_value' => $pay_node_models['view'],
    '#description' => t('A payment form will be included on the display view of your nodes. This is useful when you want to create multiple payment forms, such as fundraising campaigns, as nodes.'),
  );

  // Allowable form types.
  $pay_node_forms = variable_get('pay_node_forms', array());
  $form['pay_node_forms'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Available node payment forms'),
  );
  foreach (pay_handlers('pay_form') as $name => $info) {
    $pay_form = pay_form_load($name);
    if ($pay_form->embeddable()) {
      $form['pay_node_forms'][$name] = array(
        '#type' => 'checkbox',
        '#title' => $info['title'],
        '#default_value' => $pay_node_forms[$name],
      );
    }
  }

  return system_settings_form($form);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function pay_node_admin_node_type_form_alter(&$form, &$form_state) {
  $models = array_keys(array_filter(variable_get('pay_node_models', array())));
  $forms  = array_keys(array_filter(variable_get('pay_node_forms', array())));
  $suffix = '_' . $form['#node_type']->type;

  $form['pay_node'] = array(
    '#type' => 'fieldset',
    '#title' => t('Payment settings'),
    '#collapsible' => TRUE,
    '#collapsed' => !variable_get('pay_node_enabled' . $suffix, FALSE),
  );

  // Don't do anything further if there are no payment forms or modes available.
  if (!count($models) || !count($forms)) {
    $form['pay_node']['error'] = array(
      '#type' => 'markup',
      '#value' => t('<p>In order to accept payments on this node type, you will need to enable at least one payment model and at least one form type. You can configure these on the !pay_settings.</p>', array('!pay_settings' => l(t('payment settings page'), 'admin/config/pay/node'))),
    );
    return;
  }

  // Enabled for this node type.
  $form['pay_node']['pay_node_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable payments on this content type'),
    '#default_value' => variable_get('pay_node_enabled' . $suffix, FALSE),
    '#disabled' => (!count($models) || !count($forms)),
  );

  // Type of processing to use (pay to create a node vs. pay against a node).
  if (count($models) == 1) {
    $model = current($models);
    $form['pay_node']['pay_node_model'] = array(
      '#type' => 'value',
      '#value' => $model,
    );
  }
  else {
    $form['pay_node']['pay_node_model'] = array(
      '#type' => 'radios',
      '#title' => t('Node payment model'),
      '#options' => array(
        'form' => 'Node creation',
        'view' => 'Node display',
      ),
      '#default_value' => variable_get('pay_node_model' . $suffix, NULL),
    );
  }

  // Settings for the 'view' model.
  if (in_array('view', $models)) {

    $form['pay_node']['view_form'] = array(
      '#type' => 'fieldset',
      '#title' => t('Payment form creation options'),
      '#description' => t('Payment forms will be created or linked to your %type nodes. These options control the form-creation and linking behavior.', array('%type' => $form['#node_type']->name)),
    );

    $form['pay_node']['view_form']['pay_node_view_required'] = array(
      '#type' => 'checkbox',
      '#title' => t('Require payment forms on @type nodes', array('@type' => $form['#node_type']->name)),
      '#description' => t('Require linked or new payment forms when creating or editing content.'),
      '#default_value' => variable_get('pay_node_view_required' . $suffix, 0),
    );

    $form['pay_node']['view_form']['pay_node_method'] = array(
      '#type' => 'radios',
      '#title' => t('Form creation options'),
      '#options' => array(
        'create' => t('Create a new form for every node'),
        'select' => t('Link to an existing form'),
      ),
      '#default_value' => variable_get('pay_node_method' . $suffix, 'create'),
    );

    $embeddable = array('' => ' - None -');
    foreach (pay_forms_list(NULL, TRUE) as $pay_form) {
      $embeddable[$pay_form->pfid] = $pay_form->title();
    }

    $form['pay_node']['view_form']['pay_node_view_default_create'] = array(
      '#type' => 'fieldset',
      '#title' => t('Default form options'),
    );

    /*
     $handler = 'pay_donate'; // TODO
     $settings = variable_get($handler . $suffix, array());
     $settings['handler'] = $handler;
     $settings_form = array();
     $pay_form = pay_form_load($settings);
     $pay_form->settings_form($form, $form_state);
     if (isset($form[$handler])) {
     $form[$handler]['#type'] = 'fieldset';
     $form[$handler]['#collapsible'] = TRUE;
     $form[$handler]['#title'] = $pay_form->title();

     $form['pay_node']['view_form'][$handler] = $form[$handler];
     $form[$handler]['#access'] = FALSE;
     }

     $form['#submit'][] = 'pay_test_submit';
     */
    /*
     $pay_form->settings_form($settings_form, $form_state);
     unset($settings_form['submit']);

     $settings_form['#type'] = 'fieldset';
     $settings_form['#collapsible'] = TRUE;
     $settings_form['#title'] = t('Default form options');
     $settings_form['#tree'] = TRUE;

     $form['pay_node']['view_form']['pay_node_view_default_create'] = $settings_form;
     */

    $form['pay_node']['view_form']['pay_node_view_default_select'] = array(
      '#type' => 'select',
      '#title' => t('Default form selection'),
      '#options' => $embeddable,
      '#default_value' => variable_get('pay_node_view_default_select' . $suffix, ''),
    );

    $form['pay_node']['view_form']['pay_node_view_override'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow administrators to override the default form selection'),
      '#description' => t('Use the selected value by default, but present the selection to administrators so that they can link different @type nodes to different payment forms.', array('@type' => $form['#node_type']->name)),
      '#default_value' => variable_get('pay_node_view_override' . $suffix, ''),
    );

    $form['pay_node']['view_display'] = array(
      '#type' => 'fieldset',
      '#title' => t('Payment form display options'),
      '#description' => t('Visitors will be able to make payments on forms attached to your @type nodes. These options control how the forms will be displayed.', array('@type' => $form['#node_type']->name)),
    );

    $display_options = array('' => '<hidden>');
    $display_options += pay_form_displays_list();

    $form['pay_node']['view_display']['pay_node_display_teaser'] = array(
      '#type' => 'select',
      '#title' => t('Teaser display'),
      '#options' => $display_options,
      '#default_value' => variable_get('pay_node_display_teaser' . $suffix, ''),
    );

    $display_options += array('tab' => t('As a menu tab'));

    $form['pay_node']['view_display']['pay_node_display'] = array(
      '#type' => 'select',
      '#title' => t('Full node display'),
      '#options' => $display_options,
      '#default_value' => variable_get('pay_node_display' . $suffix, 'pay_form_default'),
    );
  }

  // Selected form handler.
  if (count($forms) == 1) {
    $form_handler = current($forms);
    $form['pay_node']['pay_node_form'] = array(
      '#type' => 'value',
      '#value' => $form_handler,
    );
  }
  else {
    $form['pay_node']['pay_node_form'] = array(
      '#type' => 'select',
      '#options' => $forms,
      '#default_value' => variable_get('pay_node_form' . $suffix, NULL),
    );
  }

  // Handler-specific settings
  /*
   foreach ($forms as $handler) {
   $settings = variable_get('pay_node_'. $handler . $suffix, array());

   $pay_form = pay_form_load($handler);
   $pay_form->settings_form($form, $form_state);

   if (isset($form[$handler])) {
   $form[$handler]['#type'] = 'fieldset';
   $form[$handler]['#collapsible'] = TRUE;
   $form[$handler]['#title'] = $pay_form->title();

   $form['pay_node']['pay_node_'. $handler] = $form[$handler];
   $form[$handler]['#access'] = FALSE;
   }
   if (isset($form['pay_node']['pay_form_settings'][$handler])) {
   $form['pay_node']['pay_form_settings'][$handler]['#type'] = 'fieldset';
   $form['pay_node']['pay_form_settings'][$handler]['#collapsible'] = TRUE;
   $form['pay_node']['pay_form_settings'][$handler]['#title'] = $handler;
   $form['pay_node']['pay_form_settings'][$handler]['#tree'] = TRUE;
   }
   }
   */
}

/**
 * A form_alter helper for node creation forms.
 */
function pay_node_admin_node_form_alter(&$form, &$form_state) {
  // Settings variable suffix.
  $node = $form['#node'];
  $suffix = '_' . $form['#node']->type;

  // Don't continue if pay_node settings aren't enabled for this node type.
  if (!variable_get('pay_node_enabled' . $suffix, FALSE)) {
    return;
  }

  // Determine the payment model and verify that it's currently valid.
  $model = variable_get('pay_node_model' . $suffix, NULL);
  if (!in_array($model, variable_get('pay_node_models', array()))) {
    return;
  }

  // Add payment form settings on the node form to control how it's displayed.
  if ($model == 'view') {
    $visible  = variable_get('pay_node_view_override' . $suffix, FALSE);
    $required = variable_get('pay_node_view_required' . $suffix, FALSE);

    // The default settings are overrideable, so present some settings.
    if ($visible) {
      switch (variable_get('pay_node_method' . $suffix, '')) {

        // Create a new payment form for each node using default values.
        case 'create':
          if (!$pay_form = pay_node_form_load($node)) {
            $pay_form = pay_form_load(variable_get('pay_node_form' . $suffix, ''));
          }

          if ($pay_form) {
            $group = $pay_form->handler();
            $pay_form->settings_form($form, $form_state);

            // Do not allow menu_path to be specified.
            $form[$group][$group]['menu_path'] = array(
              '#type' => 'value',
              '#value' => '',
            );

            // Add an after_build handler to map the pay_form to its node.
            $form['#after_build'][] = 'pay_node_admin_node_form_after_build';
          }
          break;

          // Link to an existing form.
        case 'select':
          $group = 'pay_node';

          // List of available forms.
          $options = array('' => ' - None -');
          foreach (pay_forms_list(NULL, TRUE) as $pay_form) {
            $options[$pay_form->pfid] = $pay_form->title();
          }

          $pay_form_default = pay_node_form_load($node);

          // Add a selection of available payment forms to link to this node.
          $form[$group]['pay_node'] = array(
            '#type' => 'select',
            '#title' => t('Payment form'),
            '#options' => $options,
            '#required' => $required,
            '#default_value' => $pay_form_default->pfid,
          );

          break;
      }

      // Add a fieldset to differentiate this settings form.
      if (isset($group) && $form[$group]) {
        $form[$group]['#type'] = 'fieldset';
        $form[$group]['#collapsible'] = TRUE;
        $form[$group]['#title'] = t('Payment settings');
      }
    }

    // The settings are NOT visible, so silently set the defaults.
    else {
      switch (variable_get('pay_node_method' . $suffix, '')) {
        case 'create':
          break;

          // Store the default selected form for this node type.
        case 'select':
          $pay_form_default = pay_node_form_load($node);
          $form['pay_node'] = array(
            '#type' => 'value',
            '#value' => $pay_form_default->pfid,
          );
          break;
      }
    }
  }

  /*
   if ($pay_form = pay_node_form_load($form['#node'])) {
   $model = variable_get('pay_node_model_'. $form['#node']->type, NULL);
   $active_models = variable_get('pay_node_models', array());
   if ($active_models[$model]) {
   if ($model == 'form') {
   $pay_form->form($form, $form_state);
   }

   // Embed payment form settings into the node editing form.
   if ($model == 'view') {
   $pay_form->settings_form($form, $form_state);

   // Add an after_build handler to map the pay_form to its node.
   $form['#after_build'][] = 'pay_node_admin_node_form_after_build';

   // Add a fieldset to differentiate the settings form.
   $group = $pay_form->handler();
   if (isset($form[$group])) {
   $form[$group]['#type'] = 'fieldset';
   $form[$group]['#collapsible'] = TRUE;
   $form[$group]['#title'] = t('Payment settings');
   }
   }
   }
   }
   */
}

function pay_test_submit($form, $form_state) {
//  print_r($form_state); die;
}
