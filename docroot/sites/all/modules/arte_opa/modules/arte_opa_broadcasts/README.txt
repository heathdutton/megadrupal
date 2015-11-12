ARTE OPA Broadcasts
===================

Provides synchronisation with the OPA Broadcasts API.

## API usage

Similar to `arte_opa`, this module defines a PHP iterator class to iterate over
the `broadcasts` remote resource and provides support for pagination.

For example, to fetch all french broadcasts with program data, you can use the
following code:

```php
// Create the OPA connector. Make sure you have your OPA settings correctly set.
$opa = arte_opa_get_api();

// Create a broadcasts reader to iterate over remote broadcasts.
// We pass it the query parameters for language and sorting and also specifiy to
// include 'programs' data in the result set.
$reader = new OPABroadcastsReader($opa, array(
  'language' => 'fr',
  'sort' => 'AIRDATE_DESC',
  'include' => 'programs',
));

// Loop over the resources and sort the broadcast data by program ID and language
// Check the OPA documentation for the structure of the returned array.
$broadcasts = array();
foreach ($reader as $key => $value) {
  $broadcasts[$data['programId']][$lang] = $data;
}

```

## Automatic synchronisation via drush

An automatic synchronisation of broadcasts data can be set up utilizing the
`opa-broadcasts-sync` (obs) drush command. After the broadcast data has been
fetched from OPA, `hook_arte_opa_broadcast_sync` gets triggered allowing other
modules to carry out the necessary synchronisation of broadcasts in the system.

The query used for this automatic synchronisation can be altered by modules
that would like to do so by using `hook_arte_opa_broadcasts_sync_params_alter`.


## Credits

Development by Marzee Labs, http://marzeelabs.org.

Sponsored by ARTE G.E.I.E.
