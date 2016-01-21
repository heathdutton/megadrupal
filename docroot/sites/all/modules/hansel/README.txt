
PREFACE
-------
Hansel (named after Hansel and Gretel) provides custom breadcrumbs which are
build by user defined rules. These rules are independent from the menu system.
Rules can have logic switches and can add links to the breadcrumbs.

This module is designed for site builders, not for editors. Configuring Hansel
is more difficult than just a few clicks and requires a good understanding of
the rules (see chapter 2: Configuration).

QUICKSTART
----------
For advanced users who dislike reading, you may skip to chapter 3 to take a look
at the example configurations.

CONTENTS
--------
1. Installation
2. Configuration
2.1. Breadcrumb actions
2.1.1. Add domain
2.1.2. Add single link
2.1.3. Add link to node
2.1.4. Add link to nodetype
2.1.5. Add link to user
2.1.6. Add link to current page
2.1.7. Add term path
2.1.8. Add forum path
2.1.9. Add og path (organic group context)
2.1.10. Add parents
2.2. Logical actions
2.2.1. Goto rule
2.2.2. Leave
2.2.3. Switch
2.2.3.1. Subdomain
2.2.3.2. Domain id
2.2.3.3. Url argument
2.2.3.4. Node id
2.2.3.5. Node type
2.2.3.6. Node age
2.2.3.7. Path alias
2.2.3.8. PHP
2.2.3.9. Vocabulary name
2.2.3.10. Request uri
3. Example configurations
3.1. Basic configuration
3.2. Blogs and news
3.3. Montly archive
3.4. Multidomain setup
4. Settings
5. Testing your configuration
6. Exporting / importing
7. Extending hansel
7.1. Writing actions
7.2. Writing switches
8. Helper function for theming

1. Installation
--------------------------------------------------------------------------------
Copy Hansel into your modules directory (i.e. sites/all/modules) and enable
Hansel (on admin/build/modules). You may also want to enable the extra modules:

* Hansel Domain
  Provides integration with the domain module.
* Hansel Export
  Allows you to export and import the whole Hansel configuration.
  Not required for features integration.
* Hansel Forum
  Provides integration with the forum module.
* Hansel OG
  Provides integration with the Organic Groups module.
* Hansel Taxonomy
  Provides integration with the taxonomy module.

Add the "administer hansel" permission to the roles that may access the Hansel
configuration page. No permission is required to view the breadcrumbs. You can
skip this step if only the admin user may access the configuration page.

2. Configuration
--------------------------------------------------------------------------------
After installation, the configuration page will be available at
admin/build/hansel. On this page you'll see a form to add a rule. It is
important to know that Hansel configurations only contains rules. A rule has
four properties:

* Name
  This name is used for identification and used by switches (explained later).
* Parent
  The parent item in the hierarchy. Rules can be organized in a hierarchical
  structure.
* Action with breadcrumbs:
  This specifies what links need to be added the the breadcrumbs when this rule
  is executed. You may only specify one action per rule (some actions may add
  multiple links, however).
* Logical action:
  Defines what needs to be done after executing this rule.

The hierarchical structure is used by switches, but has no affect on other
logical actions, and thus may be used to organize the rules manually.

Generating the breadcrumbs always starts with the rule named "start" on the
topmost hierarchical level (known as root). The process can end in three ways:

* With the "leave" action (see 2.2.2)
* With a switch without matching rule (see 2.2.3)
* On recursion caused by improper configured goto rules (see 2.2.1)

After the process has ended, the breadcrumbs will be replaced by the links which
were created by the breadcrumb actions (see 2.1). In some cases, however, the
original breadcrumbs may be restored (see 2.2.2).

Existing rules are shown on the configuration page (admin/build/hansel) in the
hierarchical structure, ordered alphabetically (except the start rule, which is
displayed on top). All rules have an edit and a delete button in the grey bar
containing the rule name.

2.1. Breadcrumb actions
-----------------------
Breadcrumb actions can add links to the breadcrumbs. A single action may add
multiple links, but may also add no links at all.

