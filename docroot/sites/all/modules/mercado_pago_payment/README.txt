CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requeriments
 * COnfiguration

INTRODUCTION
------------
This project implements Mercado Pago payment method for Drupal Commerce.


REQUIREMENTS
------------
1. Drupal commerce module
2. Drupal commerce Payment submodule enable
3. Libraries API
4. The MercadoPago SDK-PHP library

INSTALLATION
------------
Download the latest version of Mercado Pago SDK for PHP 
from https://github.com/mercadopago/sdk-php

Upload it to your library drupal installation, 
typically sites/all/libraries/. Rename the uncompress
mercado pago sdk folder as mp_sdk. Make sure that the 
file libraries/mp_sdk/lib/mercadopago.php is there, 
with that exact structure.

Then, continue the installation of in the usual way.


CONFIGURATION
-------------
Adjust your notifications URL on your Mercado Pago account 
to http://<yourdomain>/mercado_pago_payment_sdk/ipn

    Argentina: https://www.mercadopago.com/mla/herramientas/notificaciones
    Brazil: https://www.mercadopago.com/mlb/ferramentas/notificacoes
    Mexico: https://www.mercadopago.com/mlm/herramientas/notificaciones
    Venezuela: https://www.mercadopago.com/mlv/herramientas/notificaciones

Configure the module

    a) Go to store > configuration > payment methods
    b) On the MercadoPago SDK options, click edit
    c) On the Actions, click edit
    c) Fill the form with the corresponding values of 
       your MercadoPago account. Your can found the valuer here:
        Argentina: https://www.mercadopago.com/mla/herramientas/aplicaciones
        Brazil: https://www.mercadopago.com/mlb/ferramentas/aplicacoes
        Mexico: https://www.mercadopago.com/mlm/herramientas/aplicaciones
        Venezuela: https://www.mercadopago.com/mlv/herramientas/aplicaciones
    d) Make sure the rule is enabled and click Save

Status

    * You can set your Mercado Pago Account parameter
    * You can receives IPN notifications of accredited 
      payments from Mercado Pago
    * The payment interface is in a modalbox. So, the user
      never leaves your site.
