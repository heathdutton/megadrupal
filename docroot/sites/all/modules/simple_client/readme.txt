Simple Client is a developer tool for making simple HTTP GET and POST requests
using Guzzle.

This is a simple HTTP client module, used primarily for fetching content from
the internet.

If you are looking for additional normalisation, validation, URL parsing and
construction functionality, you may like to use Link API in conjunction with
this module.

Dependencies

Simple Client requires Guzzle to be present and autoloadable, which needs to be
done via Composer. Two workable options for this are setting it up via Composer
Vendor, or Composer Manager.

Why wrap Guzzle

Guzzle is a perfectly adept HTTP client, so why wrap it in another module? This
is reasonably straightforward. Over several projects, I found that I needed to
repeat the same wrapping code around Guzzle. Some of it is just request
construction, but there is also error handling and other parts. The result is a
"simple" client. Its not as powerful as a full Guzzle client implementaiotn, but
its not as complicated to use either.

Usage

The client can be invoked using simple_client(). Unless you disable Client
Exceptions using the SimpleClient::noExceptions() method, HTTP or other errors
will throw a \Drupal\simple_client\Client\Exception(), so the complete request
should be wrapped in a try-catch block, as below.

<?php
    try {
      $client = simple_client($some_url);

      $returned_data = $client->get();
    }
    catch(\Drupal\simple_client\Client\Exception $e) {

      // Do something with the exception, or just...
      return FALSE;
    }
?>

Known issues

- Authentication support is currently limited to Drupal session auth only, but
you could override this in your own class.
- The HTTP error code is not currently passed to the Exception handler in some
cases.
- Query strings must be passed in their entirety, as $settings['request args']