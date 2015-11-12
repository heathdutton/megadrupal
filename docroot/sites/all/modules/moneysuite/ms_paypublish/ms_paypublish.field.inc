<?php

/**
 * @file
 * Functions and hooks to facilitate the field for the choose a plan options
 * on the node form.
 */

/**
 * Implements hook_field_info().
 */
function ms_paypublish_field_info() {
  return array(
    'ms_paypublish_plan' => array(
      'label' => t('Pay to Publish Plan'),
      'description' => t('Require users to pay to publish the content.'),
      'default_widget' => 'ms_paypublish_plan_widget',
      'default_formatter' => 'ms_paypublish_plan_info',
      'instance_settings' => array(
        'allowed_plans' => array(),
      ),
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function ms_paypublish_field_is_empty($item, $field) {
  return empty($item['sku']);
}

/**
 * Implements hook_field_access().
 */
function ms_paypublish_field_access($op, $field, $entity_type, $entity, $account) {
  switch ($field['type']) {
    case 'ms_paypublish_plan':
      switch ($op) {
        case 'edit':
          // If this is a new node, we should show it.
          if (empty($entity->nid)) {
            if (empty($entity->type) OR (!user_access('publish ' . $entity->type . ' for free', $account) AND !user_access('administer nodes', $account))) {
              return TRUE;
            }
            else {
              return FALSE;
            }
          }
          else {
            // If there is already a pp_node record for this node and it is
            // not expired, then we don't need to show the form. Instead, we will
            // show the messages on the node itself that will allow the user to
            // extend/promote.
            if ($pp_node = ms_products_load_purchase_by_field('ms_paypublish', 'nid', $entity->nid)) {
              $status = (isset($pp_node->status) ? $pp_node->status : "");
              switch ($status) {
                case 'expired':
                  return TRUE;

                default:
                  return FALSE;
              }
            }
            else {
              return ms_paypublish_is_active($entity, $account);
            }
          }

          return FALSE;

        case 'view':
          // Only show this for the owner of the node or admins.
          return ($entity->uid == $account->uid) || user_access('administer nodes', $account);
      }
      break;
  }
}

/**
 * Implements hook_field_attach_insert().
 */
function ms_paypublish_field_attach_insert($entity_type, $entity) {
  switch ($entity_type) {
    case 'node':
      // Act on the selected plan.
      $fields = field_info_instances($entity_type, $entity->type);
      foreach ($fields as $field) {
        if ($field['widget']['type'] == 'ms_paypublish_plan_widget') {
          if (!user_access('publish ' . $entity->type . ' for free') && !empty($entity->{$field['field_name']}[LANGUAGE_NONE][0]['sku'])) {
            $info = $entity->{$field['field_name']}[LANGUAGE_NONE][0];

            // Add the product to the cart or just grant it if it is free.
            if (!empty($entity->nid) AND !ms_products_load_purchase_by_field('ms_paypublish', 'nid', $entity->nid)) {
              if ($plan = ms_products_plan_load($info['sku'])) {
                global $user;
                // Save the node record or get the existing record.
                $purchase = ms_paypublish_insert_node($entity, $user->uid, $plan->sku);
                $data = array(
                  'option_defaults' => !empty($info['options']) ? $info['options'] : array(),
                  'renew_id' => $purchase->id,
                  'product_title' => $entity->title,
                );
                if ($product = ms_products_add_product_to_cart($plan, $data)) {
                  ms_core_redirect('ms/checkout');
                }
              }
            }
          }
        }
      }
      break;
  }
}

/**
 * Implements hook_field_formatter_info().
 */
function ms_paypublish_field_formatter_info() {
  return array(
    'ms_paypublish_plan_info' => array(
      'label' => t('Show the current plan and options to extend/promote.'),
      'field types' => array('ms_files_field_file_plan'),
    ),
  );
}

/**
 * Implements hook_field_formatter_prepare_view().
 */
function ms_paypublish_field_formatter_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items, $displays) {
  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => $item) {
      if (!empty($entity->nid) AND $pp_node = ms_products_load_purchase_by_field('ms_paypublish', 'nid', $entity->nid)) {
        $items[$id][$delta]['pp_node'] = $pp_node;
      }
    }
  }
}

/**
 * Implements hook_field_formatter_view().
 */
