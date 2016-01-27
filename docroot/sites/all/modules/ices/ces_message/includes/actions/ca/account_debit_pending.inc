<?php
/**
 * @file
 * English debit pending template.
 *
 * @ingroup ces_message_template
 */

$subject = '[[exchange:shortname]] Autoritzar transacció de [transaction:toaccount:user:fullname]';

$body = 'Estimat/da [transaction:toaccount:user:fullname],

S\'ha creat una transacció cap a [transaction:toaccount:name] ([transaction:toaccount:user:fullname]) que espera la teva autorització per a poder ser aplicada. 

Si us plau accepta (o rebutja) aquesta transacció seguint l\'enllaç:

[site:url]/user/login?destination=ces/bank/transaction/[transaction:id]/accept

És molt important que autoritzis els teus pagaments pendents immediatament pel bon funcionament del sistema de moneda social. També pots afegir aquest compte a la llista blanca de comptes de confiança que no requereixen autorització.

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
