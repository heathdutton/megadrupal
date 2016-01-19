Polaris Auto Registration module was created by Mark Pruitt, Mark Jarrell, and Isovera for Richland Library.

The polaris_auto_register module is designed to allow library customers 14 or older to register for a library card online.

When the module is installed, a table named polaris_barcodes is added to the database.

The table has only three fields.  A primary key integer field, a string field to record barcode numbers, and a datetime field to record the
Unix timestamp when a barcode is created.

A seed value of 99999901122345 is set for the barcode value.  This value can be adjusted in the polaris_auto_register.install file.

When a customer attempts to register, the highest value for barcode is pulled from the polaris_barcodes table, and the value is incremented by 1 and submitted to Polaris.  If Polaris returns a $result['status'] == FALSE and a PAPIErrorCode of -3505, it means the barcode we're trying to assignis already in the system.

In this case, a do/while loop in the submit function will continue to increment the value of the barcode by 1 and resubmit for up to 100 times (the value of the counter can be adjusted in the submit function in the polaris_auto_register/inc/polaris_auto_register.page.inc file).

If before the counter hits 100 $result['status'] == TRUE, the barcode will be timestamped and added to the
polaris_barcodes table, and the customer will be sent to the Thank You page where they can print the barcode.

If the counter hits 100 before $result['status'] == TRUE, a form error is set and the customer is asked to see a staff member.

DEPENDENCIES:

The polaris_auto_register module requires the following dependent modules:

1.  Smarty Streets is used for address verification and to retrieve the county of the registering customer.  Users will need to register with the vendor (http://smartystreets.com) for a token to use in the smartystreets module.  Smarty Streets generously offers free registration to non-profit organizations like libraries.  See the link https://smartystreets.com/free-address-verification.

2.  Libraries module (http://drupal.org/project/libraries).

3.  Barcode Generator for PHP library (v5.1 recommended) (http://www.barcodephp.com/en/download). This should be downloaded and installed in a directory called "barcodegen" within this "libraries" module directory (i.e., /sites/all/libraries/barcodegen).

4.  PHP Mailer library (v5.1 recommended) (http://sourceforge.net/projects/phpmailer/). This should be downloaded and installed in a directory called "phpmailer" within this "libraries" directory (i.e., /sites/all/libraries/phpmailer).

NECESSARY SETUP STEPS:

1.  You will need to change any links that point to your current online registration page to point to http://mysite.com/polaris_auto_register

2.  Make sure that the directory polaris_auto_register/tmp in the module is writable by the web server

3.  To use the last four digits of the primary phone number as the default PIN, on the admin/config/content/polaris_auto_register_settings page in the "USE LAST 4 PHONE DIGITS AS PIN" frame check the control "Use last four digits of primary phone number as default pin".

    If this option is not checked, a password field appears on the application form, and the user is required to choose a four digit PIN.

4.  Set the Polaris API Default Settings:

    Logon Branch ID - set to 1
    Logon User ID - set to 1
    Logon Workstation ID - set to 1
    Patron Branch ID - set to 3 (for Main branch) - this value sets the location of the branch where the library card was created.

5.  Set the CUSTOM MARKUP PLACED BEFORE THE REGISTRATION FORM:

    These settings create the information that will be displayed on the registration page above the form.

    Page Subtitle - if you wish to have a subtitle on the page, set it here wrapped in <h2></h2> tags.
    Page Mark Up 1
    Page Mark Up 2
    Page Mark Up 3 - these three fields are essentially the same.  Add full html text in these fields.  Any markup will be displayed above the form

6.  Set the CUSTOM MARKUP PLACED FOR THE THANK YOU PAGE:

    These settings create the information that will be displayed on the Thank You page

    Thank You Page Subtitle - if you wish to have a subtitle on the page, set it here wrapped in <h2></h2> tags.
    Thank You Page Mark Up 1 and Thank You Page Mark Up 2 - these two fields are essentially the same.  Add full html text in these fields.  Any markup
      entered here will be displayed above the library card image
    Thank You Page Mark Up 3 - Add full html text in this field.  Any markup entered here will be displayed below the library card image.

7.  Set the CUSTOM MARKUP FOR THE EMAIL MESSAGE

    Email Message Markup 1 - Use full html to enter the text that will appear above the barcode number, pin number and library card image.
    Email Message Markup 2 - Use full html to enter the text that will appear after the library card image.

8.  Optional fields - Polaris System Defined Field

    Polaris allows you to enter up to five custom fields.  Polaris calls the fields User1, User2, User3, User4, and User5.

    For each field you want to use:
      1.  Enter a question/label for the field
      2.  Enter a description for the information you want to collect

    Each entry can be up to 50 characters long.

    When a record is added to Polaris, the fields will be reported as User1, User2, User3, User4, and User5.

9.  PRINTING THE NEW LIBRARY CARD

    On the Thank You page at the top right and under the library card image there are print buttons.  The card looks small on the Thank You page, but
    will print normal size.

    OPTIONAL SETUP STEP:

    When the module is first installed, the seed value for the barcode is set to 99999901122345.

    At the time you submit a registration request, the highest seed value for the barcode is pulled from the database, and the value
    incremented by 1.  This new value is submitted to Polaris.

    If Polaris returns a status of false (no registration), the module will search the watchdog table for an entry in the variables column
    with the value of the barcode submitted and the error code -3505.  If it finds one, it means that the barcode has already been
    assigned.

    In this case, the barcode will be incremented by one, and the process repeated until a barcode can be registered with the customer's
    information.  Currently there is a 100 try limit, which can be adjusted in the polaris_auto_register.install file.

    Depending on how many times this needs to happen, it may appear to take some time before a customer is sent to the Thank You
    page.

    If you wish, you can either adjust the seed value in the polaris_auto_register.install file, or you can adjust the value of the seed
    barcode in the polaris_barcodes table in the database.

10.  VIEW REPORTS (number of online registrations by month)

    6/16/2014 new feature added to show online registrations by year/month for the last 24 months.

    The page is available at admin/reports/online_registration or from the Reports Menu.

    In polaris_auto_register.module, line 61, access argument for viewing the report is set to "View site reports".
    If needed, you can change the permissions need on that line.

Support
-------
If you experience a problem with the module, file a
request or issue on the issue queue (http://drupal.org/project/issues/polaris_auto_register).
DO NOT POST IN THE FORUMS. Posting in the issue queue is a direct line of
communication with the module author.
