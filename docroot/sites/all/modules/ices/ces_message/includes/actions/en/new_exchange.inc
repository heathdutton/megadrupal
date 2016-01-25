<?php

/**
 * @file
 * English new exchange template.
 *
 * @ingroup ces_message_template
 */

$subject = '[Integral CES] New Exchange [exchange:code] - [exchange:shortname] request';

$body = 'A new exchange has been registered.

Visit the CES administrative interface at [site:url]/admin/ces to activate or discard it. See some details below:

Code: [exchange:code]
Short name: [exchange:shortname]
Full name: [exchange:name]
Country: [exchange:country]
Location: [exchange:town] ([exchange:region])
Web site: [exchange:website]
Currency: [exchange:currencysymbol] ( [exchange:currencyname] ), value [exchange:currencyvalue]
User: [exchange:admin:name] < [exchange:admin:mail] >

IntegralCES
[site:mail]
';
