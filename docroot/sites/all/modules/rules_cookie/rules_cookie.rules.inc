<?php

/**
* Implementation of hook_rules_condition_info().
* @ingroup rules
*/
function rules_cookie_rules_condition_info() {
  return array(
    'rules_cookie_condition_cookie_compare' => array(
      'label' => t('Cookie Text Comparision'),
      'parameter' => array(
        'name' => array(
          'label' => t('Cookie Name'),
          'type' => 'text',
        ),
        'value' => array(
          'label' => t('Cookie Value'),
          'type' => 'text',
        )
      ),
      'group' => t('Rules'),
    )
  );
}

function rules_cookie_condition_cookie_compare($name, $value) {
  if (isset($_COOKIE[$name])) {
    if ($_COOKIE[$name] === $value) {
      return true;
    }
  }
  return false;
}
