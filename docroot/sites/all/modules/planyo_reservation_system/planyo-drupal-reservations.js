// $Id$

var planyo_settings = window;
if (window.jQuery)
    jQuery(document).ready(init_planyo);
else
    addEventListener('load', init_planyo, false);

function planyo$(x) {
  return jQuery(x);
}

function planyo_get_param(name) {
  if (planyo_isset(window.planyo_overrides) && window.planyo_overrides[name])
    return window.planyo_overrides[name];
  name = name.replace(/[\[]/,"\\\[").replace(/[\]]/,"\\\]");
  var regexS = "[\\?&]"+name+"=([^&#]+)";
  var regex = new RegExp(regexS);
  var results = regex.exec(window.location.href);
  if (results == null && planyo_isset(planyo_settings.planyo_attribs))
    results = regex.exec (planyo_settings.planyo_attribs);
  if (results == null)
    return null;
  else
    return decodeURIComponent(results[1]).replace(/\+/g, ' ');
}

function planyo_get_prefixed_params (prefix, existing_query_string) {
  prefix = prefix.replace(/[\[]/,"\\\[").replace(/[\]]/,"\\\]");
  var regexS = "[\\?&]("+prefix+"[A-Za-z0-9_]+)=([^&#]*)";
  var regex = new RegExp(regexS,'g');
  var retstr = "";
  while ((results = regex.exec(window.location.href)) != null) {
      if (results[2]) {
          if (results[2].length > 200)
              results[2] = results[2].substr(0,200);
          if (!existing_query_string || existing_query_string.indexOf(results[1]+'=') == -1)
              retstr += (retstr.length>0?"&":"")+results[1]+"="+results[2];
      }
  }
  if (planyo_isset(planyo_settings.planyo_attribs)) {
    while ((results = regex.exec(planyo_settings.planyo_attribs)) != null) {
      if (results[2]) {
          if (results[2].length > 200)
              results[2] = results[2].substr(0,200);
          if (!existing_query_string || existing_query_string.indexOf(results[1]+'=') == -1)
              retstr += (retstr.length>0?"&":"")+results[1]+"="+results[2];
      }
    }
  }
  return retstr;
}

function planyo_get_form_data(obj) {
  var getstr = "";
  var serialized = obj.serialize();
  if (!serialized) serialized = '';
  /*
  var unchecked_checkboxes = $('#reservation_details input:[type="checkbox"]').map(function(){return (!this.checked) ? this.name+"=" : ""}).get().join("&");
  if (unchecked_checkboxes) {
    if (serialized!="" && unchecked_checkboxes!="") serialized+="&"+unchecked_checkboxes; else serialized+=unchecked_checkboxes;
  */
  return serialized;
}

function get_ppp_rs(text) {
  if (text) {
    var prefix = "<!-- PPP_RS:";
    var i_start = text.indexOf (prefix);
    if (i_start != -1) {
      i_start += prefix.length;
      var i_end = text.indexOf (":PPP_RS -->", i_start);
      if (i_end != -1) {
        return text.substring(i_start, i_end)
      }
    }
  }
  return null;
}

function planyo_on_reservation_success(reservation_id, user_text) {
  jQuery('#planyo_content').children().css('display','none');
  jQuery('#res_error_msg').css('display','none');
  jQuery('#res_ok_msg').css('display','inline');
  jQuery('#reserve_form').css('display','none');
  jQuery('#product_form').css('display','none');
  jQuery('#price_info_div').css('display','none');
  if (reservation_id && user_text.indexOf ("<!-- SHOW_ADDITIONAL_PRODUCTS -->") != -1) {
    planyo_embed_additional_products_form(reservation_id, get_ppp_rs(user_text));
  }
  else {
    jQuery('#res_ok_msg').html(user_text);
    if (planyo_isset(window.on_planyo_form_loaded)) {
      document.reservation_id = reservation_id;
      window.on_planyo_form_loaded('reservation_done');
    }
  }
  jQuery('#planyo_content').append(jQuery('#res_ok_msg'));
  jQuery('#res_ok_msg').parent().css('display','inline');
  jQuery('html, body').animate({scrollTop: jQuery("#res_ok_msg").offset().top}, 1000);
}
 
function planyo_on_reservation_failure(error_text) {
   var error_div = jQuery('#res_error_msg');
   if (!error_div || error_div.length == 0) {
       jQuery('#planyo_content').prepend("<div id='res_error_msg'></div>");
       error_div = jQuery('#res_error_msg');
   }
   error_div.css('display','block');
   error_div.html(error_text+"<p>");
   jQuery('html, body').animate({scrollTop: jQuery("#res_error_msg").offset().top}, 1000);
   if (planyo_isset(window.planyo_dates_changed)) planyo_dates_changed();
   if (planyo_isset(window.on_planyo_form_loaded)) {
     window.on_planyo_form_loaded('reservation_failure');
   }
}
 
function planyo_get_current_url() {
  if (planyo_get_param('feedback_url'))
    return encodeURIComponent(planyo_get_param('feedback_url'));
  else
    return encodeURIComponent(window.location.href);
}

function planyo_show_hourglass(hide_element) {
  document.planyo_ajax_call_pending = true;
  if (hide_element && hide_element.length > 0) {
    document.planyo_ajax_call_hide_element = hide_element;
    hide_element.css('display', 'none');
    hide_element.before("<div id='hourglass_element'><div class='hourglass_img'></div></div>");
  }
}

function planyo_hide_hourglass() {
  if (document.planyo_ajax_call_pending) {
    if (document.planyo_ajax_call_hide_element && document.planyo_ajax_call_hide_element.length > 0) {
      document.planyo_ajax_call_hide_element.css('display', 'inline');
      jQuery('#hourglass_element').remove();
      document.planyo_ajax_call_hide_element = null;
    }
    document.planyo_ajax_call_pending = false;
  }
}

function planyo_on_request_failure() {
  planyo_hide_hourglass();
  var error_div = jQuery('#res_error_msg');
  error_div.css('display', 'block');
  error_div.html("Unknown error in " + planyo_settings.ulap_script + "<p>");
}

function planyo_unserialize(url) {
  var params = url;
  if (url.match(/\?(.+)$/)) {
      params = RegExp.$1;
  }
  var pArray = params.split("&");
  var pHash = {};
  for(var i = 0; i < pArray.length; i++) {
    var temp = pArray[i].split("=");
    if (temp[0] != 'q' || !window.Drupal) {
      var param_val = decodeURIComponent(temp[1]).replace(/\+/g, ' ');
      if (param_val && param_val.length > 0)
        pHash[temp[0]] = param_val;
    }
  }
  return pHash;
}

function planyo_get_cookie(c_name) {
  if (document.cookie.length > 0) {
    c_start = document.cookie.indexOf(c_name + "=");
    if (c_start != -1) {
      c_start = c_start + c_name.length+1;
      c_end = document.cookie.indexOf(";", c_start);
      if (c_end == -1) 
        c_end = document.cookie.length;
      return unescape(document.cookie.substring(c_start, c_end));
    }
  }
  return "";
}

function planyo_set_cookie(name, value, days) {
    if (days) {
        var date = new Date();
        date.setTime(date.getTime() + (days * 24 * 60 * 60 * 1000));
        var expires = "; expires=" + date.toGMTString();
    } else var expires = "";
    document.cookie = escape(name) + "=" + escape(value) + expires + "; path=/";
}

function planyo_send_request(data, on_complete_function, hide_item) {
  planyo_show_hourglass(hide_item);
  var req_data = data;
  if (req_data.indexOf('feedback_url=') == -1)
    req_data += "&feedback_url=" + planyo_get_current_url();
  if (req_data.indexOf('ulap_url=') == -1)
    req_data += "&ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php";
  if (planyo_settings.planyo_language)
    req_data += "&language=" + planyo_settings.planyo_language;
  var shopping_cart_id = planyo_get_cookie('planyo_cart_id');
  if (shopping_cart_id && req_data.indexOf('cart_id=') == -1)
    req_data += "&cart_id=" + shopping_cart_id + "&first_reservation_id=" + planyo_get_cookie('planyo_first_reservation_id');
  if (planyo_isset(planyo_settings.planyo_login) && planyo_isset(planyo_settings.planyo_login, 'login_cs') && planyo_isset(planyo_settings.planyo_login, 'email'))
    req_data += "&login_cs=" + planyo_settings.planyo_login['login_cs'] + "&login_email=" + planyo_settings.planyo_login['email'];
  var ppp_params = planyo_get_prefixed_params('ppp_', req_data);
  if (ppp_params.length > 0)
    req_data += "&" + ppp_params;
  var cookieval = planyo_get_cookie('ppp_refcode');
  if (cookieval && !planyo_get_param('ppp_refcode'))
      req_data += "&ppp_refcode="+cookieval;
  req_data += "&user_agent=" + encodeURIComponent(navigator.userAgent);
  if (planyo_settings.ulap_script.toLowerCase() == 'jsonp')
      req_data += "&modver=2.6";
  var req_data_arr = planyo_unserialize(req_data);
  if (planyo_settings.ulap_script.toLowerCase() == 'jsonp') {
      jQuery.ajax({
          url: (planyo_settings.planyo_use_https ? "https://" : "http://") + "www.planyo.com/rest/ulap-jsonp.php",
          dataType: 'jsonp',
          data: req_data_arr,
          success: function(data) {
              on_complete_function(data);
          },
          error: function(e) {
              planyo_on_request_failure();
          }
      });
  }
  else {
      jQuery.post(planyo_settings.ulap_script, req_data_arr,
	                function(data, status) {
	                    if (status == "success")
		                      on_complete_function(data);
	                    else
		                      planyo_on_request_failure();
	                }, 'json');
  }
}

