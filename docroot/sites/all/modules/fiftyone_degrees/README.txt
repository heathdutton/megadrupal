
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Setting Up the Theme Switcher


INTRODUCTION
------------

Current Maintainer: 51Degrees.mobi <info@51Degrees.mobi>

This module provides a theme swticher with a comprehensive UI for changing theme
depending on what kind of device is accessing your Drupal site. This is
essential to optimise your site for the multitude of screen sizes and form
factors available today.

INSTALLATION
------------

No special care is needed when installing the module, do it as you would with
any other module.

SETTING UP THE THEME SWITCHER
-----------------------------

This module with create an admin page called '51Degrees.mobi'. This page will
list 'rules' that you have created, details what conditions a device must meet,
and then says if the rule should switch theme or change url (for redirection).

Multiple rules can be created for different purposes, such as using a different
theme for tablet and small screen devices. Note that rules are processed left to
right, so you should have your most specific rules to the left, going to most
generic on the right. If a device does not have a match, or is logged in as
admin, they go to the default theme.

Finally, a device can be excluded from the swticher if the have the session
variable 'NO_SWITCH' set to true. ie:

$_SESSION['NO_SWITCH'] = TRUE;	// this session will no longer be switched.

We'd love to hear any feedback or suggestion - info@51Degrees.mobi
