===========================
Async Command Documentation
===========================


Installation and Configuration
------------------------------

This module is designed to help dependent modules run complex 3rd party programs outside of Drupal on a remote server. Please read documentation of the dependent module (eg Recommender API) for more information on Installation and Configuration.

Conceptually, you need 2 computers: the Drupal server for your Drupal site, and the computation server for the 3rd party program. The Drupal server simply issues commands to the 3rd party program, which will be executed asynchronously on the computation server. On the Drupal server, the async_command module provides APIs to help dependent modules issue commands, display commands execution history, and so on. On the computation server, the async_command module provides a few Java libraries to help 3rd party programs access Drupal database (read data stored in Drupal, save data back into Drupal database, etc).

To install and config async_command, please follow these steps:

**Step 1**. Install the Async Command module to your Drupal server under sites/all/modules/async_command. Then, copy the 'lib' sub-directory and async-command.jar to any folder on the computation server. If your computation server is also your Drupal server, you can leave 'lib' as-is, but be aware that the 3rd party program might consume lots of resources on your Drupal server.

**Step 2**. Follow the example of config.properties.example and create a config.properties file on your computation server. 

**Step 3**. Follow the example of run.sh.example, and create run.sh on your computation server.

**Step 4**. Make sure to read the documentation of dependent modules (eg Recommender API) for additional settings. And then you can execute "run.sh" to run the 3rd party program on the computation server.



For Developers
-------------------------------

Please see examples of how to write 3rd party programs using the async_command APIs:
http://drupal.org/project/recommender (written in Java)
http://drupal.org/project/mturk (written in Jython)

For the Drupal server, you'll write a Drupal module for user interaction. You need "async_command_create_command()" to issue async commands. You can use the default Views to display commands execution history.

For the computation server, you can write 3rd party programs by extending "org.drupal.project.async_command.Druplet". This class provides basic functions to read Drupal system variables, read commands stored {async_command} table, and so on. (TODO: More documentation on how to program with Async Command)

To use the Java libraries in languages other than Jython/JRuby/Groovy/Java, please refer to JSR 223 (http://download.oracle.com/javase/6/docs/technotes/guides/scripting/index.html)


FAQ
------------

Q: Why is async_command different from Drush, Batch API, Queue API, or the Beanstalk module?
A: See answers on http://drupal.org/project/async_command.

Q: How to get support?
A: Please create issues on http://drupal.org/project/async_command.
