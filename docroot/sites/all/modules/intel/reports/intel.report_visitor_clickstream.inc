<?php
/**
 * @file
 * Generates visitor clickstream report
 * 
 * @author Tom McCracken <tomm@getlevelten.com>
 */

require_once './' . drupal_get_path('module', 'intel') . "/includes/intel.ga.inc";

function intel_visitor_clickstream($visitor) {
  //intel_add_report_headers();
  $visitorid = substr($visitor->vtk, 0 , 20);
  $steps = array(
    '_visits' => array(),
    '_pages' => array(),
  );
  
  list($start_date, $end_date, $number_of_days) = _intel_get_report_dates("-90 days", "Now");
  $request = array(
    'dimensions' => array('ga:hostname', 'ga:pagePath', 'ga:pageTitle', 'ga:visitLength', 'ga:visitCount', 'ga:customVarValue4'),
    'metrics' => array('ga:entrances', 'ga:pageviews', 'ga:bounces', 'ga:timeOnSite', 'ga:timeOnPage', 'ga:goalValueAll'),
    'sort_metric' => 'ga:customVarValue4',
    'start_date' => $start_date,
    'end_date' => $end_date,
    'segment' => 'dynamic::ga:customVarValue5==' . $visitorid,
    //'segment' => 'dynamic::ga:customVarValue5==1337759660',
  );

  $data = google_analytics_api_report_data($request);
//dsm($request);  dsm($data);

  $visits = array();
  $visitindex = 0;
  if (!empty($data->results) && is_array($data->results)) {
    foreach ($data->results AS $row) {
      $ts = (int)$row['customVarValue4'];
      if (!isset($steps[$ts])) { 
        $steps[$ts] = array(); 
      }      
      if (!isset($steps['_visits'][$row['visitCount']])) {
        $steps[$ts]['visitindex'] = $visitindex;
        $steps[$ts]['visit'] = array('events' => _intel_init_events_array());
        $steps[$ts]['visit']['timestamp'] = $ts;
        $steps[$ts]['visit']['visitCount'] = $row['visitCount'];
        $steps[$ts]['visit']['visitLength'] = $row['visitLength'];
        $steps[$ts]['visit']['timeOnSite'] = $row['timeOnSite'];         
        $steps[$ts]['visit']['sticks'] = 1 - $row['bounces']; 
        $steps[$ts]['visit']['pageviews'] = 0;
        $steps[$ts]['visit']['goalValueAll'] = $row['goalValueAll'];
        $steps['_visits'][$row['visitCount']] = $ts;  
        $visitindex = $ts;
      }
      $steps[$ts]['visitindex'] = $visitindex;
      if (!isset($steps[$ts]['page'])) { 
        $steps[$ts]['page'] = array('events' => _intel_init_events_array()); 
      }      
      $steps[$ts]['page']['hostname'] = $row['hostname'];
      $steps[$ts]['page']['pagePath'] = $row['pagePath'];
      $steps[$ts]['page']['pageTitle'] = $row['pageTitle'];
      $steps[$ts]['page']['timeOnPage'] = $row['timeOnPage'];
      $steps[$ts]['page']['goalValueAll'] = $row['goalValueAll'];
      $steps[$ts]['page']['eventValueAll'] = 0;
      if (!isset($steps['_pageindex']["{$row['hostname']}{$row['pagePath']}"])) {
        $steps['_pages']["{$row['hostname']}{$row['pagePath']}"][$ts] = $ts;
      }
      $steps[$visitindex]['visit']['pageviews']++;
    }
  }
  else {
    $msg = t('Sorry, no data was returned for visitor id %vid. Report cannot be generated.', array('%vid' => $visitorid));
    drupal_set_message($msg, 'error');
    return '';
  }
//dsm($steps);
  
  //$request['dimensions'] = array('ga:eventCategory', 'ga:eventAction', 'ga:eventLabel', 'ga:hostname', 'ga:pagePath', 'ga:customVarValue4');
  $request['dimensions'] = array('ga:eventCategory', 'ga:eventAction', 'ga:hostname', 'ga:pagePath', 'ga:customVarValue4');
  $request['metrics'] = array('ga:totalEvents', 'ga:uniqueEvents', 'ga:eventValue');
  $data = google_analytics_api_report_data($request);
//dsm($data);
  if (!empty($data->results) && is_array($data->results)) {
    foreach ($data->results AS $row) {
      $ts = (int)$row['customVarValue4'];  
      $pageindex = _intel_get_page_step_index($steps, $ts, $row['hostname'], $row['pagePath']);
      $visitindex = _intel_get_visit_step_index($steps, $ts);     
      if (!isset($steps[$ts]['visitindex'])) {
        $steps[$ts]['visitindex'] = $visitindex;
      } 
      if (!isset($steps[$ts])) { 
        $steps[$ts] = array();
      }
      if (!isset($steps[$ts]['events'])) {
        $steps[$ts]['events'] = _intel_init_events_array();
      }
 
      $event = array(
        'timestamp' => $ts,
        'visitindex' => $visitindex,
        'pageindex' => $pageindex,
        'hostname' => $row['hostname'],
        'pagePath' => $row['pagePath'],
        'eventCategory' => $row['eventCategory'],
        'eventAction' => $row['eventAction'],
        //'eventLabel' => $row['eventLabel'],
        'eventLabel' => '',
        'eventValue' => $row['eventValue'],
        'is_valuedevent' => (substr($row['eventCategory'], -1) == '!') ? 1 : 0,
        'value' => (substr($row['eventCategory'], -1) == '!') ? $row['eventValue'] : 0,
      );
      $steps[$ts]['events'][] = $event; 
      $steps[$ts]['events']['_all'] = _intel_update_event_all_values($steps[$ts]['events']['_all'], $event);
      $steps[$pageindex]['page']['events'][] = $event;
      $steps[$pageindex]['page']['events']['_all'] = _intel_update_event_all_values($steps[$pageindex]['page']['events']['_all'], $event);
      $steps[$visitindex]['visit']['events'][] = $event;
      $steps[$visitindex]['visit']['events']['_all'] = _intel_update_event_all_values($steps[$visitindex]['visit']['events']['_all'], $event);             
    }
  }
  
  // this is rediculous! GA won't return events without a label if you use eventLabel as a dimention
  $request['dimensions'] = array('ga:eventCategory', 'ga:eventAction', 'ga:eventLabel', 'ga:hostname', 'ga:pagePath', 'ga:customVarValue4');
  $request['metrics'] = array('ga:totalEvents', 'ga:uniqueEvents', 'ga:eventValue');
  $data = google_analytics_api_report_data($request);
//dsm($data);
  if (!empty($data->results) && is_array($data->results)) {
    foreach ($data->results AS $row) {
      $ts = (int)$row['customVarValue4'];       
      $pageindex = _intel_get_page_step_index($steps, $ts, $row['hostname'], $row['pagePath']);
      $visitindex = _intel_get_visit_step_index($steps, $ts);
      foreach ($steps[$ts]['events'] AS $i => $event) {
        if ((substr($i, 0, 1) != '_') && ($event['eventCategory'] == $row['eventCategory']) && ($event['eventAction'] == $row['eventAction'])) {
          $steps[$ts]['events'][$i]['eventLabel'] = $row['eventLabel'];
        }
      }
      foreach ($steps[$pageindex]['page']['events'] AS $i => $event) {
        if ((substr($i, 0, 1) != '_') && ($event['eventCategory'] == $row['eventCategory']) && ($event['eventAction'] == $row['eventAction'])) {
          $steps[$pageindex]['page']['events'][$i]['eventLabel'] = $row['eventLabel'];
        }
      }
      foreach ($steps[$visitindex]['visit']['events'] AS $i => $event) {
        if ((substr($i, 0, 1) != '_') && ($event['eventCategory'] == $row['eventCategory']) && ($event['eventAction'] == $row['eventAction'])) {
          $steps[$visitindex]['visit']['events'][$i]['eventLabel'] = $row['eventLabel'];
        }
      }
    }
  }  
  

  $request['dimensions'] = array('ga:source', 'ga:referralPath', 'ga:medium', 'ga:keyword', 'ga:socialNetwork', 'ga:campaign', 'ga:customVarValue4');
  $request['metrics'] = array('ga:entrances'); 
  $data = google_analytics_api_report_data($request);
//dsm($data);
  if (!empty($data->results) && is_array($data->results)) {
    foreach ($data->results AS $row) {
      $ts = (int)$row['customVarValue4']; 
      $referrer = array(
        'source' => $row['source'],
        'referralPath' => $row['referralPath'],
        'medium' => $row['medium'],
        'keyword' => $row['keyword'],
        'socialNetwork' => $row['socialNetwork'],
        'campaign' => $row['campaign'],
      );
      $steps[$ts]['referrer'] = $referrer;     
    }
  }
  
  $request['dimensions'] = array('ga:browser', 'ga:browserVersion', 'ga:operatingSystem', 'ga:operatingSystemVersion', 'ga:language', 'ga:screenResolution', 'ga:customVarValue4');
  $request['metrics'] = array('ga:entrances'); 
  $data = google_analytics_api_report_data($request);
//dsm($data);
  if (!empty($data->results) && is_array($data->results)) {
    foreach ($data->results AS $row) {
      $ts = (int)$row['customVarValue4'];
      $steps[$ts]['environment'] = array();
      $steps[$ts]['environment']['browser'] = $row['browser'];
      $steps[$ts]['environment']['browserVersion'] = $row['browserVersion'];
      $steps[$ts]['environment']['operatingSystem'] = $row['operatingSystem'];
      $steps[$ts]['environment']['operatingSystemVersion'] = $row['operatingSystemVersion'];
      $steps[$ts]['environment']['language'] = $row['language'];
      $steps[$ts]['environment']['screenResolution'] = $row['screenResolution'];
    }
  }
  
  $request['dimensions'] = array('ga:isMobile', 'ga:mobileDeviceBranding', 'ga:mobileDeviceModel', 'ga:mobileDeviceInfo', 'ga:customVarValue4');
  $request['metrics'] = array('ga:entrances'); 
  $data = google_analytics_api_report_data($request);
//dsm($data);
  if (!empty($data->results) && is_array($data->results)) {
    foreach ($data->results AS $row) {
      $ts = (int)$row['customVarValue4'];
      $steps[$ts]['environment']['isMobile'] = $row['isMobile'];
      $steps[$ts]['environment']['mobileDeviceBranding'] = $row['mobileDeviceBranding'];
      $steps[$ts]['environment']['mobileDeviceModel'] = $row['mobileDeviceModel'];
      $steps[$ts]['environment']['mobileDeviceInfo'] = $row['mobileDeviceInfo'];
    }
  }
  
  
  $request['dimensions'] = array('ga:country', 'ga:region', 'ga:city', 'ga:metro', 'ga:latitude', 'ga:longitude', 'ga:customVarValue4');
  $request['metrics'] = array('ga:entrances'); 
  $data = google_analytics_api_report_data($request);
//dsm($data);
  if (!empty($data->results) && is_array($data->results)) {
    foreach ($data->results AS $row) {
      $ts = (int)$row['customVarValue4'];
      $steps[$ts]['location'] = array();
      $steps[$ts]['location']['country'] = $row['country'];
      $steps[$ts]['location']['region'] = $row['region'];
      $steps[$ts]['location']['city'] = $row['city'];
      $steps[$ts]['location']['metro'] = $row['metro'];
      $steps[$ts]['location']['latitude'] = $row['latitude'];
      $steps[$ts]['location']['longitude'] = $row['longitude'];
    }
  }
  
  ksort($steps);

  $output = ''; 
  
  $vars = array(
    'steps' => $steps,
    'visitorid' => $visitorid,
  );

  $output .= theme_intel_visitor_clickstream($vars);

  $steps = intel_score_data($steps, 'steps');
  
  return $output;  
}

