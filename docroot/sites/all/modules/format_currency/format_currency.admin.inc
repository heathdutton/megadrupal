<?php
/**
 * @file
 * Administration file for the Format Currencies project.
 */

/**
 * Settings form.
 */
function format_currency_settings($form, &$form_state) {
  ctools_include('export');
  $currencies = array_map(function($currency) {
    return check_plain($currency->name);
  }, ctools_export_load_object('currencies', 'conditions', array('status' => FORMAT_CURRENCY_ACTIVE)));
  asort($currencies);

  if (!empty($currencies)) {
    $form['format_currency_test_wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => t('Test output:'),
      '#prefix' => '<div id="format-currency-wrapper" class="container-inline">',
      '#suffix' => '</div>',
    );
    $form['format_currency_test_wrapper']['format_currency_test_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Display'),
      '#ajax' => array(
        'callback' => 'format_currency_ajax',
        'wrapper' => 'format-currency-test',
      ),
    );
    $form['format_currency_test_wrapper']['format_currency_test_select'] = array(
      '#type' => 'select',
      '#title' => t('Currency'),
      '#options' => $currencies,
    );
    $form['format_currency_test_wrapper']['format_currency_test'] = array(
      '#type' => 'textfield',
      '#description' => theme('format_currency', array('price' => 100, 'param' => 'GBP')),
      '#prefix' => '<div id="format-currency-test">',
      '#suffix' => '</div>',
      '#default_value' => isset($form_state['post']['format_currency_test']) ? $form_state['post']['format_currency_test'] : 100,
      '#size' => 10,
    );
    if (!empty($form_state['values']['format_currency_test'])) {
      $form['format_currency_test_wrapper']['format_currency_test']['#description'] = theme('format_currency', array('price' => $form_state['values']['format_currency_test'], 'param' => $form_state['values']['format_currency_test_select']));
    }
  }
  $form['format_currency_display_code'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display Currency Code'),
    '#description' => t('Whether to display the Currency Code (eg. "USD &#36;123.45") before a formatted price.'),
    '#default_value' => variable_get('format_currency_display_code'),
  );
  $form['format_currency_display_symbol'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use symbol rather than HTML entity'),
    '#description' => t('The database table holds both a symbol and HTML entity for Currencies, check the box to use the symbol over the entity.'),
    '#default_value' => variable_get('format_currency_display_symbol'),
  );
  $form['format_currency_round_whole_numbers'] = array(
    '#type' => 'checkbox',
    '#title' => t('Round whole numbers'),
    '#description' => t('Instead of showing &pound;100.00, checking this box will display the price as &pound;100. NOTE this only affects whole numbers &pound;100.01 will still display as normal.'),
    '#default_value' => variable_get('format_currency_round_whole_numbers'),
  );
  $form['format_currency_prefer_coinage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Prefer coinage when displaying a price less than ONE unit'),
    '#description' => t('If displaying a price which is less than ONE unit, checking this box will use the coinage symbol if it is present in the configuration.
      <br />Eg. Rather than display &#36;0.99, display 99&cent;.'),
    '#default_value' => variable_get('format_currency_prefer_coinage'),
  );

  return system_settings_form($form);
}

/**
 * Overview of currencies currently in the database.
 */
