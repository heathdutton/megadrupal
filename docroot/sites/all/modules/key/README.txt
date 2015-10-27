Key provides the ability to manage keys, which can be employed by other
modules. It gives site administrators the ability to define how and where
keys are stored, which allows the option of a high level of security and
allows sites to meet regulatory or compliance requirements.

Examples of the types of keys that could be managed with Key are:

- An API key for connecting to an external service, such as PayPal,
  MailChimp, Authorize.net, UPS, an SMTP mail server, or Amazon Web
  Services
- A key used for encrypting data


Defining keys
-------------

Key provides an administration page where users with the "administer keys"
permission can add, edit, and delete keys.

Selecting a key provider

Key providers define how the key will be made available and/or how it will
be stored. There are two default providers:

Configuration: Stores the key in configuration settings in the Drupal
database. The key value can be set, edited, and viewed through the
administrative interface, making it useful during site development. However,
for better security on production websites, keys should not be stored in the
database.

File: Stores the key in a file, which can be anywhere in the file system,
as long as it's readable by the user that runs the web server. Storing the
key in a file outside of the web root is generally more secure than storing
it in the database.


Adding additional key providers
-------------------------------

Key providers are CTools plugins, so additional providers can easily be
defined. See the API.txt file for more information.