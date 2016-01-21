/********************************************************************************************
* Curlypage javascript configuration file
* v1.0
*
* @author    Fernando San Julián
*********************************************************************************************/

/********************************************************************************************
* Curlypage parameters. This shows its acceptable values.
*
*
* swf files
*   flag_swf = 'flag.swf';
*   peel_swf = 'turn.swf';
*
* unique curlypage identifier
*   cpid;
*
* wait icon parameters
*   wait_enabled = 'true';
*   wait_url = 'wait.gif';
*   wait_width = '42';
*   wait_height = '42';
*
* flag style. Values: 'style1', 'style2'.
*   flag_style = 'style1';
* peel style. Values: 'style1', 'style2'.
*   peel_style = 'style1';
*
* flag and peel sizes
*   flag_width = 100;
*   flag_height = 100;
*   peel_width = 500;
*   peel_height = 500;
*
* Position of ear. Values: 'topleft', 'topright', 'bottomleft' or 'bottomright'.
*   peel_position = 'topright';
* Position model. Values: 'absolute' or 'fixed'.
*   peel_position_model = 'absolute';
*
* URL of small image.
*   small_url = 'small.jpg';
* URL of big image.
*   big_url = 'big.jpg';
* The image is mirrored on peel. Values: 0 (false) or 1 (true).
*   mirror = 0;
* Start transition. Values: 'none', 'Blinds', 'Fade', 'Fly', 'Iris', 'Photo', 'Rotate', 'Squeeze', 'Wipe', 'PixelDissolve', 'Zoom'.
*   in_transition = 'Photo';
* Transition duration. Values: 1-9
*   transition_duration = 4;
*
* Color of Peel. Values: 'golden', 'silver', 'custom'
*   peel_color = 'golden';
* Color of Peel Style. Values: 'flat', 'gradient'
*   peel_color_style = 'gradient';
* RGB values for a Custom Peel Color. Values: 0-255.
*   red_value = 0;
*   green_value = 0;
*   blue_value = 0;
*
* enable or disable link. Values: 0 (disabled) or 1 (enabled).
*   link_enabled = 0;
* Where to open the link. Same or New Window (tab). Values: '_self' or '_blank'.
*   link_target = '_blank';
* URL of link.
*   link = 'http://www.manfer.co.cc';
*
* URL of sound on load Peel Ad.
*   load_sound_url = '';
* URL of sound when peel is opened.
*   open_sound_url = '';
* URL of sound when peel is closed.
*   close_sound_url = '';
*
* Speed of flag movement. Values: 1-9
*   flag_speed = 4;
* Speed of peel. Values: 1-9
*   peel_speed = 4;
* Seconds till an automatic open of peel. Values: 0 (no automatic open) >0 (seconds configured).
*   automatic_open = 0;
* Seconds till an automatic close of peel. Values: 0 (no automatic close) >0 (seconds configured).
*   automatic_close = 0;
*
* Close button on open Peel. Values: 0 (disabled) or 1 (enabled).
*   close_button_enable = 0;
* Text on close button.
*   text_on_close_button = 'close';
* RGB values of close button. Values: 0-255.
*   close_red_value = 0;
*   close_green_value = 0;
*   close_blue_value = 0;
*
* Delay seconds to show the curlypage.
*   delay = 0;
*
* Amoung of time the curlypage will be shown. If 0 the curlypage would be shown forever.
*   time_slot = 0;
*
* For grouped curlypages times to repeat this curlypage. Values: 0-9.
* If it is 0, the curlypage would be repeat forever.
*   repeat_times = 1;
*
* Open curlypage with a click. Curlypage by default opens with mouseover. Values: 0 (no click/default mouseover), 1 (opens on click)
*   open_onclick = 0;
*
* Enable tracking. Values 0 (false) || 1 (true)
*   tracking = 0;
*
*********************************************************************************************/

// defaults
var curlypage_getflashplayer = 1;
var curlypage_flash_version = "9.0.0";
var curlypage_flash_security_policy = "sameDomain";

var curlypages = new Object();
curlypages["topleft"] = null;
curlypages["topright"] = null;
curlypages["bottomleft"] = null;
curlypages["bottomright"] = null;

/*
 *
 * Help functions to control curlypage flash versions
 * to be sure the correct versions are retrieved from
 * server and not old ones from browser cache are used.
 *
 */
function curlypage_get_flag_version(){
  return 'flag10';
}

function curlypage_get_peel_version(){
  return 'turn10';
}

/*
 * Hide the flag and show the peel
 */
