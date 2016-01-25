## Drupal Remote API Client
The remote API client is a Drupal helper module that, in conjuction with the [RestWS module](https://www.drupal.org/project/restws), allows the [Drupal Remote API Driver](https://github.com/kirschbaum/drupal-behat-remote-api-driver) to run Behat test against Drupal sites. This client module is only needed on remote Drupal sites where testing will occur.

**Important:** this module does NOT replace the [Drupal Extention](https://github.com/jhedstrom/drupalextension) module. That said, you do not need to install the [Drupal Extention](https://github.com/jhedstrom/drupalextension) module on your remote sites. Instead, the [Drupal Remote API Driver](https://github.com/kirschbaum/drupal-behat-remote-api-driver) extends the existing functionality of the drupalextension module, translating the supported steps (e.g. create nodes, users, etc.) into appropriate REST requests that are made to the remote Drupal site or sites.

### What does this module do?
* It requires the [RestWS module](https://www.drupal.org/project/restws)
* It creates a user role called BehatAPI and assigns it a default set of permissions needed to perform remote testing operations.
* It adds some additional webservice functionality that RestWS could not provide (e.g. retrieves list of content types, fields, assigns user roles, clears cache etc.)
* It allows the remote API driver to login via a user you manually create and with credentials you provide. 
* It creates a blacklist of user roles that cannot be assigned remotely (e.g. BehatAPI)
* Optionally provide a reqex pattern that must match the username prior to performing remote user login.
* Prevents editing user 1 with any of the custom helper functionality.

### Pre-Installation
Review the [documentation for the Drupal Remote API Driver](https://github.com/kirschbaum/drupal-behat-remote-api-driver). You will need to set the Driver up in order for this module to be useful.

### Installation
1. Download and install the module on the Drupal site you wish to test remotely.
2. Create a new user specifically for testing and assign it the BehatAPI user role.
3. Check the default permissions to confirm they are acceptable for your site and your usecase. Be sure you have reviewed the [Security Notes](https://github.com/kirschbaum/drupal-behat-remote-api-driver/blob/master/doc/security_notes.md#security-notes).
4. If you haven't already, [install the Drupal Remote API Driver](https://github.com/kirschbaum/drupal-behat-remote-api-driver/blob/master/doc/installation.md#installation) and start running remote tests!

### Roadmap
* Allow management of user regex pattern from a configuration page.
* Allow management of user role black list from a configuration page.
* Considering adding a CSRF token for non RestWS requests (RestWS already has one)
* Optionally allow site admin to disable editing of user 1 even for the RestWS module.
