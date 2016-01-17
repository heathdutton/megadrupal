<?php
/**
 * @file
 * English debit rejected template.
 *
 * @ingroup ces_message_template
 */

$subject = '[[exchange:shortname]] Transaction rejected';

$body = 'Dear [transaction:toaccount:user:fullname],
  
A transaction from your account has been rejected so your account will NOT be debited.

Transaction details:

Seller: [transaction:toaccount:name] ([transaction:toaccount:user:fullname])
Buyer: [transaction:fromaccount:name] ([transaction:fromaccount:user:fullname])
Description: [transaction:concept]
Amount: [transaction:amount][transaction:fromaccount:exchange:currencysymbol]
Ordered by: [transaction:orderedby:fullname] < [transaction:orderedby:mail] >

If this rejection is an error please log in to your account and accept the transaction following this link: 

[site:url]/user/login?destination=ces/bank/transaction/[transaction:id]/accept

--
[exchange:shortname] administrator:
[exchange:admin:fullname]
[exchange:admin:mail]
[exchange:admin:mainphone]
';
