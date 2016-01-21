@api
Feature: Entity Field Translation of User Entities
  In order to prove that user entities can be translated via Entity Translation
  Site administrators should be able to
  Import and export XLIFF translations through the XLIFF portal UI for a user

  Background:
    Given I am logged in as a user with the "administer entity xliff,administer users,access user profiles" permissions
    And users:
      | name     | mail            | status |
      | Joe User | joe@example.com | 1      |
    And I am at "admin/people"
    And I click "Joe User"
    And I click "Edit"
    And I fill in "field_link[en][0][title]" with "English link title"
    And I fill in "field_link[en][0][url]" with "http://example.com"
    And I press the "Save" button

  Scenario: Access XLIFF portal local task
    When I am at "admin/people"
    And I click "Joe User"
    And I should see the link "XLIFF"
    When I click "XLIFF"
    Then the url should match "user/\d+/xliff"
    And I should see "Export as XLIFF"
    And I should see "Import from XLIFF"

  Scenario: Export XLIFF through portal
    When I am at "admin/people"
    And I click "Joe User"
    And I click "XLIFF"
    When I click "Download"
    Then the response should contain "<xliff"
    And the response should contain "<source xml:lang=\"en\">English link title</source>"
    And the response should not contain "http://example.com"

  Scenario: Import XLIFF through portal
    When I am at "admin/people"
    And I click "Joe User"
    Then I should see the link "English link title"
    And I click "XLIFF"
    When I attach a "fr" translation of this "English" user
    And I press the "Import" button
    Then I should see the success message containing "Successfully imported"
    When I click "View"
    Then I should see the link "English link title"
    When I switch to the "fr" translation of this user
    Then I should see the link "fr link title"

  Scenario: No access to XLIFF portal local task without permissions
    Given I am not logged in
    When I am logged in as a user with the "authenticated user" role
    And I move backward one page
    Then I should not see the link "XLIFF"
