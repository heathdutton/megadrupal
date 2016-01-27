
-- SUMMARY --

"Multistep Nodeform" is a module to split the node creation and node editing
form into several steps.
This may be usefull for you if you have lots of fields in your content type and
would like to provide a wizard-like UI for content creation/editing.

-- REQUIREMENTS --

* ctools: http://drupal.org/project/ctools
* "Field UI" (core module) should be enabled to configure steps manually.

-- INSTALLATION --

* Install as usual, see http://drupal.org/documentation/install/modules-themes/modules-7
  for further information.


-- CONFIGURATION --

To configure steps for a content type go to "Manage fields" (e.g.
admin/structure/types/manage/page/fields) and add steps to the type as you would
add fields.
After adding a step you may configure the step details by clicking on the gear
in column "Operations". This opens a new settings form for the step where you
can set the label, add some descriptive text, define the labels for the step
buttons, ...


-- NOTES --

* Any field not assigned to a step will not be visible on the node form and
  therefore not editable!
* You may directly access a step by adding ?step={step_name} to the current url.
  E.g.: node/1/edit?step=step_article_additional will jump to the step named
  "step_article_additional".
  If there are steps with unfilled required fields (step is "not skippable") the
  first non-skippable step will be displayed instead of the one requested by
  url.
  Warning: using this feature may result in data loss while adding or editing a
  node. Any changes you have made to the node will get lost if you jump to a
  step using this feature, so this should be used only on node/[nid]/edit to
  skip some steps the user do not need to update.

-- MAINTAINERS --

Stefan Borchert (http://drupal.org/user/36942)

Development of this module is sponsored by undpaul (http://www.undpaul.de).