function planyo_on_complete_show_status(txt) {
  jQuery('#planyo_content').html("");
  planyo_hide_hourglass();
  var obj = (txt && typeof txt == 'object') ? txt : jQuery.parseJSON(txt);
  if (!obj) 
    return;
  var code = obj['response_code'];
  if (code == 0) {
    // success
    var ok_div = jQuery('#res_ok_msg');
    if (!ok_div || ok_div.length == 0) {
      jQuery('#planyo_content').append("<div id='res_ok_msg'></div>");
      ok_div = jQuery('#res_ok_msg');
    }
    ok_div.css('display', 'inline');
    ok_div.html(obj['data']['user_text']);

    if (planyo_isset(obj, 'data', 'redirect_location'))
      eval(obj['data']['redirect_location']);

    if (planyo_isset(window.on_planyo_form_loaded))
      window.on_planyo_form_loaded(planyo_get_param('mode'));
  }
  else {
    var error_div = jQuery('#res_error_msg');
    if (!error_div || error_div.length == 0) {
      jQuery('#planyo_content').append("<div id='res_error_msg'></div>");
      error_div = jQuery('#res_error_msg');
    }
    error_div.css('display', 'block');
    error_div.html(obj['response_message']);
  }
}

function planyo_init_cancel_mode() {
  planyo_send_request("mode=cancel&c=" + planyo_get_param('c') + "&user_id=" + planyo_get_param('user_id') + "&site_id=" + planyo_settings.planyo_site_id + "&reservation_id=" + planyo_get_param('reservation_id') + "&resource_id=" + planyo_get_param('resource_id') + "&reason=" + planyo_get_param('reason'), planyo_on_complete_show_status, null);
}

function planyo_init_verify_email_mode() {
  planyo_send_request("mode=verify_email&verification_code=" + planyo_get_param('verification_code') + "&site_id=" + planyo_settings.planyo_site_id + "&reservation_id=" + planyo_get_param('reservation_id'), planyo_on_complete_show_status, null);
}

function planyo_init_payment_confirmation_mode() {
  planyo_send_request("mode=payment_confirmation&reservation_id=" + planyo_get_param('reservation_id') + "&site_id=" + planyo_settings.planyo_site_id, planyo_on_complete_show_status, null);
}

function planyo_on_add_coupon_complete (txt) {
  planyo_hide_hourglass();
  var obj = null;
  try {
    obj = (txt && typeof txt == 'object') ? txt : jQuery.parseJSON(txt);
  }
  catch (err) {
  }
  if (!obj) {
    planyo_on_reservation_failure('Unknown coupon selection error: ' + txt);
    return;
  }
  var code = obj['response_code'];
  if (code == 0) {// success
    jQuery('#planyo_content').html(obj['data']['user_text']);
    jQuery(window).scrollTop(0);
  }
  else {
    planyo_on_reservation_failure(obj['response_message']);
  }
}
 
function planyo_on_widget_complete (txt) {
  planyo_hide_hourglass();
  var obj = null;
  try {
    obj = (txt && typeof txt == 'object') ? txt : jQuery.parseJSON(txt);
  }
  catch (err) {
  }
  if (!obj) {
    planyo_on_reservation_failure('Unknown widget error: ' + txt);
    return;
  }
  var code = obj['response_code'];
  if (code == 0) {// success
    jQuery('#planyo_content').html(obj['data']['user_text']);
    jQuery(window).scrollTop(0);
  }
  else {
    planyo_on_reservation_failure(obj['response_message']);
  }
}
 
function planyo_on_reservation_complete(txt) {
  planyo_hide_hourglass();

  var obj = null;
  try {
    obj = (txt && typeof txt == 'object') ? txt : jQuery.parseJSON(txt);
  }
  catch(err) {
  }
  if (!obj) {
    planyo_on_reservation_failure('Unknown reservation error: ' + txt);
    return;
  }

  var code = obj['response_code'];
  if (code == 0) {
    // success
    planyo_on_reservation_success(obj['data']['reservation_id'], obj['data']['user_text']);
  }
  else {
    planyo_on_reservation_failure(obj['response_message']);
  }
}

function planyo_send_reservation_form() {
  planyo_hide_element (jQuery('#multipage_prev'), true);
  planyo_hide_element (jQuery('#multipage_next'), true);
  planyo_send_request("mode=make_reservation&site_id=" + planyo_settings.planyo_site_id + "&" + planyo_get_form_data(jQuery('#reserve_form')), planyo_on_reservation_complete, jQuery('#submit_button'));
  return false;
}

function get_planyo_site_id() {
  return (planyo_get_param('site_id') && String(planyo_settings.planyo_site_id).indexOf('M')==0) ? planyo_get_param('site_id') : planyo_settings.planyo_site_id;
}

function planyo_send_add_coupon_form () {
  planyo_send_request ("mode=add_coupon&site_id=" + get_planyo_site_id() +"&" + planyo_get_form_data(jQuery('#add_coupon')), planyo_on_add_coupon_complete, jQuery('#submit_button'));
  return false;
}

function planyo_send_widget_form () {
  planyo_send_request ("mode=submit_widget&site_id=" + get_planyo_site_id() +"&" + planyo_get_form_data(jQuery('#widget_form')), planyo_on_widget_complete, jQuery('#submit_button'));
  return false;
}

function planyo_send_product_form() {
  planyo_send_request("mode=reserve_products&site_id=" + planyo_settings.planyo_site_id + "&" + planyo_get_form_data(jQuery('#product_form')), planyo_on_reservation_complete, jQuery('#submit_button'));
  return false;
}

function planyo_send_modify_data_form() {
  var form_url = planyo_settings.ulap_script;
  var form_url_params = "ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php&mode=do_modify_data&" + planyo_get_form_data(jQuery('#modify_data_form')) + "&feedback_url="+planyo_get_current_url();
   planyo_embed_code (form_url,form_url_params);
   return false;
}

function planyo_on_search_success(results) {
  jQuery('#res_error_msg').css('display', 'none');
  var results_code = results;

  jQuery('#search_results').html(results_code);

  if (planyo_isset(window.on_planyo_form_loaded))
    window.on_planyo_form_loaded('search_success');
}
 
function planyo_on_search_failure(error_text) {
   var error_div = jQuery('#res_error_msg');
   error_div.css('display', 'block');
   error_div.html(error_text + '<p>');
   jQuery('#search_results').html('');
   jQuery(window).scrollTop(0);

   if (planyo_isset(window.on_planyo_form_loaded))
     window.on_planyo_form_loaded('search_failure');
}

function planyo_on_search_complete(txt) {
  planyo_hide_hourglass();
  var obj = null;
  try {
    obj = (txt && typeof txt == 'object') ? txt : jQuery.parseJSON(txt);
  }
  catch(err) {
  }
  if (!obj) {
    planyo_on_search_failure('Unknown search error: ' + txt);
    return;
  }
  var code = obj['response_code'];
  if (code == 0) {
    // success
    planyo_on_search_success(obj['data']['code']);
  }
  else {
    planyo_on_search_failure(obj['response_message']);
  }
}

function planyo_upcoming_availability_form_data_changed () {
  planyo_send_upcoming_availability_search_form();
}

function planyo_send_upcoming_availability_search_form() {
  planyo_send_request ("mode=upcoming_availability_search&output=html&site_id=" + planyo_settings.planyo_site_id + "&" + planyo_get_form_data(jQuery('#availability_form')), planyo_on_search_complete, jQuery('#search_results'));
  return false;
}

function planyo_send_search_form() {
  planyo_send_request("mode=resource_search&output=html&site_id=" + planyo_settings.planyo_site_id + "&" + planyo_get_form_data(jQuery('#search_form')), planyo_on_search_complete, jQuery('#submit_button'));
  return false;
}

function planyo_is_special_mode(mode) {
  return (mode == "verify_email" || mode == "payment_confirmation" || mode == "cancel" || mode == "cancel_code" || mode == "reservation_details" || mode == "payment_form" || mode == "reservation_list" || mode == "show_cart" || mode == "checkout" || mode == "widget" || mode == "additional_products_code" || mode == "modify_data_form" || mode == "show_coupons" || mode == "coupon_payment_confirmation" || mode == "use_coupon");
}

