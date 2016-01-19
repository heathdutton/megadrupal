This module provides a gateway between the french payment solution CM-CIC, also
called Cybermut, and Drupal Commerce module.
It works for the following banks:

- CIC
- Crédit Mutuel
- OBC

Installation and configuration:

- Download module from drupal.org
- Uncompress it into your module directory
- Enable the module
- Go to Store -> Configuration -> Payment ... And edit the rules with your own
datas. You can choose if payment gateway need to be enable for test or 
production website.
- Enable the rules if it doesn't.

TODO :

The module provides an interface based on the payment kit given by the bank.
It would be take into account another payment kit located, for instance, in the
"sites/libraries" folder.

This module is sponsored by Addvista.
