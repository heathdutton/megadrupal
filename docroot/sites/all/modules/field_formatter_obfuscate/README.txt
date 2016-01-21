
-- SUMMARY --

Adds a formatter for text / long text fields to obfuscate the value.

The *Field formatter obfuscate* module allows you to obfuscate any
text / long text field by *encoding and hiding the value* using *ROT13 cipher
and data-attributes*.
Client-side Javascript will be used to decode and display the value.

Note that this is *not an encryption*. It is merely a simple scheme to hide
text from search engines.

For a full description of the module, visit the project page:
  http://drupal.org/project/field_formatter_obfuscate

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/field_formatter_obfuscate


-- REQUIREMENTS --

None.


-- INSTALLATION --

* Install as usual, see Installing contributed modules (Drupal 7) for further
  information:
    http://drupal.org/documentation/install/modules-themes/modules-7


-- CONFIGURATION --

* Add a text / long text field to any content type (on manage
  fields admin page)

* Set the display to "Obfuscate" (on manage display admin page)


-- SUPPORTED FIELD TYPES --

This formatter can be applied to the following field types:

* Text fields
* Long Text fields
* Text With Summary fields


-- SIMILAR / RELATED MODULES --

See the field_formatter_settings module if you need more extensible field
formatters.

-- TODO --

Maybe use field_formatter_settings someday.

-- CONTACT --

Current maintainers:
* Christian W. Zuckschwerdt (zany) - http://drupal.org/user/284183

This project has been sponsored by:
* Laboratorio
  Agency for Advertisment / Strategy / Design.
  Visit http://www.laborato.de/ for more information.
