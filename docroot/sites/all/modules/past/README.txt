CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Features
 * Requirements
 * Installation
 * For More Information

 INTRODUCTION
 ------------

 Past is an extended logging framework, designed to log and later analyze
 complex data structures to a pluggable backend.

 The typical use case for this are logging communication between one or multiple
 services, where data is received, processed and forwarded.

 A single log message is called an event, which can have multiple attached
 arguments, each argument can be a scalar, an array, an object or an exception.
 Complex data structures like arrays and objects are stored separately so that
 they can be searched and displayed in a readable way, those rows are called
 "(event) data".


 FEATURES
 --------

 THE CORE

 * Procedural and object oriented interface for logging events
 * Attach any number of arguments to a single event
 * Pluggable backends. The current default backend is a database/entity based
   backend, a simpletest backend is additionally provided that allows to display
   events as debug output
 * Each element in an array or object is stored separately to be search and filterable
 * Views and drush integration for the database backend
 * Expiration of old entries

 ERROR CAPTURE

 * Watchdog implementation - can be used to replace dblog
 * PHP Exception handler
 * Implements PHP shutdown function to capture also PHP fatal errors
 * Provides server error log grabber that can extract PHP syntax errors so that
   all error events are gathered in the Past log

 EXTENDABILITY

 Past DB module provides Past Event Types as bundles for events. This feature
 can be utilised to use Past for different purpose logging. So it can server as
 error logging utility as well as messaging backend for capturing events from
 i.e. ecommerce transactions.

 EVENTS MANAGEMENT

 Past Bughunt is a module that provides a simple workflow to manage individual
 events. It has following features:

 * Provides an UI to set events as TODO and Done.
 * Inserts a list of suggested similar events into the event detail page.

 DRUSH GRABBER EXTENSION

 Past Grabber is shipped with past-grab command that can parse server error log
 files for specified error type entries and create past events for them.

 drush past-grab /var/log/apache2/error.log "PHP Fatal error" --count=100

 The above command will scan the file on provided path, parse out PHP Fatal
 error entries and process them into past events.

 REQUIREMENTS
 ------------

 Past was built for D7. There will be no back port.

 INSTALLATION
 ------------

 Enable the "Past" and "Past Database Backend" modules, unless you want to use
 a different backend.

 If you wish to use bughunt or grabber features, enable appropriate modules.

 For configuration options visit the admin/config/development/past/settings
 page. Note that logging and watchdog features needs to be enabled in the
 settings first.

 To configure event types visit the admin/config/development/past-types page.

 USAGE
 -----

 See the past_event_create() and past_event_save() functions in past.module for
 an entry point to the API.

 The default, views based log overview page is at Administration > Reports >
 Past.

 All events are in default created with past_event bundle. In case you defined
 a custom event type you can wrote your own wrapper as follows:

/**
 * Wrapper to create an event of a specific type/bundle.
 */
function YOURMODULE_event_save($machine_name, $message = NULL) {
  $event = past_event_create('YOURMODULE', $machine_name, $message);
  $event->type = 'my_event_type';
  $event->save();
  return $event;
}

 OPTIONAL USAGE
 --------------

 To add optional integration with Past in a situation where you do not want
 a hard dependency (e.g. a contrib project), you can add the following wrapper
 function to your project/module.

 The downside of this approach is that it is not possible to use the object
 oriented interface which is more flexible and easier to use when logging
 multiple arguments.

/**
 * Wrapper for past_event_save to avoid dependency to past.
 */
function YOURMODULE_event_save($module, $machine_name, $message, array $arguments = array(), array $options = array()) {
  if (module_exists('past')) {
    past_event_save($module, $machine_name, $message, $arguments, $options);
  }
  else {
    $severity = isset($options['severity']) ? $options['severity'] : WATCHDOG_NOTICE;

    // Decode exceptions, as trying to print_r() them results in recursions.
    foreach ($arguments as &$argument) {
      if ($argument instanceof Exception) {
        $decoded = _drupal_decode_exception($argument);
        $argument = $decoded;
      }
    }

    watchdog((string) $module, '@name::@message <pre>@arguments</pre>', array(
      '@name' => $machine_name,
      '@message' => $message,
      '@arguments' => print_r($arguments, TRUE),
    ), $severity);
  }
}

 DB SYNCHRONIZATION
 ------------------

 If you are using past_db for the storage of your logged data, please note that
 the size of your DB will increase and can become quite large after some time.
 This can lead to very large file sizes of your SQL dumps and thus long waiting
 time for DB synchronization using drush commands sql-sync and sql-dump.

 In order to prevent this, you can exclude the past_db tables by doing this:

 1. Add a configuration line in your drushrc.php file:
    $options['skip-tables']['common'] = array('past_event', 'past_event_argument', 'past_event_data');

 2. Use the --skip-tables-key=common arguments when executing
    drush sql-sync/dump. For further information please check the documentation
    of the commands: http://drush.ws/#sql-sync and http://drush.ws/#sql-dump

 FOR MORE INFORMATION
 --------------------

  * Project Page: http://drupal.org/project/past
  * Issue Queue: http://drupal.org/project/issues/past
