<?php

/**
 * Field handler for Party.
 *
 * @ingroup views_field_handlers
 */
class cd_sunlight_handler_field_party extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['parenthesis'] = array('default' => TRUE);
    $options['format'] = array('default' => 'letter');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['parenthesis'] = array(
      '#title' => t('Enclose in parenthesis'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['parenthesis'],
    );
    $form['format'] = array(
      '#title' => t('Format'),
      '#type' => 'radios',
      '#default_value' => $this->options['format'],
      '#options' => array(
        'letter' => 'First letter only',
        'period' => 'First letter with period',
        'full' => 'Full party name',
      ),
    );
  }

  function render($values) {
    if (empty($values->cd_sunlight_legislators_party)) {
      return;
    }
    switch ($this->options['format']) {
      case 'full':
        $party = $values->cd_sunlight_legislators_party;
        break;
      case 'period':
        $party = substr($values->cd_sunlight_legislators_party, 0, 1) . '.';
        break;
      default:
        $party = substr($values->cd_sunlight_legislators_party, 0, 1);
    }
    if (!empty($this->options['parenthesis'])) {
      $party = "($party)";
    }
    return $party;
  }

}
