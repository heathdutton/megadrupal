
== Introduction

This module creates a filter allowing user to attach everything inline with a
simple tag.

The purpose is to provide a "light" presentation, and link to heavy content
(ie., with attach_node, then link to full node view, with attach_flash or
attach_audio, then link to a lightbox style popup...)

== Features

- This module is (somewhat) extensible with plugins
- Each plugin has two mode: simple mode or full mode (default).
  In simple mode, a minimal text is used and is inserted into node content
  inside a SPAN tag. In the full mode, a DIV is used and renders more data.

== Syntaxes

  [attach_PLUGIN|op1=var1|op2=var2|...]

- PLUGIN can be node, user, comment, flash, audio...
- op1, op2... depend on PLUGIN
- Options which are valid in all plugins:
  - title: override the default title
  - simple: simple mode (default = 0)
- Other supported options, by plugins:
  - node: nid
  - user: uid

Extra options can be introduced and used in your theme (for the node plugin).

== Examples

[attach_node|nid=12|title=This title overrides the node title]
[attach_user|uid=1|simple=1]
[attach_flv|file=files/movie.flv|title=My movie|width=500|height=300]
[attach_swf|file=files/demo.swf|title=My demo|width=500|height=300]
[attach_term|tid=52|count=10|class=right]
[attach_slideshare|id=1234567]

== Customization

The output can be customized with CSS override or theming override.

In the node plugin, you can create your own attach-node-NODETYPE.tpl.php if
you want the output is dependant on node type.

If you want to reuse your node.tpl.php and node-NODETYPE.tpl.php, use build_mode
to verify and output the proper tags. Don't forget to go to
admin/settings/attach and disable attach's template file.

== Hidden variables

If you want to add a "source" to track in Google Analytics, for example, you
can set the variable "attach_utm_source" value 1. In that case, links to
attached nodes become node/%nid?utm_source=attach.

