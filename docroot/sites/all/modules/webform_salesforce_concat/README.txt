********************************************************************
D R U P A L    M O D U L E
********************************************************************
Name: Salesforce Webform Concatenate Module
Author: Robert Castelo
Sponsor: Code Positive [www.codepositive.com]
Drupal: 7.0.x
********************************************************************
DESCRIPTION:

The Webform Salesforce Concatenate module adds various features for generating a Salesforce Lead from a Drupal Webform submission, such as concatenating specific Webform fields into the 'Description' Salesforce field.

The Lead is generated via the Salesforce Web-to-Lead service, with the Webform module providing the customisable form on the Drupal site, and the Salesforce Web-to-Lead Webform Data Integration module converting the submission into a Salesforce Web-to-Lead. The Webform Salesforce Concatenate module adds some useful features to this process.

Features include:

* Webform Fields To Salesforce Description Field

Some fields of a Webform don't map to a Salesforce field, but it's useful to include their info in the Lead. This option enables you to add the info to the Salesforce Description field as [field label]: [field value].

* Webform Field Options To Salesforce Description Field

Set specific Webform select field options to be concatenated into the 'Description' Salesforce field.

*Use Select Field Labels As Salesforce Keys

If Salesforce select fields are set up with keys and values being the same, use this feature to automatically use values as keys when sending the form to Salesforce. This feature can save a lot of time when setting up a form.

*Set Site Wide Lead Source

Automatically sets the Lead Source on new Webforms and hides the option.

* Report Salesforce Integration Status

On the Report page checks the status of connection with Salesforce, checking the OID, Salesforce URL, and availability of Salesforce.


********************************************************************
PREREQUISITES:

  Salesforce Web-to-Lead Webform Data Integration module
  Webform moduleSalesforce Web-to-Lead Webform Data Integration module


********************************************************************
INSTALLATION:

Note: It is assumed that you have Drupal up and running.  Be sure to
check the Drupal web site if you need assistance.

1. Place the entire webform_salesforce_concat directory into your Drupal directory:
   sites/all/modules/


2. Enable the Webform Salesforce Concat modules by navigating to:

   administer > modules

  Click the 'Save configuration' button at the bottom to commit your
  changes.


********************************************************************
AUTHOR CONTACT

- Report Bugs/Request Features:
   http://drupal.org/project/webform_salesforce_concat

- Comission New Features:
   http://drupal.org/user/3555/contact

- Want To Say Thank You:
   http://www.amazon.com/gp/registry/O6JKRQEQ774F


********************************************************************
SPONSORS

Sage Pay
http://www.sagepay.co.uk



