ARTE OPA
========

Defines an API that enables modules to interact with the ARTE Open API (OPA).

The module supports retrieving data using the `videos` method. To retrieve
`broadcasts` data you need to separately enable the `arte_opa_broadcasts` module.
More methods will be supported soon.

Open API documentation can be found at https://github.com/ArteGEIE/API-DOC

## Configuration

You can configure the OPA access settings at /admin/config/services/opa/config.

You can get these keys from ARTE [upon request](https://github.com/ArteGEIE/API-OPA/blob/develop/app/Resources/doc/index.md#how-to-request-access-).

Alternatively, add this snippet to your `settings.php`

```php
$conf['arte_opa_client_id'] = 'CLIENT_ID';
$conf['arte_opa_client_secret'] = 'CLIENT_SECRET';
```

Give users the permission 'administer opa' or 'query opa' to change settings and use the query builder respectively.

To avoid making roundtrips to fetch an authentication token, you can also request a "never expire token". You can then configure this in the settings page.

## Example Query Builder

To demo OPA queries, the module has an interface for building OPA queries, for example to query all the videos of the CINEMA platform.

Find the query builder at /config/services/opa

## API usage

This module defines a PHP iterator class to iterate over remote resources such as videos, and provides support for pagination.

For example, to fetch all ARTEPLUS7 videos, you can use the following code.

```php
// Create the OPA connector. Make sure you have your OPA settings correctly set.
$opa = arte_opa_get_api();

// Create a video reader to iterate over remote videos. We also pass it the query parameters (platform, language and channel).
$reader = new OPAVideosReader($opa, array(
	'platform' => 'ARTEPLUS7',
	'language' => 'fr',
	'channel' => 'FR'
));

// Loop over the resources and store genrePresse in an array. Check the OPA documentation for the structure of the returned array.
$genres = array();
foreach ($reader as $key => $value) {
	$genres[] = $value['genrePresse'];
}

```

## Credits

Development by Marzee Labs, http://marzeelabs.org.

Sponsored by ARTE G.E.I.E.
