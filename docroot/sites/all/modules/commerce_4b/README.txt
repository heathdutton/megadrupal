Commerce 4B
===========

This module implements the Spanish 4B payment service [1] for use with Drupal Commerce [2].
You need to configure both the module and the payment service at the website 4B.

[1] http://www.4b.es/sistema-4b/members
[2] http://drupal.org/project/commerce

Configuration
=============

- Enable the module and configure the payment rule at admin/commerce/config/payment-methods
- You need to enter the store code ("Clave de comercio") you should have received by the bank.
- When going live, you need to deactivate the setting "Use testing platform".

4B Service configuration
========================

After signing up for the payment service from 4B, you will be given the following data:
  - Nombre de commercio
  - Clave de comercio (store code)
  - Clave de usuario y contraseña (username and password)

Login at https://tpv.4b.es/config, enter the username and the password, click "Configuración"
in the menu at the left, and enter the following URLs, replacing YOURDOMAINNAME

  URL que devuelve el desglose de la compra:
  YOURDOMAINNAME/commerce_4b/checkout_details

  URL que graba el resultado en la BD del comercio (TRANSACCIONES AUTORIZADAS):
  YOURDOMAINNAME/commerce_4b/payment_response

  URL que graba el resultado en la BD del comercio (TRANSACCIONES DENEGADAS):
  YOURDOMAINNAME/commerce_4b/payment_response

  URL de continuación posterior a la página de recibo
  YOURDOMAINNAME/commerce_4b/return

  URL de recibo (TRANSACCIÓN AUTORIZADA):
  YOURDOMAINNAME/commerce_4b/payment_response

  URL de recibo (TRANSACCIÓN DENEGADA):
  YOURDOMAINNAME/commerce_4b/payment_response


Contact
=======

Maintainer: 
- Peter Vanhee (pvhee) - http://drupal.org/user/108811
