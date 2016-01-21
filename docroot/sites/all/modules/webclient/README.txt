WebClient is an API for making HTTP requests using cURL.

Requirements
------------

This module requires the libcurl library.

Features
--------

The WebClient module supports the following features:
  - SSL and certificate verification.
  - Authentication and proxy.
  - Dynamically change the request class depending on the URL.
  - Format response data depending on the content (MIME) type.
  - Allow modules to define new response data formatters.
  - Drupal integration (drupal_curl_request).

Optional modules
----------------

  - WebClient formatters (webclient_formatters):
    Provides additional response formatters.

  - WebClient Proxy (webclient_proxy):
    Manage proxy settings for specific request through the user interface.

For developers
--------------

See documentation in the 'documentation' folder of this module.
