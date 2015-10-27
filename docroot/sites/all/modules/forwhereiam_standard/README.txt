INTRODUCTION
============

This forWhereiAm Standard module creates a widget for displaying all latest
announcements made by your organisation (and any of its associated branches)
for a given user's location. The module interacts with forWhereiAm APIs and 
implements the server-side flow (unlike the forWhereiAm Basic module which
implements the client-side flow and has limited features).

For further information about the forWhereiAm API, visit:
  https://support.forwhereiam.com/categories/20084421-API

For a full description of the module, visit the project page:
  http://drupal.org/project/forwhereiam___standard

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/forwhereiam___standard


REQUIREMENTS
============

You must have an Enterprise Account with forWhereiAm. Only through this account
will you be able to gain a valid Client ID along with a Client secret, which 
is subsequently required to configure this module correctly.

Please contact forWhereiAm support team if you wish to register via:
    info@forwhereiam.com


INSTALLATION
============

* Install as usual. For further information, see: 
    http://drupal.org/documentation/install/modules-themes/modules-7


CONFIGURATION
=============

* Configure the module settings by going to:
    admin/config/services/forwhereiam___standard/settings

  - Client ID *REQUIRED*

    To use this module, specifying a client ID is compulsory. You can obtain it
    from within your forWhereiAm Central Control Panel (you must have Enterprise
    Account with forWhereiAm to gain access to this area).

    The control panel is accessible from: https://enterprise.forwhereiam.com

    For further information, please refer to:
    https://support.forwhereiam.com/entries
                              /23277873-How-to-get-started-with-API-Flow-choices


  - Client Secret *REQUIRED*

    Specifying a client secret is also compulsory. You can obtain it
    from within your forWhereiAm Central Control Panel (you must have Enterprise
    Account with forWhereiAm to gain access to this area). You must
    ensure you use the Client Secret which is associated with the Client ID
    you use above.


  - Height of block (in pixels)

    You can optionally specify a custom height for the main area of module which 
    displays the announcement content. The pixel unit is assumed and should
    not be entered directly into the form. The default height is 280 pixels. 

  - Refresh interval (in seconds)

    You can optionally specify how frequently the module should check for latest 
    announcements. The default frequency is every 60 seconds (60 seconds is the 
    minimum interval allowed). You can change this to any frequency above
    60 seconds. E.g. 300 if you want to only check for updates every 5 minutes.

  - Show 'guess my postcode' link on the module

    Using this checkbox, you can optionally show or hide the 'guess my postcode' 
    link from the module. This link uses HTML5 geolocation facility to guess
    the user's current location.

  - Content to display on initial screen

    In this textarea, you may enter any html markup or plain text. This will be 
    rendered on the empty space given at the very start screen. E.g.
    "
    <h2>Local Info For You!</h2>
    <br/><br/>
    <p>Enter your postcode above to see all relevant announcements for you.</p>
    "

  - Show maps

    Using this checkbox you can display an embedded Google map for announcements
    which have a location associated with them. If you select this option, then
    you must also provide a Google Maps v3 API key. This key can be obtained by 
    following the instructions given here:
    https://developers.google.com/maps/documentation/javascript/tutorial#api_key

  - Show AddThis social sharing buttons

    Using this checkbox you can display social sharing buttons for announcements
    which have their sharing option enabled. This module uses AddThis social
    sharing buttons. If you select this option, then you must also provide an
    AddThis Publisher Profile ID. This ID may be obtained by following the 
    instructions given here:
      http://www.addthis.com/settings/publisher

  - Show rating stars for announcements which have ratings enabled

    By selecting this option, you allow the module to show the rating stars for 
    announcements which have the ratings option enabled.

  - Enable alerts signup

    By ticking this checkbox, you will allow visitors of your site to signup
    for email alerts without leaving your site. The user will register with
    forWhereiAm and will subsequently be notified of any relevant 
    announcements you make via email almost instantaneously.
