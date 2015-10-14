<?php

use Drupal\DrupalExtension\Context\RawDrupalContext;
use Behat\Behat\Context\SnippetAcceptingContext;
use Behat\Behat\Tester\Exception\PendingException;
use Behat\Gherkin\Node\PyStringNode;
use Behat\Gherkin\Node\TableNode;
use Drupal\Component\Utility\Random;
use Drupal\DrupalExtension\Context\DrushContext;


/**
 * Defines application features from the specific context.
 */
class FeatureContext extends RawDrupalContext implements SnippetAcceptingContext {

  private $nodeId = NULL;

  /**
   * Initializes context.
   *
   * Every scenario gets its own context instance.
   * You can also pass arbitrary arguments to the
   * context constructor through behat.yml.
   */
  public function __construct() {
  }

  /**
   * @BeforeScenario
   */
  public function beforeScenario(Behat\Behat\Hook\Scope\BeforeScenarioScope $scope)
  {
    $this->getSession()->getDriver()->resizeWindow(1000, 1000);
  }

  /**
   * @When I start creating a fresh :arg1 node named :arg2
   */
  public function iStartCreatingAFreshNodeNamed($content_type, $title)
  {
    $session = $this->getSession();
    $page = $session->getPage();

    $this->visitPath('/en/node/add/' . str_replace('_', '-', $content_type));

    $this->iFillInDrupalFieldWith('title-field', $title);
    $this->iFillInDrupalFieldWith('headline', $title);

    $submit = $page->findButton('op');
    $submit->click();

    $this->iEditTheCurrentNode();
  }

  /**
   * @Then /^I edit the current node$/
   * Helps to avoid the "Element is not currently visible and so may not be interacted with" exception.
   */
  public function iEditTheCurrentNode() {
    $shortLink = $this->_getShortLink();

    // Save node id to context.
    $fragments = explode('/', $shortLink);
    $this->nodeId = end($fragments);

    // Visit edit form.
    $this->visitPath($shortLink . '/edit');
  }

  /**
   * Delete the created node as step.
   * This can not be an AfterScenario hook because the Drupal extension
   * kills the session before we can delete the node.
   * @Then I delete the current node
   */
  public function iDeleteTheCurrentNode()
  {
    $session = $this->getSession();
    $page = $session->getPage();

    if ($this->nodeId) {
      $this->visitPath('/en/node/' . $this->nodeId . '/delete');

      $deleteButton = $page->find('css', 'input#edit-submit');

      $deleteButton->press();
    } else {
      throw new Exception('There is no node id stored at this moment.');
    }
  }

  /**
   * @When I translate the current node into :arg1
   */
  public function iTranslateTheCurrentNodeInto($language)
  {
    $session = $this->getSession();
    $page = $session->getPage();

    $shortLink = $this->_getShortLink();
    $this->visitPath($shortLink . '/translate');

    $tableRows= $page->findAll('css', '#content table:not(.sticky-header) tr');

    $found = FALSE;
    // Loop through translation table
    foreach ($tableRows as $tableRow) {
      $firstCol = $tableRow->find('css', 'td:first-child');

      // And find the right table row to click the link
      if ($firstCol && strpos($firstCol->getText(), $language) !== false) {
        $translateLink = $tableRow->find('css', 'td:last-child a');
        $translateLink->click();
        $found = TRUE;
        break;
      }
    }
    if (!$found) {
      throw new Exception('Unable to translate node into "' . $language . '".');
    }
  }

  /**
   * @Then /^I add a new "(\w+)" entity$/
   * Create a IEF form with a fresh entity
   * @todo improve form element selectors
   */
  public function iAddANewEntity($entityType) {
    $session = $this->getSession();
    $page = $session->getPage();

    // Select entity
    $select = $page->find('css', '.field-widget-inline-entity-form .form-type-select[class*="-actions-bundle"] select');
    $select->selectOption($entityType);

    // Submit form
    $submitButton = $page->find('css', '.field-widget-inline-entity-form input.form-submit[id$="-ief-add"]');
    $submitButton->press();

    //@todo use iWaitForAjaxToFinish() from Drupal\DrupalExtension\Context\MinkContext().
    //      This is currently not working. Might be an error in the RawMinkContext from Behat.
    $this->ajaxShouldBeFinishedWithinMs(5000);

    if (!$page->find('css', 'fieldset.ief-form')) {
      throw new Exception('Inline Entity Form did not appear.');
    }
  }

  /**
   * @Then I fill in Drupal field :arg1 with :arg2
   */
  public function iFillInDrupalFieldWith($field_name, $text)
  {
    $field = $this->_getDrupalField($field_name);

    $field->setValue($text);
  }

  /**
   * @When the Drupal field :arg1 is empty
   */
  public function theDrupalFieldIsEmpty($field_name)
  {
    $field = $this->_getDrupalField($field_name);

    $value = $field->getValue();

    if (!empty($value)) {
      throw new Exception('The Drupal form field named "' . $field_name . '" is not empty');
    }
  }

  /**
   * @Then I fill WYSIWYG field :arg1 with :arg2
   */
  public function iFillWYSIWYGFieldWith($field_name, $value)
  {
    $field = $this->_getDrupalField($field_name, '');

    $field->find('css', '.ckeditor_links')->click();
    $field->find('css', 'textarea')->setValue($value);
  }

  /**
   * @When the WYSIWYG field :arg1 is empty
   */
  public function theWysiwygFieldIsEmpty($field_name)
  {
    $field = $this->_getDrupalField($field_name, '');

    $field->find('css', '.ckeditor_links')->click();
    $value = $field->find('css', 'textarea')->getValue();

    if (!empty($value)) {
      throw new Exception('The WYSIWYG field "' . $field_name . '" is not empty');
    }

    $field->find('css', '.ckeditor_links')->click();
  }

