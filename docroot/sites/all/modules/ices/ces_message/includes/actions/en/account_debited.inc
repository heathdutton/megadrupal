<?php

/**
 * @file
 * English account debited template.
 *
 * @ingroup ces_message_template
 */

$subject = '[[exchange:shortname]] Your account [transaction:fromaccount:name] has been debited';

$body = 'Your account [transaction:fromaccount:name] has been debited.

Transaction details:

Seller: [transaction:toaccount:name] ([transaction:toaccount:user:fullname])
Buyer: [transaction:fromaccount:name] ([transaction:fromaccount:user:fullname])
Description: [transaction:concept]
Amount: [transaction:amount][transaction:fromaccount:exchange:currencysymbol]
Ordered by: [transaction:orderedby:fullname] < [transaction:orderedby:mail] >

View your current statement of account ([site:url]/ces/bank/account/transaction). If this debit is incorrect contact the seller or the exchange administrator immediately to have it corrected.

Seller contact: [transaction:toaccount:user:fullname]
mail: [transaction:toaccount:user:mail]
phone: [transaction:toaccount:user:mainphone]

--
[exchange:shortname] administrator:
[exchange:admin:fullname]
[exchange:admin:mail]
[exchange:admin:mainphone]
';
