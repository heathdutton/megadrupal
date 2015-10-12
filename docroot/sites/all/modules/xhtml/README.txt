HOMEPAGE

  http://drupal.org/project/xhtml

DESCRIPTION

  The purpose of the XHTML module is to send content to the browser
  with application/xhtml+xml. This will, for many browsers, turn on
  more strict validation and error checking. This helps especially
  during module development, where valid XHTML is a goal.

INSTALLATION

  Simply enable the module. It will override the `html' template (as
  long as the theme in use hasn't overridden that template). If the
  theme currently in use overrides the `html' template, that theme
  must be fixed to have the following code at its beginnning:

    <?php if (!empty($xml_shebang)) echo $xml_shebang; ?>

  Everything else is handled automatically through drupal's
  extensibility and hooks systems.

CONFIGURATION

  Visit admin/settings/xhtml to configure xhtml. The configuration
  options provide ways to conditionally disable XHTML output. Built-in
  support is included for examining a client's Accept: header and
  checking which page the user is visiting. (The method used to check
  the Accept: header is only tested on apache and probably doesn't
  work on other httpds, please report a bug if having trouble with
  this feature on another webserver!) It is possible to make delivery
  as XHTML conditional on arbitrary parameters using the hooks
  described in API (if you are willing to write a Drupal module).

API

  Please see xhtml.api.inc to hook_ stubs with Doxygen comments
  describing how to implement these hooks.

REFERENCES

  http://www.w3.org/TR/2002/NOTE-xhtml-media-types-20020801/#application-xhtml-xml

BUGS

  Please report bugs to drupal's issue tracking system,
  http://drupal.org/node/add/project-issue/xhtml

AUTHORS

  Nathan Phillip Brink (ohnobinki)
    http://drupal.org/user/829476
    freenode: binki
    http://ohnopub.net/
