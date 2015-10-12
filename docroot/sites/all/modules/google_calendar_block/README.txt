CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Permissions
 * Usage

INTRODUCTION
------------

Current Maintainers:

 * Devin Carlson http://drupal.org/user/290182

Google Calendar Block is a lightweight module which allows administrators to
create blocks which display Google Calendar events.

Google Calendar Block will never provide advanced Google Calendar integration
such as OAuth user authentication or the ability to create new calendar events
from Drupal. These capabilities are provided by other modules such as Google
Calendar API (https://drupal.org/project/gcal).

REQUIREMENTS
------------

Google Calendar has two dependencies.

Drupal core modules
 * Block

Contributed modules
 * Libraries API - 2.x

INSTALLATION
------------

To install Google Calendar Block:

 * Download the latest version of the Google API PHP Client library
   (https://github.com/google/google-api-php-client) and extract it to
   sites/all/libraries or sites/sitename/libraries as you require. The extracted
   folder must be named google-api-php-client.
 * Enable Google Calendar Block and all of the modules that it requires.

PERMISSIONS
------------

The ability to create, edit and delete Google Calendar Block blocks relies on
the block module's "Administer blocks" permission.

USAGE
-----

Administrators can visit the Blocks administration page where they can create
new Google Calendar Block blocks and update or delete existing Google Calendar
Block blocks.

Administrators can also position Google Calendar Block blocks as they can with
standard or custom blocks provided by the core Block module.

Each Google Calendar Block block requires an application name, developer key and
calendar ID in order to function.

 * Sign into the Google Developer Console
   (https://console.developers.google.com/project) using a Google Account and
   create a new project.
 * Go to the APIs page located under the APIs and auth section. Follow the
   Calendar API link under the Google Apps APIs heading. Enable the
   Google Calendar API.
 * Go to the Credentials page located under the APIs and auth section. Press the
   Create new Key button under the Public API access heading. Press the
   Server key button and then press the Create button. You can restrict API
   access to only your server if it has a dedicated IP address.

Use the Project ID, found on project overview page as the application name and
the API key, found on the credentials page, as the developer key. The
Calendar ID for the calendar you wish to display can be found on the settings
page for each calendar.

 * Visit Google Calendar (https://www.google.com/calendar), log in and browse to
   the settings page (select the gear icon and choose Settings).
 * From the settings page, select the Calendars tab and then select the calendar
   that you wish to display in the Google Calendar Block block.
 * Look for the Calendar Address label. The Calendar ID should be displayed in
   brackets.
 * Note that only public calendars can be displayed using Calendar Block. Ensure
   that your calendar has been made public by visiting the Share this Calendar
   tab and selecting both the Share this calendar with others and
   Make this calendar public options.

By default, primary calendars are not shared outside of the domain for Google
Apps users. In order to display the primary calendar of a Google Apps account,
you must set 'External Sharing options for primary calendars' to
'Share all information, but outsiders cannot change calendars' or higher. Note
that changing this option may take up to 24 hours to propagate to all users.