function curlypage_do_peel(cpid, position){

  if (position == 'topleft' || position =='topright') {

    jQuery("#curlypage_peelDiv_" + cpid).css("top", "0px");
    jQuery("#curlypage_flagDiv_" + cpid).css("top", "-1000px");

  } else {

    jQuery("#curlypage_peelDiv_" + cpid).css("bottom", "0px");
    jQuery("#curlypage_flagDiv_" + cpid).css("bottom", "-1000px");

  }

  document.getElementById('curlypage_peel' + cpid).doPeel();

}

/*
 * Hide the peel and show the flag
 */
function curlypage_do_flag(cpid, position){

  if (position == 'topleft' || position =='topright') {

    jQuery("#curlypage_flagDiv_" + cpid).css("top", "0px");
    jQuery("#curlypage_peelDiv_" + cpid).css("top", "-1000px");

  } else {

    jQuery("#curlypage_flagDiv_" + cpid).css("bottom", "0px");
    jQuery("#curlypage_peelDiv_" + cpid).css("bottom", "-1000px");

  }

}

/*
 * Notify the flag when all peel media is loaded
 */
function curlypage_notify_peel_loaded(cpid) {
  document.getElementById('curlypage_flag' + cpid).peelLoaded();
}

/*
 * Hide flag
 */
function curlypage_hide_flag(cpid, position) {
  if (position == 'topleft' || position =='topright') {
    jQuery("#curlypage_flagDiv_" + cpid).css("top", "-1000px");
  } else {
    jQuery("#curlypage_flagDiv_" + cpid).css("bottom", "-1000px");
  }
}

/*
 * Hide peel
 */
function curlypage_hide_peel(cpid, position) {
  if (position == 'topleft' || position =='topright') {
    jQuery("#curlypage_peelDiv_" + cpid).css("top", "-1000px");
  } else {
    jQuery("#curlypage_peelDiv_" + cpid).css("bottom", "-1000px");
  }
}

/*
 * Remove wait layer, show flag and start peel
 */
function curlypage_start(cpid, position) {
  jQuery("#curlypage_waitDiv_" + cpid).remove();
  if (position == 'topleft' || position =='topright') {
    jQuery("#curlypage_flagDiv_" + cpid).css("top", "0px");
  } else {
    jQuery("#curlypage_flagDiv_" + cpid).css("bottom", "0px");
  }
  document.getElementById('curlypage_peel' + cpid).startMovie();
}

/*
 * Curlypage object
 */
function Curlypage(curlypage_vars) {

  this.curlypage_vars = curlypage_vars;

  this.flag_params = {
    play: "true",
    loop: "true",
    menu: "false",
    quality: "autohigh",
    scale: "noscale",
    wmode: "transparent",
    bgcolor: "#ffffff",
    allowScriptAccess: curlypage_flash_security_policy,
    allowFullScreen: "false"
  };

  this.flag_attributes = {
    id: "curlypage_flag" + this.curlypage_vars.cpid,
    name: "curlypage_flag" + this.curlypage_vars.cpid,
    styleclass: "curlypage"
  };

  this.peel_params = {
    play: "true",
    loop: "true",
    menu: "false",
    quality: "autohigh",
    scale: "noscale",
    wmode: "transparent",
    bgcolor: "#ffffff",
    allowScriptAccess: curlypage_flash_security_policy,
    allowFullScreen: "false"
  };

  this.peel_attributes = {
    id: "curlypage_peel" + this.curlypage_vars.cpid,
    name: "curlypage_peel" + this.curlypage_vars.cpid,
    styleclass: "curlypage"
  };

  this.write = curlypage_write;
  this.destroy = curlypage_destroy;
  this.flag_vars = curlypage_get_flag_vars;
  this.peel_vars = curlypage_get_peel_vars;
  this.flag_version = curlypage_get_flag_version;
  this.peel_version = curlypage_get_peel_version;

}

/*
 * Get curlypage flag variables
 */
function curlypage_get_flag_vars () {
  var flag_vars = {};
  flag_vars.cpid = this.curlypage_vars.cpid;
  flag_vars.flagStyle = this.curlypage_vars.flag_style;
  flag_vars.flagWidth = this.curlypage_vars.flag_width;
  flag_vars.flagHeight = this.curlypage_vars.flag_height;
  flag_vars.peelPosition = this.curlypage_vars.peel_position;
  flag_vars.smallURL = this.curlypage_vars.small_url;
  flag_vars.mirror = this.curlypage_vars.mirror;
  flag_vars.inTransition = this.curlypage_vars.in_transition;
  flag_vars.transitionDuration = this.curlypage_vars.transition_duration;
  flag_vars.peelColor = this.curlypage_vars.peel_color;
  flag_vars.peelColorStyle = this.curlypage_vars.peel_color_style;
  flag_vars.redValue = this.curlypage_vars.red_value;
  flag_vars.greenValue = this.curlypage_vars.green_value;
  flag_vars.blueValue = this.curlypage_vars.blue_value;
  flag_vars.loadSoundURL = this.curlypage_vars.load_sound_url;
  flag_vars.flagSpeed = this.curlypage_vars.flag_speed;
  flag_vars.startDelay = this.curlypage_vars.delay;
  flag_vars.timeSlot = this.curlypage_vars.time_slot;
  flag_vars.open_onclick = this.curlypage_vars.open_onclick;
  flag_vars.tracking = this.curlypage_vars.tracking;
  return flag_vars;
}

