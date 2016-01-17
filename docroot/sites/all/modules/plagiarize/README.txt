README.TXT file for Plagiarize module.
================================================================================
DESCRIPTION
--------------------------------------------------------------------------------
Plagiarize is a content filter that allows authors with the proper permissions
to embed the summary of one piece of content into the body of another.

This gives the flexibility of specific placement of a summary within a node
body, and because it is a content filter, the body is rendered before Drupal's
search indexer is called thus recording keywords within the teaser as belonging
to the node in which it is "embedded."

The filter uses shortcode replacement to substitute for another node's summary
and optionally can add a title, link which can be linked back to the original
node.

A separate shortcode can be added to create a plain list or anchored list of the
"embedded" nodes.


REQUIREMENTS
--------------------------------------------------------------------------------
This module does not require any additional modules to be enabled.


INSTALLATION
--------------------------------------------------------------------------------
1. Download the tar/zip file and unzip it into your contributed modules
   directory. (usually "/sites/all/modules")

2. In the modules interface, look in the "Other" section for "Plagiarize" and
   enable the module.

3. Go to the Text Format interface (admin/config/content/formats) and configure
   one of your formats.

4. Enable the filter "Plagiarize Shortcode Embedding" and configure the options
   at the bottom of the page.

NOTE: There are no individual permissions for this filter in the Permissions
      interface so any role that is allowed to use this Text Format will be able
      to use the plagiarize shortcodes.


USAGE
--------------------------------------------------------------------------------
Before you can put a plagiarize shortcode into a node body, you first need to
create nodes and manually enter summary content for them.  When you do, it's
helpful to note the node IDs as you will need to reference them later.

In another node, set the text format on teh body to the format to which you
assigned the plagiarize filter.  Then, within the body, place the following
shortcode to embed the summary from another node:

[summary:NID]

When the node is viewed, the filter will replace the shortcode with the title
and summary of the referenced node.  If the referenced node has content in its
body, the title will link to that node.

There are three parameters that you can add to the shortcode in any order and in
any combination to alter the result:

:nolink     ex. [summary:NID:nolink]
:notitle    ex. [summary:NID:notitle]
:nolist     ex. [summary:NID:notitle:nolist]

NOLINK - Prevents the substituted title from being linked to its node,
regardless of whether or not is has a body value.

NOTITLE - Prevents the title of the substituted node from being printed, thus
creating the appearance that the substituted summary is a seamless part of the
content.

NOLIST - Prevents the title of the substituted node from being placed into the
summary list as outlined below.

There is an additional shortcode you can use to generate a list of the summaries
that are being substituted into the current body:

[summary:list]

By default, each title in the list is an anchored link to its summary within
the page.  You can omit the links and create a simple text list of the titles
by appending the following parameter:

:nolink     ex. [summary:list:nolink]

Both the substituted summaries and the summary list can be overridden with theme
functions or template files:

1. To customize the summaries, copy plagiarize-summary.tpl.php into your theme
   directory and alter the theme registry.

2. To customize the summary list, implement hook_plagiarize_summary_list($vars);


LIMITATIONS
--------------------------------------------------------------------------------
- To prevent recursion, nodes cannot reference their own summaries.
- To prevent recursion, summaries cannot contain references to any other
  summaries.
- If a node body field allows multiple instances, only the first instance is
  substituted.
- Summaries must be provided manually.  Drupal will, in most cases, try to
  create a summary by clipping the body field.  This has the potential for
  causing recursion if tow nodes are referencing each other so it is not allowed
  in this filter.


TROUBLESHOOTING
--------------------------------------------------------------------------------
If you are not seeing a summary where you thing you should see one, try the
following steps:

1. Make sure the node being referenced has an explicit summary.
2. Make sure the node body you are placing summaries into is set to the right
   input format.
3. Clear the cache to rebuild the node content.
4. In the settings for the input filter, change the "No Summary Behavior" to
   leave the shortcode on the page if it is not being replaced.  This way you
   can tell if the filter is recognizing the filter or not and whether it is
   being replaced with an empty string or not.
5. If you've overridden the template file, make sure you pointed the registry
   to the right place.
   

CONTACT
--------------------------------------------------------------------------------
Chris Albrecht, chris@162unlimited.com
http://drupal.org/user/176328
