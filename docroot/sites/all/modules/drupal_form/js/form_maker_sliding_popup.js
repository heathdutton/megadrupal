var isPopUpOpened = false;

function form_maker_createpopup(url, height, duration, description, lifetime) {  isPopUpOpened = true;
  if (form_maker_hasalreadyreceivedpopup(description) || form_maker_isunsupporteduseragent()) {
    return;
  }
  var old_div = jQuery("#sliding_popup");
  jQuery.get(url, function(data) {
		var popup = jQuery("<div style='z-index:1000000;border-top:10px solid #000000;'>" + data + "</div>")
			.attr({ "id": "sliding_popup" })
			.css({"bottom": -height})
	    .height(height)
			.hide()
			.appendTo("body");
		form_maker_showpopup(description, lifetime, popup, duration, old_div);
    setTimeout(function() {old_div.remove();}, 10);
	});
}

function form_maker_showpopup(description, lifetime, popup, duration, old_div) {
  isPopUpOpened = true;
	popup.show().animate( { bottom: 0 }, duration);
	form_maker_receivedpopup(description, lifetime);
}

function form_maker_hasalreadyreceivedpopup(description) {
  if (document.cookie.indexOf(description) > -1) {
    delete document.cookie[document.cookie.indexOf(description)];
  }
	return false; 
}

function form_maker_receivedpopup(description, lifetime) { 
	var date = new Date(); 
	date.setDate(date.getDate() + lifetime); 
	document.cookie = description + "=true;expires=" + date.toUTCString() + ";path=/"; 
}

function form_maker_isunsupporteduseragent() { 
	return (!window.XMLHttpRequest); 
}

function form_maker_destroypopup(duration) {
  while (document.getElementById('sliding_popup') != null) {
    jQuery("#sliding_popup").remove();
  }
  isPopUpOpened = false;
}