function planyo_init_special_modes() {
  var mode = planyo_get_param('mode');
  if (planyo_is_special_mode(mode)) {
    if (mode == "verify_email") {
      planyo_init_verify_email_mode();
    }
    else if (mode == "payment_confirmation") {
      planyo_init_payment_confirmation_mode();
    }
    else if (mode == "cancel") {
      planyo_init_cancel_mode();
    }
    else if (mode == 'reservation_details') {
      planyo_embed_reservation_details(planyo_settings.planyo_site_id, planyo_get_param('reservation_id'), planyo_get_param('user_id'));
    }
    else if (mode == 'reservation_list') {
      planyo_embed_reservation_list(planyo_settings.planyo_site_id, planyo_get_param('reservation_id'), planyo_get_param('user_id'));
    }
    else if (mode == 'cancel_code') {
      planyo_embed_cancel_code(planyo_settings.planyo_site_id, planyo_get_param('reservation_id'), planyo_get_param('c'), planyo_get_param('user_id'), planyo_get_param('resource_id'));
    }
    else if (mode == 'payment_form') {
      planyo_embed_payment_form(planyo_settings.planyo_site_id, planyo_get_param('reservation_id'), planyo_get_param('user_id'), planyo_get_param('auto_redirect'));
    }
    else if (mode == 'show_cart') {
      planyo_embed_cart_code(planyo_settings.planyo_site_id, planyo_get_param('cart_id'), planyo_get_param('first_reservation_id'), planyo_get_param('remove_reservation_id'));
    }
    else if (mode == 'checkout') {
      planyo_embed_checkout_code(planyo_settings.planyo_site_id, planyo_get_param('cart_id'), planyo_get_param('first_reservation_id'));
    }
    else if (mode == 'widget') {
      planyo_embed_widget(planyo_settings.planyo_site_id);
    }
    else if (mode == 'additional_products_code') {
      planyo_embed_non_reservation_step_additional_products_form(planyo_get_param('reservation_id'), planyo_get_param('user_id'));
    }
    else if (mode == 'modify_data_form') {
      planyo_embed_modify_data_form(planyo_get_param('reservation_id'), planyo_get_param('user_id'));
    }
    else if (mode == 'show_coupons') {
      planyo_embed_show_coupons_form (get_planyo_site_id(), planyo_get_param('resource_id'), planyo_get_param('coupon_user_email'));
    }
    else if (mode == 'coupon_payment_confirmation') {
      planyo_embed_coupon_payment_confirmation_code (planyo_settings.planyo_site_id, planyo_get_param('resource_id'), planyo_get_param('coupon_payment_id'));
    }
    else if (mode == 'use_coupon') {
      planyo_embed_use_coupon_code (get_planyo_site_id(), planyo_get_param('reservation_id'), planyo_get_param('coupon_id'), planyo_get_param('user_id'));
    }
    return true;
  }
  return false;
}

