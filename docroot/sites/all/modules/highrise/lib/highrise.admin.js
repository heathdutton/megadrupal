(function ($) {
  
   $(document).ready(function() {

    //  Replace the autocomplete functionality for this page so that we
    //  are able to dynamically use two fields to return results.
     if (Drupal.ACDB) {

	Drupal.ACDB.prototype.search = function (searchString) {
	var db = this;
	this.searchString = $('#edit-webform').val() + "/" +searchString;
	searchString = $('#edit-webform').val() + "/" +searchString;

	// See if this key has been searched for before.
	if (this.cache[searchString]) {
	   return this.owner.found(this.cache[searchString]);
	}

	// Initiate delayed search.
	if (this.timer) {
           clearTimeout(this.timer);
	}

	this.timer = setTimeout(function () {
	   db.owner.setStatus('begin');

	   // Ajax GET request for autocompletion.
           $.ajax({
      		type: 'GET',
      		url: db.uri + '/' + searchString,
      		dataType: 'json',
      		success: function (matches) {
        	if (typeof matches.status == 'undefined' || matches.status != 0) {
          		db.cache[searchString] = matches;
          		// Verify if these are still the matches the user wants to see.
          		if (db.searchString == searchString) {
            			db.owner.found(matches);
          		}
          		db.owner.setStatus('found');
        	}
      	   },
           error: function (xmlhttp) {
           alert(Drupal.ajaxError(xmlhttp, db.uri));
         } 
      });
  }, this.delay);
};

}

$("#edit-webform").change(function() {
	$("#edit-first-name").val("");
	$("#edit-last-name").val("");
	$("#edit-email-address").val("");
	$("#edit-company-name").val("");
	$("#edit-phone-number").val("");
	$("#edit-title").val("");
	$("#edit-edit-background").val("");
});


}); 

})(jQuery);

