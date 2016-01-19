Web Service Integration Framework (WSIF)
================================================

When connecting Drupal to an external webservice there are a number
of repeated tasks that any good web service integration requires.

1. List if the webservice is running on the report status screen
2. Take the connection to the webservice down globally if it stops
   responding or runs slow.
3. Periodically check the web service when it is globally off to
   see if it is back and working correctly.
4. Singleton and Factory patterns for resource handling.
5. Global logging and exception handling
6. Global caching.

The goal of the WSIF connector is to provide a boostrap framework
that allows other modules which talk to webservices in a standard
fashion.

How to use this module with your web services module
====================================================

1. Extend and wrapper the API.
   You will generally have an API for your webservice, typically
   a class file that must be instantiated.
   Instead of instantiating this directly we will wrapper it in
   a WSIFInterface container.
   Assuming your module is called mymodule and your service API
   class is called MyService this provides the following starting
   files or your module:
     - mymodule.module
     - mymodule.info
     - myservice.class.inc
     - myservice_wrapper.class.inc

   The myservice_wrapper.class.inc will include a class with a
   definition like this:
     class MyServiceWrapper extends MyService implements WSIFInterface { }

   You must then provide two functions inside this called
   wsifCheckConnection() and wsifVersion().  The first is run
   periodically by cron when the service is flagged as off
   and will switch it back on if it returns TRUE. The second
   is purely informative and displays the version on the report
   status screen.

   Within mymodule.module you should define the following hooks:

  /**
   * Implements hook_wsif_info().
   */
  function mymodule_wsif_info() {
    return array(
      'responsys' => array(
        'name' => t('My Service'),
        'description' => t('My Service integration'),
        'machine_name' => 'myservice',
        'extra' => '',
      ),
    );
  }

  /**
   * Implements hook_wsif().
   */
  function mymodule_wsif($reset = FALSE) {
   return new MyServiceWrapper();
  }

  The hook_wsif_info hook informs WSIF about your service.
  The hook_wsif() hook is the factory method for your API and
  should return a new version of the API. The WSIF module will
  only call this once per session to build an instance of the
  connection that gets passed around.

2. In your code, to obtain an instace of yoru API you would now
   use the WSIF function:

     $api = wsif_get_api('myservice');

   If the service is unavailable, this will generate an Exception
   of type WSIFUnavailableException. You should allow this
   WSIFUnavailableException to propergate up the stack to the
   point at which it makes sense to handle it. For example,
   when the user tries to perform an action which needs the service
   you could then tell them it did not work as the service is
   unavailable.

   try {
     mymodule_update_record($user, $data);
   }
   catch (WSIFUnavailableException $e) {
     watchdog_exception('mymodule', $e);
     drupal_set_messge(t('Unfortunatley our partner service is currently unavailable, please try again later'));
   }

3. The handling of whether a service is unavailable currently happens
   at the point the service class is instantiated. This needs improving
   so we can detect when a timeout occurs on any request. This may
   require a change to the way the wrapper is written.

Example of use
==============

The responsys module makes use of the wsif module to provide
an integration with the Responsys Email marketing tool.
http://drupal.org/project/responsys