function planyo_get_phone_codes(name) {
return "<select id='" + name + "' name='" + name + "' onchange='js_dates_changed();'><option selected value='-1'>-- Country code --&nbsp;</option><option  value='93'>Afghanistan (93)&nbsp;</option><option  value='355'>Albania (355)&nbsp;</option><option  value='213'>Algeria (213)&nbsp;</option><option  value='376'>Andorra (376)&nbsp;</option><option  value='244'>Angola (244)&nbsp;</option><option  value='264'>Anguilla (264)&nbsp;</option><option  value='672'>Antarctica (672)&nbsp;</option><option  value='54'>Argentina (54)&nbsp;</option><option  value='374'>Armenia (374)&nbsp;</option><option  value='297'>Aruba (297)&nbsp;</option><option  value='61'>Australia (61)&nbsp;</option><option  value='43'>Austria (43)&nbsp;</option><option  value='994'>Azerbaijan (994)&nbsp;</option><option  value='242'>Bahamas (242)&nbsp;</option><option  value='973'>Bahrain (973)&nbsp;</option><option  value='880'>Bangladesh (880)&nbsp;</option><option  value='246'>Barbados (246)&nbsp;</option><option  value='375'>Belarus (375)&nbsp;</option><option  value='32'>Belgium (32)&nbsp;</option><option  value='501'>Belize (501)&nbsp;</option><option  value='441'>Bermuda (441)&nbsp;</option><option  value='975'>Bhutan (975)&nbsp;</option><option  value='591'>Bolivia (591)&nbsp;</option><option  value='267'>Botswana (267)&nbsp;</option><option  value='55'>Brazil (55)&nbsp;</option><option  value='673'>Brunei (673)&nbsp;</option><option  value='359'>Bulgaria (359)&nbsp;</option><option  value='226'>Burkina Faso (226)&nbsp;</option><option  value='257'>Burundi (257)&nbsp;</option><option  value='855'>Cambodia (855)&nbsp;</option><option  value='237'>Cameroon (237)&nbsp;</option><option  value='1'>Canada / USA (1)&nbsp;</option><option  value='56'>Chile (56)&nbsp;</option><option  value='86'>China (86)&nbsp;</option><option  value='57'>Colombia (57)&nbsp;</option><option  value='242'>Congo (242)&nbsp;</option><option  value='682'>Cook Islands (682)&nbsp;</option><option  value='506'>Costa Rica (506)&nbsp;</option><option  value='385'>Croatia (385)&nbsp;</option><option  value='53'>Cuba (53)&nbsp;</option><option  value='599'>Curacao (599)&nbsp;</option><option  value='357'>Cyprus (357)&nbsp;</option><option  value='420'>Czech Republic (420)&nbsp;</option><option  value='45'>Denmark (45)&nbsp;</option><option  value='246'>Diego Garcia (246)&nbsp;</option><option  value='253'>Djibouti (253)&nbsp;</option><option  value='809'>Dominica (809)&nbsp;</option><option  value='593'>Ecuador (593)&nbsp;</option><option  value='20'>Egypt (20)&nbsp;</option><option  value='503'>El Salvador (503)&nbsp;</option><option  value='372'>Estonia (372)&nbsp;</option><option  value='298'>Faeroe Islands (298)&nbsp;</option><option  value='500'>Falkland (500)&nbsp;</option><option  value='679'>Fiji Islands (679)&nbsp;</option><option  value='358'>Finland (358)&nbsp;</option><option  value='33'>France (33)&nbsp;</option><option  value='596'>Antilles  (596)&nbsp;</option><option  value='594'>Guiana (594)&nbsp;</option><option  value='241'>Gabon Republic (241)&nbsp;</option><option  value='220'>Gambia (220)&nbsp;</option><option  value='995'>Georgia (995)&nbsp;</option><option  value='49'>Germany (49)&nbsp;</option><option  value='233'>Ghana (233)&nbsp;</option><option  value='350'>Gibraltar (350)&nbsp;</option><option  value='30'>Greece (30)&nbsp;</option><option  value='299'>Greenland (299)&nbsp;</option><option  value='473'>Grenada (473)&nbsp;</option><option  value='590'>Guadeloupe (590)&nbsp;</option><option  value='671'>Guam (671)&nbsp;</option><option  value='502'>Guatemala (502)&nbsp;</option><option  value='592'>Guyana (592)&nbsp;</option><option  value='509'>Haiti (509)&nbsp;</option><option  value='504'>Honduras (504)&nbsp;</option><option  value='852'>Hong Kong (852)&nbsp;</option><option  value='36'>Hungary (36)&nbsp;</option><option  value='354'>Iceland (354)&nbsp;</option><option  value='91'>India (91)&nbsp;</option><option  value='62'>Indonesia (62)&nbsp;</option><option  value='98'>Iran (98)&nbsp;</option><option  value='964'>Iraq (964)&nbsp;</option><option  value='353'>Ireland (353)&nbsp;</option><option  value='972'>Israel (972)&nbsp;</option><option  value='39'>Italy (39)&nbsp;</option><option  value='876'>Jamaica (876)&nbsp;</option><option  value='81'>Japan Idc (81)&nbsp;</option><option  value='81'>Japan Kdd (81)&nbsp;</option><option  value='962'>Jordan (962)&nbsp;</option><option  value='7'>Kazakhstan (7)&nbsp;</option><option  value='254'>Kenya (254)&nbsp;</option><option  value='686'>Kiribati (686)&nbsp;</option><option  value='850'>Korea, North (850)&nbsp;</option><option  value='82'>Korea, South (82)&nbsp;</option><option  value='965'>Kuwait (965)&nbsp;</option><option  value='7'>Kyrgyzstan (7)&nbsp;</option><option  value='856'>Laos (856)&nbsp;</option><option  value='371'>Latvia (371)&nbsp;</option><option  value='961'>Lebanon (961)&nbsp;</option><option  value='266'>Lesotho (266)&nbsp;</option><option  value='231'>Liberia (231)&nbsp;</option><option  value='218'>Libya (218)&nbsp;</option><option  value='423'>Liechtenstein (423)&nbsp;</option><option  value='370'>Lithuania (370)&nbsp;</option><option  value='352'>Luxembourg (352)&nbsp;</option><option  value='853'>Macao (853)&nbsp;</option><option  value='389'>Macedonia (389)&nbsp;</option><option  value='261'>Madagascar (261)&nbsp;</option><option  value='265'>Malawi (265)&nbsp;</option><option  value='60'>Malaysia (60)&nbsp;</option><option  value='960'>Maldives (960)&nbsp;</option><option  value='223'>Mali Republic (223)&nbsp;</option><option  value='356'>Malta (356)&nbsp;</option><option  value='692'>Marshall Islands (692)&nbsp;</option><option  value='222'>Mauritania (222)&nbsp;</option><option  value='230'>Mauritius (230)&nbsp;</option><option  value='269'>Mayotte Island (269)&nbsp;</option><option  value='52'>Mexico (52)&nbsp;</option><option  value='691'>Micronesia (691)&nbsp;</option><option  value='373'>Moldova (373)&nbsp;</option><option  value='33'>Monaco (33)&nbsp;</option><option  value='473'>Montserrat (473)&nbsp;</option><option  value='212'>Morocco (212)&nbsp;</option><option  value='258'>Mozambique (258)&nbsp;</option><option  value='95'>Myanmar (95)&nbsp;</option><option  value='264'>Namibia (264)&nbsp;</option><option  value='674'>Nauru (674)&nbsp;</option><option  value='977'>Nepal (977)&nbsp;</option><option  value='31'>Netherlands (31)&nbsp;</option><option  value='599'>Antilles (599)&nbsp;</option><option  value='687'>New Caledonia (687)&nbsp;</option><option  value='64'>New Zealand (64)&nbsp;</option><option  value='505'>Nicaragua (505)&nbsp;</option><option  value='234'>Nigeria (234)&nbsp;</option><option  value='227'>Niger Republic (227)&nbsp;</option><option  value='683'>Niue (683)&nbsp;</option><option  value='672'>Norfolk Island (672)&nbsp;</option><option  value='47'>Norway (47)&nbsp;</option><option  value='968'>Oman (968)&nbsp;</option><option  value='92'>Pakistan (92)&nbsp;</option><option  value='680'>Palau (680)&nbsp;</option><option  value='595'>Paraguay (595)&nbsp;</option><option  value='51'>Peru (51)&nbsp;</option><option  value='63'>Philippines (63)&nbsp;</option><option  value='48'>Poland (48)&nbsp;</option><option  value='351'>Portugal (351)&nbsp;</option><option  value='787'>Puerto Rico (787)&nbsp;</option><option  value='974'>Qatar (974)&nbsp;</option><option  value='262'>Reunion Island (262)&nbsp;</option><option  value='40'>Romania (40)&nbsp;</option><option  value='7'>Russia (7)&nbsp;</option><option  value='250'>Rwanda (250)&nbsp;</option><option  value='599'>St Eustatius (599)&nbsp;</option><option  value='290'>St Helena (290)&nbsp;</option><option  value='758'>St Lucia (758)&nbsp;</option><option  value='599'>St Maarten (599)&nbsp;</option><option  value='809'>St Vincent (809)&nbsp;</option><option  value='684'>Samoa (684)&nbsp;</option><option  value='378'>San Marino (378)&nbsp;</option><option  value='239'>Sao Tome (239)&nbsp;</option><option  value='966'>Saudi Arabia (966)&nbsp;</option><option  value='221'>Senegal (221)&nbsp;</option><option  value='232'>Sierra Leone (232)&nbsp;</option><option  value='65'>Singapore (65)&nbsp;</option><option  value='421'>Slovakia (421)&nbsp;</option><option  value='386'>Slovenia (386)&nbsp;</option><option  value='677'>Solomon Islands (677)&nbsp;</option><option  value='252'>Somalia (252)&nbsp;</option><option  value='27'>South Africa (27)&nbsp;</option><option  value='34'>Spain (34)&nbsp;</option><option  value='94'>Sri Lanka (94)&nbsp;</option><option  value='249'>Sudan (249)&nbsp;</option><option  value='597'>Suriname (597)&nbsp;</option><option  value='268'>Swaziland (268)&nbsp;</option><option  value='46'>Sweden (46)&nbsp;</option><option  value='41'>Switzerland (41)&nbsp;</option><option  value='963'>Syria (963)&nbsp;</option><option  value='886'>Taiwan (886)&nbsp;</option><option  value='7'>Tajikistan (7)&nbsp;</option><option  value='255'>Tanzania (255)&nbsp;</option><option  value='66'>Thailand (66)&nbsp;</option><option  value='228'>Togo (228)&nbsp;</option><option  value='676'>Tonga Islands (676)&nbsp;</option><option  value='216'>Tunisia (216)&nbsp;</option><option  value='90'>Turkey (90)&nbsp;</option><option  value='993'>Turkmenistan (993)&nbsp;</option><option  value='688'>Tuvalu (688)&nbsp;</option><option  value='971'>UAE (971)&nbsp;</option><option  value='256'>Uganda (256)&nbsp;</option><option  value='380'>Ukraine (380)&nbsp;</option><option  value='44'>UK (44)&nbsp;</option><option  value='1'>USA / Canada (1)&nbsp;</option><option  value='598'>Uruguay (598)&nbsp;</option><option  value='998'>Uzbekistan (998)&nbsp;</option><option  value='678'>Vanuatu (678)&nbsp;</option><option  value='39'>Vatican City (39)&nbsp;</option><option  value='58'>Venezuela (58)&nbsp;</option><option  value='84'>Vietnam (84)&nbsp;</option><option  value='681'>Wallis (681)&nbsp;</option><option  value='685'>Western Samoa (685)&nbsp;</option><option  value='967'>Yemen (967)&nbsp;</option><option  value='381'>Serbia (381)&nbsp;</option><option  value='260'>Zambia (260)&nbsp;</option><option  value='263'>Zimbabwe (263)&nbsp;</option></select> ";
}

