<?php
/**
 * @file
 * English credit rejected template.
 *
 * @ingroup ces_message_template
 */

$subject = '[[exchange:shortname]] Transaction rejected';

$body = 'Dear [transaction:toaccount:user:fullname],
  
A transaction to your account has been rejected so your account will NOT be credited.

Transaction details:

Seller: [transaction:toaccount:name] ([transaction:toaccount:user:fullname])
Buyer: [transaction:fromaccount:name] ([transaction:fromaccount:user:fullname])
Description: [transaction:concept]
Amount: [transaction:amount][transaction:fromaccount:exchange:currencysymbol]
Ordered by: [transaction:orderedby:fullname] < [transaction:orderedby:mail] >

If this transaction should be accepted, contact the buyer [transaction:toaccount:user:fullname] < [transaction:fromaccount:user:mail] > so she/he accept the transaction or contact the exchange admnistrator if necessary.

--
[exchange:shortname] administrator:
[exchange:admin:fullname]
[exchange:admin:mail]
[exchange:admin:mainphone]
';
