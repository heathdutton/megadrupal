@api
Feature: Referenced Entity Translation
  In order to prove that referenced entities are compiled and also translated
  Site administrators should be able to
  Import and export XLIFFs through the portal UI including, entity references

  Background:
    Given I am logged in as a user with the "administer entity xliff" permission
    And "page" content:
      | title                    | body                     | promote |
      | English host page title  | English host body text.  | 1       |
      | English child page title | English child body text. | 1       |
    When I am on the homepage
    And follow "English host page title"
    Given this node references the "English child page title" node
    When I reload the page
    Then I should see "English host page title"
    And I should see "English host body text"
    And I should see "English child page title"
    And I should see "English child body text"

  Scenario: Export XLIFF through portal
    Given I am on the homepage
    And follow "English host page title"
    When I click "XLIFF"
    And I click "Download"
    Then the response should contain "<xliff"
    And the response should contain "<source xml:lang=\"en\">English host page title</source>"
    And the response should contain "<source xml:lang=\"en\">English host body text.</source>"
    And the response should contain "<source xml:lang=\"en\">English child page title</source>"
    And the response should contain "<source xml:lang=\"en\">English child body text.</source>"

  Scenario: Import XLIFF through portal
    Given I am on the homepage
    And follow "English host page title"
    And I click "XLIFF"
    When I attach a "fr" translation of this "English" node
    And I press the "Import" button
    Then I should see the success message containing "Successfully imported"
    When I click "View"
    And I click "Français"
    Then I should see "fr host page title"
    And I should see "fr host body text"
    And I should see "fr child page title"
    And I should see "fr child body text"