function planyo_get_country_codes() {
  return "<select id='country' name='country' onchange='js_dates_changed();'><option selected='selected' value='none'>-- Country --&nbsp;</option><option  value='AF'>Afghanistan&nbsp;</option><option  value='AL'>Albania&nbsp;</option><option  value='DZ'>Algeria&nbsp;</option><option  value='AS'>American Samoa&nbsp;</option><option  value='AD'>Andorra&nbsp;</option><option  value='AO'>Angola&nbsp;</option><option  value='AI'>Anguilla&nbsp;</option><option  value='AQ'>Antarctica&nbsp;</option><option  value='AG'>Antigua and Barbuda&nbsp;</option><option  value='AR'>Argentina&nbsp;</option><option  value='AM'>Armenia&nbsp;</option><option  value='AW'>Aruba&nbsp;</option><option  value='AU'>Australia&nbsp;</option><option  value='AT'>Austria&nbsp;</option><option  value='AZ'>Azerbaijan&nbsp;</option><option  value='BS'>Bahamas&nbsp;</option><option  value='BH'>Bahrain&nbsp;</option><option  value='BD'>Bangladesh&nbsp;</option><option  value='BB'>Barbados&nbsp;</option><option  value='BY'>Belarus&nbsp;</option><option  value='BE'>Belgium&nbsp;</option><option  value='BZ'>Belize&nbsp;</option><option  value='BJ'>Benin&nbsp;</option><option  value='BM'>Bermuda&nbsp;</option><option  value='BT'>Bhutan&nbsp;</option><option  value='BO'>Bolivia&nbsp;</option><option  value='BA'>Bosnia and Herzegovina&nbsp;</option><option  value='BW'>Botswana&nbsp;</option><option  value='BV'>Bouvet Island&nbsp;</option><option  value='BR'>Brazil&nbsp;</option><option  value='IO'>British Indian Ocean Territory&nbsp;</option><option  value='BN'>Brunei Darussalam&nbsp;</option><option  value='BG'>Bulgaria&nbsp;</option><option  value='BF'>Burkina Faso&nbsp;</option><option  value='BI'>Burundi&nbsp;</option><option  value='KH'>Cambodia&nbsp;</option><option  value='CM'>Cameroon&nbsp;</option><option  value='CA'>Canada&nbsp;</option><option  value='CV'>Cape Verde&nbsp;</option><option  value='KY'>Cayman Islands&nbsp;</option><option  value='CF'>Central African Republic&nbsp;</option><option  value='TD'>Chad&nbsp;</option><option  value='CL'>Chile&nbsp;</option><option  value='CN'>China&nbsp;</option><option  value='CX'>Christmas Island&nbsp;</option><option  value='CC'>Cocos (Keeling) Islands&nbsp;</option><option  value='CO'>Colombia&nbsp;</option><option  value='KM'>Comoros&nbsp;</option><option  value='CG'>Congo&nbsp;</option><option  value='CD'>Congo, the Democratic Rep.&nbsp;</option><option  value='CK'>Cook Islands&nbsp;</option><option  value='CR'>Costa Rica&nbsp;</option><option  value='CI'>Cote D'Ivoire&nbsp;</option><option  value='HR'>Croatia&nbsp;</option><option  value='CU'>Cuba&nbsp;</option><option  value='CY'>Cyprus&nbsp;</option><option  value='CZ'>Czech Republic&nbsp;</option><option  value='DK'>Denmark&nbsp;</option><option  value='DJ'>Djibouti&nbsp;</option><option  value='DM'>Dominica&nbsp;</option><option  value='DO'>Dominican Republic&nbsp;</option><option  value='EC'>Ecuador&nbsp;</option><option  value='EG'>Egypt&nbsp;</option><option  value='SV'>El Salvador&nbsp;</option><option  value='GQ'>Equatorial Guinea&nbsp;</option><option  value='ER'>Eritrea&nbsp;</option><option  value='EE'>Estonia&nbsp;</option><option  value='ET'>Ethiopia&nbsp;</option><option  value='FK'>Falkland Islands (Malvinas)&nbsp;</option><option  value='FO'>Faroe Islands&nbsp;</option><option  value='FJ'>Fiji&nbsp;</option><option  value='FI'>Finland&nbsp;</option><option  value='FR'>France&nbsp;</option><option  value='GF'>French Guiana&nbsp;</option><option  value='PF'>French Polynesia&nbsp;</option><option  value='TF'>French Southern Territories&nbsp;</option><option  value='GA'>Gabon&nbsp;</option><option  value='GM'>Gambia&nbsp;</option><option  value='GE'>Georgia&nbsp;</option><option  value='DE'>Germany&nbsp;</option><option  value='GH'>Ghana&nbsp;</option><option  value='GI'>Gibraltar&nbsp;</option><option  value='GR'>Greece&nbsp;</option><option  value='GL'>Greenland&nbsp;</option><option  value='GD'>Grenada&nbsp;</option><option  value='GP'>Guadeloupe&nbsp;</option><option  value='GU'>Guam&nbsp;</option><option  value='GT'>Guatemala&nbsp;</option><option  value='GN'>Guinea&nbsp;</option><option  value='GW'>Guinea-Bissau&nbsp;</option><option  value='GY'>Guyana&nbsp;</option><option  value='HT'>Haiti&nbsp;</option><option  value='HM'>Heard Island and Mcdonald Islands&nbsp;</option><option  value='VA'>Holy See (Vatican City State)&nbsp;</option><option  value='HN'>Honduras&nbsp;</option><option  value='HK'>Hong Kong&nbsp;</option><option  value='HU'>Hungary&nbsp;</option><option  value='IS'>Iceland&nbsp;</option><option  value='IN'>India&nbsp;</option><option  value='D'>Indonesia&nbsp;</option><option  value='IR'>Iran, Islamic Republic of&nbsp;</option><option  value='IQ'>Iraq&nbsp;</option><option  value='IE'>Ireland&nbsp;</option><option  value='IM'>Isle of Man&nbsp;</option><option  value='IL'>Israel&nbsp;</option><option  value='IT'>Italy&nbsp;</option><option  value='JM'>Jamaica&nbsp;</option><option  value='JP'>Japan&nbsp;</option><option  value='JO'>Jordan&nbsp;</option><option  value='KZ'>Kazakhstan&nbsp;</option><option  value='KE'>Kenya&nbsp;</option><option  value='KI'>Kiribati&nbsp;</option><option  value='KP'>Korea, Democratic People's Rep.&nbsp;</option><option  value='KR'>Korea, Republic of&nbsp;</option><option  value='KW'>Kuwait&nbsp;</option><option  value='KG'>Kyrgyzstan&nbsp;</option><option  value='LA'>Lao People's Democratic Republic&nbsp;</option><option  value='LV'>Latvia&nbsp;</option><option  value='LB'>Lebanon&nbsp;</option><option  value='LS'>Lesotho&nbsp;</option><option  value='LR'>Liberia&nbsp;</option><option  value='LY'>Libyan Arab Jamahiriya&nbsp;</option><option  value='LI'>Liechtenstein&nbsp;</option><option  value='LT'>Lithuania&nbsp;</option><option  value='LU'>Luxembourg&nbsp;</option><option  value='MO'>Macao&nbsp;</option><option  value='MK'>Macedonia&nbsp;</option><option  value='MG'>Madagascar&nbsp;</option><option  value='MW'>Malawi&nbsp;</option><option  value='MY'>Malaysia&nbsp;</option><option  value='MV'>Maldives&nbsp;</option><option  value='ML'>Mali&nbsp;</option><option  value='MT'>Malta&nbsp;</option><option  value='MH'>Marshall Islands&nbsp;</option><option  value='MQ'>Martinique&nbsp;</option><option  value='MR'>Mauritania&nbsp;</option><option  value='MU'>Mauritius&nbsp;</option><option  value='YT'>Mayotte&nbsp;</option><option  value='MX'>Mexico&nbsp;</option><option  value='FM'>Micronesia, Federated States of&nbsp;</option><option  value='MD'>Moldova, Republic of&nbsp;</option><option  value='MC'>Monaco&nbsp;</option><option  value='MN'>Mongolia&nbsp;</option><option  value='MS'>Montserrat&nbsp;</option><option  value='MA'>Morocco&nbsp;</option><option  value='MZ'>Mozambique&nbsp;</option><option  value='MM'>Myanmar&nbsp;</option><option  value='NA'>Namibia&nbsp;</option><option  value='NR'>Nauru&nbsp;</option><option  value='NP'>Nepal&nbsp;</option><option  value='NL'>Netherlands&nbsp;</option><option  value='AN'>Netherlands Antilles&nbsp;</option><option  value='NC'>New Caledonia&nbsp;</option><option  value='NZ'>New Zealand&nbsp;</option><option  value='NI'>Nicaragua&nbsp;</option><option  value='NE'>Niger&nbsp;</option><option  value='NG'>Nigeria&nbsp;</option><option  value='NU'>Niue&nbsp;</option><option  value='NF'>Norfolk Island&nbsp;</option><option  value='MP'>Northern Mariana Islands&nbsp;</option><option  value='NO'>Norway&nbsp;</option><option  value='OM'>Oman&nbsp;</option><option  value='PK'>Pakistan&nbsp;</option><option  value='PW'>Palau&nbsp;</option><option  value='PS'>Palestinian Territory, Occupied&nbsp;</option><option  value='PA'>Panama&nbsp;</option><option  value='PG'>Papua New Guinea&nbsp;</option><option  value='PY'>Paraguay&nbsp;</option><option  value='PE'>Peru&nbsp;</option><option  value='PH'>Philippines&nbsp;</option><option  value='PN'>Pitcairn&nbsp;</option><option  value='PL'>Poland&nbsp;</option><option  value='PT'>Portugal&nbsp;</option><option  value='PR'>Puerto Rico&nbsp;</option><option  value='QA'>Qatar&nbsp;</option><option  value='RE'>Reunion&nbsp;</option><option  value='RO'>Romania&nbsp;</option><option  value='RU'>Russian Federation&nbsp;</option><option  value='RW'>Rwanda&nbsp;</option><option  value='SH'>Saint Helena&nbsp;</option><option  value='KN'>Saint Kitts and Nevis&nbsp;</option><option  value='LC'>Saint Lucia&nbsp;</option><option  value='PM'>Saint Pierre and Miquelon&nbsp;</option><option  value='VC'>Saint Vincent and the Grenadines&nbsp;</option><option  value='WS'>Samoa&nbsp;</option><option  value='SM'>San Marino&nbsp;</option><option  value='ST'>Sao Tome and Principe&nbsp;</option><option  value='SA'>Saudi Arabia&nbsp;</option><option  value='SN'>Senegal&nbsp;</option><option  value='CS'>Serbia and Montenegro&nbsp;</option><option  value='SC'>Seychelles&nbsp;</option><option  value='SL'>Sierra Leone&nbsp;</option><option  value='SG'>Singapore&nbsp;</option><option  value='SK'>Slovakia&nbsp;</option><option  value='SI'>Slovenia&nbsp;</option><option  value='SB'>Solomon Islands&nbsp;</option><option  value='SO'>Somalia&nbsp;</option><option  value='ZA'>South Africa&nbsp;</option><option  value='GS'>South Georgia &#38; S. Sandwich Is.&nbsp;</option><option  value='ES'>Spain&nbsp;</option><option  value='LK'>Sri Lanka&nbsp;</option><option  value='SD'>Sudan&nbsp;</option><option  value='SR'>Suriname&nbsp;</option><option  value='SJ'>Svalbard and Jan Mayen&nbsp;</option><option  value='SZ'>Swaziland&nbsp;</option><option  value='SE'>Sweden&nbsp;</option><option value='CH'>Switzerland&nbsp;</option><option  value='SY'>Syrian Arab Republic&nbsp;</option><option  value='TW'>Taiwan&nbsp;</option><option  value='TJ'>Tajikistan&nbsp;</option><option  value='TZ'>Tanzania, United Republic of&nbsp;</option><option  value='TH'>Thailand&nbsp;</option><option  value='TL'>Timor-Leste&nbsp;</option><option  value='TG'>Togo&nbsp;</option><option  value='TK'>Tokelau&nbsp;</option><option  value='TO'>Tonga&nbsp;</option><option  value='TT'>Trinidad and Tobago&nbsp;</option><option  value='TN'>Tunisia&nbsp;</option><option  value='TR'>Turkey&nbsp;</option><option  value='TM'>Turkmenistan&nbsp;</option><option  value='TC'>Turks and Caicos Islands&nbsp;</option><option  value='TV'>Tuvalu&nbsp;</option><option  value='UG'>Uganda&nbsp;</option><option  value='UA'>Ukraine&nbsp;</option><option  value='AE'>United Arab Emirates&nbsp;</option><option  value='GB'>United Kingdom&nbsp;</option><option  value='US'>United States&nbsp;</option><option  value='UM'>United States Minor Outlying Is.&nbsp;</option><option  value='UY'>Uruguay&nbsp;</option><option  value='UZ'>Uzbekistan&nbsp;</option><option  value='VU'>Vanuatu&nbsp;</option><option  value='VE'>Venezuela&nbsp;</option><option  value='VN'>Viet Nam&nbsp;</option><option  value='VG'>Virgin Islands, British&nbsp;</option><option  value='VI'>Virgin Islands, U.s.&nbsp;</option><option  value='WF'>Wallis and Futuna&nbsp;</option><option  value='EH'>Western Sahara&nbsp;</option><option  value='YE'>Yemen&nbsp;</option><option  value='ZM'>Zambia&nbsp;</option><option  value='ZW'>Zimbabwe&nbsp;</option></select> ";
}

