-- SUMMARY --

Bendy is a feature package that creates a responsive Featured content 
rotating banner using the Views Slideshow and Client-side adaptive 
images modules.

For a full description of the module, visit the project page:

http://drupal.org/project/bendy

To submit bug reports and feature suggestions, or to track changes:

http://drupal.org/node/add/project-issue/bendy


-- REQUIREMENTS --

Modules:

Features
http://drupal.org/project/features

Views
http://www.drupal.org/project/views

Views Slideshow
http://drupal.org/project/views_slideshow

Views Slideshow: Cycle
A submodule included with the Views Slideshow module

Client-side adaptive image
http://drupal.org/project/cs_adaptive_image

Chaos tools
http://www.drupal.org/project/ctools

Libraries API
http://drupal.org/project/libraries

Libraries:
jQuery cycle plugin
http://malsup.com/jquery/cycle/download.html

json2.js (Required for jQuery Cycle Custom Options)
https://github.com/douglascrockford/JSON-js


-- INSTALLATION --

1) Download all required modules and place them in your modules folder
(usually /sites/all/modules).

2) Download jQuery cycle plugin at http://malsup.com/jquery/cycle/download.html
and place it in sites/all/libraries
(should look like /sites/all/libraries/jquery.cycle

3) Download json2.js at https://github.com/douglascrockford/JSON-js
and place it in sites/all/libraries
(should look like /sites/all/libraries/json2).

4) Enable module dependencies and Bendy at admin/modules

5) Optional: Enable Views UI. This will be required to customize your
view.

6) Place the newly created block 'View: Bendy: Featured content rotating banner'
is a region

7) The module creates a new content type, called 'Featured content'. You
can add content by navigating to node/add/bendy-featured-content


-- CONFIGURATION --

* Configure user permissions in Administration > People > Permissions
(/admin/people/permissions).

Featured content: Create new content
Featured content: Edit own content
Featured content: Edit any content
Featured content: Delete own content
Featured content: Delete any content
Administer features: Perform administration tasks on features.
Manage features: View, enable and disable features.

*Optional (if Views UI is enabled)
Administer views: Access the views administration pages.
Bypass views access control: Bypass access control when accessing views.


-- CUSTOMIZATION --

*To customize the Bendy View Block:

1) Navigate to Structure > Views

2) Click 'edit' on Bendy featured content rotating banner

3) Make customizations (See FAQ below for common customizations/settings)

Please note that any of these customizations will override the default
settings provided by this module and will store your customizations
in the database. To undo your changes, click 'revert' on the view and
it will return to it's original state.


-- TROUBLESHOOTING --

none.


-- FAQ --

Q: How do I add a new field to the view?
1) Edit the view

2) Under 'Fields' click add

3) Select the field you want to add

4) Click Apply

5) Save your view


Q: How do I change the transition effect?
1) Edit the view

2) Under 'Format' > 'Slideshow', click 'Settings'

3) Under 'Cycle options' > 'Transition' > 'Effect'

4) Select desired transition

5) Click Apply

6) Save your view

Q: How do I change the timing on the rotation?
1) Edit the view

2) Under 'Format' > 'Slideshow', click 'Settings'

3) Under 'Cycle options' > 'Transition' > 'Effect'

4) Select 'View Transition Advanced Options'

5) In the 'Timer delay' add your time in milliseconds 

5) Click Apply

6) Save your view

Q: How do I change the adaptive image sizes?
1) Go to admin/config/media/image-styles

2) Click 'edit' on your desired syle

3) Click 'override defaults'

4) Under 'Scale and crop', click edit

5) Set your dimensions in px

6) Click 'update effect'

7) Flsuh the old styles by clicking 'Update style'
(Note: your new image style with not show until you do this)

Q: How do change the device breakpoints for the adaptive images?
1) Edit the view

2) Under 'Fields' click 'Content: Image'

3) Scroll to 'Client width breakpoint'

4) Set your breakpoint in px based on the max width of the viewport

4) Click Apply

5) Save your view


-- CONTACT --

Current maintainers:
*Nico Zdunich (nicoz) - http://drupal.org/user/693674

This project has been sponsored by:
*Pixel Sweatshop - http://www.pixelsweatshop.com