function format_currency_admin($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'format_currency') . '/format_currency.css');
  ctools_include('export');
  $rows = $header = array();

  $header =  array(
    t('Storage'),
    array('data' => t('Code'), 'field' => 'code', 'sort' => 'asc'),
    t('Num'),
    t('Symbol'),
    t('Entity'),
    t('Symbol Position'),
    t('Coinage Symbol'),
    t('Coinage Entity'),
    t('Coinage Position'),
    t('Thousands Separator'),
    t('Decimal Separator'),
    t('Decimal Places'),
    t('Base'),
    array('data' => t('Operations'), 'colspan' => 2),
  );

  $rows = array_map(function($currency) {
    $destination = drupal_get_destination();
    $row['storage'] = _format_currency_storage_type($currency);
    $row['name'] = '<a href="#' . check_plain($currency->code) . '" name="' . check_plain($currency->code) . '" class="format-currency-tooltip">' . check_plain($currency->code) . ((boolean) $currency->status == FORMAT_CURRENCY_EXPIRED ? '*' : '') . '<span>' . check_plain($currency->name) . '</span></a>';
    $row['num'] = !empty($currency->num) ? str_pad($currency->num, 3, 0, STR_PAD_LEFT) : '';
    $row['symbol'] = check_plain($currency->symbol);
    $row['html_entity'] = check_plain($currency->html_entity);
    $row['symbol_position'] = _format_currency_symbol_position($currency->symbol_position);
    $row['coinage_symbol'] = check_plain($currency->coinage_symbol);
    $row['coinage_html_entity'] = check_plain($currency->coinage_html_entity);
    $row['coinage_position'] = _format_currency_symbol_position($currency->coinage_position);
    $row['thousands_separator'] = check_plain($currency->thousands_separator);
    $row['decimal_separator'] = check_plain($currency->decimal_separator);
    $row['decimal_places'] = check_plain($currency->decimal_places);
    $row['base'] = check_plain($currency->base);
    $row['edit'] = l(t('edit'), "admin/config/regional/format_currency/$currency->code/edit", array('query' => $destination));
    $row['delete'] = l($currency->export_type & EXPORT_IN_CODE ? t('revert') : t('delete'), "admin/config/regional/format_currency/$currency->code/delete", array('query' => $destination));
    return $row;
  }, ctools_export_crud_load_all('currencies'));

  // Determine sort direction.
  if (tablesort_get_sort($header) == 'asc') {
    asort($rows);
  }
  else {
    arsort($rows);
  }

  $form['currencies'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header,
    '#empty' => t('No currencies are currently in the database.'),
  );

  $form['new_link'] = array(
    '#type' => 'item',
    '#prefix' => '<div id="format-currency-admin-links">',
    '#markup' => l(t('Add new currency'), 'admin/config/regional/format_currency/add'),
    '#suffix' => '</div>',
  );

  $form['pager'] = array('#theme' => 'pager');

  return $form;
}

/**
 * Helper function for showing the currency storage status.
 */
function _format_currency_storage_type($object) {
  if (($object->export_type & EXPORT_IN_DATABASE) && ($object->export_type & EXPORT_IN_CODE)) {
    return t('Overridden');
  }
  elseif ($object->export_type & EXPORT_IN_DATABASE) {
    return t('Database');
  }
  elseif ($object->export_type & EXPORT_IN_CODE) {
    return t('Code');
  }
  return t('Unknown');
}

/**
 * Form to edit a given currency in the database.
 */
