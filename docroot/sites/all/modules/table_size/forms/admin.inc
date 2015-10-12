<?php
/**
 * @file
 * The Table Size main admin form.
 */


/**
 * Builds the Table Size main admin form.
 */
function table_size_admin_form($form) {
  $rows = TableSizeDb::showTableStatus();
  $header = array_keys($rows[0]);
  $header[] = 'Quota (MB / Rows)';

  $quota = TableSizeQuota::getInstance();

  $form['rows'] = array();
  foreach ($rows as $k => $r) {
    $form['rows'][$k]['quota'] = array(
      '#type' => 'container',
      '#tree' => TRUE,
      '#attributes' => array(
        'class' => array('table-size-quota'),
      ),
    );
    $form['rows'][$k]['quota'][$r['name']] = array(
      '#type' => 'container',
      '#tree' => TRUE,
    );
    $common_settings = array(
      '#title_display' => 'invisible',
      '#type' => 'textfield',
      '#size' => 5,
      '#maxlength' => 6,
    );
    $form['rows'][$k]['quota'][$r['name']][TableSizeQuota::TYPE_SIZE] = array(
      '#title' => t('Size (MB)'),
      '#default_value' => $quota->get($r['name'], TableSizeQuota::TYPE_SIZE),
      '#suffix' => '/',
    ) + $common_settings;
    $form['rows'][$k]['quota'][$r['name']][TableSizeQuota::TYPE_ROWS] = array(
      '#title' => t('Rows'),
      '#default_value' => $quota->get($r['name'], TableSizeQuota::TYPE_ROWS),
    ) + $common_settings;
  }

  $form['actions'] = array(
    '#type' => 'container',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['#custom'] = array(
    '#rows' => $rows,
    '#header' => $header,
  );

  $form['#attached'] = array(
    'css' => array(
      drupal_get_path('module', 'table_size') . '/css/admin.css',
    ),
  );

  return $form;
}

/**
 * Themes the Table Size main admin form.
 */
function theme_table_size_admin_form($variables) {
  $rows = array();
  $form = $variables['form'];
  $quota = TableSizeQuota::getInstance();

  foreach ($form['rows'] as $k => $row) {
    if (isset($row['quota']) && is_array($row['quota'])) {
      $form['#custom']['#rows'][$k]['quota'] = drupal_render($form['rows'][$k]['quota']);

      $q = $quota->getTable($form['#custom']['#rows'][$k]['name']);

      $config = array(
        TableSizeDb::COL_SIZE => TableSizeQuota::TYPE_SIZE,
        TableSizeDb::COL_ROWS => TableSizeQuota::TYPE_ROWS,
      );
      $error_row = FALSE;
      foreach ($config as $cfg_key => $cfg_val) {
        $val = $form['#custom']['#rows'][$k][$cfg_key];
        if ($q[$cfg_val] && $val > $q[$cfg_val]) {
          $form['#custom']['#rows'][$k][$cfg_key] = array(
            'data' => $val,
            'class' => 'error',
          );
          $error_row = TRUE;
        }
      }
      if ($error_row) {
        $form['#custom']['#rows'][$k]['name'] = array(
          'data' => $form['#custom']['#rows'][$k]['name'],
          'class' => 'error',
        );
      }

      $rows[] = $form['#custom']['#rows'][$k];
    }
  }
  $header = $form['#custom']['#header'];
  $output = theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);

  return $output;
}


/**
 * Submit handler for the main Table Size admin form.
 */
function table_size_admin_form_validate($form, &$form_state) {
  foreach ($form_state['values']['quota'] as $table => $quota) {
    foreach (array(TableSizeQuota::TYPE_SIZE, TableSizeQuota::TYPE_ROWS) as $k) {
      if ($quota[$k] && !preg_match('/^((0\.\d+)|([1-9]\d*(\.?\d)?\d*))$/', $quota[$k])) {
        form_set_error('quota][' . $table . '][' . $k, t('Invalid numeric value for table: %table, quota type: %type.', array(
          '%table' => $table,
          '%type' => $k,
        )));
      }
    }
  }
}


/**
 * Submit handler for the main Table Size admin form.
 */
function table_size_admin_form_submit($form, $form_state) {
  $quota = TableSizeQuota::getInstance();
  $quota->save($form_state['values']['quota']);
  drupal_set_message(t('The table quotas were successfully saved.'));
}