  /**
   * @Then I attach the file :arg1 to file field :arg2
   */
  public function iAttachTheFileToFileField($path, $field_name)
  {
    $field = $this->_getDrupalField($field_name);

    if ($this->getMinkParameter('files_path')) {
      $fullPath = rtrim(realpath($this->getMinkParameter('files_path')), DIRECTORY_SEPARATOR).DIRECTORY_SEPARATOR.$path;
      if (is_file($fullPath)) {
        $path = $fullPath;
      }
    }

    $field->attachFile($path);
  }

  /**
   * Change the vms view mode
   * @Then I select the layout :arg1
   */
  public function iSelectTheLayout($layout)
  {
    $session = $this->getSession();
    $page = $session->getPage();

    $layout = str_replace('_', '-', $layout);

    $vms = $page->find('css', '.field-type-view-mode-selector label[for$="-value-' . $layout . '"]');
    $vms->click();
  }

  /**
   * @Then I submit the :arg1 ief form
   */
  public function iSubmitTheIefForm($field_name)
  {
    $session = $this->getSession();
    $page = $session->getPage();

    $button = $page->find('css', '.ief-entity-submit[id*="' . str_replace('_', '-', $field_name) . '"]');

    if (!$button) {
      throw new Exception('Cannot find any IEF form field named "' . $field_name . '"');
    }

    $button->click();
    $this->ajaxShouldBeFinishedWithinMs();

    //@todo check for form errors
  }

  /**
   * @Then I edit the :arg1 field in row :arg2
   */
  public function iEditTheFieldInRow($field_name, $row)
  {
    $session = $this->getSession();
    $page = $session->getPage();
    $class = str_replace('_', '-', $field_name);

    $selector = 'table[id*="' . $class . '"] tr:nth-child(' . $row . ') input[id$="-actions-ief-entity-edit"]';
    $editButton = $page->find('css', $selector);
    if (!$editButton) {
      throw new Exception('Cannot edit IEF form in row "' . $row. '". Used css selector: ' . $selector);
    }
    $editButton->press();

    $this->ajaxShouldBeFinishedWithinMs();
  }

  /**
   * @Then the preview for :arg1 in row :arg2 should contain :arg3
   */
  public function thePreviewForInRowShouldContain($field_name, $row, $value)
  {
    $session = $this->getSession();
    $page = $session->getPage();

    $class = str_replace('_', '-', $field_name);

    $selector = 'table[id*="' . $class . '"] tr:nth-child(' . $row . ') td[class$="-field_ief_preview"]';
    $preview = $page->find('css', $selector);

    if (!$preview) {
      throw new Exception('Cannot locate preview for field "' . $field_name);
    }

    $text = preg_replace('/ +/', ' ', $preview->getText());

    if (strpos($text, $value) === false) {
      throw new Exception('Cannot find text "' . $value . '" within the preview of field "' . $field_name . '". Text in preview is: " ' . $text . '"');
    }
  }

  /**
   * @Then the preview for :arg1 in row :arg2 should contain an :arg3 element
   */
  public function thePreviewForInRowShouldContainAnElement($field_name, $row, $element)
  {
    $session = $this->getSession();
    $page = $session->getPage();

    $class = str_replace('_', '-', $field_name);

    $selector = 'table[id*="' . $class . '"] tr:nth-child(' . $row . ') td[class$="-field_ief_preview"]';
    $preview = $page->find('css', $selector);

    if (!$preview->has('css', $element)) {
      throw new Exception('Cannot find element "' . $element . '" in the preview of field "' . $field_name . '"');
    }
  }

  /**
   * @Then /^AJAX should be finished within (\d+)ms$/
   */
  public function ajaxShouldBeFinishedWithinMs($duration = 5000) {
    $this->getMink()->getSession()->wait($duration, '(typeof(jQuery)=="undefined" || (0 === jQuery.active && 0 === jQuery(\':animated\').length))');
  }

  /**
   * @Then I wait a bit
   * For demonstrations.
   */
  public function iWaitABit() {
    $this->getMink()->getSession()->wait(2500, '1 === 2');
  }

  /**
   * Get the short link to the current page. E.g. /en/node/123
   * @return null|string
   * @throws \Exception
   */
  private function _getShortLink() {
    $session = $this->getSession();
    $page = $session->getPage();
    $selector = 'link[rel="shortlink"]';

    if (!$page->has('css', $selector)) {
      throw new Exception('Unable to find the shortlink to the current page.');
    }
    $shortLink = $page->find('css', $selector);

    return $shortLink->getAttribute('href');
  }

  /**
   * Look for a Drupal form field and return it if exists.
   * @param $field_name the field name
   * @return \DOM node instance
   * @throws \Exception
   */
  private function _getDrupalField($field_name, $type = 'input') {
    $session = $this->getSession();
    $page = $session->getPage();

    $field = $page->find('css', '.field-name-field-' . str_replace('_', '-', $field_name) . '[class*=field-widget] ' . $type);
    echo '.field-name-field-' . str_replace('_', '-', $field_name) . '[class*=field-widget] ' . $type;

    if (null === $field) {
      $field = $page->find('css', '.field-name-' . str_replace('_', '-', $field_name) . '[class*=field-widget] ' . $type);
      if (null === $field) {
        throw new Exception('Cannot find any Drupal form field named "' . $field_name . '"');
      }
    }

    return $field;
  }
}
