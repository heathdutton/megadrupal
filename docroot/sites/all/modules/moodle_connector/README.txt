Moodle Connector is a module that provides a set of functions to integrate
Drupal with Moodle.

Requirements
------------
 * Moodle

Usage
-----
 * Go to *Configuration* -> *People* -> *Moodle Connector settings*
(/admin/config/people/moodle) to set up the settings to connect to the database
 of Moodle.
 * Call moodle_connector_connect() function in your module to connect to Moodle.

Notes
-----
 * Moodle credentials are stored in the Drupal database in plain text.
 * 'Administer Moodle Connector' permission is available.

Author
------
Pere Orga Esteve <pere@orga.cat>

Initially, I used code from Drupal Moodle Courselist by Adam Gerson, see
http://drupal.org/project/moodle_courselist for details.
