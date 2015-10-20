
CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Usage
 * Reports
 * Security
 * Limitations
 * Custom Form Handling
 * Contact

INTRODUCTION
------------

Form Clerk is a semi-automated way to input data into a Drupal form.

It may be useful for development, testing, migration, and updating sites.

It is used by administrators or developers, not site visitors.

Form Clerk is intended to be used by non-programmers. It does not require
any PHP programming or database knowledge.

Form Clerk is used to input data on a remote Drupal site. In other words,
you install Form Clerk on one Drupal site and use it to input data on
other Drupal sites, if you are an authorized user on those sites.

Visit http://drupalformclerk.com for extensive documentation and examples.


REQUIREMENTS
------------

Your PHP environment must have 'Client URL Library', known as cURL, enabled.
Your PHP environment must have 'Document Object Model', known as DOM, enabled.

No Drupal modules are required.


INSTALLATION
------------

Install as usual, see http://drupal.org/node/70151 for further information.


CONFIGURATION
-------------

1. Configure user permissions in Administer >> User management >> Permissions >>
   Form_clerk module:

   - use form_clerk

     Users in roles with the "use form_clerk" permission can use
     Form Clerk. Note: Usually, using Form Clerk involves logging in to other
     sites with a separate userid and password. The permissions of the userid
     on the remote system determine what forms can be filled out. Those
     permissions are controlled by the administrator of the remote system.

Recommended practices:
 Create a special Form Clerk user on the target system. This makes it easier
 to  track changes. Turn off the 'Administrative Overlay' for that user if
 it is in use (as it is by default in Drupal 7).

 Use admin/settings/form_clerk/configuration to set a top directory for input
 and report files, and set the Watchdog message setting.


USAGE
-----

Create one or more input files. This is a big topic. Visit
http://drupalformclerk.com for examples.
Navigate to admin/settings/form_clerk.
Enter the input files in the 'Input Files' field.
Enter the password for the remote system in the password field.
Click the 'Input Form Data' button.

There is also a 'Get Form Information' function that allows you see the
structure of a form in the same way that Form Clerk sees it. To use this,
navigate to admin/settings/form_clerk/get_info and provide the URL of the
page with the form you are interested in. If, as usual, you need to log in
to see the form, provide your login information.

REPORTS
-------

Form Clerk displays a brief report on the form page.

It also writes the HTML responses that cURL gets from the server into files.
By default these go into /tmp. You can change this at
admin/settings/form_clerk/configuration, or with the FC_report_dir
setting, for example FC_report_dir = /tmp/form_clerk

The filenames for these reports take the form 'report-n.html', where 'n' is
the sequence number of the report. The reports start with 1 every time you
run Form Clerk, so old reports are overwritten.

The files are HTML and can be viewed in a browser or an editor. They can help
you see what is happening when building an input file. The series of HTML
files is essentially what you would see in your browser (perhaps without CSS)
if you followed the same sequence of inputs and clicks.

If you don't want these reports generated, include 'FC_verbose = FALSE' near
the start of your input file.


SECURITY
--------
Form Clerk uses Drupal forms indirectly through the cURL interface in a way that
is very similar to directly entering the data in the form by yourseslf. As such,
it does not usually present any special security problems.

One exception to this is if you create Drupal users with Form Clerk. In this case,
the user passwords that you assign are present in the Form Clerk input file in plain text.
Therefore, these files should be carefully secured.


LIMITATIONS
-----------

Some form pages may be presented in a way that will not work with cURL.
For example, if the page uses AJAX to add a form to the original HTML,
cURL will not be able to read the form. In cases where this happens, you
may be able to access the form with another URL that will work with cURL.

For instance, with the Drupal 7 Overlay function, an administrator can add
new users at: http://example.com/node#overlay=admin/people/create
However, cURL cannot read the form on the page, even though you can.
But if you access the same form at
http://example.com/admin/people/create
cURL can read the form.


CUSTOM FORM HANDLING
--------------------

The form_clerk_custom_form() function is called during processing. You can
provide your own custom code here to do whatever you want to the form inputs.

One use case for this would be to make external calls to get current
data that is not available at the time the input file is created.


CONTACT
-------

Current maintainers:
  Joe Casey (Joe Casey) - http://drupal.org/user/823758
