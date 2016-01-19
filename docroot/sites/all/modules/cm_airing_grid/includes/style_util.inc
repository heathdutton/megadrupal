<?
/*
 * Created on Jan 27, 2012
 * Created by Mark Libkuman
 * libkuman@openflows.com
 * www.openflows.com
 * 
 * Created for Manhattan Neighborhood Network
 */


function cm_airing_grid_grid_display_class_dictionary(&$airing) {
  $is_live = cm_airing_grid_is_live_airing($airing);
  
  if ( $is_live ) {
    //THIS IS FOR STATIONS THAT RELY ON TERM ON EPISODE, RATHER THAN ON AIRING
    if ( cm_airing_grid_get_episode_device_tid($airing->episode_nid)) {
      return 'cm_airing_grid_live';
    }
    //WE ARE RELYING ON LIVE SOURCE, SO JUST CHECK FOR AN EXISTING EPISODE
    else if ($airing->live_source && $airing->episode_nid) {
      return 'cm_airing_grid_live';
    }
    else {
      return 'cm_airing_grid_no_device';
    }
  }
  else if ((strtotime($airing->event_end) - 
	    strtotime($airing->event_start)) < 20*60) {
    return 'cm_airing_grid_short';
  }
  else {
    if ( $airing->episode_nid ) {
      return 'cm_airing_grid_data';
    }
    else {
      return 'cm_airing_grid_no_episode';
    }
  }
}
