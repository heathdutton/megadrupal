INTRODUCTION
------------

Datex and related module will help to create a better multilingual website for
languages needing a Jalali (Or in near feature of datex, any) calendar instead 
of default Gregorian calendar.

  - datex API: is the base module, And used by other modules to localize dates. 
    On it's own, It would do nothing. It has API related to converting and 
    calculating dates of different calendars.

  - datex: When enabled, Will convert all dates to Jalali before display in
    page.

  - datex_date For Jalali support in date module, This module should be enabled
    It supports "views" too.

  - datex_popup: If you want to have date_popup localized, You should enable 
    this.


Since there is not a good "hook" in Drupal's core to alter dates and their
format, Some extra effort is needed to get "datex" module working. datex_date 
will work out of the box though. There are two methods available for datex:

  - Converting dates to Jalali in theming layer. Using this method, You won't 
    have to patch the core, But you will might miss localized date in  some 
    places. It's been tried to keep it as good as possible in non-patching
    mode, Views, node edit form, scheduler module and... are handled.
    If you encounter no problem using this mode, This is the recomended (and
    default) method.

  - Patching core: There is a patch file provided within this module, If you
    apply it on 'common.inc' file, You get localized date everywhere.
    This patch adds a hook to core for altering format_date result with 
    smallest footprint possible.

Keep in mind that hacking core is a *very bad* practice, So decide for yourself.


INSTALLATION
------------

  - Download datex from http://drupal.org/project/datex and put it in your 
    sites/all/modules folder of your Drupal installation or use drush. Then enable 
    it.
  - Enable Locale
  - Go to admin/config/regional/date-time/datex and configure default schema.

If you read above and want to apply the patch, Locate the patch file in
module's folder "drupal-jalali_support-0-0.patch", and apply it to file
"includes/common.inc". Instructions about patching files is at drupal.org.
You should configure datex to use patching-mode.


JQUERY LIBRARY
--------------

Since default library of date (datepicker) does not support international
calendars, A very good robust library written by "Keith Wood" is used. Note that
version 1.2.1 is tested and fully working. Download it from:

  - http://keith-wood.name/calendars.html
  or
  - https://github.com/kbwood/calendars

And extract it in "sites/all/libraries/jquery.calendars" of your Drupal.
So finally you will have a file like this: 
sites/all/libraries/jquery.calendars/jquery.calendars.all.min.js


CONFIGURATION
-------------

For each part of Drupal, Datex follows a behaviour called 'schema'. Different 
schemas can be defined and edited at datex configuration page. By default,
everything follows the default schema unless set otherwise.
Node display page, Views and other stuff always follow the default schema, But
date field widget, Diplay formatter, And views formatter can be configured 
independently. Just go to the configuration form of each, And you see a option
indicating what datex should do. If it's Forced disabled, Then datex ignores 
the field completely.


TRANSLATION
-----------

No localized, Translated string is harcoded in the code, So a persian month is
like 'Aban', You can translate it to "آبان" using locale.
