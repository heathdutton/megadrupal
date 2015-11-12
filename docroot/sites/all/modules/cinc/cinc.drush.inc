<?php

function cinc_drush_command() {

  $items = array();

  $items['cinc-content-type'] = array(
    'description' => dt('Create, read, update, delete content types.'),
    'arguments' => array(
      'type' => dt('Content type machine name.'),
    ),
    'aliases' => array('cinc-ct'),
    'required-arguments' => 1,
  );

  return $items;

}

function drush_cinc_content_type($type) {

  $type = CINC_ContentType::init($type);

}