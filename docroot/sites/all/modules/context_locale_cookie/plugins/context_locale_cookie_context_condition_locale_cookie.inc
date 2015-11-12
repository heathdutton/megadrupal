<?php
class context_locale_cookie_context_condition_locale_cookie extends context_condition {
  function condition_values() {
    return array('locale_cookie' => 'locale_cookie');
  }

  function condition_form($context) {
    $default_form = parent::condition_form($context);
    unset($default_form['#options']);
    $default_form['#type'] = 'item';
    $default_form['#description'] = t('This condition returns TRUE when the language cookie is not set.');
    $form['explanation'] = $default_form;
    $form['locale_cookie'] = array('#type' => 'value', '#value' => 'locale_cookie');
    return $form;
  }


  function condition_form_submit($values) {
    return array('locale_cookie' => 'locale_cookie');
  }

  function execute() {
    $param = variable_get('locale_cookie_language_negotiation_cookie_param', 'language');
    foreach($this->get_contexts() as $context) {
      if(!isset($_COOKIE[$param])) {
        $this->condition_met($context);
      }
    }
      
  }
}