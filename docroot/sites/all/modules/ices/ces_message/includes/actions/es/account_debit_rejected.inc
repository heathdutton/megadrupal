<?php
/**
 * @file
 * English debit rejected template.
 *
 * @ingroup ces_message_template
 */

$subject = '[[exchange:shortname]] Transacción rechazada';

$body = 'Estimatdo/a [transaction:toaccount:user:fullname],
  
Una transacción desde tu cuenta ha estado rechazada y por tanto tu cuenta NO será cargada.

Detalles de la transacción:

Vendedor/a: [transaction:toaccount:name] ([transaction:toaccount:user:fullname])
Comprador/a: [transaction:fromaccount:name] ([transaction:fromaccount:user:fullname])
Descripción: [transaction:concept]
Cantidad: [transaction:amount][transaction:fromaccount:exchange:currencysymbol]
Entrada por: [transaction:orderedby:fullname] < [transaction:orderedby:mail] >

Si este rechazo es un error, entra a tu cuenta y acepta la transacción siguiendo este enlace: 

[site:url]/user/login?destination=ces/bank/transaction/[transaction:id]/accept

--
Administrador/a de [exchange:shortname]:
[exchange:admin:fullname]
[exchange:admin:mail]
[exchange:admin:mainphone]
';
