
Virtual Field
-------------

This is an API module. You only need to enable it if a module depends
on it, or you are interested in using it for development.

A virtual field is a field without storage. It can be handy for
implementing fields that get their value from an external service, or
otherwise programmatically. Drupal already allows for this, however,
the default storage backend always creates tables for all created
fields, and core does not have a non-editing widget.

This module provides an non-storing backend and a non-editing widget,
and modifies the appropriate forms to make it easy to add virtual
fields to entities.

Usage
-----

There's two ways of implementing a virtual field, which depends on your specific requirements.

The simplest way is to provide the virtual_field options in
hook_field_info() and implement a hook_field_load() for your
field. The hook_field_load() should populate the $items array to set
the value of the field. The value will be cached by the field module,
and from here on it behaves just like a regular field and the value
might be formatted using standard formatters.

If caching the values is inappropriate for your needs, you can use the
alternative implementation method, where you return a dummy value in
hook_field_load(), and fetch/generate the field value directly in a
custom formatter. This means that the field is updated whenever
viewed. Note that if you don't provide some dummy value in
hook_field_load(), Field API will think that the field is empty and
not run the formatters.

See virtual_field.api.php for setup example.
