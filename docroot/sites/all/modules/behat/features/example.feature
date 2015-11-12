Feature: User login
  In order to get access to more functionalities
  As a visitor
  I want to login to the site
  
  Background:
    Given a user "joe" with password "secret"

  Scenario: Login with valid credentials
    When I login as "joe" with password "secret"
    And I visit "/user"
    Then it should display "joe"

  Scenario: Login with invalid credentials
    When I login as "joe" with password "wrong"
    And I visit "/user"
    Then I should see the login form