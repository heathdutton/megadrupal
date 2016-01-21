
-- SUMMARY --

The Webform Confirmation Module sends a mail to the end users who
fills in the webform for some information. It basically picks up the
mail id of the user and sends a mail.

For a full description of the module, visit the project page:
  http://drupal.org/project/webform_confirmation

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/webform_confirmation


-- REQUIREMENTS --

* Views : http://drupal.org/project/views
* Webform : http://drupal.org/project/webform


-- INSTALLATION --

* Download Webform Confirmation, Views and Webform module.

-- CONFIGURATION --

* Setting up the Configuration

  - Go to Admin >> Config >> webform-confirmation
    a. Select the Webform for which you wish to apply this confirmation.
    b. If you wish to send the email in Short form then click on the
       "Shorten the URL" checkbox.
    c. Fill in the bit.ly credentials.

  - Go to Admin >> Config >> webform-confirmation >> Template
    a. Fill in the Email address from which you wish to send mail.
    b. Fill in the Subject of the mail.
    c. Fill in the Starting and Ending body of the mail being sent.

  - Go to Admin >> Config >> webform-confirmation >> Search
    a. In order to search for some particular submission.

  - Go to Admin >> People >> Permissions
    a. Scroll down to "Webform Confirmation"
    b. Check on for the permission "Email Configuration for webform" for
    letting user see the configuration of the module.
    c. Check on for the permission "Verification request" for
    letting user see the verification link.

-- CONTACT --

Current maintainers:

* Nitesh Sethia - https://drupal.org/user/2474982

The development of this module is sponsored by QED42.
