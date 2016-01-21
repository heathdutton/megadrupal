Slick Views Module
================================================================================

This adds a new display style to views called "Slick carousel". Similar to how
you select "HTML List" or "Unformatted List" as display styles.

This module doesn't require Views UI to be enabled but it is required if you
want to configure your Views display using Slick carousel through the web
interface. This ensures you can leave Views UI off once everything is setup.


OPTIONSETS:
--------------------------------------------------------------------------------

Arm yourself with proper optionsets. To create one, go to:

"admin/config/media/slick"


USAGE:
--------------------------------------------------------------------------------
Go to Views UI "admin/structure/views", add a new view, and a block.

Usage #1
--------
Displaying multiple (rendered) entities for the slides.
- Choose "Slick carousel" under the Format.
- Choose available optionsets you have created at "admin/config/media/slick"
- Choose "Rendered entity" or "Content" under "Show" under "Format", and its
  View mode.

Themeing is related to their own entity display outside the Views UI.

Example use case:
- Blogs, teams, testimonials, case studies sliders, etc.

Usage #2
--------
Displaying multiple entities using selective fields for the slides.
- Choose "Slick carousel" under the Format.
- Choose available optionsets you have created at "admin/config/media/slick"
- Choose "Fields" under "Show" under "Format".
- Add fields, and do custom works or markups. If having a multi-value Image
  field, recommended to only display 1.

Themeing is all yours inside the Views UI.

Example use case:
- similar as above.

Usage #3
--------
Displaying a single multiple-value field in a single entity display for the
slides. Use it either with contextual filter by NID, or filter criteria by NID.
- Under Pager", choose "Display a specified number of items" with "1 item".
- Choose "Unformatted list" under the Format, not "Slick carousel".
- Add a multi-value Image, Media or Field collection field.
- Click the field under the Fields, choose "Slick carousel" under Formatter.
- Adjust the settings.
- Be sure to Display "all" or any number > 1 under "Multiple Field settings".
- Check "Use field template" under "Style settings", otherwise no field visible.

Themeing is mostly taken care of by slick_fields.module in terms of layout, with
the goodness of Views to provide better markups manually.

Example use case:
- front or inner individual slideshow based on the entity ID, or individual user
  slideshow.


Usage #4
--------
A combination of (#1 or #2) and #3 to build nested slicks.

Example use case:
- A home slideshow containing multiple videos per slide for quick overview.
- A large product/ portfolio slideshow containing a grid of slides.
- A news slideshow containing latest related news items per slide.


GOTCHAS:
--------------------------------------------------------------------------------
If you are choosing a single multi-value field (such as images, Media files, or
Field collection fields) rather displaying various fields from multiple nodes,
make sure to:
- Choose an "Unformatted list" Format, not "Slick carousel".
- Choose "Slick carousel" for the actual field when configuring the field.
- Check "Use field template" under "Style Settings"so that the Slick field
  themeing is picked-up.

More info relevant to each option is available at their form display by hovering
over them, and click a dark question mark.


AUTHOR/MAINTAINER/CREDITS
--------------------------------------------------------------------------------
Gaus Surahman
https://drupal.org/user/159062

With the help from the community:
- https://www.drupal.org/node/2232779/committers
- https://www.drupal.org/node/2497045/committers
- CHANGELOG.txt for helpful souls with their patches, suggestions and reports.

READ MORE
--------------------------------------------------------------------------------
See the project page on drupal.org: http://drupal.org/project/slick.

More info relevant to each option is available at their form display by hovering
over them, and click a dark question mark.

See the Slick docs at:
- http://kenwheeler.github.io/slick/
- https://github.com/kenwheeler/slick/
