<?php

/**
 * @file
 * This file contains custom settings for conversejs built-in proxy.
 *
 * If you want to use the built-in proxy, You should copy this file
 * to http_bind_settings.php and edit it with your custom settings. Then enable
 * the built-in proxy in module's settings page.
 * NOTE: http_bind_settings.php should be places beside http_bind.php
 * 
 * You can use static data here or you can use any logic
 * you want to prepare variables.
 * This file must provide a global variable called $http_bind_settings
 * which should be an array with defined keys. (look below).
 */

$http_bind_settings = array(
  /**
   * 'bosh_url' is the URL of the BOSH server.
   * This key is required and should be present.
   * The following example assumes you have installed
   * BOSH server on your localhost
   * For example if you install ejabberd on your PC.
   */
  'bosh_url' => 'http://localhost:5280/http-bind',
);
