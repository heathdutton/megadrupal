<?php
// TODO: Need to prevent saving from ad_form step, the user should always have
// to visit provider_form step before saving.

class ad_manager_ad_ui extends ctools_export_ui {
  /**
   * Override the row display for listing ads.
   */
  function list_build_row($item, &$form_state, $operations) {
    // Set up sorting
    $name = $item->{$this->plugin['export']['key']};

    // Note: $item->type should have already been set up by export.inc so
    // we can use it safely.
    switch ($form_state['values']['order']) {
      case 'disabled':
        $this->sorts[$name] = empty($item->disabled) . $name;
        break;
      case 'name':
        $this->sorts[$name] = $name;
        break;
      case 'storage':
        $this->sorts[$name] = $item->type . $name;
        break;
    }

    $ops = theme('links__ctools_dropbutton', array('links' => $operations, 'attributes' => array('class' => array('links', 'inline'))));

    $providers = ad_manager_providers();
    $this->rows[$name] = array(
      'class' => array(!empty($item->disabled) ? 'ctools-export-ui-disabled' : 'ctools-export-ui-enabled'),
      'data' => array(
        array('data' => check_plain($name), 'class' => 'ctools-export-ui-name'),
        array('data' => check_plain($providers[$item->provider]['name']), 'class' => 'ctools-export-ui-provider'),
        array('data' => check_plain($item->type), 'class' => 'ctools-export-ui-storage'),
        array('data' => $ops, 'class' => 'ctools-export-ui-operations'),
      ),
      'title' => check_plain($item->name),
    );
  }

  /**
   * Override the filtering for listing ads.
   */
  function list_filter($form_state, $item) {
    // TODO: Provider filter
//    if ($form_state['values']['category'] != 'all' && $form_state['values']['category'] != $item->category) {
//      return TRUE;
//    }

    return parent::list_filter($form_state, $item);
  }

  /**
   * Override the form for listing ads.
   */
  function list_form(&$form, &$form_state) {
    parent::list_form($form, $form_state);
    // TODO: Add provider drop down
//    $options = array('all' => t('- All -'));
//    foreach ($this->items as $item) {
//      $options[$item->category] = $item->category;
//    }
//
//    $form['top row']['category'] = array(
//      '#type' => 'select',
//      '#title' => t('Category'),
//      '#options' => $options,
//      '#default_value' => 'all',
//      '#weight' => -10,
//    );
  }

  /**
   * Override the table header for listing ads.
   */
  function list_table_header() {
    $header = array();
    if (!empty($this->plugin['export']['admin_title'])) {
      $header[] = array('data' => t('Title'), 'class' => 'ctools-export-ui-title');
    }

    $header[] = array('data' => t('Name'), 'class' => 'ctools-export-ui-name');
    $header[] = array('data' => t('Ad provider'), 'class' => 'ctools-export-ui-provider');
    $header[] = array('data' => t('Storage'), 'class' => 'ctools-export-ui-storage');
    $header[] = array('data' => t('Operations'), 'class' => 'ctools-export-ui-operations');

    return $header;
  }
}

/**
 * CTools ad_manager_ad wizard, step ad_form.
 */
function ad_manager_ad_form($form, &$form_state) {
  // Attach export key field, just like a non-wizard form would do.
  $export_key = $form_state['plugin']['export']['key'];
  $schema = ctools_export_get_schema($form_state['plugin']['schema']);
  $form[$export_key] = array(
    '#default_value' => $form_state['item']->name,
    '#description' => t('The unique export ID of the @export. May only consist of lowercase letters, underscores, and numbers.', array('@export' => $form_state['plugin']['title singular'])),
    '#maxlength' => 255,
    '#required' => TRUE,
    '#title' => t($schema['export']['key name']),
    '#type' => 'textfield',
  );
  if ($form_state['op'] === 'edit') {
    $form[$export_key]['#disabled'] = TRUE;
    $form[$export_key]['#value'] = $form_state['item']->name;
  }
  else {
    $form[$export_key]['#element_validate'] = array('ctools_export_ui_edit_name_validate');
  }

  $providers = array();
  foreach (ad_manager_providers() as $key => $value) {
    $providers[$key] = t($value['name']);
  }

  $form['provider'] = array(
    '#default_value' => $form_state['item']->provider,
    '#description' => t('The source of ad population.'),
    '#options' => $providers,
    '#required' => TRUE,
    '#title' => t('Ad provider'),
    '#type' => 'select',
  );
  if (empty($providers)) {
    drupal_set_message(t('No ad providers have been installed. Please install an ad provider to create an ad.'), 'warning');
  }
  return $form;
}

/**
 * CTools ad_manager_ad wizard, step ad_form submit handler.
 *
 * Copies items to the storage of $form_state['item'].
 */
function ad_manager_ad_form_submit(&$form, &$form_state) {
  $cache_fields = array('name', 'provider');
  foreach ($cache_fields as $field) {
    $form_state['item']->$field = $form_state['values'][$field];
  }
}

/**
 * CTools ad_manager_ad wizard, step provider_form.
 *
 * Call the provider form selected as the ad's provider.
 */
function ad_manager_ad_provider_form($form, &$form_state) {
  $function = $form_state['item']->provider;
  if (function_exists($function)) {
    $function($form, $form_state);
  }
  return $form;
}

/**
 * CTools ad_manager_ad wizard, step provider_form submit handler.
 *
 * Attempt to call the provider form's submit function. The provider will need
 * to provide a submit function to assign items to the array
 * $form_state['item']->settings from $form_state['values'] for their form to
 * have any effect.
 *
 * It is recommended that providers assign to an array within the settings, if
 * administrators should be able to switch back and forth between providers
 * without needing to reconfigure.
 */
function ad_manager_ad_provider_form_submit(&$form, &$form_state) {
  // Attempt to call the provider's form submit.
  $function = $form_state['item']->provider . '_submit';
  if (function_exists($function)) {
    $function($form, $form_state);
  }
}

/**
 * CTools ad_manager_ad wizard, step provider_form validate handler.
 *
 * Attempt to call the provider form's validate function.
 */
function ad_manager_ad_provider_form_validate(&$form, &$form_state) {
  // Attempt to call the provider's form validate.
  $function = $form_state['item']->provider . '_validate';
  if (function_exists($function)) {
    $function($form, $form_state);
  }
}

