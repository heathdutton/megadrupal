SUMMARY: This module adds a temperature field to Drupal 7; it uses the Field
API. It has settings (via the temperature_ui module) that let users choose their
preferred temperature unit and then it allows them to view the value of this
field in their preferred value, if field settings allow such localization. It
supports fahrenheit, celsius and kelvin. It also provides useful functions to
format and convert temperature values. At this time is has a select list widget.
I may add other widgets in the future if needed, such as a simple textfield.


REQUIREMENTS:


INSTALLATION:
* Download and unzip this module into your modules directory.
* Goto Administer > Site Building > Modules and enable this module.


CONFIGURATION:
* Add a temperature field to an existing entity
* Click on Manage Display and choose the format for the field.


USAGE:
* Unless you're using another module to set $user->temperature_units, you will
  want to enable temperature_ui module as well; it provides a setting to the
  user edit page that allows the user to choose his temperature units
  preference.


API & USEFUL FUNCTIONS:
* The following are helpful functions provided by this module:
  - temperature_convert()
  - temperature_format()
  - temperature_units()
  - temperature_units_by_user()

* $user->temperature_units: The units that are used for a given user will be
  taken from the value of $user->temperature_units; custom modules should set
  this to an allowable value during hook_user_load. If the value is not set for
  the current user, then the default value set up in the field instance settings
  will be used.


LOCALIZATION:
* On node/add, the storage temperature unit is set based on user or field
  preferences. Once the node is submitted the temperature unit is set and won't
  be changed in the db.
* On node/edit the temperature unit displayed in the dropdown will be based on
  user preferences, but the stepping of the options is based on the db unit; it
  is possible to see different celsius options on a node/add form and a
  node/edit form in the following case:
  
    1. user A has a pref for celsius and visits node/add. The field settings
    step for celsius is 1. The select list will be a list of celsius temps
    stepped by 1.
  
    2. user B has a pref for fahrenheit and visits the node/edit page of the
    above. The field settings step for fahrenheit is .1. The select list will
    show temperatures in FAHRENHEIT but it will be stepped in celsius units by 1
    degree. If user B were to visit node/add, she would see the temps in
    FAHRENHEIT, but they would be stepped by .1 degrees in fahrenheit. In this
    case the select lists are different for user B.

* Temperatures will be stored in the database in the default units as set in the
  global field settings; however in the UI for editing, the units will be
  localized based on user preferences, in all select lists.
* The localization of the output of a field is based on the display settings of
  the field as set in Manage Display; the field can be forced to always display
  in a set unit, or follow the author's preference, or the viewing user's
  preference. Other view options may also be available.

  
--------------------------------------------------------
CONTACT:
In the Loft Studios
Aaron Klump - Web Developer
PO Box 29294 Bellingham, WA 98228-1294
aim: theloft101
skype: intheloftstudios

http://www.InTheLoftStudios.com