CONTENTS OF THIS FILE
---------------------
   
 * Introduction
 * Requirements


INTRODUCTION
------------

Webform Sermepa module provides a webform component to offers support for 
Spanish banks that use Sermepa/Redsys systems.

Full list of banks managed by sermepa: 
http://www.servired.es/servired/miembros-servired/

This module can be used to create a donate form or an other simple form in 
order to send a Sermepa TPV request without the need to use a complete 
commerce solution.

It works this way: when payment has been completed, the form submission data 
is updated to register it. The webform mails are sent if the transaction has 
completed successfully.


REQUIREMENTS
------------

PHP >= 5.3
Mcrypt
Sermepa/Redsys Library - https://github.com/CommerceRedsys/sermepa

Also, this module requires the following modules:
 * Webform (https://drupal.org/project/webform)
 * Webform Ajax (https://drupal.org/project/webform_ajax)
