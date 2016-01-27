<?php

class vr_campaign_field_campaign_id extends views_handler_field {
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['vr_campaign_display'] = array(
      '#title' => t('Display'),
      '#type' => 'select',
      '#options' => array(
        'id' => t('Id'),
        'name' => t('Name'),
      ),
      '#description' => t('Choose the display method for this field'),
      '#default_value' => $this->options['vr_campaign_display'],
    );
  }

  function render($values) {
    $vrid = $values->{$this->field_alias};
    if ($this->options['vr_campaign_display'] == 'name') {
      $campaign = vr_campaign_fetch($vrid);
      return $campaign->name;
    }
    return $vrid;
  }
}
