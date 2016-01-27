<?php

/**
 * @file
 * Administration page callbacks for the persiantools module.
 */

/**
 * Form builder. Configure persiantools settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function persiantools_admin_settings() {
  // TODO: add option to convert decimal symbol: no, smart, comma.
  $form['digit_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Digits and Numbers Settings'),
  );

  $form['digit_settings']['persiantools_digit_method'] = array(
    '#type' => 'radios',
    '#title' => t('Conversion Method'),
    '#options' => array(
      'none' => t('None'),
      'smart' => t('Smart'),
      'full' => t('Full'),
    ),
    '#description' => t('Select method for converting english numbers to persian.'),
    '#default_value' => variable_get('persiantools_digit_method', 'full'),
  );

  $form['persiantools_rtl_ltr_fix'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fix multi-directional texts'),
    '#description' => t('Fix mess in mixed english and persian texts.'),
    '#default_value' => variable_get('persiantools_rtl_ltr_fix', TRUE),
  );

  $form['persiantools_sort_fix'] = array(
    '#type' => 'fieldset',
    '#title' => t('Persian Sort'),
  );
  $form['persiantools_sort_fix']['submit_btn'] = array(
    '#type' => 'submit',
    '#value' => t('Fix persian sort in all tables'),
    '#submit' => array('persiantools_sort_fix_submit'),
  );

  return system_settings_form($form);
}

function persiantools_sort_fix_submit() {

  global $databases;
  $dbname = $databases['default']['default']['database'];
  db_query('alter database ' . $dbname . ' DEFAULT CHARACTER SET utf8 COLLATE utf8_persian_ci');

  $table_ref = "Tables_in_$dbname";
  $result = db_query("SHOW TABLES");
  foreach ($result as $tableinfo) {
    $tablename = $tableinfo->$table_ref;
    db_query('alter table ' . $tablename . ' convert to character set utf8 collate utf8_persian_ci');
  }

  drupal_set_message(t('Default database collation, and collation for all tables changed to persian (utf8_persian_ci).'), 'status', FALSE);
}

