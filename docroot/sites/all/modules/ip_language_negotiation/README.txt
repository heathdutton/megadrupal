Module: IP Language Negotiation
Author: Baris Wanschers <https://drupal.org/user/107229>


Description
===========
Detect the user's language by IP for language negotiation.

Requirements
============
* IP-based Determination of a Visitor's Country (ip2country)

Installation
============
* Enable the ip2country and the ip_language_negotiation module.

Usage
=====
Navigate to admin/config/regional/language/configure.

* Enable the Country method.
* You usually want the user to be able to override the automatic detected
  settings. You can do this by using URL detection. Set a unique domain, or
  a path prefix FOR EACH LANGUAGE. Eg: /en and /nl. Don't use a prefix for
  one language only, because in that case the user is not able to override
  the automatic detected setting.
* Make sure that the user-overrideable setting is on top. A good setting is
  URL detection first, and Country detection as second.
* Go to admin/config/regional/language/configure/ip and set the default
  language per country. As a fall-back, the default language is used.
