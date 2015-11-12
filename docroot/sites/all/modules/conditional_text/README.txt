Conditional Text module for Drupal

This module adds a text filter called "Conditional text" to your site. The
conditional text filter allows you to write content for your site that contains
sections that are displayed or hidden depending on certain conditions, allowing
you to write content that can be used in different situations or for different
audiences. For example, if you are writing software documentation, you might
want to write content explaining how to use a particular feature, and include
certain sections that apply to one version of the software, and other sections
that apply to another version. If you were displaying this documentation to a
user of one version, you would hide the inapplicable information from the other
version. This allows you to avoid duplicating the content (which makes it more
difficult to maintain) -- instead of writing a different manual for each
version, you write one version with conditional sections.

This module uses a plugin system to define the available conditions and display
options. Condition plugins allow you to define different types of conditional
sections in your content. Display plugins govern how the conditional content is
displayed. How this works:
a) When filtering content for display, the main module looks for sections in
your content with the syntax:
[condition TYPE DETAILS](conditional text here)[/condition]
where TYPE is the name of an installed condition plugin, and DETAILS are
specific to each condition type.
b) The condition plugin evaluates DETAILS to see whether the condition is
considered "true" or "false". This can depend on who is viewing the content,
what versions of what modules are enabled on your site, and other factors,
depending on the condition plugin.
c) The display plugin decides how to render the (conditional text here),
depending on whether the condition was true or false.

This module comes with several condition and display plugins (details below);
other modules may provide additional plugins (to write your own plugins, see the
conditional_text.api.php file).

--- Basic Usage ---

1. Install and enable the module.
2. Visit the Text formats page (Admin >> Configuration >> Content authoring >>
   Text filters, or admin/config/content/formats).
3. Edit an existing text format, or add a new format.
4. Check the "Conditional text" box, to enable this filter for your text format.
5. Configure custom options for condition and display plugins, if desired (see
   below for details).
6. When editing content using this text format, include conditional sections
   using the syntax:
   [condition TYPE DETAILS](conditional text here)[/condition]
   where TYPE is the condition plugin type, and DETAILS depends on which
   condition plugin is used (see below for details).

--- Condition Plugins --

Two condition plugins come with this module:

a) Custom values: Allows you to define groups of condition values on the text
format configuration page. For example, you might define an "experience" group,
with values "beginner", "intermediate", and "advanced". You can also assign
roles to the values -- checking a role box means that for users having that
role, the condition evaluates to true. For example, if you check the box for the
"anonymous user" role for the "beginner" value, and leave the other values
unchecked, when an anonymous site visitor is viewing content, "beginner"
conditions evaluate to true, while "intermediate" and "advanced" conditions
evaluate to false.

After setting up your custom groups of values, you can define conditional
sections using this syntax:
[condition custom GROUP VALUE]
For example, [condition custom experience beginner] would create a section
appropriate to beginners.

b) Modules enabled: Allows you to specify that certain text relates to a certain
Drupal module version being enabled on your site. This plugin does not require
any configuration on the Text formats page, but you can optionally add modules
to the plugin -- the plugin will behave as though these additional modules are
also enabled.

To add a module-enabled conditional section to your content, use the syntax:
[condition enabled MODULENAME] or [condition enabled MODULENAME OP VERSION]
Examples:
[condition enabled block] (displays if the Block module is enabled)
[condition enabled block > 7.1] (displays if the Block module, any version after
                                 7.1, is enabled)
[condition enabled drupal = 7.x] (displays if Drupal is version 7.0, 7.5, etc.)
[condition enabled conditional_text >= 7.x-1.2] (displays if Conditional Text,
                                                 any version 7.x-1.2 or later,
                                                 is enabled)

--- Display plugins ---

Four display plugins come with this module -- you can choose which to use on the
text filter configuration page.

a) Filter (Text is shown/hidden based on conditions): If a condition evaluates
to true, the text is shown. If a condition evaluates to false, the text is
removed from the display completely.

b) Fieldsets (User can show/hide text, closed by default): All conditional text
is displayed using a "fieldset", which allows the reader to show or hide the
text as desired. All fieldsets containing conditional text are closed when the
content is first displayed.

c) Fieldsets (User can show/hide text, open by default): Same as (b), except all
fieldsets are open when the content is first displayed.

d) Fieldsets (User can show/hide text, open/closed by default based on
conditions): Same as (b), except the fieldsets start out open for conditions
that evaluate to true, and closed for conditions that evaluate to false.
