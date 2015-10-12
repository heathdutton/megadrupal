SUMMARY
  Feedmine is a Drupal feedback interface to Redmine, a rails-based
  ticket/issue management system. Feedmine provides a block that,
  when enabled, allows users to enter website feedback information
  which is submitted as a ticket into Redmine automatically along with
  user and session data.

LIMITATIONS:
  Feedmine utilizes the Redmine REST API and currently supports only
  Redmine v.1.1.0 or later.

ADDITIONAL INFORMATION:
  For a full description of the module, visit the project page:
    http://drupal.org/project/feedmine

  To submit bug reports and feature suggestions, or to track changes:
    http://drupal.org/project/issues/feedmine

  Module help:
  admin/help/feedmine

  Module config:
  admin/settings/feedmine/feedmine_settings

  For more information on Redmine, visit:
    http://www.redmine.org

  For information on the Redmine API, visit:
    http://www.redmine.org/projects/redmine/wiki/Rest_api

  For a more through implementation of the Redmine API for Drupal:
    http://drupal.org/project/redmine

INSTALLATION:
  NOTE:
    Before configuring Feedmine in Drupal site you must configure
    Redmine first. From within Redmine, check 'Enable REST API'
    locate at Administration -> Settings -> Authentication.

  * Download and enable the Feedmine module appropriate for your
    version of Drupal.

  * Configure the module settings in your Drupal site at:
    [admin/config/system/feedmine/feedmine_settings]
    >> Redmine URL:
        The URL to your redmine installation.
    >> Redmine API access key:
        You can find your Redmine API access key on your account page
        [/my/account]
        When logged into Redmine, it's on the right-hand pane of the
        default layout.  Click 'Show' under 'API access key'.
    >> Redmine Project:
        Use the Redmine project selector to pick a existing Redmine
        project where the issues will be created.
    >> Configure module permissions [admin/user/permissions#module-feedmine].
    >> Enable the 'Feedmine: Feedback submission' block.
