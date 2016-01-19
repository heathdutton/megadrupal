<?php

/**
 * Field handler to expand the title abbreviations.
 *
 * @ingroup views_field_handlers
 */
class cd_sunlight_handler_field_title extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['format'] = array('default' => 'abbreviated');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['format'] = array(
      '#title' => t('Format'),
      '#type' => 'radios',
      '#default_value' => $this->options['format'],
      '#options' => array(
        'abbreviated' => 'Abbreviated title',
        'full' => 'Full title',
      ),
    );
  }

  function render($values) {
    if (empty($values->cd_sunlight_legislators_title)) {
      return;
    }
    if ($this->options['format'] != 'full') {
      return $values->cd_sunlight_legislators_title . '.';
    }
    $titles = array(
      'Sen' => t('Senator'),
      'Rep' => t('Representative'),
      'Del' => t('Delegate'),
      'Com' => t('Commissioner'),
    );
    return $titles[$values->cd_sunlight_legislators_title];
  }

}