function planyo_set_element_value(el, value) {
	if (el.is(":checkbox"))
		  el.prop ('checked', (value == 'on' || value == 'yes' || value == '1') ? true : false);
  else
		el.val(convert_entities_to_utf8(value));
}

function planyo_prefill_params(params) {
  for (var p in params) {
    var name = params[p];
    if (typeof name == 'string') {
      var value = planyo_get_param(name);
      if (value && jQuery('#' + name).length > 0)
					planyo_set_element_value(jQuery('#' + name), value);
      if (value && jQuery('#box_' + name).length > 0)
					planyo_set_element_value(jQuery('#box_' + name), value);
      if (value && name.indexOf('box_') == 0 && jQuery('#' + name.substr(4)).length > 0)
					planyo_set_element_value(jQuery('#' + name.substr(4)), value);
    }
  }
}

function get_planyo_mode() {
  if (window.planyo_force_mode)
    return window.planyo_force_mode;
  if (planyo_get_param('mode'))
    return planyo_get_param('mode');

  var resource_id = planyo_get_param('resource_id') || (planyo_isset(planyo_settings.planyo_resource_id) && planyo_settings.planyo_resource_id > 0);
  if (resource_id && (planyo_get_param('prefill') || planyo_get_param('submitted')))
    return 'reserve';
  else if (resource_id)
    return 'resource_desc';
  else if (planyo_get_param('presentation_mode') == '1') {
    if (String(window.planyo_site_id).indexOf('M') == 0 && !planyo_get_param('site_id'))
      return 'site_list';
    else
      return 'resource_list';
  }

  if (planyo_settings.planyo_default_mode)
    return planyo_settings.planyo_default_mode;

  return 'search';
}

function planyo_get_additional_props(form, els) {
  jQuery(":input[name*='prop_res_']").each(function() {
    els[els.length] = this.name;
  });
  jQuery(":input[name*='rental_prop_']").each(function() {
    els[els.length] = this.name;
  });
  return els;
}

