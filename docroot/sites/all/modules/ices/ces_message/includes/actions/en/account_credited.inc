<?php

/**
 * @file
 * English account credited template.
 *
 * @ingroup ces_message_template
 */

$subject = '[[exchange:shortname]] Your account [transaction:toaccount:name] has been credited';

$body = 'Your account [transaction:toaccount:name] has been credited.

Transaction details:

Seller: [transaction:toaccount:name] ([transaction:toaccount:user:fullname])
Buyer: [transaction:fromaccount:name] ([transaction:fromaccount:user:fullname])
Description: [transaction:concept]
Amount: [transaction:amount][transaction:fromaccount:exchange:currencysymbol]
Ordered by: [transaction:orderedby:fullname] < [transaction:orderedby:mail] >

View your current statement of account ([site:url]/ces/bank/account/transaction). If this credit is incorrect click on the sale description and edit the transaction to update it.

--
[exchange:shortname] administrator:
[exchange:admin:fullname]
[exchange:admin:mail]
[exchange:admin:mainphone]
';