function theme_intel_visitor_clickstream($variables) {
//dsm($variables);
  $steps = $variables['steps'];
  $visitorid = $variables['visitorid'];
//dsm($steps);
  
  drupal_add_css(drupal_get_path('module', 'intel') . '/css/intel.report.css');
  //drupal_add_js(drupal_get_path('module', 'intel') . '/intel.report.js');
  
  $tldata = array(
    'dates' => array(),
    'eras' => array(),
  );
  $output = <<<EOD
<style>
  //#visitor-report div { margin: 0; }
 .visit { clear: left; padding-top: 20px; width: 100%; }
 .summary { float: left; margin-left: 2%; padding-top: 5px; width: 29%; }
 .details { float: left; padding-top: 5px; width: 69%; }
 .chronology { width: 100%; clear: left; } 
  

 .report-box {
    margin-top: 5px;
    border-collapse: collapse;
    border-top-color: #ccc;
    border-right-color: #ddd;
    border-bottom-color: #ddd;
    border-left-color: #ddd;
    border-width: 1px;
    border-style: solid; 
  }
 .report-box-body {
    padding-left: 2%;
    padding-right: 2%;
    padding-bottom: 5px; 
    padding-bottom: 5px;   
  }
 .location .report-box-body { padding-right: 0px; padding-bottom: 0px; }
 .location .text { float: left; width: 50%; }
 .location .map { float: left; width: 50%; height: 100px; }
 .map-canvas { width: 100%; height: 100%; }
  
 .pageview {width: 100%; clear: left; }
 .pageview .item {width: 82%; float: left; padding-left: 0%; }
 .time {width: 8%; float: left; text-align: center; }
 .value {width: 6%; float: left; text-align: center; }
 .event {width: 100%; clear: left; }
 .event .item {width: 80%; float: left; padding-left: 2%;}
 .event-category-cta-impression { color: #BBB; }
 .event-category-cta-impression a { color: #95CDDB; }
 .event-valued { font-weight: bold; color: #333; }
</style>

EOD;
  $output .= '<div id="intel-report"><div id="visitor-report">' . "\n";

  $locations = array();
  $locstr = '';
  $vi = 0;
  $col1 = $col2 = '';
  $vouts = array();
  $vout = '';
  $last_visit_ts = 0;
  $timedata = array();
  $startday = '';
  $endday = '';
  $visit_ts = 0;
  $page_ts = 0;
  $tldata = array(
    'visits' => array(),
    'pages' => array(),
    'events' => array(),
  );  // tag, start
  foreach ($steps AS $ts => $step) {
    if (substr($ts, 0, 1) == '_') {
      continue;
    }
    $day = date('Y-m-d', $ts);
    if (!$startday) {
      $startday = $day;
    }
    $endday = $day;
    if (isset($step['visit'])) {
      
      if ($vi) { 
        $col2 .= "        <div style=\"clear: left;\"></div>\n";
        $col2 .= "      </div>\n";
        $col2 .= "    </div>\n";
        $col2 .= "  </div>\n";
        $vout .= $col2 . $col1;
        //$col1 = $col2 = '';
        $vout .= "</div>\n"; 
        $vouts[$visit_ts] = $vout;
        $vout = '';  
      }
      $visit_ts = $ts;
      $vi ++;
      $tldata['visits'][$ts] = _intel_init_tl_visit($step, $ts, $vi);
      $vout .= '<div class="visit">' . "\n";
      $vout .= '  <h3>' . t('Visit') . " " . $step['visit']['visitCount'] . ': ' . date("D F j, Y, g:i a", $ts) . "</h3>\n";
      $col1 = '  <div class="summary">' . "\n";

      $col1 .= '    <div class="referrer report-box">' . "\n";
      $col1 .= '      <h4>' . t('Referrer') . "</h4>\n";
      $col1 .= '      <div class="environment report-box-body">' . "\n";
      $col1 .= theme_intel_visitor_referrer($step, '        ', $tldata['visits'][$ts]);
      $col1 .= '      </div>' . "\n";
      $col1 .= '    </div>' . "\n";      
      
      if (!empty($step['location'])) {
        $lat = $step['location']['latitude'];
        $lon = $step['location']['longitude'];
        if (empty($locations["$lat,$lon"])) {
          $locations["$lat,$lon"] = array(
            'lat' => $lat,
            'lon' => $lon,
            'name' => $step['location']['metro'],
            //'country' => $step['location']['country'],
            'count' => 1,
          );
        }
        else {
          $locations["$lat,$lon"]['count'] ++;
        }
        $col1 .= '    <div class="location report-box">' . "\n";
        $col1 .= '      <h4>' . t('Location') . "</h4>\n";
        $col1 .= '      <div class="report-box-body">' . "\n";
        $col1 .= '        <div class="text">' . "\n";
        $col1 .= '          ' . $step['location']['city'] . ', ' . $step['location']['region'] . "<br />\n";
        $col1 .= '          ' . $step['location']['country'] . "<br /> <br />\n";
        $col1 .= '          ' . $step['location']['metro'] . "<br />\n";
        $col1 .= '        </div>' . "\n";
        $col1 .= '        <div class="map">' . "\n";
        $col1 .= theme_intel_visitor_map($locations["$lat,$lon"], array(), '          ');
        $col1 .= '        </div>' . "\n";
        $col1 .= '        <div style="clear: left;"></div>' . "\n";
        $col1 .= '      </div>' . "\n";
        $col1 .= '    </div>' . "\n";        
      }
      $col1 .= '    <div class="environment report-box">' . "\n";
      $col1 .= '      <h4>' . t('System') . "</h4>\n";
      $col1 .= '      <div class="report-box-body">' . "\n";
      $col1 .= theme_intel_visitor_environment($step, '        ');
      $col1 .= '      </div>' . "\n";
      $col1 .= '    </div>' . "\n";
      
      $col1 .= '  </div>' . "\n"; // end summary column
      
      $col2 = '  <div class="details">' . "\n";

      $col2 .= '    <div class="chronology report-box">' . "\n";
      $col2 .= '      <h4>' . t('Chronology') . "</h4>\n";
      $col2 .= '      <div class="chronology report-box-body">' . "\n";
    }
    //$time = '""';
    //if ($ts != $visit_ts) {
      $time = "+" . _intel_format_delta_time($ts - $step['visitindex']);
    //}
    $s = '        ';
    if (!empty($step['page'])) {
      $col2 .= $s . '<div class="pageview">' . "\n";
      $col2 .= $s . '  <div class="time">' . $time . "</div>\n";      
      $col2 .= $s . '  <div class="item">' . l($step['page']['pageTitle'], "http://" . $step['page']['hostname'] . $step['page']['pagePath']) . "</div>\n";
      //$col2 .= $s . '  <div class="value">' . $step['page']['goalValueAll'] . "</div>\n";
      $col2 .= $s . '  <div class="value">' . '-' . "</div>\n";
      $col2 .= $s . '</div>' . "\n";
      $vars = array('text' => "time: $time");
      $tldata['pages'][$ts] = _intel_init_tl_page($step, $ts, $visit_ts, $vi, $vars);
      $page_ts = $ts;
    }
    if (!empty($step['events']) && is_array($step['events'])) {

      $etime = '"';
      if ($ts != $page_ts) {
        $etime = "+" . _intel_format_delta_time($ts - $step['visitindex']);
      }
      else {
        $tlevents = '';
      }
      $ignore_events = array(
        'cta impression'
      );
      $label_is_link = array(
        'cta impression',
        'cta click',
        'cta click!',
        'cta conversion',
        'cta conversion!',
        'landing page view',
        'landing page conversion',
        'landing page conversion!',
        'form submission',
        'form submission!',
      );
      foreach ($step['events'] AS $i => $event) {
        if (substr($i, 0 , 1) == '_') {
          continue;
        }
        $hostpath = $event['hostname'] . $event['pagePath'];
        $item = $event['eventCategory'] . ": " . $event['eventAction'];
        if (in_array(strtolower($event['eventCategory']), $label_is_link)) {
          if (strpos($event['eventLabel'], '://') !== FALSE) {
            $url = $event['eventLabel'];
          }
          else {
            $url = "http://" . $event['hostname'] . '/' . $event['eventLabel'];
          }
          $l = l($event['eventAction'], $url);
          $item = $event['eventCategory'] . ": " . $l;
        }
        $value = '-';
        if ($event['eventValue']) {
          $value = "$" . number_format($event['eventValue'], 2);
        }
        $cat = drupal_clean_css_identifier("event category " . strtolower($event['eventCategory']));
        $valued = (!empty($event['eventValue'])) ? ' event-valued' : '';
        $col2 .= $s . "<div class=\"event $cat$valued\">" . "\n";
        $col2 .= $s . '  <div class="time">' . $etime . "</div>\n";        
        $col2 .= $s . '  <div class="item">' . $item . "</div>\n";
        $col2 .= $s . '  <div class="value">' . $value . "</div>\n";
        $col2 .= $s . '</div>' . "\n";
       
        if (strtolower($event['eventCategory']) == 'cta impression') {
          _intel_tl_add_event_to_page($tldata, $event['pageindex'], $l, 'ctaimpressions');
        }
        else {
          if (!in_array(strtolower($event['eventCategory']), $ignore_events)) {
            //if (strtolower($event['eventCategory']) == 'cta click') {
              // TODO special stuff here
            //}
            //else {
              $type = 'events';
              if (substr($event['eventCategory'], -1) == '!') {
                $type = 'valuedevents';
              }
              if (substr($event['eventCategory'], -1) == '+') {
                $type = 'goals';
              }
              $text = $item . (($value != '-') ? " +$value" : '');
              $vars = array(
                'text' => $text,
                'valuetext' => (($value != '-') ? "+$value" : ''),
              );
              _intel_tl_add_event_to_page($tldata, $event['pageindex'], $text, $type);
              // only show events that don't occur on a page
              if (!empty($step['page'])) {
                //$tldata['events'][$ts][] = _intel_init_tl_event($step, $i, $ts, $visit_ts, $vi, $vars);
              }
            }
          //}
        } 
      }
    }
  }
  $col2 .= "        <div style=\"clear: left;\"></div>\n";
  $col2 .= "      </div>\n";
  $col2 .= "    </div>\n";
  $col2 .= "  </div>\n";
  $vout .= $col2 . "\n" . $col1;
  $vout .= "</div>\n"; 
  $vouts[$visit_ts] = $vout;
  ksort($vouts);
  foreach ($vouts AS $vout) {
    $output .= $vout;
  }
  $output .= '</div></div>'; // close id=visitor-report div
  //if (isset($tldate)) {
  //  if ($tlevents) {
  //    $tldate['text'] = '<strong>' . t('Events') . ':</strong>' . "<br />\n" . $tlevents ;
  //  }
  //  $tldata['dates'][] = $tldate;
  //}  
  
$visitor = intel_visitor_load($visitorid);
$data = array();
$start = date("m/d/Y", strtotime("$startday -1 day"));
$end = date("m/d/Y", strtotime("$endday +1 day"));
$charts = array(
  "startDate" => $start,
  "endDate" => $end,
  "headline" => "Headline Goes Here, Not sure what this is for",
  "value" => "28",  // not sure what this is for
);

$tf = array(
  '@pages' => format_plural(count($steps['_pages']), '1 page', '@count pages'),
  '@visits' => format_plural(count($steps['_visits']), '1 visit', '@count visits'),
);
$data['timeline'] = array(
  'headline' => t("%name clickstream", array('%name' => $visitor->label())),
  'type' => "default",
  'text' => t("viewed @pages in @visits in the last 90 days.", $tf),
  'date' => array(),
  'era' => array(),
  'chart' => $charts, 
);
$vdata = array();
foreach ($tldata['visits'] AS $ts => $visit) {
  $vdata[$visit['visitindex']]['timeline'] = $data['timeline'];
}
//dsm($vdata);
$visit_start_slides = array();
$i = 1;
foreach ($tldata['visits'] AS $ts => $visit) {
  $data['timeline']['date'][] = $visit;
  $data['timeline']['era'][] = array(
    'startDate' => $visit['startDate'],
    'endDate' => $visit['endDate'],
    'headline' => $visit['headline'],
    'tag' => 'visits',
  );
  $vdata[$visit['visitindex']]['timeline']['date'][] = $visit;
}

foreach ($tldata['pages'] AS $ts => $page) {
  if (!empty($page['goals'])) {
    $page['text'] .= "<br>\n<br>\n<strong>" . t('Goals') . "</strong><br>\n" . implode("<br>\n", $page['goals']);
  }
  if (!empty($page['valuedevents'])) {
    $page['text'] .= "<br>\n<br>\n<strong>" . t('Valued events') . "</strong><br>\n" . implode("<br>\n", $page['valuedevents']);
  }
  if (!empty($page['events'])) {
    $page['text'] .= "<br>\n<br>\n<strong>" . t('Events') . "</strong><br>\n" . implode("<br>\n", $page['events']);
  }
  if (!empty($page['ctaimpressions'])) {
    $page['text'] .= "<br>\n<br>\n<strong>" . t('CTA impressions') . "</strong><br>\n" . implode("<br>\n", $page['ctaimpressions']);
  }
  

  unset($page['ctaimpressions']);
  unset($page['events']);
  unset($page['valuedevents']);
  unset($page['goals']);
  unset($page['hostpath']);
  $data['timeline']['date'][] = $page;
  $vdata[$page['visitindex']]['timeline']['date'][] = $page;
}

$lastts = 0;
foreach ($tldata['events'] AS $ts => $events) {
  foreach ($events AS $event) {
    $data['timeline']['date'][] = $event;
    $vdata[$event['visitindex']]['timeline']['date'][] = $page;
    break;
  }
}

usort($data['timeline']['date'], '_intel_sort_tldata');

$visit_starts = array();
$start_at_slide = 0;
foreach ($data['timeline']['date'] AS $i => $slide) {
  if (!empty($slide['visitcount'])) {
    $visit_starts[$slide['visitcount']] = $i;
    if (!empty($_GET['visit-count']) && ($_GET['visit-count'] == $slide['visitcount'])) {
      $start_at_slide = $i+1;
    }
  }
}
//dsm($start_at_slide);
//dsm($tldata);
//dsm($data);
//dsm($vdata);
$json = drupal_json_encode($data);

//$json = drupal_json_encode($vdata[3]);
 
$script = <<<EOT5
jQuery(document).ready(function() {
  var width = jQuery('#main-timeline').width();
  var dataObject = {$json};
  createStoryJS({
   type:       'timeline',
   width:      width,
   height:     '600',
   source:     dataObject,
   embed_id:   'main-timeline',
   start_at_slide: $start_at_slide
   });
});
EOT5;
  $options = array('type' => 'file', 'weight' => -1, 'preprocess' => FALSE);

  // older TimelineJS library put compiled files under compiled.
  if (file_exists(libraries_get_path('TimelineJS') . '/compiled/js/storyjs-embed.js')) {
    drupal_add_js(libraries_get_path('TimelineJS') . '/compiled/js/storyjs-embed.js', $options);
  }
  else {
    drupal_add_js(libraries_get_path('TimelineJS') . '/build/js/storyjs-embed.js', $options);
  }
  drupal_add_js($script, array('type' => 'inline', 'scope' => 'header'));
  //drupal_add_js($script, array('type' => 'inline'));
  
  $top = '<div id="main-timeline"></div>';
  $output = $top . $output;

  return $output;
}

function _intel_sort_tldata($a, $b) {
  return ($a['startDate'] > $b['startDate']) ? 1 : -1; 
}

function _intel_init_tl_visit($step, $ts, $visitindex) {
  static $visit_count;
  $visit_count++;
  $ret = array(
    'startDate' => _intel_format_timelinejs_timestamp($ts-1),
    'endDate' => _intel_format_timelinejs_timestamp($ts + $step['visit']['visitLength']),
    'headline' => 'Visit ' . $step['visit']['visitCount'],
    'text' => '',
    'tag' => 'visits',
    'asset' => array(),
    'visitindex' => $visitindex,
    'visitcount' => $step['visit']['visitCount'],
  );
  return $ret;
}

function _intel_init_tl_page($step, $ts, $visit_ts, $visitindex, $vars = array()) {
  // TODO quite a hackish way to filter off end of page titles
  $site_name = variable_get('site_name', "notgonnabeaname");
  $headline = $step['page']['pageTitle'];
  //$headline = str_replace(" | $site_name", "", $headline);
  //$headline = str_replace("$site_name", "", $headline);
  $ret = array(
    'startDate' => _intel_format_timelinejs_timestamp($ts),
    'endDate' => _intel_format_timelinejs_timestamp($ts + $step['page']['timeOnPage']),
    'headline' => $headline,
    'text' => '',
    'tag' => 'pages',
    'asset' => array(
       'media' => "http://" . $step['page']['hostname'] . $step['page']['pagePath'],
       'thumbnail' => 'http://free.pagepeeker.com/v2/thumbs.php?size=x&url=' . $step['page']['hostname'] . $step['page']['pagePath'],
    ),
    'ctaimpressions' => array(),
    'events' => array(),
    'valuedevents' => array(),
    'goals' => array(),
    'originevents' => array(),
    'hostpath' => $step['page']['hostname'] . $step['page']['pagePath'], 
    'visitindex' => $visitindex,    
  );
  $d = $ts - $step['visitindex'];
  $text = "session: +" . _intel_format_delta_time($ts - $step['visitindex']) . ', on-page: ' . _intel_format_delta_time($step['page']['timeOnPage']);
  $ret['text'] = $text;
  return $ret;
}

function _intel_init_tl_event($step, $i, $ts, $visit_ts, $visitindex, $vars = array()) {
  $headline = $step['events'][$i]['eventCategory'] . ": " . $step['events'][$i]['eventAction'];
  $d = $ts - $step['visitindex'];
  if ($d) {
    $text = "session time: +" . _intel_format_delta_time($ts - $step['visitindex']);
  }
  $tag = 'events';
  if (substr($step['events'][$i]['eventCategory'], -1) == '!') {
    $tag = 'valued events';
    $headline = $vars['valuetext'] . " " . $headline;
  }
  $ret = array(
    'startDate' => _intel_format_timelinejs_timestamp($ts+1),
    'endDate' => _intel_format_timelinejs_timestamp($ts+1),
    'headline' => $headline,
    'text' => $text . ((!empty($vars['$text'])) ? "<br>\n<br>\n" . $vars['$text'] : ""),
    'tag' => $tag,
    'asset' => array(
      'media' => (!empty($vars['media'])) ? $vars['media'] : "http://" . $step['events'][$i]['hostname'] . $step['events'][$i]['pagePath'],
    ),
    'visitindex' => $visitindex,
  );
  return $ret;
}

function _intel_tl_add_event_to_page(&$tldata, $ts, $text, $type = 'events', $originevent = array()) {
  $vc = 0;
  $tldata['pages'][$ts][$type][] = $text;
  if (count($tldata['pages'][$ts]['goals'])) {
    //$tldata['pages'][$ts]['tag'] = 'goals';
  }
  elseif (count($tldata['pages'][$ts]['valuedevents'])) {
    //$tldata['pages'][$ts]['tag'] = 'valued events';
  }
  if (!empty($originevent)) {
    $type = strtolower($originevent['eventCategory']);
    if (!isset($tldata['pages'][$ts]['originevents'][$type])) {
      $tldata['pages'][$ts]['originevents'][$type] = array();
    }
    $tldata['pages'][$ts]['originevents'][$type][] = $originevent;
  }
}

function _intel_format_timelinejs_timestamp($time) {
  //return date("Y,m,d H,i,s", $time);
  return date("m/d/Y H:i:s", $time);
}

function theme_intel_visitor_environment($step, $s = '') {
  $output = '';
  $output .= $s . t('browser') . ': ' . $step['environment']['browser'] . " v" . $step['environment']['browserVersion'] . "<br />\n";
  $output .= $s . t('operating system') . ': ' . $step['environment']['operatingSystem'] . " " . $step['environment']['operatingSystemVersion'] . "<br />\n";
  if (!empty($step['environment']['mobileDeviceInfo'])) {
    $output .= $s . t('mobile device') . ': ' . $step['environment']['mobileDeviceInfo'] . "<br />\n";
  }
  $output .= $s . t('screen resolution') . ': ' . $step['environment']['screenResolution'] . "<br />\n";
  $output .= $s . t('language') . ': ' . $step['environment']['language'] . "<br />\n";
  return $output;
}

function theme_intel_visitor_referrer($step, $s = '', &$tlvisit) {
  $output = '';
  $tldata_link = '';
  //$tlreferrer = '';
  $tltext = (($tlvisit['text']) ? "<br>\n<br>\n" : '') . '<strong>' . t('Referrer') . "</strong><br>\n";
  $dldate = array();
  $ref_alts = array(
    'http://google.com/search?q=(not provided)' => 'http://google.com/search',
    'http://facebook.com' => 'http://www.facebook.com'
  );
  if ($step['referrer']['medium'] == '(none)') {
    $tltext .= t("Source") . ': ' . $step['referrer']['source'];
    $output .= $s . t('source') . ': ' . $step['referrer']['source'] . "<br />\n";
  }
  elseif ($step['referrer']['medium'] == 'referral') {
    $tltext .= t("Source") . ': ' . $step['referrer']['medium'];
    $output .= $s . t('source') . ': ' . $step['referrer']['medium'] . "<br />\n";
  } 
  else {
    $tltext .= t("Source") . ': ' . $step['referrer']['medium'] . ' / ' . $step['referrer']['source'];
    $output .= $s . t('source') . ': ' . $step['referrer']['medium'] . ' / ' . $step['referrer']['source'] . "<br />\n";
  }
  if ($step['referrer']['medium'] != '(none)') {
    if ($step['referrer']['medium'] == 'organic') {
      $output .= $s . t('keyword') . ': ' . $step['referrer']['keyword'] . "<br />\n";
      if ($step['referrer']['source'] == 'google') {
        $tltext .= "<br>\n" . t("Keyword") . ": " . $step['referrer']['keyword'];
        $tldata_link = 'http://google.com/search?q=' . $step['referrer']['keyword'];
      }
    }
    if ($step['referrer']['medium'] == 'referral') {
      $url = $step['referrer']['source'] . $step['referrer']['referralPath'];
      $l = l($url, "http://" . $url, array('attributes' => array('target' => $step['referrer']['source'])));
      $output .= $s . t('path') . ': ' . $l . "<br />\n";
      $tldata_link = "http://" . $step['referrer']['source'] . $step['referrer']['referralPath'];
      $tltext .= "<br>\n" . $l;
    }
    if ($step['referrer']['socialNetwork'] != '(not set)') {
      $output .= $s . t('social network') . ': ' . $step['referrer']['socialNetwork'] . "<br />\n";
    }
    if ($step['referrer']['campaign'] != '(not set)') {
      $output .= $s . t('campaign') . ': ' . $step['referrer']['campaign'] . "<br />\n";
    }    
  }
  
  if (!empty($ref_alts[$tldata_link])) {
    $tldata_link = $ref_alts[$tldata_link];
  }
  
  if ($tltext) {
    $tlvisit['text'] .= (($tlvisit['text']) ? "<br>\n" : '') . $tltext;
  }  
  
  if ($tldata_link) {
    $tlvisit['asset']['media'] = $tldata_link;
  }
  
  return $output;
}

function theme_intel_visitor_map($locations, $options = array(), $s = '') {
  static $map_index;
  if (!isset($map_index)) {
    $map_index = 0;
  }
  $div_id = 'map_div_' . $map_index;
  // check if single element was passed
  if (isset($locations['lat'])) {
    $locations = array(
      $locations,
    );
  }
  $mode = 1;
  $output = '';
  if ($mode == 1) {
    $options = array('type' => 'external', 'weight' => -1);
    drupal_add_js('https://maps.googleapis.com/maps/api/js?v=3.exp&sensor=false', $options);
    $locstr = '';
    $center = array('lat' => 0, 'lon' => 0, );
    foreach ($locations AS $loc) {
      $locstr .= "[" . $loc['lat'] . ", " . $loc['lon'] . ", " . "'" . $loc['name'] . "'],\n";
      $center['lat'] = $loc['lat']; 
      $center['lon'] = $loc['lon']; 
    }
    $locstr = substr($locstr, 0, -1); // chop last comma
    //$output .= <<<EOT
    $script = <<<EOT
function initialize_map_$map_index() {
  var mapOptions = {
    zoom: 4,
    center: new google.maps.LatLng({$center['lat']}, {$center['lon']}),
    disableDefaultUI: true,
    mapTypeId: google.maps.MapTypeId.ROADMAP
  };

  var map = new google.maps.Map(document.getElementById('map-canvas-$map_index'),
      mapOptions);
      
  var circleOptions = {
    strokeColor: '#FF0000',
    strokeOpacity: 0.8,
    strokeWeight: 1,
    fillColor: '#FF0000',
    fillOpacity: 0.35,
    map: map,
    center: new google.maps.LatLng({$center['lat']}, {$center['lon']}),
    radius: 50000
  };
  locCircle = new google.maps.Circle(circleOptions);
}
google.maps.event.addDomListener(window, 'load', initialize_map_$map_index);
EOT;
    drupal_add_js($script, array('type' => 'inline', 'scope' => 'header'));
    $output = $s . '<div id="map-canvas-' . $map_index . '" class="map-canvas"></div>' . "\n";
    $map_index++;
  } 
  return $output; 
}

function _intel_get_visit_timedif($visits, $timestamp) {
  $timestamp = (int) $timestamp;
  $vi = 0;
  foreach ($visits AS $vt) {
    if (!isset($visits[$vi+1]) || ($visits[$vi+1] > $timestamp)) {
      break;
    }
    $vi++;
  }
  return $timestamp - $visits[$vi];
}

function _intel_init_events_array() {
  $a = array();
  $a['_all'] = _intel_init_events_array_element();
  return $a;
}

function _intel_init_events_array_element() {
  $a = array(
    'value' => 0,
    'totalEvents' => 0,
    'uniqueEvents' => 0,
    'totalValuedEvents' => 0,
    'uniqueValuedEvents' => 0,
  );
  return $a;
}

function _intel_get_page_step_index($steps, $timestamp, $hostname, $pagepath) {
  // check if event is on current page
  if (!empty($steps[$timestamp]['page']['hostname'])) {
    if (($steps[$timestamp]['page']['hostname'] == $hostname) && ($steps[$timestamp]['page']['pagePath'] == $pagepath)) {
      return $timestamp;
    }
  }
  // walk pages array backwards
  end($steps);
  while ( !is_null($ti = key($steps)) ) {
    // if the page element's timestamp is less
    if ($ti <= ($timestamp+1)) {  // note add a second to timestamp to correct for possible timing delays
      if (isset($steps[$ti]['page']) && ($steps[$ti]['page']['hostname'] == $hostname) && ($steps[$ti]['page']['pagePath'] == $pagepath)) {
        return $ti;
      }
    }
    prev($steps);
  }
  return FALSE;
}

function _intel_get_visit_step_index($steps, $timestamp) {
  end($steps);
  while ( !is_null($ti = key($steps)) ) {
    if (isset($steps[$ti]['visit']) && ($ti <= $timestamp)) {
      return $ti;
    }
    prev($steps);
  }
  return FALSE;
}

function _intel_update_event_all_values($all, $event) {
  $all['value'] += $event['value'];
  $all['totalEvents'] += (!empty($event['totalEvents'])) ? $event['totalEvents'] : 1;
  $all['uniqueEvents'] += (!empty($event['uniqueEvents'])) ? $event['uniqueEvents'] : 1;
  $all['totalValuedEvents'] += (!empty($event['totalValuedEvents'])) ? $event['totalValuedEvents'] : (($event['is_valuedevent']) ? 1 : 0);
  $all['uniqueValuedEvents'] += (!empty($event['uniqueValuedEvents'])) ? $event['uniqueValuedEvents'] : (($event['is_valuedevent']) ? 1 : 0);
  return $all;
} 

function _intel_format_delta_time($secs) {
  $s = floor($secs%60);
  $str = (($s < 10) ? '0' : '') . $s;
  $m = floor($secs/60);
  if ($m == 0) {
    return "0:$str";
  }
  elseif ($m < 60) {
    return "$m:$str";
  }
  else {
    $m = floor($m%60);
    $str = (($m < 10) ? '0' : '') . $m . ":" . $str;
    $h = floor($secs/3600); 
    return "$h:$str";
  }
}

function intel_score_data($data, $mode) {
  if ($mode == 'steps') {
    return intel_score_steps_data($data);
  }
  elseif ($mode == 'pages') {
    return intel_score_pages_data($data);
  } 
}

function intel_score_steps_data($steps) {
  $scoring = intel_get_scorings();
//dsm($steps);
//dsm($scoring);
  $datascore = 0;
  $pagescore = 0;
  $pagecount = 0;
  $visitindex = 0;
  foreach ($steps AS $ts => $step) {
    if (substr($ts, 0, 1) != '_') {
      if (isset($step['entrance'])) {
        if ($visitindex) {
          $steps[$visitindex]['entrance']['score'] = $visitscore;
          $datascore += $visitscore;
        }
        $visitindex = $ts;
        $visitscore = 0;
        $visitscore += $scoring['entrance'];
        $visitscore += $step['entrance']['goalValueAll'];
        if (isset($step['entrance']['events']['_all'])) {
          $visitscore += $step['entrance']['events']['_all']['value']; 
        }
        $pagecount = 1;
      }

      if (isset($step['pageview'])) {
        $pagescore = 0;
        if (isset($step['pageview']['events']['_all'])) {
          $pagescore += $step['pageview']['events']['_all']['value']; 
        }
        $steps[$ts]['pageview']['score'] = $pagescore;      
        
        if ($pagecount == 2) {
          $visitscore += $scoring['stick'];
        }
        elseif ($pagecount > 2) {
          $visitscore += $scoring['additional_pages'];
        }
        $pagecount++;
      }
    }
  }
  if ($visitindex > 1) {
    $steps[$visitindex]['entrance']['score'] = $visitscore;
    $datascore += $visitscore;
    $steps['_score'] = $datascore;
  }
//dsm($steps);
  return $steps;
}