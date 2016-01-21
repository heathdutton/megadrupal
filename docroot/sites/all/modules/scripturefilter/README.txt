Readme
------

This is a simple filter module. It converts a Scripture reference into
a clickable link that points to one of several on-line Bibles. The
default is the NIV in English on biblegateway.com's site. It can also
use the ESV online Bible and the NET Bible.

It is an adaptation of a PHP port of the Scripturizer Perl plug-in. A
full list of credits is in the CREDITS file.

If you upgrade from the Drupal 7 version having previously used the module
on an early core version of Drupal, your settings will not be preserved. This
is because Drupal 7 has changed the way that settings for input filters are
stored in the database, and trying to migrate them risks them being migrated
wrongly because of assumptions that would have to be made. You therefore need
to enable the Drupal 7 version of the module, and then re-enable this filter
in any text formats that are going to use it. You will also need to re-enter
your default Bible translation for any of those text formats.
