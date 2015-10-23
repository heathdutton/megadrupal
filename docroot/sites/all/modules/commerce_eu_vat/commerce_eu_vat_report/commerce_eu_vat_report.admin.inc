<?php
/**
 * @file
 * Administrative forms.
 */

 /**
  * Configuration form.
  */
function commerce_eu_vat_report_form($form, &$form_state) {
  $form['generate'] = array(
    '#type' => 'submit',
    '#value' => t('Generate all vat information'),
    '#submit' => array('commerce_eu_vat_report_form_submit_generate'),
  );

  return $form;
}

/**
 * Submit handler that generates a tax report.
 */
function commerce_eu_vat_report_form_submit_generate($form, &$form_state) {
  commerce_eu_vat_report_generate();
}

/**
 * Callback for tax report generation.
 */
function commerce_eu_vat_report_generate_callback() {
	commerce_eu_vat_report_generate();

	batch_process('admin/commerce/reports/vat');
}

/**
 * Returns the table containing the tax report.
 */
function commerce_eu_vat_report_table($start = 0, $end = REQUEST_TIME) {
  if (!variable_get('commerce_eu_vat_report_batch_finished', FALSE)) {
    drupal_set_message();
  }

  $rows = commerce_eu_vat_report($start, $end);
  
  if (!variable_get('commerce_eu_vat_report_batch_finished', FALSE)) {
    $rows = array();

  	$rows[] = array(
      'data' => array(array('data' => t("It seems you have not yet extracted vat information from orders that occurred before enabling the vat reporting module. You will have to <a href='@generate-url'>generate</a> past data before anything will show up.", array('@generate-url' => url('admin/commerce/config/vat-reports/generate'))), 'colspan' => 3)),
    );
  }
  elseif (!$rows) {
    $rows[] = array(
      'data' => array(array('data' => t("There's currently no vat data available. You might want to <a href='@generate-url'>generate</a> past data.", array('@generate-url' => url('admin/commerce/config/vat-reports/generate'))), 'colspan' => 3)),
    );
  }

  return array(
    '#theme' => 'table',
    '#header' => array(t('Tax rate'), t('Taxable'), t('Taxed')),
    '#rows' => $rows,
  );
}
