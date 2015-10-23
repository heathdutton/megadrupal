Feature: Workbench Moderation
  In order to know that Workbench Moderation is functioning correctly
  As a site administrator
  I need to be able to trust that nodes are moderated consistently

Background:
    Given I am logged in as a user with the "administrator" role
    When I visit "/node/add/wetkit-page"
      And I fill in the following:
        | title_field[und][0][value]  | Published title |
        | Editor                      | plain_text      |
        | Body                        | Published body  |
      And I select "Published" from "workbench_moderation_state_new"
    When I press "edit-submit"
    Then the "h1" element should contain "Published title"
    When I click "New draft" in the "Tabs" region
      And I fill in the following:
        | title_field[und][0][value]  | Draft title |
        | Editor                      | plain_text  |
        | Body                        | Draft body  |
    When I press "edit-submit"
    Then the "h1" element should contain "Draft title"
    When I click "View published" in the "Tabs" region
    Then the "h1" element should contain "Published title"

  @api @wetkit_admin
  Scenario: Reverting a revision should create a new draft copy
    When I click "Moderate" in the "Tabs" region
      And I click "Revert" in the "Moderation" region
      And I press "edit-submit"
    Then the ".current-revision" element should not contain "This is the published revision"
