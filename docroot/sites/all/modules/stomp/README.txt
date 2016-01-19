
STOMP is the Simple (or Streaming) Text Orientated Messaging Protocol. The Stomp
module implements an alternative Drupal Queue backend for queues supporting the
STOMP protocol - such as ActiveMQ, RabbitMQ.


Installation
------------

1. Install and enable like a normal Drupal module.
2. In your settings.php you need to set the $conf variables to the correct 
settings.

If you want to set STOMP as the default queue manager then add the 
following to your settings.php

$conf['queue_default_class'] = 'StompQueue';

Alternatively you can also set for each queue to use Stomp

$conf['queue_class_{queue name}'] = 'StompQueue';


Configuration
-------------

Default configuration for all STOMP queues can be specified by setting the 
stomp_default_queue $conf variable as follows:

$conf['stomp_default_queue'] = array(
  'credentials' => array(
    'user' => 'user1',
    'pass' => 'pass',
  ),
  'brokers' => array(
    'tcp://127.0.0.1:61613',
    'tcp://127.0.0.1:61614',
  ),
  'randomize' => 'false',
);

Specifying multiple items in the brokers array will enable STOMP to use 
Now instead of trying to connect to the single broker, the client will execute 
reconnection logic if needed. For the previous example, if we don't have a 
broker connector running on port 61613 it will try to connect on port 61614.

Alternatively, individual queues can override default settings by setting
the following $conf variable - any of the settings can be overridden, so maybe 
you have a queue running on the same server but different port using the default
credentials:

$conf['stomp_queue_{queue_name}'] = array(
  'brokers' => array(
    'tcp://127.0.0.2:61615'
  ),
);

Or you might want to override all the settings for a specific queue:

$conf['stomp_default_queue'] = array(
  'credentials' => array(
    'user' => 'user2',
    'pass' => 'pass',
  ),
  'brokers' => array(
    'tcp://127.0.0.3:61613',
    'tcp://127.0.0.4:61614',
  ),
);


Each STOMP queue can set its own headers. These can be defined in additional
$conf variables as below:

For example: by default, each queue will be non-persistant. To set 

$conf['stomp_queue_{queue name}'] = array(
  'headers' => array(
    'persistent' => 'true',
  ),
);
