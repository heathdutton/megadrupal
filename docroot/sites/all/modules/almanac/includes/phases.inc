<?php
// $Id: phases.inc,v 1.1 2011/02/18 10:11:53 nhwebworker Exp $

/**
 * @file
 *
 * June 2003 (updated Jan 2009))
 * authors:
 *     dan sheeler <dsheeler@cs.uchicago.edu>
 *     kumar mcmillan <kumar@chicagomodular.com>
 *
 * description:
 *     do moon phase calculation stuff
 *   You will see a slight drift in the cycle if you compare the results to other phase calculations...
 *   this is probably because of different degrees of precision among phase periods used, as well as
 *   float precision from computer to computer.  It is more or less accurate and seems to always report the
 *   correct phase name.
 */

define('MP_NEW_MOON_NAME', 'New Moon');
define('MP_NEW_MOON_ID', 0);
define('MP_WAXING_CRESCENT_NAME', 'Waxing Crescent');
define('MP_WAXING_CRESCENT_ID', 1);
define('MP_FIRST_QUARTER_NAME', 'First Quarter');
define('MP_FIRST_QUARTER_ID', 2);
define('MP_WAXING_GIBBOUS_NAME', 'Waxing Gibbous');
define('MP_WAXING_GIBBOUS_ID', 3);
define('MP_FULL_MOON_NAME', 'Full Moon');
define('MP_FULL_MOON_ID', 4);
define('MP_WANING_GIBBOUS_NAME', 'Waning Gibbous');
define('MP_WANING_GIBBOUS_ID', 5);
define('MP_THIRD_QUARTER_MOON_NAME', 'Third Quarter');
define('MP_THIRD_QUARTER_MOON_ID', 6);
define('MP_WANING_CRESCENT_NAME', 'Waning Crescent');
define('MP_WANING_CRESCENT_ID', 7);
define('MP_DAY_IN_SECONDS', 86400);

class moonPhase {

  protected $allMoonPhases = array();
  protected $dateAsTimeStamp;
  protected $moonPhaseIDforDate;
  protected $moonPhaseNameForDate;
  protected $periodInDays = 29.53058867; // == complete moon cycle
  protected $periodInSeconds = -1; // gets set when you ask for it
  protected $someFullMoonDate;

  /*
  * CONSTRUCTOR
  * $timestamp (int) date of which to calculate a moon phase and relative phases for
  */
  function moonPhase($timeStamp = -1) {
    $this->allMoonPhases = array(
        MP_NEW_MOON_NAME,
        MP_WAXING_CRESCENT_NAME,
        MP_FIRST_QUARTER_NAME,
        MP_WAXING_GIBBOUS_NAME,
        MP_FULL_MOON_NAME,
        MP_WANING_GIBBOUS_NAME,
        MP_THIRD_QUARTER_MOON_NAME,
        MP_WANING_CRESCENT_NAME);
    // set base date, that we know was a full moon:
    // (http://aa.usno.navy.mil/data/docs/MoonPhase.html)
    $this->someFullMoonDate = strtotime("July 15 2011 06:40 UTC");
    //$this->someFullMoonDate = strtotime("December 12 2008 16:37 UTC");
    if (empty($timeStamp) or $timeStamp == -1) {
      $timeStamp = time();
    }
    $this->setDate($timeStamp);
  }

  /*
  * PRIVATE
  * sets the moon phase ID and moon phase name internally
  */
  function calcMoonPhase() {
    $position = $this->getPositionInCycle();
    $phaseInfoForCurrentDate = $this->getPhaseInfo($position);
    list($this->moonPhaseIDforDate, $this->moonPhaseNameForDate) = $phaseInfoForCurrentDate;
  }

  /*
  * PUBLIC
  * return (array) all moon phases as ID => Name
  */
  public function getAllMoonPhases() {
    return $this->allMoonPhases;
  }

  /*
  * PUBLIC
  */
  public function getBaseFullMoonDate() {
    return $this->someFullMoonDate;
  }

  /*
  * PUBLIC
  * return (int) timestamp of the current date being calculated
  */
  public function getDateAsTimeStamp() {
    return $this->dateAsTimeStamp;
  }

  /*
  * PUBLIC
  */
  public function getDaysUntilNextFullMoon($round = 1) {
    $position = $this->getPositionInCycle();
    return round((1 - $position) * $this->getPeriodInDays(), $round);
  }

  /*
  * PUBLIC
  */
  public function getDaysUntilNextLastQuarterMoon($round = 1) {
    $days = 0;
    $position = $this->getPositionInCycle();
    if ($position < 0.25) {
      $days = (0.25 - $position) * $this->getPeriodInDays();
    }
    elseif ($position >= 0.25) {
      $days = (1.25 - $position) * $this->getPeriodInDays();
    }
    return round($days, $round);
  }

  /*
  * PUBLIC
  */
  public function getDaysUntilNextFirstQuarterMoon($round = 1) {
    $days = 0;
    $position = $this->getPositionInCycle();
    if ($position < 0.75) {
      $days = (0.75 - $position) * $this->getPeriodInDays();
    }
    elseif ($position >= 0.75) {
      $days = (1.75 - $position) * $this->getPeriodInDays();
    }
    return round($days, $round);
  }

  /*
  * PUBLIC
  */
  public function getDaysUntilNextNewMoon($round = 1) {
    $days = 0;
    $position = $this->getPositionInCycle();
    if ($position < 0.5) {
      $days = (0.5 - $position) * $this->getPeriodInDays();
    }
    elseif ($position >= 0.5) {
      $days = (1.5 - $position) * $this->getPeriodInDays();
    }
    return round($days, $round);
  }

