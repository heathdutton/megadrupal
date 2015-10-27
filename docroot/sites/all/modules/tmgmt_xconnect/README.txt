X-Connect Translator (tmgmt_xconnect)
=====================================

The X-Connect translator is a plugin for the Translation Management Tool (TMGMT)
and provides support to send and receive translation jobs to the Euroscript
Global Content Management (GCM) language services : http://goo.gl/VW0KK6.



Installation instructions
=========================

The X-Connect Translator is build for Drupal 7 and is a translator plugin for
the TMGMT module (https://www.drupal.org/project/tmgmt).

To use the X-Connect Translator you need to install the TMGMT module and its
dependencies.

* Download the module and unpack it in the sites/all/modules/contrib directory.
* Install the module trough the module management page. The module will be
  listed under the name "X-Connect Translator". The required dependencies will
  be installed with it.
* Enable the "Content Source User Interface" or/and the "Entity Source
  Interface" depending on the translation strategies in use on the website.



Configuration
=============

Open the Translation Management Translators page
(admin/config/regional/tmgmt_translator) this lists all the available
translators for the TMGMT module. The X-Connect translator will be listed as
"X-Connect (auto created)".

Tip: You can add extra X-Connect translators by clicking on the "Add Translator"
link and adding a new translator using the "X-Connect" translator plugin.

Click on "Edit" to configure an existing translator:


Order
-----

Scroll down and open the "Order" fieldset, fill in the order parameters as
received from the GCM service:

* Client ID : The client ID to order the translations for.
* Due date : What is the deadline for the file(s) to be translated. The deadline
  should be set in days from the moment the translation is ordered.
* Issued By : The email address of the, by the translation known, issuer of the
  translation.
* Confidential : Is the content for the translation confidential?
* Needs confirmation : Should there be a conformation send when the translation
  is ready?
* Needs quotation : Should a quotation be created and send before the
  translation is performed?


Connection
----------

Scroll down and open the "Connection" fieldset, fill in the connection
parameters as received from the GCM service:

* Connection type : What type of connection to use for the GCM service.
* Host : The GCM service server hostname.
* Username : The username to connect to the GCM service.
* Password : The password to connect to the GCM service.
* Request folder : The directory on the GCM service server where the translation
  order files should be saved.
* Receive folder : The directory on the GCM service server where the translated
  order files will be available.
* Processed folder : The directory where the translated order files should be
  moved to when the translation is processed (received) by the TMGMT module.
  This to inform the GCM service that a translation has been picked-up and is
  processed.

As the GCM service is a Human translation service, translations are not
processed in real-time. The X-Connect translator uses the Drupal cron process to
scan the service to see if there are translations available. The connection
configuration has two more settings about the cron:

* Cron status : If you don't want to automatically receive the translations, you
  can disable the cron integration for the Translator.
* Limit : By default all translated orders will be processed during the cron
  run. If there are to much translations available at the same moment, the cron
  could run out of time and stop without completing the translations processing
  and without performing other cron tasks. This option allows you to limit the
  number of translations to process during a single cron run.


Remote Language Mappings
------------------------

Scroll down and open the "Remote Language Mappings" fieldset. Drupal uses by
default a short language indication string (eg. en, fr, nl, ...).

The Remote Language Mappings shows a list of enabled website languages, you can
fill in the proper language code as required by the GCM service. The list of GCM
language codes can be obtained on request.



Usage
=====

Use the TMGMT user interface to select the content that needs to be translated
(admin/tmgmt/sources) or use the "Translate" tab on content nodes.

When requesting a new translation, select the "X-Connect" Translator as the
translation service. Add optional instructions for the translator and a
reference.

The translation order will be send immediately to the GCM service.



Manual send & receive
=====================

When there was a communication problem when sending the request, orders will be
kept to be send later. You can send them by going to the Jobs overview
(admin/tmgmt) and click on the submit link for the Job you want to send.

There is also a dedicated X-Connect managament page (admin/tmgmt/x-connect).
You can manual trigger actions to be performed for each X-Connect powered
translator on the platform:

* Send : will send all unprocessed jobs for the specific translator service.
* Scan : will scan the remote service and report about the number of processed
  jobs that are ready to be picked up.
* Receive : will pick up any processed (translated) request and import them back
  into the platform.



Overwrite configuration in settings.php
=======================================

The connection configuration can be overwritten by adding $config variables in
the settings.php file (<translator> is the machine name of the translator):


Order related settings
----------------------

* tmgmt_xconnect_<service>_order_client_id : The client ID.
* tmgmt_xconnect_<service>_order_due_date : What is the deadline for the file(s)
  to be translated. The deadline should be set in number of days from the moment
  the translation is ordered.
* tmgmt_xconnect_<service>_order_issued_by : The email address of the, by the
  translation known, issuer of the translation.
* tmgmt_xconnect_<service>_order_is_confidential : Is the content for the
  translation confidential?
* tmgmt_xconnect_<service>_order_needs_confirmation : Should there be a
  conformation send when the translation is ready?
* tmgmt_xconnect_<service>_order_needs_quotation : Should a quotation be
  created and send before the translation is performed?


Connection related settings
---------------------------

* $conf['tmgmt_xconnect_<translator>_connection_protocol'] : What type of
  connection to use for the GCM service (FTP / SFTP).
* $conf['tmgmt_xconnect_<translator>_connection_host'] : The GCM service server
  hostname.
* $conf['tmgmt_xconnect_<translator>_connection_user'] : The username to connect
  to the GCM service.
* $conf['tmgmt_xconnect_<translator>_connection_pass'] : The password to connect
  to the GCM service.
* $conf['tmgmt_xconnect_<translator>_connection_folder_request'] : The directory
  on the GCM service server where the translation order files should be saved
  (default To_LSP).
* $conf['tmgmt_xconnect_<translator>_connection_folder_receive'] : The directory
  on the GCM service server where the translated order files will be available.
  (default From_LSP).
* $conf['tmgmt_xconnect_<translator>_connection_folder_processed'] : The
  directory where the translated order files should be moved to when the
  translation is processed (default From_LSP/processed).


Cron related settings
---------------------

* $conf['tmgmt_xconnect_<translator>_cron_enabled'] : Enable (1) or
  disable (0) the cron for this translator.
* $conf['tmgmt_xconnect_<translator>_cron_limit'] : The number of
  responses to process during a cron run (0 = no limit, 1, 2, 5, 10, 20, 50,
  100, 200, 500, 1000).
