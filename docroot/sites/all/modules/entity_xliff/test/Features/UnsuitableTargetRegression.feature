@api
Feature: Unsuitable Translation Target (Regression)
  In order to prove that entities with previously non-existent embedded entities are translatable
  Site administrators should be able to
  Import XLIFF translations through the XLIFF portal UI for such content

  Background: Set up a translation set with no embedded entities.
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

  Scenario: Field Collection values added after initial translation
    When I click "English"
    And I click "Edit"
    And I fill in "English field collection" for "field_field_collection[en][0][field_long_text][und][0][value]"
    And I press "Save"
    And I click "XLIFF"
    And I attach a "fr" translation of this "English" node
    And I press the "Import" button
    Then I should see the success message containing "Successfully imported"
    When I click "View"
    And I click "Français"
    Then I should see "fr page title"

  Scenario: Field collection cardinality differs between source and target
    When I click "English"
    And I click "Edit"
    And I fill in "English page title field collection 0" for "field_field_collection[en][0][field_long_text][und][0][value]"
    And I press "Save"
    And I click "XLIFF"
    And I attach a "fr" translation of this "English" node
    And I press the "Import" button
    Then I should see the success message containing "Successfully imported"
    When this node has 2 additional field collections
    And I click "View"
    Then I should see "English page title field collection 2"
    When I click "XLIFF"
    And I attach a "fr" translation of this "English" node
    And I press the "Import" button
    Then I should see the success message containing "Successfully imported"
    When I click "View"
    And I click "Français"
    Then I should see "fr page title field collection 0"
    And I should see "fr page title field collection 2"

  Scenario: Referenced entity added after initial translation
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
    And I click "Français"
    Then I should see "fr page title"

  Scenario: Paragraphs added after initial translation
    Given I am viewing a "page" content with paragraphs and the title "English page title"
    When I click "XLIFF"
    And I attach a "fr" translation of this "English" node
    And I press the "Import" button
    Then I should see the success message containing "Successfully imported"
    When I click "View"
    And I click "Français"
    Then I should see "fr page title"