  /*
  * PUBLIC
  * returns the percentage of how much lunar face is visible
  */
  public function getPercentOfIllumination() {
    // from http://www.lunaroutreach.org/cgi-src/qpom/qpom.c
    // C version: // return (1.0 - cos((2.0 * M_PI * phase) / (LPERIOD/ 86400.0))) / 2.0;
    $percentage = (1.0 + cos(2.0 * M_PI * $this->getPositionInCycle())) / 2.0;
    $percentage *= 100;
    $percentage = round($percentage, 1) . '%';
    return $percentage;
  }

  /*
  * PUBLIC
  */
  public function getPeriodInDays() {
    return $this->periodInDays;
  }

  /*
  * PUBLIC
  */
  public function getPeriodInSeconds() {
    if ($this->periodInSeconds > -1) {
      return $this->periodInSeconds; // in case it was cached
    }
    $this->periodInSeconds = $this->getPeriodInDays() * MP_DAY_IN_SECONDS;
    return $this->periodInSeconds;
  }

  /*
  * PUBLIC
  */
  public function getPhaseID() {
    return $this->moonPhaseIDforDate;
  }

  public function getPhaseInfo($position) {
    if ($position >= 0.474 && $position <= 0.53) {
      $phaseInfoForCurrentDate = array(MP_NEW_MOON_ID, MP_NEW_MOON_NAME);
    }
    elseif ($position >= 0.53 && $position <= 0.724) {
      $phaseInfoForCurrentDate = array(MP_WAXING_CRESCENT_ID, MP_WAXING_CRESCENT_NAME);
    }
    elseif ($position >= 0.724 && $position <= 0.776) {
      $phaseInfoForCurrentDate = array(MP_FIRST_QUARTER_ID, MP_FIRST_QUARTER_NAME);
    }
    elseif ($position >= 0.776 && $position <= 0.974) {
      $phaseInfoForCurrentDate = array(MP_WAXING_GIBBOUS_ID, MP_WAXING_GIBBOUS_NAME);
    }
    elseif ($position >= 0.974 || $position <= 0.026) {
      $phaseInfoForCurrentDate = array(MP_FULL_MOON_ID, MP_FULL_MOON_NAME);
    }
    elseif ($position >= 0.026 && $position <= 0.234) {
      $phaseInfoForCurrentDate = array(MP_WANING_GIBBOUS_ID, MP_WANING_GIBBOUS_NAME);
    }
    elseif ($position >= 0.234 && $position <= 0.295) {
      $phaseInfoForCurrentDate = array(MP_THIRD_QUARTER_MOON_ID, MP_THIRD_QUARTER_MOON_NAME);
    }
    elseif ($position >= 0.295 && $position <= 0.4739) {
      $phaseInfoForCurrentDate = array(MP_WANING_CRESCENT_ID, MP_WANING_CRESCENT_NAME);
    }
    return $phaseInfoForCurrentDate;
  }
  /*
  * PUBLIC
  * $ID (int) ID of phase, default is to get the phase for the current date passed in constructor
  */
  public function getPhaseName($ID = -1) {
    if ($ID <= -1) {
      return $this->moonPhaseNameForDate; // get name for this current date
    }
    return $this->allMoonPhases[$ID]; // or.. get name for a specific ID
  }

  /*
  * PUBLIC
  * return (float) number between 0 and 1.  0 or 1 is the beginning of a cycle (full moon)
  *        and 0.5 is the middle of a cycle (new moon).
  */
  public function getPositionInCycle() {
    $diff = $this->getDateAsTimeStamp() - $this->getBaseFullMoonDate();
    $periodInSeconds = $this->getPeriodInSeconds();
    $position = ($diff % $periodInSeconds) / $periodInSeconds;
    if ($position < 0) {
      $position = 1 + $position;
    }
    return $position;
  }

  /*
  * PUBLIC
  * $newStartingDateAsTimeStamp (int) set a new date to start the week at, or use the current date
  * return (array[6]) weekday timestamp => phase for weekday
  */
  public function getUpcomingWeekArray($newStartingDateAsTimeStamp = -1) {
    $newStartingDateAsTimeStamp = ($newStartingDateAsTimeStamp > -1)
        ? $newStartingDateAsTimeStamp
        : $this->getDateAsTimeStamp();
    $moonPhaseObj = get_class($this);
    $weeklyPhase = new $moonPhaseObj($newStartingDateAsTimeStamp);
    $upcomingWeekArray = array();
    for ($day = 0, $thisTimeStamp = $weeklyPhase->getDateAsTimeStamp();
                $day < 7; $day++, $thisTimeStamp += MP_DAY_IN_SECONDS) {
      $weeklyPhase->setDate($thisTimeStamp);
      $upcomingWeekArray[$thisTimeStamp] = $weeklyPhase->getPhaseID();
    }
    unset($weeklyPhase);
    return $upcomingWeekArray;
  }

  /*
  * PUBLIC
  * sets the internal date for calculation and calulates the moon phase for that date.
  * called from the constructor.
  * $timeStamp (int) date to set as unix timestamp
  */
  public function setDate($timeStamp = -1) {
    if (empty($timeStamp) or $timeStamp == -1) {
      $timeStamp = time();
    }
    $this->dateAsTimeStamp = $timeStamp;
    $this->calcMoonPhase();
  }

}