function format_currency_edit($form, &$form_state, $currency = array()) {
  ctools_include('export');
  $currency = (array) $currency;

  // If this is a new currency then add the default values.
  $currency += array(
    'name' => '',
    'code' => '',
    'num' => '',
    'symbol' => '',
    'html_entity' => '',
    'symbol_position' => '',
    'coinage_symbol' => '',
    'coinage_html_entity' => '',
    'coinage_position' => '',
    'thousands_separator' => '',
    'decimal_separator' => '',
    'decimal_places' => '',
    'base' => '',
    'status' => '',
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Currency name'),
    '#maxlength' => 64,
    '#default_value' => $currency['name'],
  );
  $form['code'] = array(
    '#type' => 'textfield',
    '#title' => t('Currency code'),
    '#required' => TRUE,
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => $currency['code'],
  );
  $form['num'] = array(
    '#type' => 'textfield',
    '#title' => t('Currency number'),
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => $currency['num'],
  );
  $form['symbol'] = array(
    '#type' => 'textfield',
    '#title' => t('Symbol'),
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => $currency['symbol'],
  );
  $form['html_entity'] = array(
    '#type' => 'textfield',
    '#title' => t('HTML Entity'),
    '#size' => 10,
    '#maxlength' => 32,
    '#default_value' => $currency['html_entity'],
  );
  $form['symbol_position'] = array(
    '#type' => 'select',
    '#title' => t('Symbol Position'),
    '#options' => array(
      FORMAT_CURRENCY_LEFT => t('Left'),
      FORMAT_CURRENCY_RIGHT => t('Right'),
    ),
    '#required' => TRUE,
    '#default_value' => $currency['symbol_position'],
  );
  $form['coinage_symbol'] = array(
    '#type' => 'textfield',
    '#title' => t('Coinage Symbol'),
    '#size' => 15,
    '#maxlength' => 15,
    '#default_value' => $currency['coinage_symbol'],
  );
  $form['coinage_html_entity'] = array(
    '#type' => 'textfield',
    '#title' => t('Coinage HTML Entity'),
    '#size' => 10,
    '#maxlength' => 10,
    '#default_value' => $currency['coinage_html_entity'],
  );
  $form['coinage_position'] = array(
    '#type' => 'select',
    '#title' => t('Coinage Symbol Position'),
    '#options' => array(
      FORMAT_CURRENCY_LEFT => t('Left'),
      FORMAT_CURRENCY_RIGHT => t('Right'),
    ),
    '#required' => TRUE,
    '#default_value' => $currency['coinage_position'],
  );
  $form['thousands_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Thousands Separator'),
    '#required' => TRUE,
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => $currency['thousands_separator'],
  );
  $form['decimal_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Decimal Separator'),
    '#required' => TRUE,
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => $currency['decimal_separator'],
  );
  $form['decimal_places'] = array(
    '#type' => 'textfield',
    '#title' => t('Decimal Places'),
    '#required' => TRUE,
    '#size' => 3,
    '#maxlength' => 2,
    '#default_value' => $currency['decimal_places'],
  );
  $form['base'] = array(
    '#type' => 'textfield',
    '#title' => t('Base'),
    '#required' => TRUE,
    '#size' => 6,
    '#maxlength' => 6,
    '#default_value' => $currency['base'],
  );
  $form['status'] = array(
    '#type' => 'select',
    '#title' => t('Currency Status'),
    '#options' => array(
      FORMAT_CURRENCY_EXPIRED => t('Expired'),
      FORMAT_CURRENCY_ACTIVE => t('Active'),
    ),
    '#default_value' => $currency['status'],
  );
  $form['old_code'] = array(
    '#type' => 'hidden',
    '#value' => $currency['code'],
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validation callback for the format_currency_edit form.
 */
function format_currency_edit_validate(&$form, &$form_state) {
  if ($form_state['values']['old_code'] != $form_state['values']['code'] && format_currency_currencies_load($form_state['values']['code'])) {
    form_set_error('code', t('Error this currency code already exists.'));
  }
  if (!empty($form_state['values']['num']) && !is_numeric($form_state['values']['num'])) {
    form_set_error('num', t('Currency number field can be numeric only.'));
  }
  if (!is_numeric($form_state['values']['base'])) {
    form_set_error('base', t('Base field can be numeric only.'));
  }
  if (!is_numeric($form_state['values']['decimal_places'])) {
    form_set_error('decimal_places', t('Decimal Place field can be numeric only.'));
  }
}

/**
 * Submission callback for the format_currency_edit form.
 */
function format_currency_edit_submit($form, &$form_state) {
  ctools_include('export');
  $currency = (object) $form_state['values'];
  if (empty($currency->export_type)) {
    $currency->export_type = NULL;
  }
  // Handle machine name changes.
  if (!empty($currency->old_code) && $currency->code != $currency->old_code) {
    ctools_export_crud_delete('currencies', $currency->old_code);
    $currency->export_type = NULL;
  }
  ctools_export_crud_save('currencies', $currency);
  drupal_set_message(t('Successfully altered currency %currency.', array('%currency' => $currency->old_code)));
  watchdog('format_currency', 'Currency %currency has been updated.', array('%currency' => $currency->old_code), WATCHDOG_NOTICE, l(t('Edit'), 'admin/config/regional/format_currency/' . $currency->old_code . '/edit'));
  $form_state['redirect'] = 'admin/config/regional/format_currency/overview';
}

/**
 * Form to confirm deletion of a currency from the database.
 */
function format_currency_delete($form, &$form_state, $currency) {
  $form['currency'] = array(
    '#type' => 'value',
    '#value' => $currency,
  );
  $question = ($currency->export_type & EXPORT_IN_CODE)
    ? t('Are you sure you want to revert currency %currency?', array('%currency' => $currency->code)) : t('Are you sure you want to delete currency %currency?', array('%currency' => $currency->code));

  return confirm_form(
    $form,
    $question,
    'admin/config/regional/format_currency/overview',
    t('This action cannot be undone.'),
    ($currency->export_type & EXPORT_IN_CODE) ? t('Revert') : t('Delete'),
    t('Cancel')
  );
}

/**
 * Submission callback for the format_currency_delete form.
 */
function format_currency_delete_submit($form, &$form_state) {
  $currency = $form['currency']['#value'];
  ctools_export_crud_delete('currencies', $currency->code);
  drupal_set_message(t('Successfully deleted currency %currency.', array('%currency' => $currency->code)));
  watchdog('format_currency', 'Currency %currency has been %action.', array('%currency' => $currency->code, '%action' => $currency->export_type & EXPORT_IN_CODE ? 'reverted' : 'deleted'), WATCHDOG_NOTICE);
  $form_state['redirect'] = 'admin/config/regional/format_currency/overview';
}

/**
 * AJAX callback to alter the format_currency_test description to show themed price output.
 */
function format_currency_ajax($form, $form_state) {
  return $form['format_currency_test_wrapper']['format_currency_test'];
}
