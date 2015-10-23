CakeMail
========

This project provides integration of [CakeMail](http://cakemail.com/) within
Drupal. It is composed of the following modules:

 - CakeMail API (cakemail_api): An object oriented PHP API to [CakeMail's HTTP API](http://dev.cakemail.com/api),
   build on top of drupal_http_request().
 - CakeMail Mail System (cakemail_relay): An implementation of Drupal's MailSystemInterface
   using [CakeMail's Relay API](http://dev.cakemail.com/api/Relay).

Usage of CakeMail API
---------------------

The module provides a simple object oriented PHP API for CakeMail's HTTP API.
The various resource of CakeMail's HTTP API can be consumed using this interface
using the simple ```cakemail_api()->Service->Nethod($arguments)``` pattern. An
exception will be thrown if any error occurs when using CakeMail's HTTP API.

The following code can be used to subscribe 'john.snmith@example.com' to the
List identified by ```$list_id```.

```.php
try {
  cakemail_api()
    ->List
    ->SubscribeEmail(array(
      'user_key' => $user_key,
      'list_id' =>  $list_id,
      'email' => 'john.snmith@example.com',
  ));
  drupal_set_message('john.snmith@example.com has been subscribed to the list.', 'success');
}
catch (Drupal\cakemail_api\APIException $e) {
  drupal_set_message($e->getMessage(), 'error');
}
```

Usage of CakeMail's HTTP API requires an API key which must be configured using
the ```cakemail_api_key``` variable in yout ```settings.php``` file:

```.php
$conf['cakemail_api_key'] = '<api key provided by cakemail>'
```

In addition, the following variable are available to configure the API:

 - cakemail_api_url: The base URL for the CakeMail API HTTP endpoint (defaults
   to https://api.wbsrvc.com),
 - cakemail_cache_maxage: The maximum age (in seconds) of cached CakeMail API
   results (defaults to 600).
 - cakemail_api_ssl_verify_peer: Set to TRUE to require verification of SSL
   certificate used when contacting the Cakemail API HTTP endpoint (defaults to
   FALSE).

Usage of CakeMail MailSystem
----------------------------

Before enabling the module, check that either the [PECL Mailparse library](http://www.php.net/manual/en/book.mailparse.php)
or the [IMAP extension](http://www.php.net/manual/en/book.imap.php) are enabled
on your system.

After enabling the module, configure the system use CakeMail to send mail in
your ```settings.php``` file:

```.php
$conf['mail_system'] = array(
  'default-system' => 'Drupal\cakemail_relay\CakeMailSystem',
);
$conf['cakemail_api_key'] = '<api key provided by cakemail>'
$conf['cakemail_relay_user_key'] = '<user key provided by cakemail>'
$conf['cakemail_relay_client_id'] = '<client id provided by cakemail>'
```

The module can be configured to wrap Drupal email body using a CakeMail managed
template via the ```cakemail_relay_template_id``` variable. When set, the
variable must be the ID of an existing CakeMail template.

The following variables are available in the template:

 - ```[body]```: The message to be sent.
 - ```[subject]```: Subject of the e-mail to be sent.
 - ```[id]```: An ID to identify the mail sent. Look at module source code or
   [```drupal_mail()```](https://api.drupal.org/api/drupal/includes!mail.inc/function/drupal_mail/7)
   for possible id values.

Global [tokens](https://drupal.org/node/390482) such as site name or URL can
also be used in the template.

### References

 - [```drupal_mail_system()``` API documentation](http://api.drupal.org/api/drupal/includes--mail.inc/function/drupal_mail_system/7)
 - [```MailSystemInterface``` API documentation](http://api.drupal.org/api/drupal/includes--mail.inc/interface/MailSystemInterface/7)