<?php

/*
 * Implementation of hook_migrate_api().
 */
function party_migrate_migrate_api() {
  $api = array(
    'api' => 2,
  );
  return $api;
}

