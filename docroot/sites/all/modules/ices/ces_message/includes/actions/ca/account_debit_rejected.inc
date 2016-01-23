<?php
/**
 * @file
 * English debit rejected template.
 *
 * @ingroup ces_message_template
 */

$subject = '[[exchange:shortname]] Transaction rejected';

$body = 'Estimat/da [transaction:toaccount:user:fullname],
  
Una transacció des el teu compte ha estat rebutjada i per tant el teu compte NO serà carregat.

Detalls de la transacció:

Venedor/a: [transaction:toaccount:name] ([transaction:toaccount:user:fullname])
Comprador/a: [transaction:fromaccount:name] ([transaction:fromaccount:user:fullname])
Descripció: [transaction:concept]
Quantitat: [transaction:amount][transaction:fromaccount:exchange:currencysymbol]
Entrada per: [transaction:orderedby:fullname] < [transaction:orderedby:mail] >

Si aquest rebuig és un error, entra al teu compte i accepta la transacció seguint aquest enllaç:

[site:url]/user/login?destination=ces/bank/transaction/[transaction:id]/accept

--
Administrador/a de [exchange:shortname]:
[exchange:admin:fullname]
[exchange:admin:mail]
[exchange:admin:mainphone]
';
