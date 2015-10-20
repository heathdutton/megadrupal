# Introduction

This module provides a stream wrapper to integrate Storage API with Drupal,
as an alternative to Storage API's core_bridge submodule.

The stream wrapper is tied to a Storage API class, and can be configured per
field instance or globally as the "Default download method" at
`admin/config/media/file-system`. Since it is a visible stream wrapper, it
integrates naturally with File entity (thus with media) and other modules.

This is an alternative to Storage API's core_bridge submodule.


# Populating your files to storage api

If you have a site where you've already uploaded files and want to start using
storage api, you may want to put your files under storage api control.

See https://www.drupal.org/project/storage_api_populate


# Migrating from `storage_core_bridge` module

In case you're already using storage api and core bridge submodule, you want to
change the «Upload destination» in the field settings.
In some cases, you can't do that via web because the field already has data.

You can run this php snippet to enforce changing upload destination scheme.
This is a safe change and doesn't affect already uploaded files.

```
$field_name = 'field_image';
$field = field_read_field($field_name);
$field['settings']['uri_scheme'] = 'storage-api';
field_update_field($field);
```


## Sanitization

`storage_core_bridge` creates incomplete entries for image styles in some
circumstances (when the derivative has never been generated). Such entries
have no storage file_id associated (don't confuse with core fid). This
leads to errors when propagating files on cron run.

Removing broken entries is enough. They will be correctly created whenever
the derivative image is generated.

To remove broken entries, run this query:

```
DELETE s, scb FROM storage_core_bridge scb LEFT JOIN storage s ON scb.storage_id = s.storage_id WHERE s.parent_id IS NOT NULL AND s.file_id IS NULL;
```

## Upgrade from `storage_core_bridge` NOT patched

This is done per field basis. Given a field named `field_image`,
the corresponding stream wrapper created by `storage_core_bridge` is
`storage-field-image`.

Queries below use `storage-field-image` as an example. Replace it to suit your
field names.

Those queries are idempotent, you can safely run them several times.


### Copy into `storage_stream_wrapper` all rows related to `storage-field-image` in `storage_core_bridge`

```
INSERT INTO storage_stream_wrapper SELECT scb.storage_id, REPLACE(scb.uri, 'storage-field-image', 'storage-api') FROM storage_core_bridge scb LEFT JOIN storage_stream_wrapper ssw ON scb.storage_id = ssw.storage_id WHERE ssw.storage_id IS NULL AND scb.uri LIKE 'storage-field-image%';
```


### Update uris in `storage`

This is done only for files that are in stream wrapper. We don't rely on
`storage-field-image` for comparisons, because derivative images are not tied
to the field name. We do a join with `storage_stream_wrapper` instead.

```
UPDATE storage s LEFT JOIN storage_stream_wrapper ssw ON s.storage_id = ssw.storage_id SET selector_id='storage_stream_wrapper' WHERE ssw.storage_id IS NOT NULL AND selector_id LIKE 'storage_core_bridge:%';
```


### Update uris in `file_managed`

```
UPDATE file_managed SET uri=REPLACE(uri, 'storage-field-image://', 'storage-api://') WHERE uri LIKE 'storage-field-image%';
```


## Upgrade from `storage_core_bridge` PATCHED

The patch for media compatibility creates a stream wrapper
named `storage-file-default-scheme`.

Queries to migrate to `storage_api_stream_wrapper`, are similar to the above ones:


### Copy into `storage_stream_wrapper` all `storage-file-default-scheme` rows from `storage_core_bridge`

```
INSERT INTO storage_stream_wrapper SELECT scb.storage_id, REPLACE(scb.uri, 'storage-file-default-scheme', 'storage-api') FROM storage_core_bridge scb LEFT JOIN storage_stream_wrapper ssw ON scb.storage_id = ssw.storage_id WHERE ssw.storage_id IS NULL AND scb.uri LIKE 'storage-file-default-scheme%';
```


### Update uris in `storage`

```
UPDATE storage s LEFT JOIN storage_stream_wrapper ssw ON s.storage_id = ssw.storage_id SET selector_id='storage_stream_wrapper' WHERE ssw.storage_id IS NOT NULL AND selector_id LIKE 'storage_core_bridge:%';
```


### Update uris in `file_managed`

```
UPDATE file_managed SET uri=REPLACE(uri, 'storage-file-default-scheme://', 'storage-api://') WHERE uri LIKE 'storage-file-default-scheme%';
```

