<?php

class vr_campaign_argument_campaign_id extends views_handler_argument {
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['apply'] = array(
      '#type' => 'checkbox',
      '#title' => t('Apply argument to query.'),
      '#description' => t('Do not apply this argument when using Views Bulk Operations to add
        users to a campaign'),
      '#default_value' => isset($this->options['apply']) ? $this->options['apply'] : 1,
      '#return_value' => 1,
    );
  }

  function query() {
    if ($this->options['apply']) {
      parent::query();
    }
  }
}
