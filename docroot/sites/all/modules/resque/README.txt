
Drupal Resque Module
-----------------
by Arturo Contreras, arturo.kontreras@gmail.com

This module makes use of an existing php resque implementation:
https://github.com/chrisboulton/php-resque

This module extends DrupalQueue to be able to use Drupal\resque\Resque instead,
also being able to use Drupal\resque\ResqueUnique so that jobs with the same
payload key are not queued at any point in time.

This README is for interested developers.

--------------------------------------------------------------------------------
                                Requirements
--------------------------------------------------------------------------------

  * Redis installed locally or use a remote server.
  * PHP-Resque library https://github.com/chrisboulton/php-resque version 1.3,
    I recommend using Composer to install to a directory in your app.

--------------------------------------------------------------------------------
                                How To Use
--------------------------------------------------------------------------------

  * In order for drupal to start using your new queue mechanism you need to set
    the variable, e.g. variable_set('queue_default_class', 'Drupal\resque\Resque').
    You can also set the class to use for a particular queue by setting that
    variable, like ('queue_class_update_fetch_tasks', 'Drupal\resque\Resque').
  * I recommend installing the Resque gem
    https://github.com/resque/resque/tree/1-x-stable so that you can startup
    the web interface and look at queued jobs or you can also use
    https://github.com/resque/resque-web
  * In order to start a worker, you need to tell your workers where your drupal
    installation is and also where composer has installed php-resque by setting
    the environment variables DRUPAL_ROOT and DRUPAL_COMPOSER_ROOT.
  * You also need to set an environment variable that will dictate what queue
    your worker will be responsible for running so the end result of the script
    I included that you can use to startup your workers:

    QUEUE=update_fetch_tasks DRUPAL_ROOT=/var/www/myapp
    COMPOSER_ROOT=/var/www/php-resque php bin/resque_worker_run
  * If you want to start multiple workers I suggest using monit
    (https://bitbucket.org/tildeslash/monit) or god (http://godrb.com/)
