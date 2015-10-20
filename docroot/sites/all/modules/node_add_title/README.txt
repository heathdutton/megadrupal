This module allows you to configure the title of the Node Add pages.

The default title of the page where users create new nodes is set by Drupal in a
dynamic way that not always looks good. By default Drupal concatenates the word
Create and the name of the content type. The result could be something like 
"Create Basic page" or "Create article". This is not always acceptable.

With this little module you can replace Drupal's default title with the title of
your choice.

-- DEPENDENCIES --

None.

-- INSTALLATION --

Install as usual, see http://drupal.org/node/70151 for further information.

-- CONFIGURATION --

Configure at: [Your Site]admin/structure/types/[your-content-type]
...or: Administration > Structure > Content types > [Your content type]

You will see two new fields:

* Node Add page title
* Node Edit page title

Use these fields to override the title of the new node creation page and the
node editing pages for the particular content type.

-- PERMISSIONS --

In order to use this module you need the "Administer content types" permission.

-- CONTACT --

Current maintainers:
* Juan Carlos Martinez (jcmartinez) - http://drupal.org/user/275152
