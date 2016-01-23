Ooyala Episode Example
======================

This is an example module showing one method of integrating custom metadata
into the Ooyala field. When this module is enabled, it will add an "episode"
column to all video fields and store that attribute in the backlot as a custom
metadata entry.

Saving custom metadata can be done by calling the Ooyala backlot API directly,
or by using the ooyala_api_custom_metadata_save() function. For simple
integration, that function is all that is needed to save data. Custom metadata
will be automatically loaded when a single video is loaded from the API, or
when the $load_custom_metadata parameter is TRUE when loading multiple videos
at once.

The vast majority of this module is to integrate the custom metadata as an
additional property on an Ooyala field. There are three main tasks to do this,
all of which are standard for any Field in Drupal:

 1. Alter the ooyala_upload FAPI #type to include the new field.
 2. Validate during form processing.
 3. Describing our additional column to the Field API and saving our data to
    the backlot when it is saved by the Field API.

Of special note is the hook_ooyala_columns() implementation. Drupal 7 doesn't
have an alter hook for hook_field_schema(), so we can't inject our column
specification. Without this, the Field Storage API won't create a column to
store the local data in. This step can be skipped, at the expense of not being
able to expose the episode to Views and other parts of Drupal.

