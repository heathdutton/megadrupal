@api
Feature: Entity Field Translation of Comment Entities
  In order to prove that comments can be translated via Entity Translation
  Site administrators should be able to
  Import and export XLIFF translations through the XLIFF portal UI for a comment

  Background:
    Given I am logged in as a user with the "administer entity xliff,administer comments" permissions
    And I am viewing an "article" content with the title "English article title"
    And I fill in "subject" with "Test English comment"
    And I fill in "comment_body[und][0][value]" with "This is an English comment."
    And I press the "Save" button

  Scenario: Access XLIFF portal local task
    When I click "delete"
    Then the url should match "comment/\d+/delete"
    And I should see the link "XLIFF"
    When I click "XLIFF"
    Then the url should match "comment/\d+/xliff"
    And I should see "Export as XLIFF"
    And I should see "Import from XLIFF"

  Scenario: Export XLIFF through portal
    When I click "delete"
    And I click "XLIFF"
    When I click "Download"
    Then the response should contain "<xliff"
    And the response should contain "<source xml:lang=\"en\">This is an English comment.</source>"

  Scenario: Import XLIFF through portal
    When I click "delete"
    And I click "XLIFF"
    When I attach a "fr" translation of this "English" comment
    And I press the "Import" button
    Then I should see the success message containing "Successfully imported"
    When I click "View"
    Then I should see "This is an English comment"
    When I switch to the "fr" translation of this comment
    Then I should see "This is an fr comment"

  Scenario: No access to XLIFF portal local task without permissions
    Given I am not logged in
    When I am logged in as a user with the "administer comments,access administration pages" permissions
    And I am at "admin/content/comment"
    When I click "edit"
    Then I should not see the link "XLIFF"
