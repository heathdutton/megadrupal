Feature: Metatags
  In order to know that metatags are providing meaningful text for search engines
  As a website user
  I need to be able to trust that metatag works consistently

  @api
  Scenario: Evaluating migrated metatags
    Given I am on "en/content/drupal-wxt"
    Then the metatag attribute "dcterms.title" should have the value "Drupal WxT"
    And I am on "fr/contenu/wxt-drupal"
    Then the metatag attribute "dcterms.title" should have the value "WxT Drupal"

  @api @javascript
  Scenario: Evaluating created metatags
    Given I am logged in as a user with the "administrator" role
    When I visit "/node/add/wetkit-page"
      And I click "Defaults" in the "Edit Metatags" region
      And I fill in the following:
        | title_field[und][0][value]     | Testing Title                |
        | body[und][0][format]           | wetkit_wysiwyg_text          |
        | Page title                     | SEO optimized title          |
        | Description                    | SEO optimized description    |
        | metatags[und][keywords][value] | SEO optimized keywords       |
        | workbench_moderation_state_new | published                    |
      And I type "Testing metatags" in the "edit-body-und-0-value" WYSIWYG editor
      And I press "edit-submit"
      And I wait 2 seconds
    Then the "h1" element should contain "Testing title"