/*
 * Get curlypage peel variables
 */
function curlypage_get_peel_vars() {
  var peel_vars = {};
  peel_vars.cpid = this.curlypage_vars.cpid;
  peel_vars.peelStyle = this.curlypage_vars.peel_style;
  peel_vars.peelWidth = this.curlypage_vars.peel_width;
  peel_vars.peelHeight = this.curlypage_vars.peel_height;
  peel_vars.peelPosition = this.curlypage_vars.peel_position;
  peel_vars.bigURL = this.curlypage_vars.big_url;
  peel_vars.mirror = this.curlypage_vars.mirror;
  peel_vars.peelColor = this.curlypage_vars.peel_color;
  peel_vars.peelColorStyle = this.curlypage_vars.peel_color_style;
  peel_vars.redValue = this.curlypage_vars.red_value;
  peel_vars.greenValue = this.curlypage_vars.green_value;
  peel_vars.blueValue = this.curlypage_vars.blue_value;
  peel_vars.linkEnabled = this.curlypage_vars.link_enabled;
  peel_vars.linkTarget = this.curlypage_vars.link_target;
  peel_vars.link = this.curlypage_vars.link;
  peel_vars.openSoundURL = this.curlypage_vars.open_sound_url;
  peel_vars.closeSoundURL = this.curlypage_vars.close_sound_url;
  peel_vars.peelSpeed = this.curlypage_vars.peel_speed;
  peel_vars.automaticOpen = this.curlypage_vars.automatic_open;
  peel_vars.automaticClose = this.curlypage_vars.automatic_close;
  peel_vars.close_button_enable = this.curlypage_vars.close_button_enable;
  peel_vars.text_on_close_button = this.curlypage_vars.text_on_close_button.toLowerCase();
  peel_vars.close_redValue = this.curlypage_vars.close_red_value;
  peel_vars.close_greenValue = this.curlypage_vars.close_green_value;
  peel_vars.close_blueValue = this.curlypage_vars.close_blue_value;
  peel_vars.tracking = this.curlypage_vars.tracking;
  return peel_vars;
}

/*
 * Write curlypage layers and flash objects
 */
