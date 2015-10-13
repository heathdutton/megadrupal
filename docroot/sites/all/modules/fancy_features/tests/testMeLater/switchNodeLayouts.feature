Feature: View Mode Selector related tests
  Tests if sections can change the view mode via the View Mode Selector module

  @api @javascript
  Scenario: Switch node layout and test it
    Given I am logged in as a user with the "administrator" role

    When I start creating a fresh "panelized_page" node named "Translated Fancy Page with text elements"

    # basic node setup
    Then I add a new "flexible" entity

    When I add a new "text" entity
    Then I fill WYSIWYG field "ref_content_elements" with "English text."
    And I submit the "ref_content_elements" ief form

    When I add a new "picture" entity
    Then I attach the file "example-image.jpg" to file field "image"
    And I fill in Drupal field "link" with "http://www.google.com"
    And I submit the "ref_content_elements" ief form
    And I submit the "ref_layout_element" ief form
    When I press "edit-submit"
    # basic node setup

    # check for node with default layout

    When I edit the current node
    Then I edit the "ref_layout_element" field in row "1"
    # change layout


    When I submit the "ref_layout_element" ief form
    And I press "edit-submit"

    # check for changed layout

    # Then I delete the current node

