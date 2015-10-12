Content Language Access Module restricts the access of only contents with
language (except neutral language) that are equal of the actual Drupal
language being accessed or others that were previous configured in the admin
page.

This module helps when you have a content that needs to have access restriction
by Drupal language.

Example
------------
  Domains:
    www.example.com (EN_US)
    www.example.com.br (PT_BR)

  Contents:
    node/20 (EN_US)
    node/21 (PT_BR)
    node/22 (Language Neutral)

  Results:
    www.example.com/node/20 - response: 200 - OK
    www.example.com.br/node/20 - response: 403 - Access Denied

    www.example.com/node/21 - response: 403 - Access Denied
    www.example.com.br/node/21 - response: 200 - OK

    www.example.com/node/22 - response: 200 - OK
    www.example.com.br/node/22 - response: 200 - OK


Installation
------------
Just enable it. No configuration required.


Running multiple node access modules
-----------------------------------------------------------
This module rewrites the access callback of node/%nid URL, but if you already
have another module doing it, just let Content Language Access Module with a
higher weight in the system table on database and the other module will still
working.


Author
------
Rodrigo Elizeu Goncalves - https://drupal.org/user/725006
