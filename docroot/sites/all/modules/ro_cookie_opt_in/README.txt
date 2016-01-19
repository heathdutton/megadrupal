---DESCRIPTION---

This module integrates the Rijksoverheid Cookie Opt-in solution
(http://www.rijksoverheid.nl/cookies/rijksoverheid-cookie-opt-in) with Drupal.

Dutch law requires that websites:

 * Inform visitors about the use of cookies (or other ways of storing
   information) by the website.
 * Ask visitors approval to use cookies (and based on that approval allow or
   disallow certain techniques).
 * Provide proof about the given approval.

The Dutch government offers a standard (JavaScript-based) solution that deals
with the approval requirements and offers example texts to comply with the
information requirement. The solution may be used by government as well as
non-government organisations under the conditions of the Creative Commons
Attribution-ShareAlike 3.0 Unported (CC BY-SA 3.0)
(http://creativecommons.org/licenses/by-sa/3.0/).

---REQUIREMENTS---

The module requires the Libraries 7.x-2.x module (http://drupal.org/project/
libraries) and the Variable 7.x-2.x module (http://drupal.org/project/variable)
to be installed and that the Rijksoverheid Cookie Opt-in version 1.1 library
(http://www.rijksoverheid.nl/bestanden/documenten-en-publicaties/brochures/2012/
11/22/download-rijksoverheid-cookie-opt-in-zip/rijksoverheid-cookie-opt-in-v1-1.
zip) is put in the /sites/all/libraries/rijksoverheid-cookie-opt-in folder.

---FEATURES---

The module provides the following features:

 * Integration of the Rijksoverheid Cookie Opt-in JavaScript solution with
  Drupal
 * All texts in the cookiebar are configurable and have token support.
 * You can choose to include or exclude the css from the library (the css is
   based on the Rijkshuisstijl of the Dutch government).
 * I18n support by using the Variable Translation submodule of the I18n module.
 * Approval logs are kept in the Drupal database or can be disabled (note that
   you still have to provide another way to log the approvals to comply with the
   Dutch law).
 * A block with a link to open the cookiebar to change cookie preferences.
 * A JavaScript function that can be called by modules/themes to check if
   cookies are allowed (Drupal.ro_cookie_opt_in.cookiesAllowed(): returns true
   or false).
 * A PHP function that can be called by modules/themes to check if cookies are
   allowed (ro_cookie_opt_in_cookies_allowed(): returns true or false).
 * An optional css fix to make the cookie popup responsive when using the
   Rijkshuisstijl css.
 * Drush integration to automatically download the Rijksoverheid Cookie Opt-in
   library into the library folder and optionally remove unnecessary files from
   the library.

Since the library requires an xml:lang attribute on the html element in the
output of the website (issue #1873792), the module contains a workaround to add
it if is not there using the value of the lang attribute. Without this
workaround you will get JavaScripts errors saying "TypeError: langCode is
undefined". As soon as the library fixes this behaviour, the workaround will be
removed from the module.

---EXAMPLE USAGE FOR GOOGLE ANALYTICS---

The following example prevents Google Anaytics from setting cookies until
consent is given by the visitor. Put the behavior in your theme JavaScript or a
JavaScript added by a custom module.

<code>
// Disable Google Analytics if the visitor has not given consent to place
// cookies.
Drupal.behaviors.disableGACookiesIfNoConsent = {
  attach: function (context, settings) {
    if (!Drupal.ro_cookie_opt_in.cookiesAllowed()) {
      // The Google Analytics account number. Preferably you put this in JS
      // settings in a custom module and read it from the Drupal GA module
      // settings.
      var ga_account = 'UA-12345678-12';
      window['ga-disable-' + ga_account] = true;
    }
  }
};
</code>

Development of this module is sponsored by LimoenGroen.

---INSTALLATION---

Prerequisites:

* Installed Libraries 7.x-2.x
* Installed Variables 7.x-2.x
* For I18n: installed Internationalization (i18n) variable translation submodule

Installation steps:

1 > Download the Rijkoverheid Cooke Opt-in module from drupal.org.
2 > Extract the module in the contributed modules directory.
3 > Download the Rijksoverheid Cookie Opt-in library from rijksoverheid.nl.
4 > Extract the library in the sites/all/lbraries/rijksoverheid-cookie-opt-in
    folder so that that folder contains the behaviour and presentation folders
    containing the library's js and css files.
5 > Enable the Rijsoverheid Cookie Opt-in module at http://www.example.com/
    admin/modules
6 > Set the permissions at http://www.example.com/admin/people/permissions
7 > Configure the module at http://www.example.com/admin/config/system/
    ro-cookie-opt-in
8 > Optionally enable the cookie preferences block at http://www.example.com/
    admin/structure/block or, when you are using the context module, enable it
    in the block reaction of a context.
9 > Optionally remove the unnecessary files from the library (see unnecessary
    file list below).

NOTE I: Steps 3, 4, and 9 can be performed by using the Drush command supplied
by the module: "drush dl-ro-cookie-opt-in". The drush command will prompt if you
want the unnecessary files in the library to be removed.
NOTE II: If you enabled the module before you downloaded the library, you might
have to clear the cache for Drupal to properly recognize that the library is
installed.

Unnecessary files:

The library contains some files that are unnecessary for the functionality of
the library. You can delete the following folders and files from the
Rijksoverheid Cookie Opt-in library folder:

/content
/behaviour/cookies.js
/behaviour/cookiebar-init.js
/behaviour/jquery-1.8.1-ui-1.8.23.custom.min.js
/presentation/screen.css
/presentation/screen-cookies.css
/presentation/screen-ie6.css
/presentation/screen-ie7.css
/voorbeeld.html
/cookie overzicht Voorbeeldtekst v5.doc
/cookie overzicht Voorbeeldtekst v5 Engels.doc
/Documentatie-Rijksoverheid-Cookie-opt-in-v1.1.pdf
/Documentatie-Rijksoverheid-Cookie-opt-in-v1.1.pdf
/handleiding technische implementatie.txt
