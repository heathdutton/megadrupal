<?php

function education_form_system_theme_settings_alter(&$form, &$form_state) {
  $form['others'] = array(
    '#type' => 'fieldset',
    '#title' => t('Others'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE
  );
  
  $form['others']['hide_pager_frontpage'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Hide pager in frontpage'),
    '#default_value' => theme_get_setting('hide_pager_frontpage')
  );
}