function curlypage_write() {

  var xPos;
  var yPos;

  // Absolute not available for bottom curlypages.
  // Set it always to fixed to prevent a wrong configuration with absolute.
  if (this.curlypage_vars.peel_position == 'bottomleft' || this.curlypage_vars.peel_position == 'bottomright') {
    this.curlypage_vars.peel_position_model = 'fixed';
  }

  // Check position of curlypage.
  if(this.curlypage_vars.peel_position == 'topleft' || this.curlypage_vars.peel_position == 'bottomleft') {
    xPos = 'left';
  } else {
    xPos = 'right';
  }

  if(this.curlypage_vars.peel_position == 'topleft' || this.curlypage_vars.peel_position == 'topright') {
    yPos = 'top';
  } else {
    yPos = 'bottom';
  }

  // Fixed not available for IE6
  if (jQuery.browser.msie && parseInt(jQuery.browser.version) <= 6) {
    if (yPos == 'bottom') {
      return;
    } else {
      this.curlypage_vars.peel_position_model = 'absolute';
    }
  }

  html = "";

  if (this.curlypage_vars.wait_enable == "1") {
    //Write wait div layer
    html += '<div class="curlypage" id="curlypage_waitDiv_' + this.curlypage_vars.cpid + '" style="position:' + this.curlypage_vars.peel_position_model + '; width:' + this.curlypage_vars.wait_width +'px; height:' + this.curlypage_vars.wait_height + 'px; z-index:9999; ' + xPos + ':0px; ' + yPos + ':0px;">';
    html += '<img src="' + this.curlypage_vars.wait_url + '" />';
    html += '</div>';
  }

  // Write peel div layer
  html += '<div class="curlypage" id="curlypage_peelDiv_' + this.curlypage_vars.cpid  + '" style="position:' + this.curlypage_vars.peel_position_model + '; width:' + this.curlypage_vars.peel_width + 'px; height:' + this.curlypage_vars.peel_height + 'px; z-index:9999; ' + xPos + ':0px; ' + yPos + ':0px;">';

  html += '<div id="curlypage_peelDivAlternative_' + this.curlypage_vars.cpid + '">';
  if(curlypage_getflashplayer) {
    html += '<a href="http://www.adobe.com/go/getflashplayer">';
    html += '<img src="http://www.adobe.com/images/shared/download_buttons/get_flash_player.gif" alt="Get Adobe Flash player" />';
    html += '</a>';
  }
  html += '</div>';

  // Close peel div layer
  html += '</div>';


  // Write flag div layer
  html += '<div class="curlypage" id="curlypage_flagDiv_' + this.curlypage_vars.cpid  + '" style="position:' + this.curlypage_vars.peel_position_model + '; width:' + this.curlypage_vars.flag_width + 'px; height:' + this.curlypage_vars.flag_height + 'px; z-index:9999; ' + xPos + ':0px; ' + yPos + ':0px;">';

  html += '<div id="curlypage_flagDivAlternative_' + this.curlypage_vars.cpid + '">';
  if(curlypage_getflashplayer) {
    html += '<a href="http://www.adobe.com/go/getflashplayer">';
    html += '<img src="http://www.adobe.com/images/shared/download_buttons/get_flash_player.gif" alt="Get Adobe Flash player" />';
    html += '</a>';
  }
  html += '</div>';

  // Close flag div layer
  html += '</div>';

  // Append the curlypage layers at the end of body
  jQuery("body:first").append(html);

  // Embed flag
  swfobject.embedSWF(curlypage_flag_swf + "?" + this.flag_version(), "curlypage_flagDivAlternative_" + this.curlypage_vars.cpid, this.curlypage_vars.flag_width, this.curlypage_vars.flag_height, curlypage_flash_version, "expressInstall.swf", this.flag_vars(), this.flag_params, this.flag_attributes);


  // Embed peel
  swfobject.embedSWF(curlypage_peel_swf + "?" + this.peel_version(), "curlypage_peelDivAlternative_" + this.curlypage_vars.cpid, this.curlypage_vars.peel_width, this.curlypage_vars.peel_height, curlypage_flash_version, "expressInstall.swf", this.peel_vars(), this.peel_params, this.peel_attributes);

}

/*
 * Destroy curlypage layers and flash objects
 */
function curlypage_destroy() {
  var cpid = this.curlypage_vars.cpid;
  var position = this.curlypage_vars.peel_position;
  jQuery("#curlypage_flagDiv_" + cpid).fadeOut(
    500,
    function(){
      jQuery("#curlypage_flagDiv_" + cpid).remove();
      curlypage_write_next(position);
    }
  );
  jQuery("#curlypage_peelDiv_" + cpid).fadeOut(
    500,
    function(){
      jQuery("#curlypage_peelDiv_" + cpid).remove();
    }
  );
}

/*
 * Destroy curlypage layers and flash objects on ioerror
 */
function curlypage_error_destroy(cpid, position) {
  curlypages[position].shift();
  jQuery("#curlypage_flagDiv_" + cpid).remove();
  jQuery("#curlypage_peelDiv_" + cpid).remove();
  curlypage_write_next(position);
}


/*
 * Compute next curlypage
 */
function curlypage_next(cpid, position) {

  var curlypage;

  if (curlypages[position].length == 1) {

    curlypage = curlypages[position].shift();

    if (curlypage.curlypage_vars.repeat_times == 0) {
      return;
    }

    if (curlypage.curlypage_vars.repeat_times > 1) {
      curlypage.curlypage_vars.repeat_times--;
      curlypages[position].push(curlypage);
      document.getElementById('curlypage_flag' + cpid).startTimeSlot();
      return;
    }

  }
  else {

    curlypage = curlypages[position].shift();

    if (curlypage.curlypage_vars.repeat_times == 0) {
      curlypages[position].push(curlypage);
    }

    if (curlypage.curlypage_vars.repeat_times > 1) {
      curlypage.curlypage_vars.repeat_times--;
      curlypages[position].push(curlypage);
    }

  }

  curlypage.destroy();

}

/*
 * Write next curlypage layers and flash objects
 */
function curlypage_write_next(position) {

  var curlypage;

  // check if there is another curlypage to show
  if (curlypages[position].length > 0) {

    curlypage = curlypages[position][0];
    curlypage.write();

  }

}

/*
 * Write all initial curlypages
 */
function curlypage_write_curlypages() {

  var corners = ["topleft", "topright", "bottomleft", "bottomright"];
  var position;

  for (var i=0; i<corners.length; i++) {
    position = corners[i];
    if (curlypages[position] != null) {
      curlypage_write_next(position);
    }
  }

}
