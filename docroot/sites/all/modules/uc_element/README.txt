INTRODUCTION
------------
The Ubercart Element Payment module allows the processing of credit cards
through Ubercart using Element's payment services platform. 
http://www.elementps.com/
This module allows for normal processing of credit cards, as well as the
ability for users to keep a card "on file" using Element's PASS token storage
system. The module will store the card through Element's services and keep
a local "token" to allow customers to charge their stored card without
storing the credit card locally. This module strives to meet PCI compliance.
  * For a full description of the module, visit the project page:
    http://drupal.org/project/uc_element
  * To submit bug reports and feature suggestions, or to track changes:
    http://drupal.org/project/issues/uc_element


REQUIREMENTS
------------
  * Installation of Ubercart module 
    http://drupal.org/project/ubercart
  * Existing Element Payment Services account from
    http://www.elementps.com/


INSTALLATION
------------
  * Requires the installation of the PHP SOAP Client.
    http://php.net/manual/en/class.soapclient.php.
    This can be installed on most Linux-based systems by running command
    yum install php-soap
    or
    sudo apt-get install php-soap
  * Install as usual, see 
    https://www.drupal.org/documentation/install/modules-themes/modules-7 
    for further information.


CONFIGURATION
-------------
  * Configure module settings in 
    Store » Configuration » Payment methods » Credit card
    - Choose the type of transaction (authorization only, or authorization 
      and charge)
    - Determine if you will allow customers to store their cards through
      Element
    - Determine if you would like to use Element's address verification 
      system

TROUBLESHOOTING
---------------
  * If you are having trouble charging or storing cards, verify that you 
    have the correct values set in the payment configuration page above. 
    URLs should end in "?WSDL" in order to function properly.

MAINTAINERS
-----------
Current maintainers:
  * Daniel Moberly (Daniel.Moberly) - http://drupal.org/user/1160788

This project has been sponsored by:
  * Antique Electronic Supply
    All the parts you need to modify, repair, or build guitars, guitar 
    amplifiers, antique radios  and more. Visit 
    http://www.tubesandmore.com/ for more information.
