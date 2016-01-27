<?php

function crush_setting_fields() {
  return array('crushContentTypes' => array_keys(node_type_get_types()));
}

