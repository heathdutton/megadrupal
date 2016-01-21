CONTENTS OF THIS FILE
---------------------

* Introduction
* Dependencies
* Installation
* Configuration


INTRODUCTION
------------
This module allows for Two Factor authentication when users login to their
account with the option of forcing all users to use Authy(https://www.authy.com/).

You will need to sign up for an account with Authy in order to use this module
but there is a FREE tier (https://www.authy.com/developer/pricing) that will
work for most users. 

DEPENDENCIES
------------
libraries - http://drupal.org/projects/libraries

authy-php - https://github.com/authy/authy-php

INSTALLATION
------------
Install as usual, see http://drupal.org/node/70151 for further information.

Install dependencies:

Download and install the authy-php library

    wget https://github.com/authy/authy-php/tarball/master -O authy-php.tar.gz
    tar xvzf authy-php.tar.gz
    mv authy-authy-php-* authy-php


CONFIGURATION
-------------
After installation, go to admin/config/people/authy_tfa and add your authy API
key. You will also need to go to admin/people/permissions#module-authy_tfa and
set the permissions.
