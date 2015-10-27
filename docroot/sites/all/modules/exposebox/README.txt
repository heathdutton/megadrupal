Expose Box
==========

This module is an alternative to the collapsible div/fieldset field groups.

Expose Box is a light weight module that provides a simple js-driven mechanism
for wrapping field content in a "show/hide" box. It does this by providing a new
field group. Bundling fields under the Expose Box field-group will wrap it in the
Box. The Expose Box functions similar to a collapsible fieldset field-group except
for a couple of differences:

1) The link that will "expose the box" when clicked can be shown at the top or the
   bottom of the box, and is easily styled.
2) The height of the box when "closed" can be configured to show a certain "height"
   of content. Then the entire content is "exposed" when the link is clicked.

Installation/Configuration:
1) Install the module in the ususal way.
2) Edit the field settings of any entity, and create a new "Expose Box" field-group.
3) Drag fields under the field-group like any other.
4) Configure the field-group:
   a) Set the text for the "expose link" that will toggle
      between closed and exposed states. Defaults to "Expose It!" and "Close It!".
   b) Set the height of the box when closed. If no height is provided then the box
      will default to the height of the parent element of the div.exposebox wrapper.
   c) Select whether to put the expose link above or below the box.
