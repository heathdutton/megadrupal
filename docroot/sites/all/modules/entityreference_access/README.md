# Entity reference access

## Description

This module adds new field formatters for entityreference which check access
before displaying rendered entities. Views has access filters such as
'published', but entityreference rendered entity formatter only checks
`entity_access()` for the current user.


## Current formatters

-   Role

    Adds an access formatter for configurable role. This formatter is helpful if
    a user of one role wants to preview a list of referenced entities as another
    role without forfeiting their current role (ruling out solutions
    like Masquerade). For example, for an editor to use SPS preview mode, they
    must retain their current role.

## Dependencies

- Entity reference

## To do

1. Make a patch to invoke something like `hook_field_formatter_view_alter()` or
    `hook_field_formatter_prepare_view_alter()`. With one of those, we could
    then add our setting to the existing 'entityreference_entity_view' formatter
    with `hook_field_formatter_info_alter()`,
    `hook_field_formatter_settings_form_alter()`, and
    `hook_field_formatter_settings_summary_alter()`.
