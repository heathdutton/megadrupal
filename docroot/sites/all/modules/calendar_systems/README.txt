=== DESCRIPTION  ===

  Support for various calendar systems like Jalali, Gregorian, Hijir, Hebew, Thai, etc.

  - Supports Iranian/Persian , Arabic/Hijri/Islamic, Thai, Gregorian Calendar systems
  - Integration with Drupal's node "authored on" and "admin authored" fields
  - Supports Single language websites
  - Views Support including views built-in date filters
  - Compatible with all Drupal standard forms out of the box
  - Supports Multi language websites via locale module. Each language can have a different calendar system
  - Integration with jquery world calendars date picker
  - Supports all date module's widgets
  - Scheduler module support (If set to use standard text field)
  - API for third party usage

=== INSTALLATION ===

  - Install and enable the module as usual: http://drupal.org/node/70151

  - Add the following code right after "$timezones = &$drupal_static_fast['timezones'];" in "/includes/common.inc" file
    or apply the patch located in patches/ in module's directory. if you don't know how to apply patches, there is a
    good guide here: http://drupal.org/patch/apply

  // Calendar Systems module new hook (The actual hook is hook_format_date this one is only a workaround to 
  // prevent incompatibility with modules that already have a function called module_name_format_date)
  foreach (module_implements('format_date_calendar_systems') AS $module) {
    $function = $module .'_format_date_calendar_systems';
    $r = $function($timestamp, $type, $format, $timezone, $langcode);
    
    if ($r != FALSE) {
      return $r;
    }
  }

  - Goto "admin/config/regionals/calendar-systems" and configure your profiles.
  - Optionally you can download and install JQuery Calendar API (http://drupal.org/project/jquery_calendar) module to add date popup support
    Will be automatically actived for popup date fields
  
=== API ===

calendar_systems_get_calendar_instance

  You can use calendar_systems_get_calendar_instance($calendar_system = NULL, $language = NULL) to get and instance
  of a calendar system.
  For exmaple to get an instance of iranian calendar system : 
  $calendar = calendar_systems_get_calendar_instance('iranian');
  $calendar->date('Y-m-d',time());

Available methods :
  $calendar->timestampToStr($format, $timestamp=null);
  $calendar->strToTimestamp($string);
  $calendar->timestampToInfoArray($timestamp=null);
  $calendar->infoArrayToTimestamp($arr);
  $calendar->date($format, $maket=null);
  $calendar->fromGregorian($g_y, $g_m, $g_d);
  $calendar->toGregorian($j_y, $j_m, $j_d);
  $calendar->smartGet($type,$value="now");
  $calendar->makeTime($hour="",$minute="",$second="",$jmonth="",$jday="",$jyear="");
  $calendar->isDateValid($month,$day,$year);
  $calendar->dateDiff($first,$second);

Converting from one calendar system to another :
  $iranian_calendar = calendar_systems_get_calendar_instance('iranian');
  $val = $iranian_calendar->strToTimestamp('1380-05-10');
  $arabic_calendar->date('Y-m-d',$val);

calendar_systems_get_calendar_system_name
  calendar_systems_get_calendar_system_name()
  Result : 'default'
  
  calendar_systems_get_calendar_system_name(NULL, 'fa')
  Result : 'iranian'


=== Extend : Plugin System and adding new calendar system ===

Calendar systems mopdule has a plugable architecure which means that each calendar system is a plugin
new calendar system can easily be added.
Plugins are located at calendar\v1\calendarSystems, they will be automatically detected and included by
Caledanr systems module once put on this location.
Copy and rename one of the calendar system in this folder and start implementing you new calendar :)

=== Support ===

  Found a bug? report it here http://drupal.org/node/add/project-issue/calendar_systems

AUTHORS AND MAINTAINERS
=======================

  Sina Salek - http://sina.salek.ws
  Sepehr Lajevardi - D7 co-maintainer
