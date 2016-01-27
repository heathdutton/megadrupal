Fool's Rules

Finally! Long gone are the days of creating and maintaining a dummy module just
for the sake of holding line after line of bundle, form or page alters!

What's included?


RULES ACTIONS
-------------
- Alter bundle: Modify any value, for any property, on any bundle!
- Alter form: Easily hide elements, create new ones, and so much more!
- Alter page: Override the entire look and feel of the site within seconds!


RULES EVENTS
------------
- Bundle is viewed.
- Form is viewed.
- Page is viewed.


USAGE
-----
Fool's Rules adds functionality to the Rules module for users with the
permission: 'administer fools rules'.

Some features of this module require minor knowledge of how Drupal processes
data including renderable arrays and their key names for the properties to be
set. For more information see the requirements section.


PERFORMANCE
-----------
No formal performance testing has been completed, however, some data is
available using microtiming and Rules Debug.

Performance for an bundle, form or page load with 10 alters (averaged):
- In a custom module using hooks: 2.5 milliseconds.
- Fool's Rules alter actions: 10.5 milliseconds.

From this very limited data, it shows Fool's Rules to be about 5x less
performant than a custom module. However, for most sites the trade off might be
appropriate considering the ease of use and unlimited potential of using other
Rules events, conditions and actions.


REQUIREMENTS
------------
The following information is required to alter properties:

Bundle:
- Bundle name
- Property to modify

Form:
- Form name
- Property to modify

Page:
- Property to modify

Tip: The Devel module's 'Display $page array' setting can be helpful for finding
the appropriate properties.


TODO
----
- Add Rules condition to check if a property exists and has a specified value.
  For example: #view_mode = 'teaser'.
  - Bundle property has value.
  - Form property has value.
  - Page property has value.

- Add a Fools Rules block that shows available bundles, forms and regions
  on the current page.

- Add #post_render callback to Alter actions. Replace, prepend, or append any
  element with your own tokenized HTML!
