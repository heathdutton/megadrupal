# features/responsive_tables_filter.feature

@api @javascript
Feature: Responsive Tables Filter
  In order to have nodes with table responsive
  As an authenticated user
  I need to be able to add tables that will respond to device width

Scenario: A Drupal Views table is responsive with aggregation on and off
  Given I am logged in as a user with the "administrator" role on this site
  And I set browser window size to "1200" x "900"
  When I visit "/admin/structure/views/add"
  And I fill in "Tablesaw Test" for "human_name"
  And I fill in "Tablesaw Test" for "page[title]"
  And I fill in "tablesaw-test" for "page[path]"
  And I check the box "edit-block-create"
  And I select "table" from "page[style][style_plugin]"
  And I select "table" from "block[style][style_plugin]"
  And I press the "Continue & edit" button
  And I wait for css element "#edit-actions-save" to appear
  And I press the "Save" button
  And I visit "/tablesaw-test"
  Then I should see the css element "table" with the attribute "class" with the value containing "tablesaw tablesaw-stack"
  And I should see the "b.tablesaw-cell-label" css selector with css property "display" containing "none"
  When I set browser window size to "350" x "900"
  And I wait for 2 seconds
  Then I should see the "b.tablesaw-cell-label" css selector with css property "display" containing "block"
  When I set browser window size to "1200" x "900"
  And I run drush "vset 'preprocess_css' 0"
  And I run drush "vset 'preprocess_js' 0"
  And I run drush "cache-clear all"
  And I wait for 5 seconds
  When I visit "/tablesaw-test"
  Then I should see the css element "table" with the attribute "class" with the value containing "tablesaw tablesaw-stack"
  And I should see the "b.tablesaw-cell-label" css selector with css property "display" containing "none"
  When I set browser window size to "350" x "900"
  And I wait for 2 seconds
  Then I should see the "b.tablesaw-cell-label" css selector with css property "display" containing "block"
  # Clean up the view for subsequent test iterations.
  Then I visit "/admin/structure/views/view/tablesaw_test/delete"
  And I press the "Delete" button

Scenario: Demonstrate adding a plain table to a body field with CSS/JS aggregation disabled
  Given I am logged in as a user with the "administrator" role
  And I set browser window size to "1200" x "900"
  And I run drush "vset 'preprocess_css' 0"
  And I run drush "vset 'preprocess_js' 0"
  And I run drush "cache-clear all"
  And I wait for 5 seconds
  When I go to "node/add/page"
  And I fill in "Tablesaw Test" for "edit-title" in the "form_item_title" region
  And I fill in "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.<table><thead><tr><th>Header One<th>Header Two<tbody><tr><td>Easily add tables with the WYSIWYG toolbar<td>Styled to match the brand<tr><td>Tables respond to display on smaller screens<td>Fully accessible to screen readers</table>" for "edit-body-und-0-value"
  And I press the "Save" button
  # Verify themed output #
  Then I should see the heading "Tablesaw Test"
  And I should see the css selector "table" with the attribute "class" with the exact value "tablesaw tablesaw-stack"
  And I should see "Easily add tables with the WYSIWYG toolbar" in the "field_name_body" region
  Then I should see the "b.tablesaw-cell-label" css selector with css property "display" containing "none"
  And I set browser window size to "350" x "900"
  And I wait for 2 seconds
  Then I should see the "b.tablesaw-cell-label" css selector with css property "display" containing "block"

Scenario: Demonstrate table filter works with pre-existing attributes & HTML 5 elements and encoded characters
  Given I am logged in as a user with the "administrator" role
  And I set browser window size to "1200" x "900"
  When I go to "node/add/page"
  And I fill in "Tablesaw Test" for "edit-title" in the "form_item_title" region
  And I fill in "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. <tspan>Lorem</tspan><svg>Ipsum</svg>Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.<table class='test-class' additional='test'><thead><tr><th>Header One<th>Header 2<tbody><tr><td>Easily add tables with the WYSIWYG toolbar<td>Encoded characters test öô & , ?<tr><td>Tables respond to display on smaller screens<td>Fully accessible to screen readers</table>" for "edit-body-und-0-value"
  And I press the "Save" button
  # Verify themed output #
  Then I should see the heading "Tablesaw Test"
  And I should see the css selector "table" with the attribute "class" with the exact value "test-class tablesaw tablesaw-stack"
  And I should see the css selector "table" with the attribute "additional" with the exact value "test"
  And I should see "Easily add tables with the WYSIWYG toolbar" in the "field_name_body" region
  And I should see "Encoded characters test öô &" in the "field_name_body" region
  Then I should see the "b.tablesaw-cell-label" css selector with css property "display" containing "none"
  And I set browser window size to "350" x "900"
  And I wait for 2 seconds
  Then I should see the "b.tablesaw-cell-label" css selector with css property "display" containing "block"

Scenario: Demonstrate table filter works with CSS/JS Aggregation enabled
  Given I am logged in as a user with the "administrator" role
  And I set browser window size to "1200" x "900"
  And I run drush "vset preprocess_css 1"
  And I run drush "vset preprocess_js 1"
  And I run drush "cache-clear all"
  And I wait for 5 seconds
  When I go to "node/add/page"
  And I fill in "Tablesaw Test" for "edit-title" in the "form_item_title" region
  And I fill in "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. <tspan>Lorem</tspan><svg>Ipsum</svg>Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.<table class='test-class' additional='test'><thead><tr><th>Header One<th>Header Two<tbody><tr><td>Easily add tables with the WYSIWYG toolbar<td>Styled to match the brand<tr><td>Tables respond to display on smaller screens<td>Fully accessible to screen readers</table>" for "edit-body-und-0-value"
  And I press the "Save" button
  # Verify themed output #
  Then I should see the heading "Tablesaw Test"
  And I should see the css selector "table" with the attribute "class" with the exact value "test-class tablesaw tablesaw-stack"
  And I should see the css selector "table" with the attribute "additional" with the exact value "test"
  And I should see "Easily add tables with the WYSIWYG toolbar" in the "field_name_body" region
  Then I should see the "b.tablesaw-cell-label" css selector with css property "display" containing "none"
  And I set browser window size to "350" x "900"
  And I wait for 2 seconds
  Then I should see the "b.tablesaw-cell-label" css selector with css property "display" containing "block"
