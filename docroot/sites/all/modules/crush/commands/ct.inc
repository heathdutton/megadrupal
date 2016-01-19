<?php

function crush_setting_ct() {
  return array('crushContentTypes' => array_keys(node_type_get_types()));
}

