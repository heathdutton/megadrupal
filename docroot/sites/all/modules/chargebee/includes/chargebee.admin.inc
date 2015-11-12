<?php
/**
 * @file
 * Module settings forms.
 */

/**
 * ChargeBee API settings form.
 */
function chargebee_settings_form($form, &$form_state) {
  $form['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('API'),
  );
  $form['api']['chargebee_api_site_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Site name'),
    '#default_value' => variable_get('chargebee_api_site_name', ''),
  );
  $form['api']['chargebee_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Key'),
    '#default_value' => variable_get('chargebee_api_key', ''),
  );
  $form['api']['chargebee_webhook_hash'] = array(
    '#type' => 'textfield',
    '#title' => t('Webhook hash'),
    '#default_value' => variable_get('chargebee_webhook_hash', ''),
  );
  $form['service'] = array(
    '#type' => 'fieldset',
    '#title' => t('Service'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['service']['verify'] = array(
    '#type' => 'submit',
    '#value' => t('Verify subscription'),
    '#submit' => array('chargebee_service_verify_submit'),
  );
  return system_settings_form($form);
}

/**
 * Plans settings form.
 */
function chargebee_plans_settings_form($form, &$form_state) {
  $plans = chargebee_plan_load_multiple();
  $roles = user_roles(TRUE);
  $form['plans'] = array(
    '#type' => 'fieldset',
    '#title' => t('Plans'),
  );
  $form['plans']['list'] = _chargebee_create_plans_table($plans);
  $form['associate'] = array(
    '#tree' => TRUE,
  );
  foreach ($plans as $plan_id => $plan) {
    $form['associate'][$plan_id] = array(
      '#type' => 'select',
      '#options' => $roles,
      '#title' => $plan->name,
    );
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#submit' => array('chargebee_plans_roles_associate_submit'),
  );
  $form['actions']['download'] = array(
    '#type' => 'submit',
    '#value' => t('Download plans'),
    '#submit' => array('chargebee_update_available_plans_submit'),
  );
  return $form;
}

/**
 * Theme plans list as table.
 *
 * @param array $plans
 *   Array plans objects.
 *
 * @return array
 *   Plans list table markup.
 */
function _chargebee_create_plans_table($plans) {
  $plans_table = array(
    'header' => array(
      t('ID'),
      t('Name'),
      t('Price'),
      t('Payment Period'),
      t('Trial Period'),
      t('Status'),
      t('Operations'),
    ),
    'rows' => array(),
  );
  foreach ($plans as $plan) {
    $plan_data = array(
      $plan->plan_id,
      $plan->name,
      $plan->price,
      $plan->period,
      $plan->trial_period,
    );
    $plan_data[] = isset($plan->nupdate) ? $plan->status . ' (Need to update)' : $plan->status;
    $plan_data[] = '<a href="/admin/config/services/chargebee/plans/' . $plan->id . '/edit">' . t('Edit') . '</a>';
    $plans_table['rows'][] = $plan_data;
  }
  $table = array(
    '#markup' => theme('table', $plans_table),
  );
  return $table;
}

/**
 * Service form submit.
 */
function chargebee_service_verify_submit($form, &$form_state) {
  $subscriptions = chargebee_subscription_load_multiple();
  $batch = chargebee_service_batch_verify_prepare($subscriptions);
  batch_set($batch);
}

/**
 * Update plans submit.
 */
function chargebee_update_available_plans_submit() {
  $plans = chargebee_get_plans();
  foreach ($plans as $plan_id => $plan_array) {
    $plan = chargebee_plan_load($plan_id);
    if ($plan) {
      chargebee_plan_update($plan, $plan_array);
    }
    else {
      chargebee_plan_create($plan_array);
    }
  }
  drupal_set_message(t('Plans updated'));
}

/**
 * Prepare batch for verify subscriptions.
 */
function chargebee_service_batch_verify_prepare($subscriptions) {
  $batch = array(
    'title' => t('ChargeBee verify subscriptions data'),
    'operations' => array(),
    'finished' => 'chargebee_batch_subscriptions_verify_finished',
    'file' => drupal_get_path('module', 'chargebee') . '/includes/chargebee.batch.inc',
  );
  foreach ($subscriptions as $subscription) {
    $batch['operations'][] = array('chargebee_batch_subscriptions_verify_callback', array($subscription->uid));
  }
  return $batch;
}

/**
 * Plans update form.
 */
function chargebee_plans_update_form($form, &$form_state) {
  $form['#prefix'] = '<div id="chargebee-update-form">';
  $form['#suffix'] = '</dv>';
  $form['#tree'] = TRUE;
  $step = empty($form_state['storage']['step']) ? 1 : $form_state['storage']['step'];
  $form_state['storage']['step'] = $step;
  if ($step == 1) {
    $form = chargebee_plans_update_form_first($form, $form_state);
  }
  else {
    $form = chargebee_plans_update_form_second($form, $form_state);
  }
  return $form;
}

/**
 * Plans update form. First step.
 */
function chargebee_plans_update_form_first($form, &$form_state) {
  $plans = chargebee_plan_load_multiple();
  $new_plans = chargebee_get_plans();
  _chargebee_plans_update_check($plans, $new_plans);
  $form_state['storage']['update'] = array();
  foreach ($plans as $plan) {
    if (isset($plan->nupdate)) {
      $form_state['storage']['update'][$plan->id] = $plan->name;
    }
  }
  $form['plans'] = array(
    '#type' => 'fieldset',
    '#title' => t('Local plans'),
  );
  $form['plans']['list'] = _chargebee_create_plans_table($plans);
  if ($new_plans) {
    $form['new_plans'] = array(
      '#type' => 'fieldset',
      '#title' => t('New plans'),
    );
    $form['new_plans']['list'] = _chargebee_create_plans_select_table($new_plans);
    $form['new_plans']['actions'] = array('#type' => 'actions');
    $form['new_plans']['actions']['add'] = array(
      '#type' => 'submit',
      '#value' => t('Add plans'),
      '#submit' => array('chargebee_plans_update_form_add_submit'),
    );
    $form['new_plans']['actions']['update'] = array(
      '#type' => 'submit',
      '#value' => t('Update plans'),
      '#name' => 'next',
      '#submit' => array('chargebee_plans_update_form_rebuild_submit'),
      '#validate' => array('chargebee_plans_update_form_rebuild_validate'),
    );
  }
  return $form;
}

/**
 * Plans update form. Second step.
 */
function chargebee_plans_update_form_second($form, &$form_state) {
  $form['plans'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select the plans that you want to replace'),
    '#tree' => TRUE,
  );
  foreach ($form_state['values']['new_plans']['list'] as $new_plan) {
    $form['plans'][$new_plan] = array(
      '#type' => 'select',
      '#title' => $new_plan,
      '#options' => $form_state['storage']['update'],
    );
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
    '#name' => 'back',
    '#submit' => array('chargebee_plans_update_form_rebuild_submit'),
  );
  $form['actions']['update'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#validate' => array('chargebee_plans_update_form_update_validate'),
    '#submit' => array('chargebee_plans_update_form_update_submit'),
  );
  return $form;
}

/**
 * Update plans rebuild validate.
 */
function chargebee_plans_update_form_rebuild_validate($form, &$form_state) {
  if (!empty($form_state['values']['new_plans']['list'])) {
    $count = 0;
    $new_plans = $form_state['values']['new_plans']['list'];
    foreach ($new_plans as $name => $state) {
      if ($state) {
        $count++;
      }
      else {
        unset($form_state['values']['new_plans']['list'][$name]);
      }
    }
    if (!$count) {
      form_set_error('new_plans][list', t('You must select plans'));
    }
  }
  else {
    form_set_error('new_plans][list', t('There are no plans to update'));
  }
}

/**
 * Update plans rebuild submit.
 */
function chargebee_plans_update_form_rebuild_submit($form, &$form_state) {
  switch ($form_state['triggering_element']['#name']) {
    case 'next':
      $form_state['storage']['step']++;
      break;

    case 'back':
      $form_state['storage']['step']--;
      break;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Add plans submit.
 */
function chargebee_plans_update_form_add_submit($form, &$form_state) {
  foreach ($form_state['values']['new_plans']['list'] as $plan_id) {
    if ($plan_id) {
      $plan = chargebee_plan_retrieve($plan_id);
      $saved_plan = chargebee_plan_create($plan);
      if ($saved_plan) {
        drupal_set_message('Plan ' . $plan_id . ' added');
      }
    }
  }
}

/**
 * Update plans validate.
 */
function chargebee_plans_update_form_update_validate($form, &$form_state) {
  $values = array_count_values($form_state['values']['plans']);
  foreach ($values as $value) {
    if ($value > 1) {
      form_set_error('plans', t('You need to choose different plans'));
    }
  }
}

/**
 * Update plans submit.
 */
function chargebee_plans_update_form_update_submit($form, &$form_state) {
  foreach ($form_state['values']['plans'] as $new_plan_id => $local_plan_id) {
    $new_plan = chargebee_plan_retrieve($new_plan_id);
    $old_plan = chargebee_plan_load($local_plan_id);
    $plan = chargebee_plan_update($old_plan, $new_plan);
    drupal_set_message(t('Plan @plan_name updated', array('@plan_name' => $plan->name)));
  }
}

/**
 * Find and unset plans without changes.
 */
function _chargebee_plans_update_check(&$plans, &$new_plans) {
  $no_check = array('id', 'description');
  foreach ($plans as $plan) {
    if (isset($new_plans[$plan->plan_id])) {
      foreach ($plan as $key => $attr) {
        if (!in_array($key, $no_check) && $attr != $new_plans[$plan->plan_id][$key]) {
          $plan->nupdate = TRUE;
        }
      }
      if (!isset($plan->nupdate) || $plan->nupdate != TRUE) {
        unset($new_plans[$plan->plan_id]);
      }
    }
    else {
      $plan->nupdate = TRUE;
    }
  }
}

/**
 * Create tableselect for plans list.
 */
function _chargebee_create_plans_select_table($plans) {
  $header = array(
    'plan_id' => t('ID'),
    'name' => t('Name'),
    'price' => t('Price'),
    'period' => t('Payment Period'),
    'trial_period' => t('Trial Period'),
    'status' => t('Status'),
  );
  $rows = array();
  foreach ($plans as $plan) {
    $rows[$plan['plan_id']] = array(
      'plan_id' => $plan['plan_id'],
      'name' => $plan['name'],
      'price' => $plan['price'],
      'period' => $plan['period'],
      'trial_period' => $plan['trial_period'],
      'status' => $plan['status'],
    );
  }
  $table = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#empty' => t('No plans found'),
  );
  return $table;
}

/**
 * Plan edit form.
 */
function chargebee_plans_edit_form($form, &$form_state, $plan) {
  $roles = user_roles(TRUE);
  $roles_associate = variable_get('chargebee_plans_roles');
  $form['about_plan'] = array(
    '#markup' => theme('chargebee_plan_info', array('plan' => $plan)),
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => 'Description',
    '#default_value' => $plan->description,
  );
  $form['associate'] = array(
    '#type' => 'select',
    '#title' => t('Role associate'),
    '#options' => $roles,
    '#default_value' => $roles_associate[$plan->id],
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/config/services/chargebee/plans'),
  );
  return $form;
}

/**
 * Plan edit form submit.
 */
function chargebee_plans_edit_form_submit($form, &$form_state) {
  $plan = $form_state['build_info']['args'][0];
  $update = array(
    'description' => $form_state['values']['description'],
  );
  chargebee_plan_update($plan, $update);
  $roles = variable_get('chargebee_plans_roles');
  $roles[$plan->id] = $form_state['values']['associate'];
  variable_set('chargebee_plans_roles', $roles);
}
