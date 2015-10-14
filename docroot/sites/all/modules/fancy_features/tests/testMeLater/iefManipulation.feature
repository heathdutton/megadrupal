Feature: Test
  Tests if fancy nodes can be translated correctly.

  @api @javascript
  Scenario: Switch order of rows
    Given I am logged in as a user with the "administrator" role

    When I start creating a fresh "panelized_page" node named "Translated Fancy Page with text elements"

    # basic node setup
    Then I add a new "flexible" entity

    When I add a new "text" entity
    Then I fill WYSIWYG field "ref_content_elements" with "First text in first section."
    And I submit the "ref_content_elements" ief form

    When I add a new "text" entity
    Then I fill WYSIWYG field "ref_content_elements" with "First text in first section."
    And I submit the "ref_content_elements" ief form

    Then I add a new "flexible" entity

    When I add a new "text" entity
    Then I fill WYSIWYG field "ref_content_elements" with "Single text in second section."
    And I submit the "ref_content_elements" ief form

    And I submit the "ref_layout_element" ief form


    When I press "edit-submit"
    # basic node setup

    # check output

    # reorder sections

    # reorder content

    # check reordered output

  @api @javascript
  Scenario: Delete a section and replace it with a new one

  @api @javascript
  Scenario: Delete a section and replace it with a new one, while reordering all contents