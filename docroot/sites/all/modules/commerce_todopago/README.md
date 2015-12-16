# Commerce Todo Pago

This project integrates [Todo Pago][0] payment services into the
[Drupal Commerce][1] payment and checkout systems. Todo Pago provides off-site
payment, so no payment information is collected by the module. After
confirmation the user wil be redirected to the Todo Pago website.

The module implements a queue to check the status of the pending payments in 
case there is a communication error when the user returns to the site after
the payment.

## Installation

First download and enable the module as usual.

Then you need to get the [Todo Pago PHP SDK][2]. Download it and place it in one
of the possible libraries directories (i.e. sites/all/libraries) and it will be
detected. If you use [drush make][3] (you should) it will be automatically
downloaded using the included commerce\_todopago.make.


[0]: http://www.todopago.com.ar                  "Todo Pago"
[1]: https://www.drupal.org/project/commerce     "Drupal Commerce"
[2]: https://github.com/TodoPago/sdk-php         "Todo Pago PHP SDK"
[3]: http://drushcommands.com/drush-7x/make/make "drush make"
