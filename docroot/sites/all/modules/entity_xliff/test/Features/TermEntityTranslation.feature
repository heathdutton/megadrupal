@api
Feature: Entity Field Translation of Taxonomy Term Entities
  In order to prove that taxonomy terms can be translated via Entity Translation
  Site administrators should be able to
  Import and export XLIFF translations through the XLIFF portal UI for a user

  Background:
    Given I am logged in as a user with the "administer entity xliff,edit terms in 1" permissions
    And I am viewing a tags term with the name "Great content"
    And I click "Edit"
    And I fill in "field_text[en][0][value]" with "English list text 1"
    And I fill in "field_text[en][1][value]" with "English list text 2"
    And I press the "Save" button

  Scenario: Access XLIFF portal local task
    Then I should see the link "XLIFF"
    When I click "XLIFF"
    Then the url should match "taxonomy\/term/\d+/xliff"
    And I should see "Export as XLIFF"
    And I should see "Import from XLIFF"

  Scenario: Export XLIFF through portal
    When I click "XLIFF"
    And I click "Download"
    Then the response should contain "<xliff"
    And the response should contain "<source xml:lang=\"en\">English list text 1</source>"
    And the response should contain "<source xml:lang=\"en\">English list text 2</source>"

  Scenario: Import XLIFF through portal
    When  I click "XLIFF"
    And I attach a "fr" translation of this "English" taxonomy_term
    And I press the "Import" button
    Then I should see the success message containing "Successfully imported"
    When I click "View"
    Then I should see "English list text 1"
    And I should see "English list text 2"
    When I switch to the "fr" translation of this taxonomy_term
    Then I should see "fr list text 1"
    And I should see "fr list text 2"

  Scenario: No access to XLIFF portal local task without permissions
    Given I am not logged in
    When I am logged in as a user with the "administer taxonomy,access administration pages" permissions
    And I am at "admin/structure/taxonomy/tags"
    When I click "Great content"
    Then I should see the heading "Great content"
    And I should not see the link "XLIFF"
