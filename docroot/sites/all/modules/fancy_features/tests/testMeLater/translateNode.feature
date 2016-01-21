Feature: Translate fancy nodes
  Tests if fancy nodes can be translated correctly.

  @api @javascript
  Scenario: Creates a node, adds a text and picture element and translates them
    Given I am logged in as a user with the "administrator" role

  @api @javascript
  Scenario: Creates a node, adds a text and picture element and translates them
    Given I am logged in as a user with the "administrator" role
    And I start creating a fresh "panelized_page" node named "Translated Fancy Page with text elements"
    Then I select "English" from "edit-language"
    # check for language neutral disabled
    And I should not see an "#edit-language option[value=und]" element

    When I add a new "flexible" entity
    Then I select the layout "1_6_7_12"

    When I add a new "text" entity
    Then I fill WYSIWYG field "text" with "English text."
    And I submit the "ref_content_elements" ief form

    When I add a new "picture" entity
    Then I attach the file "example-image.jpg" to file field "image"
    And I fill in Drupal field "link" with "http://www.google.com"
    And I submit the "ref_content_elements" ief form

    When the preview for "ref_content_elements" in row "1" should contain "English text."
    Then the preview for "ref_content_elements" in row "2" should contain "Link: http://www.google.com"
    And the preview for "ref_content_elements" in row "2" should contain an "img[src*=example-image]" element

    Then I submit the "ref_layout_element" ief form
    Then the preview for "ref_layout_element" in row "1" should contain "English text."
    And the preview for "ref_layout_element" in row "1" should contain an "picture img[srcset*=example-image]" element

    When I press "edit-submit"
    Then I should see an "picture img[srcset*=example-image]" element
    And I should see "English text."

    # Translate node into german
    When I translate the current node into "German"
    Then the preview for "ref_layout_element" in row "1" should contain "English text."
    And the preview for "ref_layout_element" in row "1" should contain an "picture img[srcset*=example-image]" element

    When I edit the "ref_layout_element" field in row "1"
    Then the preview for "ref_content_elements" in row "1" should contain "English text."
    And the preview for "ref_content_elements" in row "2" should contain "Link: http://www.google.com"
    And the preview for "ref_content_elements" in row "2" should contain an "img[src*=example-image]" element

    # Check and translate text entity
    When I edit the "ref_content_elements" field in row "1"
    And the WYSIWYG field "text" is empty
    Then I fill WYSIWYG field "text" with "German text."
    And I submit the "ref_content_elements" ief form

    # Check and translate picture entity
    When I edit the "ref_content_elements" field in row "2"
    And the Drupal field "image" is empty
    And the Drupal field "link" is empty
    Then I attach the file "example-image2.jpg" to file field "image"
    And I fill in Drupal field "link" with "http://www.google.de"
    And I submit the "ref_content_elements" ief form



    # check preview should contain ger

    # edit link to check tokens
    # save
    # check preview
    # check output in english AND german

    # log out
    # check output again

    #Then I delete the current node


