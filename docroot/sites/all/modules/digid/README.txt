********************************************************************
                     D R U P A L    M O D U L E
********************************************************************
Name:
  DigiD

Author:
  Rik van Dalen <r.vdalen at forceit dot nl>

********************************************************************
DESCRIPTION:

DigiD stands for Digital Identity and is a system shared between cooperating
governmental agencies (in the Netherlands), allowing to digitally authenticate
the identity of a person who applies for a transaction service via internet.

The DigiD Drupal module facilitate Drupal login through the DigiD CGI API.


********************************************************************
INSTALLATION:

1. Place the entire digid directory into your Drupal
   sites/all/modules directory.

2. Enable the DigiD module by navigating to:
     Administer > Site building > Modules

   When the module is enabled and the user has the "access administration
   pages" permission, a "DigiD Settings" menu should appear in the
   menu system under Administer -> Content management and Administer ->
   Site configuration.

********************************************************************
GETTING STARTED:

1. Set the correct values on the admin/settings/digid page.
   - DigiD URL: You can change the url of the DigiD server. Please verify this
   with the details you have received.

   - DigiD Application ID: You should set the application ID received from
   DigiD, this is private information and should not be given to others.

   - DigiD ID: You should set the ID of the DigiD Webservice.

   - DigiD Authentication Code: You should set the authentication code of this
   application.

2. At this point users can login on Drupal with their DigiD credentials. The
   Drupal login form will show two links, to login or to request new DigD
   credentials for new users.

3. To login with non-DigiD accounts, for example the Drupal Admin account, you
   can still login through /user?digid=0
