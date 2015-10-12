<?php

/**
 * @file
 * English account credited template.
 *
 * @ingroup ces_message_template
 */

$subject = '[[exchange:shortname]] Tu cuenta [transaction:toaccount:name] ha sido abonada';

$body = 'Estimado/a [transaction:toaccount:user:fullname],

Tu cuenta [transaction:toaccount:name] ha sido abonada.

Detalles de la transacción:

Vendedor/a: [transaction:toaccount:name] ([transaction:toaccount:user:fullname])
Comprador/a: [transaction:fromaccount:name] ([transaction:fromaccount:user:fullname])
Descripción: [transaction:concept]
Cantidad: [transaction:amount][transaction:fromaccount:exchange:currencysymbol]
Entrada por: [transaction:orderedby:fullname] < [transaction:orderedby:mail] >

Mira tu Estado de la cuenta ([site:url]/ces/bank/account/transaction). Si este abono es incorrecto, clica en la descripción de la venta y edita la transacción para solucionarlo.

--
Administrador/a de [exchange:shortname]:
[exchange:admin:fullname]
[exchange:admin:mail]
[exchange:admin:mainphone]
';
