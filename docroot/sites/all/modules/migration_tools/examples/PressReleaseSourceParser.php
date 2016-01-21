<?php
/**
 * @file
 * Includes PressReleaseSourceParser class.
 *
 * This class contains customization to parse press releases.
 *
 * This is an example of how to parse something with a date and id number.
 */

/**
 * Class PressReleaseSourceParser.
 *
 * @package migration_tools
 */

abstract class PressReleaseSourceParser extends MTNodeSourceParser {
  protected $date;
  protected $subTitle;
  protected $prNumber;

  /**
   * Getter.
   */
  public function getDate() {
    $date_string = $this->getProperty('date');
    MigrationMessage::makeMessage("Raw Date: @date_string", array('@date_string' => $date_string), WATCHDOG_DEBUG, 2);

    if (empty($date_string)) {
      $date = '';
    }
    else {
      $date = date('n/d/Y', strtotime($date_string));
      if (!empty($date)) {
        // Output success to show progress to aid debugging.
        MigrationMessage::makeMessage("Formatted Date: @date", array('@date' => $date), WATCHDOG_DEBUG, 2);
      }
    }

    return $date;
  }

  /**
   * Getter $this->subtitle property.
   */
  public function getSubTitle() {
    $subtitle = $this->getProperty('subtitle');

    return $subtitle;
  }

  /**
   * Gets $this->prNumber property.
   */
  public function getPrNumber() {
    $pr_number = $this->getProperty('prNumber');

    return $pr_number;
  }

  /**
   * Clean the html beforing pulling the body.
   */
  protected function cleanHtml() {
    parent::cleanHtml();
    // If the first paragraph in the content div says archive, lets remove it.
    $elem = HtmlCleanUp::matchText($this->queryPath, ".contentSub > div > p", "Archives");
    if ($elem) {
      $elem->remove();
    }

    $elem = HtmlCleanUp::matchText($this->queryPath, "table", "FOR IMMEDIATE RELEASE");
    if ($elem) {
      $elem->remove();
    }

    // Build selectors to remove.
    $selectors = array(
      "#PRhead1",
      "#navWrap",
      "#Layer3",
      "#Layer4",
      "img",
      "h1",
      ".breadcrumb",
      ".newsLeft",
      "#widget",
      "#footer",
      "a[title='Printer Friendly']",
      "a[href='#top']",
    );
    HtmlCleanUp::removeElements($this->queryPath, $selectors);
  }

  /**
   * {@inheritdoc}
   */
  protected function setDefaultObtainersInfo() {
    parent::setDefaultObtainersInfo();

    $title = new ObtainerInfo("title", 'ObtainTitlePressRelease');
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array("h2", 1));

    $this->addObtainerInfo($title);

    $subtitle = new ObtainerInfo('subtitle', "ObtainSubTitle");
    // Intentionally has no methods defined so it will not run unless a
    // migration specifies methods.
    $this->addObtainerInfo($subtitle);

    $date = new ObtainerInfo("date");
    $date->addMethod('pluckTableRow1Col2');
    $date->addMethod('pluckTableRow1Col1');
    $date->addMethod('pluckSelector', array(".BottomLeftContent", 1));
    $date->addMethod('pluckProbableDate');
    $this->addObtainerInfo($date);

    $pr_number = new ObtainerInfo('prNumber', "ObtainID");
    $pr_number->addMethod("pluckTable3y1x");
    $this->addObtainerInfo($pr_number);
  }
}
