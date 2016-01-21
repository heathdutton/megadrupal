The Nudity module strips all wrappers and labels from body fields.

To apply the naked style to other fields simply implement
hook_preprocess_field() from your module or theme and add the theme hook
suggestion as per nudity_preprocess_field().

For example:

function MYTHEME_preprocess_field(&$variables) {
  if ($variables['element']['#field_name'] == 'field_tags') {

    // Add the theme hook suggestion with low priority.
    array_unshift($variables['theme_hook_suggestions'], 'field_naked');

    // Add the theme hook suggestion with high priority.
    $variables['theme_hook_suggestions'][] = 'field_naked';

  }
}

To apply the naked style to all fields enable the Nudist Colony submodule. Use
with caution; none of your fields will have wrappers or labels. This would
essentially be the same as overriding theme_field() in your theme.

In future I may decide to add a setting to fields/field instances/formatters to
apply the naked style to other fields as well. Please create an issue if this
would be useful to you.
