-- SUMMARY --

Antispam module by CleanTalk to protect Drupal sites from spam bot registration
 and spam comments publication.

Key features.
* No needs in CAPTCHA, etc.
* Protection from spam bots and manual spam comments.
* Automoderation - automatic publication of relevant comments.
* Contact forms protection.

What CleanTalk is.
 CleanTalk is a SaaS spam protection service for Web-sites.
 CleanTalk uses protection methods which are invisible for site visitors.
 Using CleanTalk eliminates needs in CAPTCHA, questions and answers, and other
  methods of protection, complicating the exchange of information on the site.

How it works.
 Messages or registration requests are sent to the CleanTalk cloud, data is
  tested with several methods on the cloud, then the site receives a response
  decision to approve or deny the message/registration.

Note:
 This module depends on both Libraries API module and CleanTalk PHP-antispam
  classes.
 See REQUIREMENTS and INSTALLATION for details.


-- REQUIREMENTS --

* Libraries API module
  http://drupal.org/project/libraries

* CleanTalk PHP-antispam classes
  https://github.com/CleanTalk/php-antispam


-- INSTALLATION --

1. Download latest CleanTalk PHP-antispam classes from

     https://github.com/CleanTalk/php-antispam

   and extract the following files into a 'cleantalk' library directory:

     sites/all/libraries/cleantalk/JSON.php
     sites/all/libraries/cleantalk/cleantalk.class.php

   It is recommended that you create your libraries directory at one of the
   following locations:

     sites/all/libraries/cleantalk
     sites/<site>/libraries/cleantalk

   Consult the Libraries API documentation for further information.

2. Get access key on http://CleanTalk.org.
3. Install CleanTalk module as usual and put access key into its settings.


-- TESTING --

* Try to register account with "stop_email@example.com" as email address.
* Set "Enable comments test via stop list" flag at https://cleantalk.org/my/
* Then try to put comment with "stop_word" in its body.


-- CONTACT --

Feel free to contact us at https://cleantalk.org/contacts

Current maintainers:
* Alexey Znaev - https://drupal.org/user/2727147
