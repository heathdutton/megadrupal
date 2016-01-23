CONTENTS OF THIS FILE
---------------------

* Introduction
* Installation
* Configuration


INTRODUCTION
------------

Who Sends API Integration
.........................

Current Maintainer: Darren Whittington <darren.whittington@gmail.com>

This module integrates with the WhoSends email spoofing and phishing system API 
enabling administrators and users to send email that is tagged with their own 
secure key image.

The module integrates with drupals core mail module to add the key image to the 
top or bottom of outgoing email, more information about whosends can be found 
here https://www.whosends.com/

INSTALLATION
------------

The module should be installed by decompressing the archive into your contrib 
folder or using the user interface in Drupal.

CONFIGURATION
-------------

Once installed you should go to the configuration page and enter the details 
supplied by whosends, the only option that differs from the whosends API 
configuration instructions is the placement of text.

The user can either send email as HTML or plain text, a plain test email will 
just contain a link to varify if the email is valid, a HTML version will hold
a image. when sending HTML email the sender should format the text in html.
