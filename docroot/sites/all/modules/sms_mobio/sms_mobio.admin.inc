<?php
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function sms_mobio_add_more($form, &$form_state, $no_js_use = FALSE) {
  
  if (variable_get('sms_mobio_services', 0)) {
    $services = unserialize(variable_get('sms_mobio_services', 0));
    $count_services = count($services);
  }

  // Because we have many fields with the same values, we have to set
  // #tree to be able to access them.
  $form['#tree'] = TRUE;
  $form['services_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('SMS Mobio services'),
    '#prefix' => '<div id="services-fieldset-wrapper">',
    '#suffix' => '</div>',
  );

  if (empty($count_services)) {
    $count_services = 1;
  }
  for ($i = 0; $i < $count_services + 1; $i++) {
    $form['services_fieldset']['service'][$i] = array(
      '#type' => 'textfield',
      '#title' => t('SMS Service'),
      '#default_value' => @$services[$i],
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Final submit handler.
 *
 * Reports what values were finally set.
 */
function sms_mobio_add_more_submit($form, &$form_state) {
  
  $output = t('Saved Mobio services: @names', array('@names' => implode(', ', $form_state['values']['services_fieldset']['service'])));
  drupal_set_message($output);
  
  if (count($form_state['values']['services_fieldset']['service'])) {
    foreach ($form_state['values']['services_fieldset']['service'] AS $val) {
      if ($val) {
        $services[] = $val;
      }
    }
    variable_set('sms_mobio_services', serialize($services));
  }
}
