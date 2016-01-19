<?php
/**
 * @file
 * English credit pending template.
 *
 * @ingroup ces_message_template
 */

$subject = '[[exchange:shortname]] Transaction waiting authoriztion';

$body = 'Dear [transaction:toaccount:user:fullname],
  
A new transaction has been created and is waiting to be manually authorized by [transaction:fromaccount:user:fullname].

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
