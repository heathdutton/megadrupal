@api
Feature: Multiple Language Import (Regression)
  In order to prove that multiple XLIFFs can be imported without issue
  Site administrators should be able to
  Import XLIFF translations for multiple languages through the XLIFF portal

  Background:
    Given I am logged in as a user with the "administer entity xliff" permission

  Scenario: Import XLIFF through portal
    Given "page" content:
      | title              | body                    | promote |
      | English page title | English page body text. | 1       |
    When I am on the homepage
    And follow "English page title"
    And I click "XLIFF"
    When I attach fr, de translations of this "English" node
    And I press the "Import" button
    Then I should see the success message containing "Successfully imported"
    And I should not see the message containing "There was a problem importing"
    When I click "View"
    And I click "Français"
    Then I should see the heading "fr page title"
    And I should see "fr page body text."
    When I click "Deutsch"
    Then I should see the heading "de page title"
    And I should see "de page body text."
