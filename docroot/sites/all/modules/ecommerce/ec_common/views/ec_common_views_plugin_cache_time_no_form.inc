<?php

class ec_common_views_plugin_cache_time_no_form extends views_plugin_cache_time  {
  function cache_get($type) {
    if (!empty($_POST)) {
      return FALSE;
    }
    else {
      return parent::cache_get($type);
    }
  }
}