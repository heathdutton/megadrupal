<?php
function ds_ds_panel() {
  return array(
    'label' => t('Bootstrap Panel'),
    'regions' => array(
      'ds_panel_heading' => t('Header'),
      'ds_panel_body' => t('Body'),
      'ds_panel_footer' => t('Footer'),
    ),
  );
}
