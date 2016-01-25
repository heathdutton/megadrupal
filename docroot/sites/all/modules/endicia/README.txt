# README file for Grindflow Endicia Cloud

What does Grindflow Endicia Cloud do?
--------------------
This module integrates the label printing services provided by Grindflow Endicia
Cloud (GEC) to your Drupal sites, allowing to you generate and print postage
labels from any computer.

You may also be interested in modules that further integrate GEC to your Drupal
e-commerce installations to streamline your workflow:
* For Ubercart: Download & install uc_endicia
* More coming soon!


Requirements
------------
Please keep in mind that while the Grindflow Endicia Cloud modules are free of
charge, a paid monthly subscription is required in order to print any labels.

It's easy! Sign up for an account today: http://grindflow.com/usps-grindflow-endicia-cloud-services

The requirements for GEC are:
* PHP 5.2 or greater
* A subscription to the Grindflow Endicia Cloud API service
* If using a ZPL/EPL "barcode" thermal printer, the jZebra printing module is
  also required.

The 7.x version of Grindflow Endicia Cloud depends on the Address Field module
and requires the patch in comment #49 of #970048 to be applied:
http://drupal.org/files/addressfield-element_info-970048-49.patch

This issue is expected to be resolved shortly. If you need assistance applying
patches, visit the following page for instructions: http://drupal.org/patch/apply


Installation
------------
For detailed instructions on how to install modules to your Drupal site, please
visit: http://drupal.org/documentation/install/modules-themes

1. Download the latest Address Field release from http://drupal.org/project/addressfield
   and extract it to your site modules folder. Until issue #970048 is resolved,
   a patch to addressfield is required to define the form element. At this time,
   the most recent patch is available in comment #71. Direct link to patch:
   https://drupal.org/files/addressfield-element_info-970048-71.patch.

   For details applying patches, visit https://drupal.org/patch/apply.

2. Download & extract Grindflow Endicia Cloud to your site modules folder.

3. Visit "Administer > Site Building > Modules" and enable the Endicia *and*
   the Endicia UI module.

4. Optionally, if you are going to use thermal barcode printers, download and
   install the jZebra module from http://drupal.org/project/jzebra.


Configuration
-------------
For more resources on how to configure your site for Grindflow Endicia Cloud,
visit the support documentation:
http://www.grindflow.com/grindflow-endicia-cloud-services/documentation/configuring-account-settings

1. Login to your Grindflow account

2. Click the Endicia tab on your profile

3. Copy your Authorization Token to the clipboard

4. Visit yoursite.com/admin/endicia/account/settings and fill in:
  * Your Endicia account ID (9 digits);
  * Your software passphrase;
  * Your API token from step #3 (it is recommended you paste from clipboard to
    avoid errors retyping it)

5. Visit yoursite.com/admin/config/media/file-system and configure both the
   public *and* private filesystem paths. If unsure, use site/default/files and
   site/default/files/private respectively.

6. Credit your account and start printing labels!


Credits and Sponsors
--------------------
Development of this module was performed by Grindflow Management LLC,
http://www.grindflow.com.

If you require technical assistance, please contact support@grindflow.com.
