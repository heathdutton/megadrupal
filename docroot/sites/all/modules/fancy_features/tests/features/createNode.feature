Feature: Create Fancy pages
  Tests the simple creation of Fancy pages

  @api @javascript
#  Scenario: Create fresh node as administrator, do basic checks and test if IEF is disabled
#    Given I am logged in as a user with the "administrator" role
#    And I am on "/node/add/fancy-page"
#
#    # Initial node creation
#    Then I should see "Create Fancy Page"
#    When I fill in the following:
#      | Title    | Testnode #1            |
#    And I press "op"
#
#    Then I should see "Testnode #1"
#
#    # Edit the node the second time
#    And I edit the current node
#    Then I should see "Edit Fancy Page Testnode #1"
#
#    # Delete node to keep the db clean
#    When I press "edit-delete"
#    And I press "edit-submit"

  @api @javascript
  Scenario: Creates a Fancy page with sections containing text content elements.
    Given I am logged in as a user with the "administrator" role
    When I start creating a fresh "fancy_page" node named "Test Fancy Page with text elements"
    Then I add a new "responsive" entity
    Then I add a new "rich_text" entity
    Then I fill WYSIWYG field "fancy_text" with "This is a test."

    When I submit the "field_fancy_rel_contents" ief form
    And the preview for "field_fancy_rel_contents" in row "1" should contain "This is a test."

    When I submit the "field_fancy_rel_sections" ief form
    And the preview for "field_fancy_rel_sections" in row "1" should contain "This is a test."

    When I press "edit-submit"
    Then should see "This is a test."

    Then I delete the current node

  @api @javascript
  Scenario: Creates a fancy page with sections containing picture content elements.
    Given I am logged in as a user with the "administrator" role
    When I start creating a fresh "fancy_page" node named "Test Fancy Page with picture elements"
    Then I add a new "responsive" entity
    Then I add a new "picture" entity
    Then I attach the file "example-image.jpg" to file field "image"
    And I fill in Drupal field "link" with "http://www.google.com"

    When I submit the "field_fancy_rel_contents" ief form
    And the preview for "field_fancy_rel_contents" in row "1" should contain "Link: http://www.google.com"
    And the preview for "field_fancy_rel_contents" in row "1" should contain an "img[src*=example-image]" element

    When I submit the "field_fancy_rel_sections" ief form
    And the preview for "field_fancy_rel_sections" in row "1" should contain an "picture img[srcset*=example-image]" element

    When I press "edit-submit"
    Then I should see an "picture img[srcset*=example-image]" element

    Then I delete the current node