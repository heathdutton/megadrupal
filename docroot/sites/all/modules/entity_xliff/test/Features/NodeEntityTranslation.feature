@api
Feature: Entity Field Translation of Nodes
  In order to prove that nodes can be translated via Entity Field Translation
  Site administrators should be able to
  Import and export XLIFF translations through the XLIFF portal UI for a node

  Background:
    Given I am logged in as a user with the "administer entity xliff" permission

  Scenario: Access XLIFF portal local task
    Given I am viewing an "article" content with the title "English article title"
    Then I should see the link "XLIFF"
    When I click "XLIFF"
    Then the url should match "node/\d+/xliff"
    And I should see "Export as XLIFF"
    And I should see "Import from XLIFF"

  Scenario: Export XLIFF through portal
    Given "article" content:
      | title                 | body                       |
      | English article title | English article body text. |
    When I am on the homepage
    And follow "English article title"
    Given this "node" has an image attached with alt text "Image English alt text"
    When I click "XLIFF"
    And I click "Download"
    Then the response should contain "<xliff"
    And the response should contain "<source xml:lang=\"en\">English article body text.</source>"
    And the response should contain "<source xml:lang=\"en\">Image English alt text</source>"

  Scenario: Import XLIFF through portal
    Given "article" content:
      | title                 | body                       | promote |
      | English article title | English article body text. | 1       |
    When I am on the homepage
    And follow "English article title"
    And I click "XLIFF"
    Given this "node" has an image attached with alt text "Image English alt text"
    When I attach a "fr" translation of this "English" node
    And I press the "Import" button
    Then I should see the success message containing "Successfully imported"
    When I click "View"
    And I switch to the "fr" translation of this node
    Then I should see "fr article body text."
    And the response should contain "alt=\"Image fr alt text\""

  Scenario: No access to XLIFF portal local task without permissions
    Given I am not logged in
    When I am logged in as a user with the "authenticated user" role
    And I am viewing an "article" content with the title "English article title"
    Then I should not see the link "XLIFF"
