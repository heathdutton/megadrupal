<?php
/**
 * @file
 * English credit rejected template.
 *
 * @ingroup ces_message_template
 */

$subject = '[[exchange:shortname]] Transacció rebutjada';

$body = 'Estimat/da [transaction:toaccount:user:fullname],
  
Una transacció cap al teu compte ha estat rebutjada i per tant NO rebràs l\'import al teu compte.

Detalls de la transacció:

Venedor/a: [transaction:toaccount:name] ([transaction:toaccount:user:fullname])
Comprador/a: [transaction:fromaccount:name] ([transaction:fromaccount:user:fullname])
Descripció: [transaction:concept]
Quantitat: [transaction:amount][transaction:fromaccount:exchange:currencysymbol]
Entrada per: [transaction:orderedby:fullname] < [transaction:orderedby:mail] >

Si aquesta transacció hauria d\'haver estat acceptada, contacta amb el/la comprador/a [transaction:toaccount:user:fullname] < [transaction:fromaccount:user:mail] > per a que accepti la transacció, o contacta l\'administrador/a si hi ha cap problema.

--
Administrador/a de [exchange:shortname]:
[exchange:admin:fullname]
[exchange:admin:mail]
[exchange:admin:mainphone]
';
