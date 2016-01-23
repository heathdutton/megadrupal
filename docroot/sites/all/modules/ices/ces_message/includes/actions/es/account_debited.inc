<?php

/**
 * @file
 * English account debited template.
 *
 * @ingroup ces_message_template
 */

$subject = '[[exchange:shortname]] Tu cuenta [transaction:fromaccount:name] ha sido cargada';

$body = 'Estimado/a [transaction:fromaccount:user:fullname]

Tu cuenta [transaction:fromaccount:name] ha sido cargada.

Detalles de la transacción:

Vendedor/a: [transaction:toaccount:name] ([transaction:toaccount:user:fullname])
Comprador/a: [transaction:fromaccount:name] ([transaction:fromaccount:user:fullname])
Descripción: [transaction:concept]
Cantidad: [transaction:amount][transaction:fromaccount:exchange:currencysymbol]
Entrada por: [transaction:orderedby:fullname] < [transaction:orderedby:mail] >

Mira tu Estado de la cuenta ([site:url]/ces/bank/account/transaction). Si la transacción errónea, contacta inmediatamente con el/la venedor/a o si es necessario con el/la administrador/a de la red de intercambio para solucionarlo.

Vendedor/a: [transaction:toaccount:user:fullname]
Email: [transaction:toaccount:user:mail]
Teléfono: [transaction:toaccount:user:mainphone]

--
Administrador/a de [exchange:shortname]:
[exchange:admin:fullname]
[exchange:admin:mail]
[exchange:admin:mainphone]
';
