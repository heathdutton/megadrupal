Localization update bundled
============================

The Localization update bundled module exposes bundled project translations to
Localization update so you can easily track changes and update when needed.

Dependencies
------------
- Localization update 1.x

Installation
------------
Enable the module as normal and make sure the site base URL is set at
admin/config/regional/language/update

Your bundled translations should be available after refreshing the
Localization update information at admin/config/regional/translate/update

Important note
--------------
The po files must have the same name as in the languages table, e.a. "nl.po"
and they must be stored in the "translations" folder of your theme/module.
For example: my_module/translations/nl.po
