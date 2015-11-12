Feature: Admin type listing
  In order to verify the class structure is working
  As an admin user
  View the listing of message types

  @api
  Scenario: View the listing of message types
    Given I am logged in as a user with the "administrator" role
    When I go to "/admin/structure/comstack/types"
    Then I should get a "200" HTTP response
    And I should see the text "Add message type"
