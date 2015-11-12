<?php
/**
 * @file
 * English credit pending template.
 *
 * @ingroup ces_message_template
 */

$subject = '[[exchange:shortname]] Transacción esperando autoritzación';

$body = 'Estimado/a [transaction:toaccount:user:fullname],
  
Se ha creado una nueva transacción que está esperando ser autorizada manualmente por [transaction:fromaccount:user:fullname] pera hacer-se efectiva. Quando sea autorizada se hará el traspaso entre las cuentas.

Detalles de la transacción:

Vendedor/a: [transaction:toaccount:name] ([transaction:toaccount:user:fullname])
Comprador/a: [transaction:fromaccount:name] ([transaction:fromaccount:user:fullname])
Descripción: [transaction:concept]
Cantidad: [transaction:amount][transaction:fromaccount:exchange:currencysymbol]
Entrada por: [transaction:orderedby:fullname] < [transaction:orderedby:mail] >

--
Administrador/a de [exchange:shortname]:
[exchange:admin:fullname]
[exchange:admin:mail]
[exchange:admin:mainphone]
';
