<?php
/**
 * @file
 * English credit pending template.
 *
 * @ingroup ces_message_template
 */

$subject = '[[exchange:shortname]] Transacció esperant autorització';

$body = 'Estimat/da [transaction:toaccount:user:fullname],
  
S\'ha creat una nova transacció que està esperant ser autoritzada manualment per [transaction:fromaccount:user:fullname] per a fer-se efectiva. Quan sigui autoritzada es farà el traspàs entre els comptes.

Detalls de la transacció:

Venedor/a: [transaction:toaccount:name] ([transaction:toaccount:user:fullname])
Comprador/a: [transaction:fromaccount:name] ([transaction:fromaccount:user:fullname])
Descripció: [transaction:concept]
Quantitat: [transaction:amount][transaction:fromaccount:exchange:currencysymbol]
Entrada per: [transaction:orderedby:fullname] < [transaction:orderedby:mail] >

--
Administrador/a de [exchange:shortname]:
[exchange:admin:fullname]
[exchange:admin:mail]
[exchange:admin:mainphone]
';
