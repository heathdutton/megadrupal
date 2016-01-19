CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration

INTRODUCTION
------------
miniOrange SAML 2.0 SSO allows users residing at SAML 2.0 compliant Identity Provider to login to your Drupal. We support all known IdPs - Google Apps, ADFS, Okta, Salesforce, Shibboleth, SimpleSAMLphp, OpenAM, Centrify, Ping, RSA, IBM, Oracle, OneLogin, Bitium, WSO2, NetIQ etc. If you need detailed instructions on setting up these IdPs, we can give you step by step instructions.

REQUIREMENTS
------------
Libraries module is required for this Module to work.

INSTALLATION
------------

Download Libraries project from https://www.drupal.org/project/libraries. Install and enable this module.

Download xmlseclibs from https://github.com/simplesamlphp/xmlseclibs. Extract the archive and place it under sites/all/libraries.

Install as you would normally install a contributed Drupal module. See:
https://drupal.org/documentation/install/modules-themes/modules-7
for further information.

CONFIGURATION
-------------
 * Configure user permissions in Administration » People » miniOrange SAML Login Configuration:


   - Setup Customer account with miniOrange


     Create/Login with miniOrange by entering email address, phone number and
     password.


   - Identity Provider Setup.


     Make note of the Service Provider information. This will be required to configure your IdP.


   - Service Provider Setup


     Configure the Drupal site to act as a Service Provider(SP). Information such as IdP Entity ID, x.509 certificate and SAML Login URL are taken from IdP and stored here.

