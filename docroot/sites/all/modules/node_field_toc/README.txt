node_auto_fieldtoc
==================

A drupal module for adding a content types fields to a TOC pane.

##What it does

This module adds in-page anchors to your content types for all of the fields
visible in a given view mode.

You can then use the supplied content pane in a panels page with a node context
of an Node Auto field TOC enabled content type to provide a TOC style link list
to those anchors.

##Installation

Download and enable the module.

##Configuration

This module requires a few content type configuration settings and
the addition of a panels custom content pane to a page that has a
node context.

###Content Type Configuration:

On the content type configuration screen in the settings tabs, choose the
Node Auto Field TOC tab and check the enable checkbox.

You may also choose what you would like to be used as the fragment
(i.e. www.example.com/page#fragment).

You may select either the field machine name or the label of the field name.

###Panels Pane Configuration:
On a page where you have a node context for an enabled content type add a pane.

In the pane menu go to the "Miscellaneous" tab and choose the
Node Auto Field TOC pane.

The pane configuration allows you to change a few elements of the TOC.

You may add a List title or exclude it entirely by typing in: <none>

You may choose to use an ordered (ol) or unordered (ul) list style.

You may add a custom class to the toc list.

You may choose the view mode you want to use for field visibility settings for
this content type. The TOC will only add links that are visible (not hidden)
in the view mode you specify.
