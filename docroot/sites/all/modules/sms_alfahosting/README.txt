Module: Alfahosting SMS Gateway
Author: Sergey Fayngold

DESCRIPTION
-------
This module enables the SMS Framework to sent SMS via Alfahosting SMS-Gateway

REQUIREMENTS
-------
- SMS Framework module
- An Alfahosting customer center account

INSTALL
-------
1) Install the SMS Framework module from http://drupal.org/project/smsframework
2) Drop this modul into a subdir of your choice (regarding the Drupal standards)
    for example sites/all/modules
3) enable the module under admin/build/modules
4) Configure the module under admin/smsframework/gateways/alfahosting
  a) Enter your "Alfahosting customer center" username
  b) Enter your API-Key (can be found here:
      https://alfahosting.de/kunden/index.php/Kundencenter:SMS/SMS-Gateway)
  c) Enter a sender identifier wich should be used
  d) Your settings will be validated when you click save.
     If you recive an error, it will hopefully give you futher instructions on
     what to do. If you got an error without instructions, compare the error
     code with the list in the Gateway documentation wich you can find in your
     Alfahosting customer center.
5) Use the SMS Framework module or any other module that uses it.

ROADMAP
-------
- Track critical errors in the "Status report"
