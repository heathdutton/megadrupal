<?php
/**
 * @file
 * English debit pending template.
 *
 * @ingroup ces_message_template
 */

$subject = '[[exchange:shortname]] Authorize transaction of [transaction:toaccount:user:fullname]';

$body = 'A new transaction to [transaction:toaccount:name] ([transaction:toaccount:user:fullname]) has been created and is awaiting your authorization to be applied. 

Please accept (or reject) this transaction following this link:

[site:url]/user/login?destination=ces/bank/transaction/[transaction:id]/accept

It is very important to authorize your pending payments very quickly for the good work of the social economic system.

Transaction details:

Seller: [transaction:toaccount:name] ([transaction:toaccount:user:fullname])
Buyer: [transaction:fromaccount:name] ([transaction:fromaccount:user:fullname])
Description: [transaction:concept]
Amount: [transaction:amount][transaction:fromaccount:exchange:currencysymbol]
Ordered by: [transaction:orderedby:fullname] < [transaction:orderedby:mail] >

--
[exchange:shortname] administrator:
[exchange:admin:fullname]
[exchange:admin:mail]
[exchange:admin:mainphone]
';