function ms_paypublish_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  switch ($display['type']) {
    case 'ms_paypublish_plan_info':
      foreach ($items as $delta => $item) {
        if (!empty($item['pp_node'])) {
          $purchase = ms_products_purchase_load($item['pp_node']->id);
          $element[$delta]['#markup'] = theme('ms_paypublish_plan_field',
            array(
              'node' => $entity,
              'plan' => ms_products_plan_load($purchase->sku),
              'pp_node' => $purchase,
              'is_sticky' => !empty($purchase->options['sticky']->status) && $purchase->options['sticky']->status == 'active',
              'sticky_expiration' => !empty($purchase->options['sticky']->status) ? $purchase->options['sticky']->expiration : 0,
              'is_promoted' => !empty($purchase->options['promote']->status) && $purchase->options['promote']->status == 'active',
              'promoted_expiration' => !empty($purchase->options['promote']->status) ? $purchase->options['promote']->expiration : 0,
              'is_published' => $entity->status == NODE_PUBLISHED,
              'published_expiration' => $purchase->expiration,
              'status' => ms_products_get_purchase_status_text($purchase->status),
              'order' => ms_core_order_load($purchase->oid),
              'actions' => ms_paypublish_get_node_actions($purchase),
            )
          );
        }
      }
      break;
  }

  return $element;
}

/**
 * Implements hook_field_widget_info().
 */
function ms_paypublish_field_widget_info() {
  return array(
    'ms_paypublish_plan_widget' => array(
      'label' => t('Pay to Publish Plans'),
      'field types' => array('ms_paypublish_plan'),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function ms_paypublish_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $widget = $instance['widget'];
  $settings = $widget['settings'];
  switch ($instance['widget']['type']) {
    case 'ms_paypublish_plan_widget':
      $plans = ms_products_get_plans('ms_paypublish');
      if (!empty($settings['allowed_plans'])) {
        $allowed = array();
        foreach (array_filter($settings['allowed_plans']) as $id) {
          $allowed[$id] = $plans[$id];
        }
        $plans = $allowed;
      }

      $options = array();
      if (!$instance['required']) {
        $options[0] = t('None');
      }

      foreach ($plans as $plan) {
        $options[$plan->sku] = $plan->name . ' - ' . ms_core_get_recurring_string($plan->recurring_schedule, $plan->cart_type == 'recurring');
      }

      $default = !empty($items[$delta]['sku']) ? $items[$delta]['sku'] : 0;

      if (!empty($form_state['values'][$field['field_name']][LANGUAGE_NONE][$delta]['sku'])) {
        $default = $form_state['values'][$field['field_name']][LANGUAGE_NONE][$delta]['sku'];
      }
      else if (isset($instance['default_value'][$delta]['sku'])) {
        $default = $instance['default_value'][$delta]['sku'];
      }
      else if (isset($instance['default_value'])) {
        $default = $instance['default_value'];
      }
      else if (isset($items[$delta]['sku'])) {
        $default = $items[$delta]['sku'];
      }

      $default_options = array();

      $element['#delta'] = $delta;

      $element += ms_products_plans_element($form_state, '', '', $options, 'ms_paypublish_field', $instance['required'], $default, $default_options);

      $element['sku']['#attributes'] = array('class' => array('ms_paypublish_pid_field'));

      $element['sku']['#attached'] = array(
        'css' => array(drupal_get_path('module', 'ms_paypublish') . '/css/ms_paypublish_fields.css'),
        'js' => array(drupal_get_path('module', 'ms_paypublish') . '/js/ms_paypublish_fields.js'),
      );
      break;
  }
  return $element;
}

/**
 * AJAX callback for the plan choices selector.
 *
 * Renders new options like sticky.
 */
function ms_paypublish_plan_field_choice($form, $form_state) {
  return $form[$form_state['triggering_element']['#parents'][0]];
}

/**
 * Implements hook_field_widget_settings_form().
 */
function ms_paypublish_field_widget_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings'];
  switch ($widget['type']) {
    case 'ms_paypublish_plan_widget':
      $form['allowed_plans'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Allowed plans'),
        '#options' => ms_products_get_plans_list('ms_paypublish'),
        '#default_value' => !empty($settings['allowed_plans']) ? $settings['allowed_plans'] : array(),
        '#required' => TRUE,
      );
      return $form;
  }
}

/**
 * Implements hook_field_widget_error().
 */
function ms_paypublish_field_widget_error($element, $error, $form, &$form_state) {
  switch ($error['error']) {
    case 'ms_paypublish_invalid':
      form_error($element, $error['message']);
      break;
  }
}
