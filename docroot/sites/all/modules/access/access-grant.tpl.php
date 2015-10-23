<?php
/**
 * @file
 * Default theme implementation to present an access grant.
 *
 * This template is used when viewing an access grant page,
 * e.g., example.com/admin/access/grant/123, with "123" being the grant ID.
 *
 * Use render($ack) to print all information pertaining to the grant, or print a
 * subset such as render($ack['user']). Always call render($ack) at the end in
 * order to print all remaining items.
 *
 * Available variables:
 * - $ack: An array of items describing the grant, such as the user and role
 *   assignments. Use render() to print them.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values are:
 *   - access-grant: The current template type, i.e., "theming hook".
 *   - access-grant-[scheme]: The current access scheme's machine name. For
 *     example, if the grant is part of the "Sections" scheme, it would result
 *     in "access-grant-sections". Note that the machine name will often be a
 *     short form of the human readable label.
 * Other variables:
 * - $access_grant: Full grant object. Contains data that may not be safe.
 * - $access_field: The name of the access control kit realm field. Depends on
 *   the grant's scheme; e.g., if the grant is part of the "Sections" scheme,
 *   the value would be "ack_sections".
 * - $ack_realms: The access realm field's raw values. Use this instead of the
 *   normal field variable for the realm field (see "Field variables" below),
 *   because the actual name of the field varies per scheme.
 * Field variables: For each field instance attached to the access grant, a
 *   corresponding variable is defined; e.g., $access_grant->field_example has a
 *   variable $field_example defined. When needing to access a field's raw
 *   values, developers/themers are strongly encouraged to use these variables.
 *   Otherwise, they will have to explicitly specify the desired field language,
 *   e.g., $access_grant->field_example['en'], thus overriding any language
 *   negotiation rule that was previously applied.
 *
 * @see template_preprocess_access_grant()
 */
?>
<div id="access-grant-<?php print $access_grant->gid; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php print render($ack); ?>
</div>
