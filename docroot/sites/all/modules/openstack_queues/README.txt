Openstack Zaqar is an OpenStack project designed to be an open alternative to Amazon
SQS and SNS. The Openstack Queues module implements Openstack Openstack Queues as an alternative
Drupal Queue backend.

Installation (via Drush - recommended)
------------

  drush dl openstack_queues
  drush en openstack_queues
  drush composer-manager install

Installation (Manual)
------------

1. Download and install Composer Manager module.
2. Download and install Openstack Queues module.
3. Rebuild dependencies at admin/config/system/composer-manager.

4A. If you have Composer installed, at sites/default/files/composer run:

      composer install

4B. If you do not have Composer installed, at sites/default/files/composer run:

      curl -sS https://getcomposer.org/installer | php
      php composer.phar install

5. Check admin/config/system/composer-manager to ensure that all dependencies
   have been properly installed.

Configuration
-------------

If you want to use Openstack Queues as the default queue manager, add the following to
your settings.php:

  $conf['queue_default_class'] = 'OpenstackQueuesQueue';

Alternatively, you can also use Openstack Queues for specific queues:

  $conf['queue_class_{queue_name}'];

Default configuration for all Openstack queues can be specified by setting the
openstack_queues_default_queue variable as follows:

  $conf['openstack_queues_default_queue'] = array(
    'client_id' => '00000000-0000-0000-0000-000000000000', // Optional.
    'auth_url' => 'https://example.com/v2/identity',
    'credentials' => array(
      'username' => 'username',
      'password' => 'password',
      'tenantName' => 'tenant',
    ),
    'queue' => 'openstack_queues_queue', // Custom non-Drupal queue name. Optional.
    'region' => 'region',
    'service' => 'service',
    'provider' => 'provider', // Optional.
    'prefix' => 'my_prefix', // Optional prefix to namespace queue.
  );

The 'provider' setting is optional and can be used to load a php-opencloud
connection class specific to that provider. The 'client_id' is used to ensure
that messages are not echoed back unless explicitly requested.

For example, to use Rackspace Cloud Queues, the following settings array would
be required (assuming a queue name of 'openstack_queues_queue' and the Chicago region):

  $conf['openstack_queues_default_queue'] = array(
    'client_id' => '00000000-0000-0000-0000-000000000000', // Required, UUID.
    'auth_url' => 'https://identity.api.rackspacecloud.com/v2.0/',
    'credentials' => array(
      'username' => 'username',
      'apiKey' => 'API-Key',
    ),
    'region' => 'ORD',
    'service' => 'cloudQueues',
    'provider' => 'Rackspace', // There is an OpenCloud\Rackspace class
  );

As another example, HP Cloud MSGaaS would likely work with the following config-
uration (currently untested):

  $conf['openstack_queues_default_queue'] = array(
    'auth_url' => 'https://region-a.geo-1.identity.hpcloudsvc.com:35357/v2.0/',
    'credentials' => array(
      'username' => 'username',
      'password' => 'password',
    ),
    'region' => 'region-b.geo-1',
    'service' => 'messaging',
  );

Specifying multiple items in the queue array will enable you to use multiple
Openstack queues.

Individual queues can override default settings by setting a variable in the
following format. Please note that any setting can be overridden this way, and
any setting that is not specified will use the relevant setting from
$conf['openstack_queues_default_queue'].

  $conf['openstack_queues_queue_{queue_name}'] = array(
    'queue' => 'my_other_queue',
  );

TESTING
-------

The following environment variables need to be set to run tests. Right now, the
tests run against Rackspace Cloud Queues. It is advised to run the tests via
Drush (drush test-run OpenstackQueues).

  openstack_queues_client_id (UUID to identify your site. Can be anything.)
  openstack_queues_username (Your Rackspace Cloud username.)
  openstack_queues_apikey (Your Rackspace Cloud API Key.)
  openstack_queues_region (The region you would like to use, such as IAD or DFW.)
