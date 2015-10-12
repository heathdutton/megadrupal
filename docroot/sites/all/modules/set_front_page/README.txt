Set Front Page
--------------
Adds a new "Front page" tab to all node and taxonomy term pages, allowing a
quick way to assign the current page as the site's front page.

A "default" front page can also be assigned. The intended use case for this is
when the site maintainers wish to temporarily change the front page but have a
way of quickly reverting back to the "normal" front page.


Features
------------------------------------------------------------------------------
The primary features include:

* Allows any node or taxonomy term page to be quickly set as the homepage.

* Control which content types or vocabularies are suitable.

* Allow assignment of a 'default' front page.


Configuration
------------------------------------------------------------------------------
 1. On the People Permissions administration page ("Administer >> People
    >> Permissions") you need to assign:

    - The "Set front page" permission to the roles that are allowed to
      control which nodes or terms can be assigned as the front page.

 2. The main settings page controls which content types and vocabularies can be
    used as the front page:
      admin/config/system/set-front-page


Credits / Contact
------------------------------------------------------------------------------
Currently maintained by Damien McKenna [1] based on code originally written by
Jay Callicott [2].

Development is sponsored by Mediacurrent [3].

The best way to contact the authors is to submit an issue, be it a support
request, a feature request or a bug report, in the project issue queue:
  http://drupal.org/project/issues/set_front_page


References
------------------------------------------------------------------------------
1: https://www.drupal.org/u/damienmckenna
2: https://www.drupal.org/u/drupalninja99
3: http://www.mediacurrent.com/
