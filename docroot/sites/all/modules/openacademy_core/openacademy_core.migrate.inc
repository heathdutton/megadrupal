<?php

class OpenacademyDemoMigration extends Migration {
  public function __construct() {
    parent::__construct();
  }

  public function prepareRow($current_row) {
    if (empty($current_row->image)) {
      $options = array('placeholder-blue.jpg', 'placeholder-green.jpg', 'placeholder-red.jpg');
      $rand_key = array_rand($options, 1);
      $current_row->image = $options[$rand_key];
      $current_row->source_dir = drupal_get_path('module', 'openacademy_core') . '/images';
    }
    return TRUE;
  }

}
