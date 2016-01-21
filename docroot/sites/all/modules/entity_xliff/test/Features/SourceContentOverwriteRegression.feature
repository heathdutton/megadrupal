@api
Feature: Source Content Overrrite (Regression)
  In order to prove that XLIFF imports do not overwrite source content
  Site administrators should be able to
  Import XLIFF translations through the XLIFF portal for existing content without affecting source content

  Background: Set up a translation set
    Given I am logged in as a user with the "administer entity xliff,translate content,bypass node access" permissions
    And "page" content:
      | title              | body                    | language | promote |
      | English page title | English page body text. | en       | 1       |
    And I am on the homepage
    And follow "English page title"
    And I click "Translate"
    And I click "add translation"
    And I fill in "French page title" for "title"
    And I press "Save"

  Scenario: Import over existing translation set (focus on field collections)
    When I click "English"
    And I click "Edit"
    And I fill in "English field collection" for "field_field_collection[en][0][field_long_text][und][0][value]"
    And I press "Save"
    And I click "XLIFF"
    And I attach a "fr" translation of this "English" node
    And I press the "Import" button
    Then I should see the success message containing "Successfully imported"
    When I click "View"
    Then I should see "English field collection"
    When I click "XLIFF"
    And I attach a "fr" translation of this "English" node
    And I press the "Import" button
    Then I should see the success message containing "Successfully imported"
    When I click "View"
    Then I should see "English field collection"
    And I should not see "fr field collection"

  Scenario: Import over existing translation set (focus on entity references)
    Given "page" content:
      | title                    | body                     |
      | English regression child | English child body text. |
    When I am on the homepage
    And follow "English page title"
    And this node references the "English regression child" node
    When I click "XLIFF"
    And I attach a "fr" translation of this "English" node
    And I press the "Import" button
    Then I should see the success message containing "Successfully imported"
    When I click "View"
    Then I should see "English child body text."
    When I click "XLIFF"
    And I attach a "fr" translation of this "English" node
    And I press the "Import" button
    Then I should see the success message containing "Successfully imported"
    When I click "View"
    And I should see "English regression child"
    And I should not see "fr regression child"

  Scenario: Import over existing translation set (focus on paragraphs)
    Given I am viewing a "page" content with paragraphs and the title "English page title"
    When I click "XLIFF"
    And I attach a "fr" translation of this "English" node
    And I press the "Import" button
    Then I should see the success message containing "Successfully imported"
    When I click "View"
    Then I should see "English page title paragraph 1"
    When I click "XLIFF"
    And I attach a "fr" translation of this "English" node
    And I press the "Import" button
    When I click "View"
    Then I should see "English page title paragraph 1"
    And I should not see "fr page title paragraph 1"
