var isPopUpOpened = false;

function spider_calendar_createpopup(url, height, duration, description, lifetime) {
  // if (isPopUpOpened == true) {
    // return;
  // }
  isPopUpOpened = true;
  if (spider_calendar_hasalreadyreceivedpopup(description) || spider_calendar_isunsupporteduseragent()) {
    return;
  }
  var old_div = jQuery("#sliding_popup");
  jQuery.get(url, function(data) {
		var popup = jQuery("<div style='z-index:1000000;border-top:10px solid #000000;overflow:scroll;'>" + data + "</div>")
			.attr({ "id": "sliding_popup" })
			.css({"bottom": -height})
	    .height(height)
			.hide()
			.appendTo("body");
		spider_calendar_showpopup(description, lifetime, popup, duration, old_div);
    setTimeout(function() {old_div.remove();}, 10);
	});
}

function spider_calendar_showpopup(description, lifetime, popup, duration, old_div) {
  isPopUpOpened = true;
  popup.show().animate( { bottom: 0 }, duration);
	spider_calendar_receivedpopup(description, lifetime);
}

function spider_calendar_hasalreadyreceivedpopup(description) {
  if (document.cookie.indexOf(description) > -1) {
    delete document.cookie[document.cookie.indexOf(description)];
  }
	return false; 
}

function spider_calendar_receivedpopup(description, lifetime) { 
	var date = new Date(); 
	date.setDate(date.getDate() + lifetime); 
	document.cookie = description + "=true;expires=" + date.toUTCString() + ";path=/"; 
}

function spider_calendar_isunsupporteduseragent() { 
	return (!window.XMLHttpRequest); 
}

function spider_calendar_destroypopup(duration) {
  while (document.getElementById('sliding_popup') != null) {
    jQuery("#sliding_popup").remove();
    //jQuery("#sliding_popup").animate({ bottom: jQuery("#sliding_popup").height() * -1 }, duration, function () { jQuery("#sliding_popup").remove(); });
  }
  isPopUpOpened = false;
}
