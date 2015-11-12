About
===========
This module integrates with the Acquia Lift suite and provides functionality allowing mapping of the "current" entity's fields/properties to UDF fields on the Acquia Lift Profiles page.

The configuration page at admin/config/content/personalize/entity_context allows you to create new contexts by drilling down from an Entity Type to an individual Field or Property in either a specific or all bundles (within the chosen Entity Type). After creating a new context, that context can be assigned to a UDF field on admin/config/content/personalize/acquia_lift_profiles.

The following field types are currently supported (and support for others can be added by implementing hook_entity_context_field_types()): boolean, date, entityreference, integer, list_boolean, text, text_formatted, text_with_summary, token, user, uri.

The "current" entity is defined as whatever is returned by menu_get_object().

If you use an entityreference field, you'll be able to further drill down to the referenced entity type/bundle and choose its own property/field as the final value for the context. Note that recursing down (ie. entityreference field -> entityreference field) is not currently supported, and the chosen entityreference field must only allow for entities from a single bundle (if entities from multiple bundles are allowed for the entityreference field, the entity_context module will attempt to work with the first selected bundle).

Important
===========
This module uses a string token ("entity_context_catchall") as a catch-all
to identify "all bundles within an entity type," and as such, this string
should not be used as a bundle name in any custom development.

