<?php

/**
 * @file
 * English account debited template.
 *
 * @ingroup ces_message_template
 */

$subject = '[[exchange:shortname]] El teu compte [transaction:fromaccount:name] ha estat carregat';

$body = 'Estimat/da [transaction:fromaccount:user:fullname]

El teu compte [transaction:fromaccount:name] ha estat carregat.

Detalls de la transacció:

Venedor/a: [transaction:toaccount:name] ([transaction:toaccount:user:fullname])
Comprador/a: [transaction:fromaccount:name] ([transaction:fromaccount:user:fullname])
Descripció: [transaction:concept]
Quantitat: [transaction:amount][transaction:fromaccount:exchange:currencysymbol]
Entrada per: [transaction:orderedby:fullname] < [transaction:orderedby:mail] >

Mira el teu Estat del compte ([site:url]/ces/bank/account/transaction). Si la transacció és errònia, contacta immediatament amb el/la venedor/a o si és necessari amb l\'administrador/a de la xarxa d\'intercanvi per a resoldre-ho.

Venedor/a: [transaction:toaccount:user:fullname]
Email: [transaction:toaccount:user:mail]
Telèfon: [transaction:toaccount:user:mainphone]

--
Administrador/a de [exchange:shortname]:
[exchange:admin:fullname]
[exchange:admin:mail]
[exchange:admin:mainphone]
';
