<?php

/**
 * @file
 * adzerk.admin.inc
 */

/**
 * Custom settings form.
 */
function adzerk_settings_form() {
  $adzerk_code = unserialize(variable_get('adzerk_code', NULL));
  $form = array();

  $text = array(
    t('To add Adzerk ad slots to the site, go to <a href="http://adzerk.net">Adzerk.net</a> and access your account.'),
    t('Go to Inventory'),
    t('Access the preferred \'Site name\''),
    t('From the submenu press \'AD CODE\' (Currently only works with standard ad slots)'),
    t('(Step 1) Add all the ad slots you want on the site'),
    t('(Step 2) Paste all code from the \'Script tag (include in the <head> tag)\' field in to the field below'),
    t('Press \'Save settings\''),
    t('Go to !link configuration page', array('!link' => l('block', 'admin/build/block'))),
    t('Enable the \'Adzerk blocks\' like normal drupal blocks'),
  );
  $form['adzerk'] = array(
    '#title' => t('Configuration help'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['adzerk']['help'] = array(
    '#markup' => theme('item_list', array('items' => $text)),
);

  $form['adzerk_code'] = array(
    '#type' => 'textarea',
    '#title' => t('Adzerk javascript code'),
    '#description' => t('Paste the Javascript code block from adzerk management page (Check configuration help above).'),
    '#default_value' => $adzerk_code['code'],
    '#rows' => 15,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save settings',
  );

  return $form;
}

/**
 * Custom settings form validate handler.
 */
function adzerk_settings_form_validate($form, &$form_state) {
  $ad_blocks = array();
  
  $adzerk_code = $form_state['values']['adzerk_code'];
  preg_match_all('/ados_add_placement\((?:(?:\w)+(?:,\s?))+(?:\"(azk\d+)\")/im', $adzerk_code, $matches);

  if (is_array($matches[1]) && count($matches[1])) {
    $ad_blocks = $matches[1];
  }
  else {
    form_set_error('adzerk_code', t('Javascript code block is wrong, check that all code from adzerk is pasted into the field.'));
  }
  $form_state['adzerk_block_ids'] = $ad_blocks;
}

/**
 * Custom settings form submit handler.
 */
function adzerk_settings_form_submit($form, &$form_state) {
  $data = array(
    'code' => $form_state['values']['adzerk_code'],
    'ad_blocks' => $form_state['adzerk_block_ids'],
  );

  variable_set('adzerk_code', serialize($data));

  drupal_set_message(t('Adzerk settings is saved.'));
}
