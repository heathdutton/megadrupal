
The uc_iats module integrates the IATS payment gateway (http://iats.ticketmaster.com) with Ubercart.

## INSTALL ##
Just place this module in your modules directory and turn it on.


## REQUIREMENTS ##
This module, obviously, requires Ubercart.

Also, this module requires the iATS PHP library. You can download it from http://home.iatspayments.com/developers/code-wrappers/php-install and follow the steps mentioned at their. Currently the process is already done for this module.

Added creditcard.php file to validate creditcard and get the creditcard type ex. "VISA".

The module looks for the library in its directory, but default as a directory named ''. So if you would like to set up with the least possible hassle, rename the library's directory to 'iATS' and place it the 'uc_iats' module directory. This should work with no extra configuration. If you would like to name the folder something else, you can set the path to the library on the uc_iats settings page (the same place where you set the iATS agent code and password).
