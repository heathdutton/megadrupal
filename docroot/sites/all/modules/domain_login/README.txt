CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation


INTRODUCTION
------------

Current Maintainer: Ian Whitcomb - http://drupal.org/user/771654

Domain login solves the problem of cross-domain logging in without enabling the
$cookie_domain variable by providing an additional domain field on the user
login forms for the main site.

Rather than enabling the $cookie_domain variable in your settings.php file thus
allowing users to login to every subdomain of a site, Domain Login will force
them to specify which subdomain they want to login to by altering Drupals core
login form to include an additional "domain" field at the top.

As an admin, you can leave the domain field blank to login to the primary
domain.


INSTALLATION
------------

1. This module REQUIRES the domain module.

2. Copy this domain_login/ directory to your sites/SITENAME/modules directory.

3. Enable the module.
