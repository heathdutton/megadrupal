CONTENTS OF THIS FILE
---------------------

 * Description
 * Installation
 * Start consumers

DESCRIPTION
------------

The module sends the watchdog logs to RabbitMQ queue and this way reduces db 
writes made on the current request.


INSTALLATION & CONFIGURATION
------------

The module requires installed and running Message Broker module.
 - Configuring Message Broker and MessageBroker AMQP
    Go to admin/config/system/message_broker_amqp and set you AMQP connection 
    settings.
    In "Path to json configuration file" enter the path to 
    /rabbitmq_logs/config.json file.
    (You can edit the file config.json if you want to use different names for 
    queues and exchanges or to change other settings)

    Go to admin/config/system/message_broker and choose AMQP implementation.
    Optionally you can enter a "Name of this drupal instance". This is useful 
    if you have multiple uniform drupal   instances where each installation has
    its own queues. Set the string to the name you use within your queue names.

    After that watchdog messages must be sent to the specified AMQP queue.
    
 - Other configuration (needs to be setup in settings.php):

    // Stop the output to the std out.
    $conf['rabbitmq_logs_stdout_message'] = FALSE;  
    
    // The queue name we need to listen in message broker. 
    // This is a setting for message broker.
    $conf['rabbitmq_logs_queue'] = 'logging';
    
    // The routing key message broker should use to add the message.
    // This is a setting for message broker.
    $conf['rabbitmq_logs_routing_key'] = 'log_key';
    
    //The delivery mode. Default is 2 which means durable.
    // This is a setting for message broker.
    $conf['rabbitmq_logs_delivery_mode'] = 2;

START CONSUMERS
------------
  Consumers are started with the drush command 'consume-amqp-messages' provided 
  by message_broker_amqp module. So to start a consumer for the log messages 
  simply type:

  drush consume-amqp-messages getlogs

  Command drush list-amqp-consumers - lists all currently running consumers

  You can stop the std output by using the following line 
  into your settings.php file: 
  $conf['rabbitmq_logs_stdout_message'] = FALSE;
