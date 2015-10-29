(function ($) {

  $(document).ready(function () {
    function stemm_phrase(event) {
      var phrase_in = $(this).attr("value");
      var url = $(this).attr("href");
      var module = $(this).attr("module");

	 // alert('module='+module+'\nphrase='+phrase_in+'\nurl='+url);
      var phrase = function(data) 
      {
        $('#phrase-out').attr('value',data.phrase_out);
      }

	// make the AJX call. 
	// If successful the function pointed to by "success" is called
      $.ajax({
        type:	"POST",
        url:	url,
	dataType: "json",
	data:	{ "js" : "1",
		  "phrase_in" : phrase_in,
		  "module" : module,
		},
	success: phrase,
        error: function (xmlhttp,textStatus,errorThrown) {
                 alert('module='+module+
                      '\nphrase='+phrase_in+
                      '\nurl='+url+
                      '\n\nerror='+textStatus + 
                      '\nstatus='+ xmlhttp.status + 
                      '\nreadyState='+ xmlhttp.readyState);
      		 return false;
               }
      });
	// prevent the browser from handling the event
      return false;
    };

    $('#phrase-in').bind('change', stemm_phrase);

    $('#phrase-in').bind('keydown keypress', function(event){
	// when ENTER is pressed then stemm_phrase is called twice. But why?
	// When the call to trigger() is commented out, 
	// then stemm_phrase is called not at all
        if ( event.keyCode == 13 ) {	// enter key pressed
	  $('#phrase-in').trigger('change');
          event.preventDefault();
	  event.stopPropagation();
      	  return false;
	}
    });

  });
})(jQuery);