function planyo_form_loaded(code) {
  if (!code || code.substr (0, 5) == "Error") {
    window.location = (planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/reserve.php?calendar=" + planyo_settings.planyo_site_id;
  }
  else {
    code = code.replace("{PH-mobile_country_param}", planyo_get_phone_codes("mobile_country_param"));
    code = code.replace("{PH-phone_country_param}", planyo_get_phone_codes("phone_country_param"));
    code = code.replace("{CO-country_param}", planyo_get_country_codes());
    jQuery('#planyo_content').html(code);

    var planyo_mode = planyo_get_param('mode');
    if (!planyo_is_special_mode(planyo_mode))
      planyo_mode = get_planyo_mode();
    if (planyo_mode == 'search') {
      jQuery('#search_form').submit(planyo_send_search_form);
        var params_array = new Array('start_date', 'end_date', 'one_date', 'start_time', 'end_time', 'sort', 'box_start_date', 'box_end_date', 'box_one_date', 'box_start_time', 'box_end_time', 'rental_time_value', 'is_night');
      planyo_get_additional_props(jQuery('#search_form'), params_array);
      planyo_prefill_params(params_array);
      if (planyo_get_param('submitted')) {
	        planyo_send_search_form();
      }
    }
    else if (planyo_mode == 'upcoming_availability') {
        var params_array = new Array('sort');
      planyo_get_additional_props (jQuery('#availability_form'), params_array);
      planyo_prefill_params(params_array);
    }
    else if (planyo_mode == 'reserve') {
      document.planyo_no_hourglass = true;
      setTimeout("if(window.planyo_dates_changed) planyo_dates_changed();", 100);
      //setTimeout("if(window.planyo_dates_changed) planyo_dates_changed();", 1500);
      setTimeout("if(window.planyo_dates_changed) {planyo_dates_changed();document.planyo_no_hourglass = false;}", 5000);
      jQuery('#reserve_form').submit(planyo_send_reservation_form);
      jQuery('#product_form').submit(planyo_send_product_form);
      if (planyo_isset(planyo_settings.planyo_login)) {
        for (key in planyo_settings.planyo_login) {
          if (key == 'login_cs' || key == 'first_name' || key == 'last_name' || key == 'email') {
            var value = planyo_settings.planyo_login[key];
            var mod_key = key;
            if (key == 'first_name') 
              mod_key = 'first';
            else if (key == 'last_name')
              mod_key = 'last';
            else if (key == 'email')
              value = value.replace("[at]", "@");
            if (value && jQuery('#'+mod_key) && !jQuery('#'+mod_key).val())
              jQuery('#'+mod_key).val(value);
          }
        }
      }
        var params_array = new Array('start_date', 'end_date', 'one_date', 'start_time', 'end_time', 'box_start_date', 'box_end_date', 'box_one_date', 'box_start_time', 'box_end_time', 'rental_time_value', 'first', 'last', 'address', 'city', 'zip', 'state', 'country', 'user_notes', 'email', 'mobile_country_param','mobile_number_param','phone_country_param','phone','quantity');
      planyo_get_additional_props(jQuery('#reserve_form'), params_array);
      planyo_prefill_params(params_array);
      handle_custom_price_element ();
      if (window.js_qty_changed) js_qty_changed();
    }
    else if (planyo_mode == 'modify_data_form') {
      jQuery('#modify_data_form').submit(planyo_send_modify_data_form);
    }
    else if (planyo_mode == 'additional_products_code') {
      jQuery('#product_form').submit(planyo_send_product_form);
      handle_custom_price_element ();
      jQuery('html, body').animate({scrollTop: jQuery("#planyo_content").offset().top}, 1000);
    }
    else if (planyo_mode == 'checkout') {
      if (planyo_isset(window.js_submit_payment_form))
        window.js_submit_payment_form();
    }
    else if (planyo_mode == 'show_coupons') {
      jQuery('#add_coupon').submit(planyo_send_add_coupon_form);
    }
    else if (planyo_mode == 'widget') {
      if(jQuery('#widget_form').length)
	  jQuery('#widget_form').submit(planyo_send_widget_form);
    }
    if (planyo_isset(window.on_planyo_form_loaded))
      window.on_planyo_form_loaded((planyo_mode == 'widget' && planyo_get_param('ppp_widget_type')) ? planyo_get_param('ppp_widget_type') : planyo_mode);
  }
}

function handle_custom_price_element () {
  if(jQuery('#planyo_price_holder').length) {
    jQuery('#planyo_price_holder').append(jQuery('#price_info'));
    jQuery('#planyo_price_holder').css('display','')
    if (jQuery('#price_info_div'))
      jQuery('#price_info_div').css('display','none')
  }
}

function planyo_embed_code(form_url, form_url_params) {
  if (planyo_settings.planyo_language)
    form_url_params += "&language=" + planyo_settings.planyo_language;
  form_url_params += "&html_content_type=1";
  form_url_params += "&plugin_mode=10";
  var shopping_cart_id = planyo_get_cookie('planyo_cart_id');
  if (shopping_cart_id && form_url_params.indexOf('cart_id=') == -1)
    form_url_params += "&cart_id=" + shopping_cart_id + "&first_reservation_id=" + planyo_get_cookie('planyo_first_reservation_id');
  if (planyo_isset(planyo_settings.planyo_login) && planyo_isset(planyo_settings.planyo_login, 'login_cs') && planyo_isset(planyo_settings.planyo_login, 'email'))
    form_url_params += "&login_cs=" + planyo_settings.planyo_login['login_cs'] + "&login_email=" + planyo_settings.planyo_login['email'];
  var ppp_params = planyo_get_prefixed_params('ppp_', form_url_params);
  if (ppp_params.length > 0)
    form_url_params += "&" + ppp_params;
  var cookieval = planyo_get_cookie('ppp_refcode');
  if (cookieval && !planyo_get_param('ppp_refcode'))
      form_url_params += "&ppp_refcode="+cookieval;
  form_url_params += "&user_agent=" + encodeURIComponent(navigator.userAgent);
  document.planyo_file_path = get_full_planyo_file_path('');
  if (planyo_settings.ulap_script.toLowerCase() == 'jsonp')
      form_url_params += "&html_mode=1&modver=2.6";
  var form_url_params_arr = planyo_unserialize(form_url_params);
  if (planyo_settings.ulap_script.toLowerCase() == 'jsonp') {
      jQuery.ajax({
          url: (planyo_settings.planyo_use_https ? "https://" : "http://") + "www.planyo.com/rest/ulap-jsonp.php",
          data: form_url_params_arr,
          dataType: 'jsonp',
          success: function(data) {
              planyo_form_loaded(data.html);
          },
          error:function(e) {
              console.log(e.message);
          }
      });
  }
  else {
      jQuery.post(form_url,form_url_params_arr,planyo_form_loaded);
  }
}

function planyo_embed_non_reservation_step_additional_products_form (reservation_id, user_id) {
  if (reservation_id) {
    var form_url = planyo_settings.ulap_script;
    var form_url_params = "ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php&mode=display_additional_products_code&reservation_id="+reservation_id+"&user_id="+user_id+"&non_reservation_step=1&feedback_url="+planyo_get_current_url();
    planyo_embed_code(form_url,form_url_params);
  }
}

function planyo_embed_modify_data_form (reservation_id, user_id) {
  if (reservation_id) {
    var form_url = planyo_settings.ulap_script;
    var form_url_params = "ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php&mode=modify_data_form&reservation_id="+reservation_id+"&user_id="+user_id+"&feedback_url="+planyo_get_current_url();
    planyo_embed_code(form_url,form_url_params);
  }
}

function planyo_embed_cancel_code(site_id, reservation_id, c, user_id, resource_id) {
  if (reservation_id) {
    var form_url = planyo_settings.ulap_script;
    var form_url_params = "ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php&mode=display_cancel_code&reservation_id="+reservation_id+"&c="+c+"&resource_id="+resource_id+"&user_id="+user_id+"&feedback_url="+planyo_get_current_url()+"&site_id="+site_id;
    planyo_embed_code(form_url,form_url_params);
  }
}

function planyo_embed_use_coupon_code (site_id, reservation_id, coupon_id, user_id) {
  if (site_id && reservation_id && coupon_id) {
    var form_url = planyo_settings.ulap_script;
    var form_url_params = "ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php&mode=use_coupon&feedback_url="+planyo_get_current_url()+"&site_id="+site_id;
    form_url_params += "&reservation_id=" + reservation_id + "&coupon_id=" + coupon_id + "&user_id=" + user_id;
    planyo_embed_code (form_url,form_url_params);
  }
}

function planyo_embed_coupon_payment_confirmation_code (site_id, resource_id, coupon_payment_id) {
  if (site_id) {
    var form_url = planyo_settings.ulap_script;
    var form_url_params = "ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php&mode=coupon_payment_confirmation&feedback_url="+planyo_get_current_url()+"&site_id="+site_id;
    if (resource_id)
      form_url_params += "&resource_id=" + resource_id;
    form_url_params += "&coupon_payment_id=" + coupon_payment_id;
    planyo_embed_code (form_url,form_url_params);
  }
}

function planyo_embed_show_coupons_form (site_id, resource_id, coupon_user_email) {
  if (site_id) {
    var form_url = planyo_settings.ulap_script;
    var form_url_params = "ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php&mode=display_coupons_code&feedback_url="+planyo_get_current_url()+"&site_id="+site_id;
    if (resource_id)
      form_url_params += "&resource_id=" + resource_id;
    if (!coupon_user_email)
      coupon_user_email = planyo_get_param('coupon_user_email_prefill');
    if (coupon_user_email)
      form_url_params += "&coupon_user_email=" + coupon_user_email;
    planyo_embed_code (form_url,form_url_params);
  }
}

function planyo_embed_reservation_details(site_id, reservation_id, user_id) {
  if (reservation_id) {
    var form_url = planyo_settings.ulap_script;
    var form_url_params = "ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php&mode=display_reservation_details_code&reservation_id="+reservation_id+"&user_id="+user_id+"&feedback_url="+planyo_get_current_url()+"&site_id="+site_id;
    planyo_embed_code(form_url,form_url_params);
  }
}

function planyo_embed_reservation_list(site_id, reservation_id, user_id) {
  var form_url = planyo_settings.ulap_script;
  var form_url_params = "ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php&mode=display_reservation_list_code";
  form_url_params += "&feedback_url="+planyo_get_current_url()+"&site_id="+site_id;
  if (reservation_id && user_id)
    form_url_params += "&reservation_id="+reservation_id+"&user_id="+user_id;

  planyo_embed_code(form_url,form_url_params);
}

function planyo_embed_payment_form(site_id, reservation_id, user_id, auto_redirect) {
  if (reservation_id) {
    var form_url = planyo_settings.ulap_script;
    var form_url_params = "ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php&mode=display_payment_form_code&reservation_id="+reservation_id+"&user_id="+user_id+"&feedback_url="+planyo_get_current_url()+"&site_id="+site_id+(auto_redirect ? "&auto_redirect="+auto_redirect : "");
    var amount = planyo_get_param('amount');
    if (amount)
      form_url_params += "&amount="+amount;
    planyo_embed_code(form_url,form_url_params);
  }
}

function planyo_embed_cart_code(site_id, cart_id, first_reservation_id, remove_reservation_id) {
  if (!cart_id) {
    cart_id = planyo_get_cookie('planyo_cart_id');
    first_reservation_id = planyo_get_cookie('planyo_first_reservation_id');
  }
  var form_url = planyo_settings.ulap_script;
  var form_url_params = "ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php&mode=display_cart_code&first_reservation_id="+first_reservation_id+"&remove_reservation_id="+(remove_reservation_id ? remove_reservation_id : "")+"&cart_id="+cart_id+"&feedback_url="+planyo_get_current_url()+"&site_id="+site_id;
  planyo_embed_code(form_url,form_url_params);
}

function planyo_embed_checkout_code(site_id, cart_id, first_reservation_id) {
  if (!cart_id) {
    cart_id = planyo_get_cookie('planyo_cart_id');
    first_reservation_id = planyo_get_cookie('planyo_first_reservation_id');
  }
  var form_url = planyo_settings.ulap_script;
  var form_url_params = "ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php&mode=display_checkout_code&first_reservation_id="+first_reservation_id+"&cart_id="+cart_id+"&feedback_url="+planyo_get_current_url()+"&site_id="+site_id;
  planyo_embed_code(form_url,form_url_params);
}

function planyo_embed_widget(site_id) {
  var form_url = planyo_settings.ulap_script;
  var form_url_params = "ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php&mode=display_widget&feedback_url="+planyo_get_current_url()+"&site_id="+site_id;
  planyo_embed_code(form_url,form_url_params);
}

function planyo_embed_reservation_form(site_id, resource_id) {
  var form_url = planyo_settings.ulap_script;
  if (!resource_id) {
    if (planyo_isset(window.planyo_resource_id))
      resource_id = window.planyo_resource_id;
    else
      resource_id = '';
  }
  var date_str = '';
  if (planyo_get_param('start_date'))
    date_str = "&start_date=" + planyo_get_param('start_date');
  else if (planyo_get_param('one_date'))
    date_str = "&one_date=" + planyo_get_param('one_date');
  var form_url_params = "ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php&mode=display_reservation_form_code&site_id=" + site_id + "&resource_id=" + resource_id + "&feedback_url=" + planyo_get_current_url() + date_str;
  if (resource_id)
    window.planyo_resource_id = resource_id;
  planyo_embed_code(form_url,form_url_params);
}

function planyo_embed_additional_products_form(reservation_id, ppp_rs) {
  if (reservation_id) {
    var form_url = planyo_settings.ulap_script;
    var form_url_params = "ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php&mode=display_additional_products_code&reservation_id=" + reservation_id + (ppp_rs ? ("&ppp_rs=" + ppp_rs) : "") + "&feedback_url=" + planyo_get_current_url();
    planyo_embed_code(form_url,form_url_params);
  }
}

function planyo_embed_upcoming_availability_search_form(site_id) {
  var form_url = planyo_settings.ulap_script;
  var form_url_params = "ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php&mode=display_upcoming_availability_search_form_code&feedback_url=" + planyo_get_current_url() + "&extra_search_fields=" + (planyo_get_param('extra_search_fields') ? planyo_get_param('extra_search_fields') : planyo_settings.extra_search_fields) + "&site_id="+site_id + "&sort_fields=" + (planyo_get_param('sort_fields') ? planyo_get_param('sort_fields') : planyo_settings.sort_fields);
  planyo_embed_code (form_url, form_url_params);
}

function planyo_embed_search_form(site_id) {
  var form_url = planyo_settings.ulap_script;
  var form_url_params = "ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php&mode=display_search_form_code&feedback_url=" + planyo_get_current_url() + "&extra_search_fields=" + (planyo_get_param('extra_search_fields') ? planyo_get_param('extra_search_fields') : planyo_settings.extra_search_fields) + "&site_id=" + site_id;
  if (planyo_isset(planyo_settings.sort_fields))
    form_url_params += "&sort_fields=" + planyo_settings.sort_fields;
  var range_search = planyo_get_param('range_search');
  if (!range_search)
    range_search = document.range_search;
  if (range_search)
    form_url_params += "&range_search="+range_search;
  if (planyo_get_param('separate-units'))
      form_url_params += "&separate-units="+planyo_get_param('separate-units');
  var search_model_site = planyo_get_param('search_model_site');
  if (search_model_site)
    form_url_params += "&search_model_site="+search_model_site;
  planyo_embed_code(form_url, form_url_params);
}

function planyo_embed_resource_list(site_id) {
  var form_url = planyo_settings.ulap_script;
  var form_url_params = "ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php&mode=display_resource_list_code&feedback_url=" + planyo_get_current_url() + "&site_id=" + site_id;
  if (String(planyo_settings.planyo_site_id).indexOf('M')==0)
    form_url_params += "&metasite_id="+planyo_settings.planyo_site_id.substr(1);
  if (planyo_get_param('res_filter_name'))
    form_url_params += '&' + planyo_get_param('res_filter_name') + '=' + planyo_get_param('res_filter_value');
  if (planyo_get_param('cal_filter_name'))
    form_url_params += '&' + planyo_get_param('cal_filter_name') + '=' + planyo_get_param('cal_filter_value');
  if (planyo_get_param('sort'))
    form_url_params += '&sort=' + planyo_get_param('sort');
  else if (planyo_settings.planyo_resource_ordering)
    form_url_params += '&sort=' + planyo_settings.planyo_resource_ordering;
  planyo_embed_code(form_url, form_url_params);
}

function planyo_embed_site_list(metasite_id) {
  var form_url = planyo_settings.ulap_script;
  var form_url_params = "ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php&mode=display_site_list_code&feedback_url=" + planyo_get_current_url() + "&metasite_id=" + metasite_id;
  planyo_embed_code(form_url, form_url_params);
}

function planyo_embed_resource_desc(resource_id, site_id) {
  if (!resource_id) {
    if (planyo_isset(window.planyo_resource_id))
      resource_id = window.planyo_resource_id;
    else
      resource_id = '';
  }
  if (resource_id)
    window.planyo_resource_id = resource_id;

  var form_url = planyo_settings.ulap_script;
  var form_url_params = "ulap_url="+(planyo_settings.planyo_use_https ? "https" : "http")+"://www.planyo.com/rest/planyo-reservations.php&mode=display_single_resource_code&feedback_url=" + planyo_get_current_url() + "&resource_id=" + resource_id +"&site_id="+site_id;
  planyo_embed_code(form_url, form_url_params);
}

function planyo_hide_element (el, hide) {
  if (el)
    el.css('display',hide ? 'none' : '');
}

function planyo_on_calprev_data_fetched(month, year, data_root) {
  js_save_fetched_data(month, year, data_root);
  if (document.current_picked_month == month && document.current_picked_year == year && document.current_picked_id) {
    planyo_show_calendar_picker (document.current_picked_month, document.current_picked_year, document.current_picked_id, 'planyo_calendar_date_chosen');
  }
}

function planyo_calprev_msg_listener(event) {
  if (event && event.data && event.data.length > 3 && (event.data.substr(0, 3) == 'AVA' || event.data.substr(0, 3) == 'RES' || event.data.substr(0, 3) == 'SIZ')) {
    var data_root = jQuery.parseJSON(event.data.substr(3));
    if (data_root && event.data.substr(0, 3) == 'RES') {
      document.resources = data_root;
      document.preview_sync_src = event.source;
    }
    else if (data_root && event.data.substr(0, 3) == 'SIZ') {
      if (jQuery('#'+data_root.iframe_id).length) {
        var ifr=jQuery('#'+data_root.iframe_id);
        if(planyo_isset(data_root.is_mobile) && data_root.is_mobile)
          ifr.width('100%');
        else
          ifr.width(data_root.x+'px');
        ifr.height(data_root.y+'px');
        ifr.css('overflow','visible');
      }
    }
    else if (data_root && planyo_isset(data_root, 'db_data') && event.data.substr(0, 3) == 'AVA') {
      planyo_on_calprev_data_fetched (data_root['month'], data_root['year'], data_root);
			for (var m = 2; m <= 4; m++) {
			  if (planyo_isset (data_root, 'month'+m)) {
			    planyo_on_calprev_data_fetched (data_root['month'+m]['month'], data_root['month'+m]['year'], data_root['month'+m]);
        }
      }
      if(document.prev_planyo_start_date && window.planyo_check_av_hours && !document.planyo_all_start_hours)
          window.planyo_check_av_hours(document.prev_planyo_start_date);
    }
  }
}

function init_planyo() {
  if(!window.jQuery) {
    document.getElementById('planyo_content').innerHTML = "<span id='nolib_error' style='color:red'>Planyo error: jQuery has not been loaded. Either add jQuery to this page or change the plugin settings to include it for you.</span>";
  }
  if (window.Drupal && !planyo_isset(window.planyo_dont_use_drupal_plugin) && planyo_isset(Drupal.settings.planyo))
    planyo_settings = Drupal.settings.planyo;
  if(jQuery('#planyo_price_holder').length)
    jQuery('#planyo_price_holder').css('display','none')
  if ((!window.Drupal || planyo_isset(window.planyo_dont_use_drupal_plugin)) && !planyo_isset(window.ulap_script_modified) && planyo_settings.ulap_script.toLowerCase() != 'jsonp') {
      planyo_settings.ulap_script = get_full_planyo_file_path(planyo_settings.ulap_script);
      window.ulap_script_modified = true;
  }
  if (planyo_settings.planyo_site_id == 'demo')
    planyo_settings.planyo_site_id = 11; // display the demo site
  if (planyo_get_param('planyo_lang'))
    planyo_settings.planyo_language = planyo_get_param('planyo_lang');
  else if (planyo_get_param('lang'))
    planyo_settings.planyo_language = planyo_get_param('lang');
  if (!planyo_init_special_modes()) {
    var planyo_mode = get_planyo_mode();
    if (planyo_mode == 'empty' && planyo_get_param('submitted'))
      planyo_mode = 'reserve';
    if (planyo_mode == 'search')
      planyo_embed_search_form(planyo_settings.planyo_site_id);
    else if (planyo_mode == 'upcoming_availability')
      planyo_embed_upcoming_availability_search_form(planyo_settings.planyo_site_id);
    else if (planyo_mode == 'reserve')
      planyo_embed_reservation_form(planyo_settings.planyo_site_id, planyo_get_param('resource_id'));
    else if (planyo_mode == 'resource_list')
      planyo_embed_resource_list((planyo_get_param('site_id') && String(planyo_settings.planyo_site_id).indexOf('M')==0) ? planyo_get_param('site_id') : planyo_settings.planyo_site_id);
    else if (planyo_mode == 'site_list' && String(planyo_settings.planyo_site_id).indexOf('M')==0)
      planyo_embed_site_list(planyo_settings.planyo_site_id.substr(1));
    else if (planyo_mode == 'resource_desc')
      planyo_embed_resource_desc(planyo_get_param('resource_id'), planyo_settings.planyo_site_id);
    else if (planyo_mode == 'empty')
      jQuery('#planyo_content').html('');
  }
  var refcode_param = planyo_get_param('ppp_refcode');
  if (refcode_param)
      planyo_set_cookie('ppp_refcode', refcode_param, 1);
}

if (window.addEventListener)
  addEventListener("message", planyo_calprev_msg_listener, false);
else if (window.attachEvent)
  attachEvent("onmessage", planyo_calprev_msg_listener);
