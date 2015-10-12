DESCRIPTION
===========
Feedback & Suggestion Tab adds a tab to the viewport, next to the scrollbar, which solicits feedback and suggestions from website visitors. GetSatisfaction.com, UserVoice.com and others provide this feature and are the inspiration for this module. The webpage in the modal dialog that appears after clicking the tab is configurable, but only pages in an <iframe> are supported for now. Contributions for AHAH content are encouraged.

INSTALL AND CONFIGURE
=====================
   Copy the 'feedbacktab' folder to your modules directory (probably sites/all/modules).
   Go to Administer > Site building > Modules and enable the module.
   Edit your site's settings.php file (probably sites/default/settings.php) to set the URI for the <iframe>.
   Add the following line to your settings.php file.
      $conf['feedbacktab_iframe_uri'] = 'http://example.com/';
   Change http://example.com to be the URL of your feedback or survey page. This could be an internal or external site. For example if your survey is hosted on an external site
      $conf['feedbacktab_iframe_uri'] = 'http://yourdomainnamehere.com/path/surveypage';
   Or if your survey is hosted on your internal Drupal site
      $conf['feedbacktab_iframe_uri'] = '/path/surveypage';


UPGRADING
=========
Standard module upgrade procedures: http://drupal.org/node/250790

TODO
=====
- make iframe URI configurable
- make position of the feedback button configurable