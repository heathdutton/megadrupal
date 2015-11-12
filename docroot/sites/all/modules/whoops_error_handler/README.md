# Whoops error handler module.

This module implements the [Whoops](http://filp.github.io/whoops/) pretty error handler library into Drupal to catch and provide better debug information at runtime during development, rather than having to investigate the error log pages.

The module registers an error handler once enabled, therefore
should be disabled in a production environment. It also does NOT currently register the error handler if the request being made is being made with AJAX, to prevent strange errors.

