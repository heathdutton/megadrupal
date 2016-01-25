<?php

/**
 * @file
 * English new account template.
 *
 * @ingroup ces_message_template
 */

$subject = '[[exchange:shortname]] New account request [account:name]';

$body = 'Another account request for [exchange:name]. Go to the administrative interface at [site:url]/ces/admin/account to activate or discard it.

See registration details below:

Exchange: [exchange:code]
Account name: [account:name]
Account type: [account:kind]
User: [account:user:fullname]

--
IntegralCES
[site:mail]
';
