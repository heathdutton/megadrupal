Bronto API - an API to allow your modules to interact with Bronto, an email list
service provider.

To install, extract as usual into the modules directory and enable at 
/admin/modules. Your version of PHP must have the built-in SOAP extension 
installed (http://www.php.net/soap).

This module has a few admin settings, located at 
/admin/config/brontoapi/settings.
To access these a user must have "administer bronto" permissions. These settings
apply globally to all modules which use the Bronto API. They consist only of
your Bronto token and default mail parameters (the latter can be overridden by
implementor modules).

An explanation on using the API can be found in API.txt.

For more about Bronto, see their website at http://www.bronto.com. This API is
basically a bridge giving you a few easy-to-use Drupal hooks and functions to
submit content to Bronto, but the full scope of their services and templating
system are not covered here, so please do consult their documentation before
putting this module into a production environment. Note that you will also need 
to have API access enabled within Bronto for them to accept any API calls for 
your account.