When adding a rule, you can select the action in the dropdown box with the title
"Action with breadcrumbs". Some actions have configuration options, those are
shown after clicking the "next" button.

In the overview, the breadcrumb action is displayed right below the rule name.

2.1.1. Add domain
-----------------
This action is only available when the hansel_domain module is enabled. It adds
the current domain name pointing to the domain path.

For more information about what domains are in this context, read the readme
file of the domain module, or visit http://drupal.org/project/domain

2.1.2. Add single link
----------------------
Adds a single - static - link to the breadcrumbs. This action has three
configuration options:

* Link title
  The visible text of the link.
* Translate title
  Make the link title available for translation.
* Link path
  Destination of the link. Do not use leading or trailing slashes. Use "<front>"
  to link to the frontpage or "<none>" to output the crumb as plaintext.

You may use "[arg-N]", which will be replaced by the url argument. "N" must be
replaced by the argument number, where 1 is the first argument. Tokens provided
by the token module (http://drupal.org/project/token) are also available.

2.1.3. Add link to node
-----------------------
When on node pages (thus having "node/%" as path), this will add a link to that
node. The node title is used as link text.

2.1.4. Add link to nodetype
-----------------------
When on node pages (thus having "node/%" as path) or on node add pages
("node/add/%"), this will add a link with the nodetype as title, linking to a
configurable path (defaults to "node/add/[type]"). There are 2 tokens available,
[type], which will be replaced by the machine name of the nodetype with hyphens
("news-feed") and [type-raw], which will be replaced by the original machine
name ("news_feed").

2.1.5. Add link to user
-----------------------
When on user pages (thus having user/% as path), a link to the user profile of
that user is added to the breadcrumbs.

When on node pages (thus having node/% as path), a link to the node author's
user profile is added.

You can configure the text which is used for links to the profile of the current
user. You may enter something like "Your account". This field is optional, the
username of the current user will be used if empty.

A checkbox is available to use the realname provided by the realname module
instead of the username. This option is only visible when the "realname" module
is enabled.

2.1.6. Add link to current page
-------------------------------
This will add a link to the current page, with the page title used as link text.
No link is added if the page title is empty.

2.1.7. Add term path
--------------------
This action is only available when the hansel_taxonomy module is enabled. When
on node pages (thus having node/% as path), this will add the taxonomy term path
for that node to the breadcrumbs. You may force this action to only take terms
from a specified vocabulairy. By default, all vocabulairies are used.

2.1.8. Add forum path
---------------------
Adds the forum path. This works for topic listings ("forum/%") and forum nodes.
The containers are not added to the path by default. There is a configuration
option to change this behavior.

2.1.9. Add og path (organic group context)
------------------------------------------
This actions adds a link to the group node, where the current node belongs to.
The Hansel OG module must be installed in order to have this action available.

2.1.10. Add parents
-------------------
Adds the parent pages to the breadcrumbs. Hansel will use the menu-structure to
get the parents. Other modules may extend this to use other relations as well.

2.2. Logical actions
--------------------
Each rule has a logical action, which is "goto rule", "leave" or "switch". This
action defines what is going to happen after this rule is executed. All actions
have a configuration option, which is displayed after clicking the "next" button
when addding or editing a rule.

2.2.1. Goto rule
----------------
After this rule is executed, we will execute another rule. That rule is
specified in the configuration. The destination may be anywhere in the
hierarchical structure.

2.2.2. Leave
------------
End the breadcrumb generation process as described in the introduction of this
chapter.

You may optionally check the "Restore original breadcrumbs" box. If checked,
the original breadcrumbs (generated by Drupal core) will be used instead of the
breadcrumbs generated by Hansel. This is usefull for sections where you do not
want to override the breadcrumbs (like the admin section, see 3.1).

2.2.3. Switch
-------------
The switch action is like the goto action, but can go to different destinations,
depending on the chosen "switch action".

A switch action is, for example, "node type". This action has a value, which
is the node type for the currently displayed node, for example "story". This
value is compared with the rule names of the child rules of the switch rule.
For example we have four rules configured:

Name: start
Action: switch on node type
  Name: page (child rule of "start")
  Name: story
  Name: <default>

The rule "start" has "switch" as logical action with "node type" as switch type.
On a node page with nodetype "story", the rule "story" is the next rule that
will be executed. On page nodes, the rule "page" is executed. If there is no
corresponding rule to the node type, or if we aren't at a node page at all, the
rule with name "<default>" is executed. The default rule is recognised by the
name, and thus always must be named "<default>". If no matching rule is found
and the default rule is missing, the breadcrumb generation process will end as
described in the introduction of this chapter.

Switches may implement the special empty-rule (rule with the name "<empty>").
This rule is used when the value is empty. This function is only available for
the "url argument" switch.

Switches may have its own configuration options. You don't see them when adding
a rule, but you can find them by clicking on the switch type name on the
configuration page (admin/build/hansel).

2.2.3.1. Subdomain
------------------
This action is only available when the hansel_domain module is enabled. It uses
the subdomain name for the current domain as value to switch on. The subdomain
is likely something is "something.example.com". This is the value of the
"Domain" field under the "Create domain record" tab (on
admin/build/domain/create).

2.2.3.2. Domain id
------------------
This action is like the "subdomain" action, but uses the domain id instead of
subdomain. The domain id is found on the domain list (admin/build/domain). The
advantage of using the domain id instead of the subdomain is that you may rename
the domain without having to change the Hansel rules. However, using the
subdomain provides more readability to the Hansel configuration.

2.2.3.3. Url argument
---------------------
This switch type compares the rule names with a part of the URL path (called
arguments). By default, the first argument is used, but this can be changed on
the switch configuration page. You can get there by clicking on "url argument"
on the main page (admin/build/hansel). There are three options available:

* The url argument number (1 is the first argument).
* Case sensitive matching (disabled by default).
* Match rules by regular expressions (disabled by default).

The first argument can be something like "node" or "admin". URL arguments never
includes slashes.

There is a special "<front>" rule available for this switch. Rules with this
name matches the frontpage. Its working is independent of the url argument
number configured for this switch.

2.2.3.4. Node id
----------------
Switches on the node id of the current displayed node. This only has a value
on paths starting with "node/%".

2.2.3.5. Node type
------------------
Switches on the node type of the current displayed node, like "story" or "page".
This only has a value on paths starting with "node/%".

2.2.3.6. Node age
-----------------
Switches on the age of a node based on node creation time. Subrules can be named
as ranges ("0-1") or with an operator ("< 2"). The unit definition can be
changed in the rule configuration form. When set to 1 day, "0-1" means between
0 and 1 days old. This includes an age of 0 seconds, but not an age of exactly
one day (86400 seconds). The following operators are available: "<", ">", "<="
and ">=". Numbers may contain floating points, where only a dot is allowed as
decimal separator.

2.2.3.7. Path alias
-------------------
Switches on the alias for the current path. By default, child rules matches when
the alias starts with the rule name. You can change this behaviour in the switch
configuration page, where you can choose between 4 modes:

* starts with (rule "content" matches on path "content/first-story")
* ends with (rule "story" matches on path "content/first-story")
* contains (rule "first" matches on path "content/first-story")
* matches regular expression (rule "[a-z]{5}" matches on path
  content/first-story).

Matching is case insensitive by default. You can turn on case sensitive matching
on the switch configuration page.

2.2.3.8. PHP
------------
Switch on the result of custom PHP code. The code must be entered between
<?php and ?> and must return the value to switch on.

In order to be able to configure switches from this type, you need the
"use PHP for hansel configuration" permission.

2.2.3.9. Vocabulary name
------------------------
Switch on the name of the vocabulary. This works on the term pages
(taxonomy/term/%). Matching is always case insensitive.

2.2.3.10. Request uri
---------------------
Switch on the request uri (like "/user"). By default, child rules matches when
the uri starts with the rule name. You can change this behaviour in the switch
configuration page, where you can choose between 4 modes:

* starts with (rule "content" matches on path "content/first-story")
* ends with (rule "story" matches on path "content/first-story")
* contains (rule "first" matches on path "content/first-story")
* matches regular expression (rule "[a-z]{5}" matches on path
  content/first-story).

Matching is case insensitive by default. You can turn on case sensitive matching
on the switch configuration page.

3. Example configurations
--------------------------------------------------------------------------------
This section contains some example configuration. This shows how Hansel is
intended to be used and what the most efficient way is to take control of your
breadcrumbs.

3.1. Basic configuration
------------------------
Name: start (process always start with the rule named "start", see chapter 2).
Breadcrumb action: add link "Home" (use "<front>" as path)
Logic action: switch on url argument 1

  Name: admin
  Breadcrumb action: none
  Logic action: leave (restore original breadcrumbs)

  Name: node
  Breadcrumb action: none
  Logic action: switch on node type

    Name: <default>
    Breadcrumb action: add link to node
    Logic action: leave

  Name: user
  Breadcrumb action: add link to user
  Logic action: leave

This leaves the breadcrumbs for the admin section untouched. Other possibilities
for the first url argument are "node" and "user". For user pages, this will
generate the breadcrumb "Home > Username" or "Home > Your Account" if you filled
in "Your Account" in the breadcrumb action configuration (see 2.1.4). On node
pages this will always have the breadcrumb "Home > Node title". For the reason
why whe didn't set the "add link to node" action on the rule "node", take at
look at the blog example (see 3.1).

3.2. Blogs and news
-------------------
Name: start (process always start with the rule named "start", see chapter 2).
Breadcrumb action: add link "Home" (use "<front>" as path)
Logic action: switch on url argument 1

  Name: admin
  Breadcrumb action: none
  Logic action: leave (restore original breadcrumbs)

  Name: blogs
  Breadcrumb action: add single link (with the text "Blogs")
  Logic action: goto rule "add blog link"

    Name: add blog link
    Breadcrumb action: add link to user
    Logic actions: goto rule "<default>"

  Name: news
  Breadcrumb action: add single link (with the text "News")
  Logic action: goto rule "<default>"

  Name: node
  Breadcrumb action: none
  Logic action: switch on node type

    Name: blog
    Breadcrumb action: add single link (with the text "Blogs")
    Logic action: goto rule "blogs"

    Name: news
    Breadcrumb action: none
    Logic action: goto rule "news" (under "start", not this rule!)

    Name: <default>
    Breadcrumb action: add link to node
    Logic action: leave

  Name: user
  Breadcrumb action: add link to user
  Logic action: leave

Compared to the basic configuration, we have added 2 rules containing node
types, which are "blog" and "news". The "news" rule goes to the rule "news"
under "start", which is a rule we use for the whole news section. That rule adds
the link "News" to the breadcrumbs, linking to "news", which is our news
overview page. After adding that link, we would also like to add a link to the
node. However, we cannot add multiple breadcrumb actions to one rule. The
easiest way to do this is to set the logic action to go to the rule "<default>",
which will add the link to the node, resulting in breadcrumbs like
"Home > News > My news item". The news overview page also ends at the default
rule. However, that rule does not add any links since the overview page is not
a node page. Thus the news overview will have the breadcrumbs "Home > News".

For the blogs, we want breadcrumbs like "Home > Blogs > Author's name > Title".
We need an extra rule to add the author's name in the crumbs. You can see this
in the example. The "blogs" rule adds the link to to "Blogs", then the rule
"add blog link" adds the author's username and at last the "<default>" rule
adds the link to the actual blogpost.

3.3. Montly archive
-------------------
The views module (http://drupal.org/project/views) provides a default view
"archive". This page provides a list of nodes per month. The path of this view
is "archive", clicking on a month leads to a page on "archive/201004" (for April
2010). The following example configuration provides the breadcrumbs
"Montly archive > April 2010".

Name: start (process always start with the rule named "start", see chapter 2).
Breadcrumb action: add link "Home" (use "<front>" as path)
Logic action: switch on url argument 1

  Name: archive
  Breadcrumb action: add link "Montly archive" (use "archive" as path)
  Logic action: switch on url argument 2 with regular expressions

    Name: [0-9]{6}
    Breadcrumb action: add link to current page
    Logic action: leave

Note that you have to change the default settings for the url argument switch
(see 2.2.3.3. Url argument).

3.4. Multidomain setup
----------------------
There are several ways of how you can configure Hansel when using domains. When
you want to have "Home" as the first element for the main domain, and the domain
name for other domains, without configuring the breadcrumbs per domain, we
suggest the following configuration:

Name: start
Breadcrumb action: none
Logic action: switch on domain id

  Name: 0
  Breadcrumb action: add link "Home" (use "<front>" as path)
  Logic action: goto rule "all domains"

  Name: <default>
  Breadcrumb action: add link to domain
  Logic action: goto rule "all domains"

Name: all domains
Breadcrumb action: none
Logic action: switch on url argument 1

  Name: admin
  Breadcrumb action: none
  Logic action: leave (restore original breadcrumbs)

  Name: node
  Breadcrumb action: none
  Logic action: switch on node type

    Name: <default>
    Breadcrumb action: add link to node
    Logic action: leave

  Name: user
  Breadcrumb action: add link to user
  Logic action: leave

4. Settings
--------------------------------------------------------------------------------
A settings page is available on admin/build/hansel/settings. Available options
are:

Render last item as a link
  Determines if the last item of the breadcrumb is a link (checked by default).
Hide last item
  Remove the last item of the breadcrumb.
Maximum length for individual items
  Maximum length in characters. Use 0 to disable trimming.
Trim on word boundary
  Try to trim on whole words. Titles are still trimmed within words if we would
  lose more than 66% of the characters by this action.
Ellipsis
  Text to add after trimmed texts. You may leave this empty.
Maximum number of items
  Maximum number of breadcrumb items. Items will be removed from the middle.
  Use 0 to disable.
Removed items replacement
  Removed breadcrumb items will be replaced by this text. Leave empty to
  disable.
Remove first item from Hansel path token
  The [hansel-path] token should not contain a "Home" item. Check this box if
  the first item of the breadcrumbs is something which should not show in your
  path aliases.
Cache
  Only parts which require interaction with the database are cached. Enabling
  this cache is only recommended when you use an efficient caching system (such
  as memcache). Note that Hansel uses cache only per action / switch. The output
  of the add term path action is cached per node, for example.
Set active menu item
  Set the active menu item based on the breadcrumbs. For example: if your
  breadcrumbs are News > Archive > 2008, it will activate the same menu item.
  If "2008" is not a menu item, it will activate "Archive". If that's not a menu
  item either it will only active "News" if present as a root element.
Skip first crumb
  Ignore the first item of the breadcrumbs when looking for a corresponding menu
  item. Check this box if the first item of the breadcrumbs is something like
  "Home" or "Start".
Set active menu name for current path
  The "Restore original breadcrumbs" option for the "Leave" action will only
  work for the default menu if this box is not checked. But checking this box
  can sometimes result in side-effects.

5. Testing your configuration
--------------------------------------------------------------------------------
Click on the tab "test", this will lead you to admin/build/hansel/test. On this
page you can fill in a path from which you do want to know what the breadcrumbs
for that page will look like.

Hansel tells you how the breadcrumbs are generated. This trace may look like:

    * Executing rule start
    * Executing crumb action add single link
    * Added 1 crumb(s)
    * switch on url argument
    * No match on rule admin
    * Match on rule node
    * Executing rule node
    * switch on node type
    * No match on rule <default>
    * Using default rule
    * Executing rule <default>
    * Executing crumb action add link to node
    * Added 1 crumb(s)
    * Leave

This shows exactly which rules are used and how the actions are handled.

For large configuration, it might be helpful to see the configuration in a
graph. This can be done using the DOT export function and Graphviz (see 6).

6. Exporting / importing
------------------------
The Hansel export module provides three ways of exporting your configuration:

    * Via features module.
    * Dump of your whole configuration, importable at every Hansel installation.
    * Export as DOT file, for visualisation using Graphviz.

For features integration, check the features admin page. Other exports are
available under the "Export" tab. There is also an "Import" tab available where
you can import any Hansel configuration dump.

7. Extending hansel
--------------------------------------------------------------------------------
Developers can extend the list of available breadcrumb actions and switches by
implementing the hooks which Hansel provides.

7.1. Writing actions
--------------------
Breadcrumb actions can be added by the hansel_action_types hook. Take a look at
hansel.module and hansel_taxonomy.module for examples.

Any implementation of this hook must always return an associative array where
the keys are the names of the actions and the values are associative arrays with
the action information. In PHP, this structure is as follows:

  array(
    'action name' => array(
      'get crumbs' => 'callback to generate the breadcrumbs',
      'info' => 'callback to generate the function line',
      'config form' => 'callback to generate the config form',
      'file' => 'file.containing.callbacks.inc',
      'show token list' => FALSE,
    ),
    ...
  )

This array may contain 0..N actions. The elements 'info', 'config form',
'file' and 'show token list' are optional. The callbacks are function names.

The 'get crumbs' callback takes the array $arguments as it's only argument. This
array contains the values from the config form. This function must return an
array with 0 or more elements (1 element per link) in the following format:

  array(
    array(
      'title' => 'text',
      'href' => 'link'
    ),
    ...
  )

Always use hansel_arg() instead of arg() within this callback function.
hansel_arg() may return parts of the test path given in the test tab (see
chapter 4: Testing your configuration).

The 'info' callback also takes only the $arguments array as it's only argument.
This function must return a string with information of what this action does.
A string must be in plain text and something like "add link to something".

The 'config form' callback also takes only the $arguments array as it's only
argument. It must generate a valid form! There is no need to write a submit
handler, values are stored automatically by Hansel with the same names as used
for the form elements. These values are passed to the callbacks in the
$arguments array. You have to fill the #default_value for the form elements
with the values from $arguments. Please be aware that $arguments may be an empty
array!

The value for 'file' is the filename where the callbacks resides. This file must
be in the module's folder (from the module implementing this hook). No file is
included when this value is omitted.

When 'show token list' is set to TRUE, the action configuration form shows the
list of available tokens. This setting defaults to FALSE.

7.2. Writing switches
---------------------
Switches can be added by the hansel_switch_types hook. Take a look at
hansel.module and hansel_domain.module for examples.

Any implementation of this hook must always return an associative array where
the keys are the names of the switches and the values are associative arrays
with the switch information. In PHP, this structure is as follows:

  array(
    'switch name' => array(
      'compare' => 'callback to compare the value',
      'info' => 'callback to generate the function line',
      'config form' => 'callback to generate the config form',
      'file' => 'file.containing.callbacks.inc',
    ),
    ...
  )

This array may contain 0..N switches. The items 'info', 'config form' and 'file'
are optional. The values for the callbacks are function names.

All callbacks take the array $arguments as its first argument. This array
contains the values from the configuration form.

The 'compare' function has a second argument which is the string $value. When
executing this rule, this function is called for each child rule. The rule name
for that child rule is passed as $value. This callback must return TRUE when
this should be the next rule to be processed and FALSE otherwise.

The 'info' callback also takes only the $arguments array as it's only argument.
This function must return a string with information of what this switch does.
A string must be in plain text and something like "node type" (which is
displayed to the user as "switch on node type").

The 'config form' callback also takes only the $arguments array as it's only
argument. It must generate a valid form! There is no need to write a submit
handler, values are stored automatically by Hansel with the same names as used
for the form elements. These values are passed to the callbacks in the
$arguments array. You have to fill the #default_value for the form elements
with the values from $arguments. Please be aware that $arguments may be an empty
array!

The value for 'file' is the filename where the callbacks resides. This file must
be in the module's folder (from the module implementing this hook). No file is
included when this value is omitted.

8. Helper function for theming
--------------------------------------------------------------------------------
bool hansel_breadcrumb_was_restored()
  Returns TRUE if the "restore breadcrumbs" functionality was triggered.