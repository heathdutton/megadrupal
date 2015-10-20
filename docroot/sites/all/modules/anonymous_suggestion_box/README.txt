CONTENTS OF THIS FILE
---------------------
* Introduction
* Requirements
* Recommended modules
* Installation
* Configuration
* Maintainers

INTRODUCTION
------------
A simple module to collect Anonymous feedback from users and provide a formatted
email to a settable email address.

REQUIREMENTS
------------
No addition contrib modules required

Recommended modules
-------------------
* Views (https://drupal.org/project/views)
  When enabled, display if submissions saved to database is improved.
* CAPTCHA (https://www.drupal.org/project/captcha)
  Helps prevent spam submissions. Important if submission permissions are
  available to anonymous users.

INSTALLATION
------------
* Install as you would normally install a contributed drupal module. See:
  https://drupal.org/documentation/install/modultes-themes/modules-7
  for further information.

CONFIGURATION
-------------
* Configure user permissions in Administration » People » Permissions:
  - Submit anonymous suggestions
    Users in roles with the "Submit anonymous suggestions" will be able to
    submit the anonymous suggestion box form. If assigned to anonymous users you
    may want to enable the CAPTCHA module to prevent spam bot submissions.
  - View anonymous suggestion box submissions
    Allows users with this permission to view any suggestions that have been
    saved to the database (if enabled).
  - admin anonymous suggestion box
    Allows users with this permission to modify module settings.
* Configure module settings in
  Administration » Configuration » System  » Anonymous Suggestionbox
  - Select whethere new submissions should be saved to the database, emailed or
    both.
  - Specify to and from email addresses. These default to the site mail address.
  - Modify the "Form Instructions Text" if desired to present information to
    users filling in the submission form.
* Configure the CAPTCHA Module if desired to protect from spam submissions.
  - Add the anonymous_suggestion_box form_id to the Form Protection section
    of the Gerneral Settings section for the CAPTCHA module.
  - User form_id "anonymous_suggestion_box_form" and select a challenge type.
  - Alternately modules such as Mollom can be used to to protect this form
    from spam submissions. See that modules documentation on adding a form.

MAINTAINERS
-----------
Current maintainers:
* Benjamin Townsend (benjaminarthurt) - https://drupal.org/user/2501220
