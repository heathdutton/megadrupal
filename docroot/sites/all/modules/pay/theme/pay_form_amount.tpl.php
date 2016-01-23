<?php
  drupal_add_js(drupal_get_path('module', 'pay') .'/theme/pay_form_amount.js');
  echo drupal_get_form('pay_form', $pay_form, NULL, 'amount');
?>
