CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Functional vs. Object Oriented API


INTRODUCTION
------------

Current Maintainer: David Numan <dave@gv.ca>

"Trello is a Web based project management application from Fog Creek Software
that can also be synced in real time with a Smartphone app."
 - http://en.wikipedia.org/wiki/Trello

The goal of this module is to provide an API for developers to interface with
Trello and supply reasonable related submodules.


INSTALLATION
------------

 1. Download and enable the Trello module. You will likely also want to enable
    the trello_api_explorer sub-module.
 2. Go to the Configuration - Trello page: admin/config/services/trello/conf
 3. Follow the link at the top of that form to get your Trello API key (opening
    in a new window would be convenient for copy and pasting), then save your
    API Key and OAuth Secret.
 4. At this point your Drupal site can only read from public boards. To be able
    to read and write your Trello data, click the link at the bottom of the form
    to go to the Trello Authentication page: trello/auth
 5. Select a link with an expiration of your choice: 1 day, 30 days, or never.
    After allowing your site you should be able to use the API explorer to view
    your Trello data.


FUNCTIONAL VS. OBJECT ORIENTED API
----------------------------------
The API provides both functional and OOP API for the developers preference. As
Trello modules develop further the API may also grow to include new features and
hopefully mature into a more solid design.
