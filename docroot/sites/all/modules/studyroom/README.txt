
-- SUMMARY --

Studyroom is a family of modules to create an online system by which
users can reserve administrator-defined assets of an organization for
set periods of time. Users can create, edit, and delete their own
reservations, as well as peruse a gallery of all assets that can be
reserved at a given location, view calendars of all reservations, and
report issues.

Its primary audience is universities and libraries.

The module make use of Drupal 7 entities, using the Entity API module
to do most of the heavy lifting.

-- REQUIREMENTS -

Drupal version 7.14 or greater.

Entity_API > 1.2 (patched)
https://drupal.org/node/2123923
This patch allows the bundle fields to show up under the "manage display" tab
on the locations and reservations types.


-- PHP REQUIREMENTS --

The use of the DateInterval class means that we require PHP version >5.3.

-- INSTALLATION --

- Activate all the Studyroom modules and their dependencies.
- Visit admin/studyroom/locations create a location type (e.g. Estrella Hall).
- Visit admin/studyroom/spaces and create spaces (e.g. Room 1, Lounge 3).

-- CONTACT --

Current maintainers:
* Scott Worthington - http://drupal.org/user/325316

-- SPONSORS --

This project has been sponsored by:
Estrella Mountain Community College
http://estrellamountain.edu/
