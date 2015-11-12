DOMAIN DEFAULT REDIRECT
-----------------------

Introduction
============

This small module provides a redirection feature instead of providing the default domain contents in the Domain Access suite.

In a nutshell, subdomain redirection handling will be shifted from your domain's DNS settings to your Drupal installation, redirecting all subdomains to the specified URL while still serving Drupal content to the subdomains set in Domain Access.


Requirements
============
1. Domain Access
2. A subdomain wildcard record in your DNS settings. Otherwise all subdomains won't even make it to your Drupal installation...


How to set up
=============
To set up this module, go to the global Domain settings page at admin/structure/domain/settings. There will be an extra textfield on top of the form in which you can enter the URL.