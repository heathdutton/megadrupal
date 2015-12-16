Introduction
============

This is not your typical theme. It is not focused on style and markup
overrides. It's goal is to make maintaining a theme easier. It adds
functionality in the form of extra hooks, granular include files, and .info
extras. With all of these, the theme should also need fewer .tpl file overrides
and simplify the process.

The philosophy behind the project is that every project is new and one should
not be shoehorned into any single framework or structure. Instead, you should
be anabled to go any direction needed to accomplish your goals.


Expanding on the .info
======================

The ability to add css frameworks and Internet Explorer conditional styles via
a theme's .info file is baked into Pure. It is not necessary to call 
drupal_add_css() to get these files added or in the proper position. Another
functionality included is to declare include directories. More on this later,
but it will allow for structuring your php overrides.

For frameworks like grid systems and resets, simply add a "framework
stylesheets" entry to your sub-theme's .info like in the example below:

framework stylesheets[all][] = styles/reset.css

In this example, the reset.css file will be loaded at the top of the css stack
and thus be one of the first to load. You can load multiple frameworks in one
theme. They will simply load in the order you place them in the .info.

Internet Explorer is a little more complicated, but still quite simple to
understand. It just has one extra layer of nesting for declaring the versions
of IE you are targetting. The following example will add styles for any version
of IE under 8.

conditional stylesheets[lt ie 8][all][] = styles/ie-lt-8.css

All of these stylesheets will be loaded last in the stack in the order they
are declared. This will assure they can properly override what they are
supposed to override.

Something new is the idea of include directories. There is some functionality
in Drupal to allow modules to set files to be included via a .info. This is
only for individual files and works nicely for the focused nature of modules.
Themes are a bit of a different beast in that they will potentially contain
logic to override a bevy of different modules. The standard method of doing
this is to drop it all into your theme's template.php. In large sites this can
become rather unwieldy fast.

To make things simpler, you can declare directories in your theme that contain
multiple include files. Pure will recursively look through any directories in
these to find any .inc files within. The system then logs them to a cache so
the process is not performed every page load.

As an example, the following will add files contained in the overrides
directory to the system:

include directories[] = overrides


Form & field theming improvements
=================================

There are a couple of things added to Pure to make form and field theming less
painful and more flexible. These are the only cases where a template override
has been added to this theme.

It was found that theme_field() and theme_form() could not see preprocess and
process functions related to them. This was only resolved with adding tpl
overrides. The bonus to this is that it allows for some of the logic in the
theme functions to be moved to preprocess where it could be manipulated. Now
you can add extra theme hook suggestions to your hearts content to these items.

The other issue with form theming is there were previously 2 ways to alter the
output of a form: declare a new theme function for the form or use a
hook_form_alter(). The theme hook tends to bloat your hook_theme() function if
there are a lot of forms on the site and make separation of logic difficult.
The latter is an alter hook and, although allowed, should be used sparingly.
Alters increase the chances of breaking the functionality of a site since you
are tapping into module level logic.

To get around these issues a more general theme_form_content() function was
added. In addition, to make use of preprocess and process functions easier, it
is implemented via a tpl file. It is even loaded with at least one theme hook
suggestion: theme_form_content__FORMID().


Preprocess and process functions without extra templates
========================================================

Sometimes you want to use a preprocess or process function for a theme hook
suggestion without needing to put the related template file in your theme. One
instance of this is if you just want to add some attribute to the untouched
markup structure. Adding in a tpl file for this instance just adds duplicate
code and makes things harder to maintain.

Pure will make sure that the preprocess and process functions for every theme
hook suggestion is called. So do not worry about if the tpl is available, but
just add the function and revel in the bliss of a simple template struture.


hook_suggestions()
==================

One source fo failure of making sure that all of the preprocess and process
functions are called is that it all has to be done in the Pure theme itself.
This means that any additions to $variables['theme_hook_suggestions'] in a
preprocess function will not be recognized reliably. To work around this little
issue the hook_suggestions() system was created.

The naming convention for this is the same as any preprocess or process
function. For example, if you wanted to declare a new suggestion based on the
author something like the following would be used:

mytheme_suggestions_node($variables, $hook, $suggestions) {
  $suggestions[] = 'node__' . $variables['node']->author->uid;
  return $suggestions;
}


Expanded alter functions
========================

As much as it is suggested against, sometimes one has to use and alter function
in a theme. In a couple cases the hooks available has been expanded to get
allow for more separation of function.

First up is hook_block_view_alter(). The only available extention to this in
Drupal core is hook_block_view_MODULE_BLOCK_alter(). This was sometimes a bit
too specific when altering a lot of similar blocks coming from the same module.
Pure will also call hook_block_view_MODULE_alter() jsut for those instances.

On the opposite end of the spectrum there is hook_page_alter(). This is the only
variant available. In order to provide a more granular division of code,
hook_page_CALLBACK_alter() is available for any page with a valid callback
function attached to the page definition.
