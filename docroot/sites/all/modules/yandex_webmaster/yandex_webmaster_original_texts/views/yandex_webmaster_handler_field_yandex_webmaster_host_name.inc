<?php
/**
 * @file
 * Definition of yandex_webmaster_handler_field_yandex_webmaster_host.
 */

class yandex_webmaster_handler_field_yandex_webmaster_host_name extends views_handler_field {
  function render($values) {
    $value = $this->get_value($values);
    $hosts = yandex_webmaster_get_hosts();
    return (isset($hosts[$value]) ? $hosts[$value]['name'] : '');
  }
}
