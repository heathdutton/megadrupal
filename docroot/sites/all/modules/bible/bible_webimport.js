(function ($) {

Drupal.behaviors.biblewebimport = {
	attach : function (context, settings) {
	    $('#progress').each(function () {
			var holder = this;
			var url = Drupal.settings.bibleurl;
			var para = Drupal.settings.biblepara;
			var inimsg = Drupal.settings.bibleimsg;
			var errmsg = Drupal.settings.bibleemsg;
			if (!url) return;	

			// Success: redirect to the summary.
			var updateCallback = function (progress, status, pb) {
				if (progress == 100) {
				  pb.stopMonitoring();
				  window.location = url;
				}
			}
	
			// Failure: point out error message and provide link to the summary.
			var errorCallback = function (pb) {
				var div = document.createElement('p');
				div.className = 'error';
				$(div).html(errmsg);
				$(holder).prepend(div);
				$('#wait').hide();
			}
	
			var progress = new Drupal.progressBar('updateprogress', updateCallback, "POST", errorCallback);
			progress.setProgress(-1, inimsg);
			$(holder).append(progress.element);
			progress.startMonitoring(url + para + "&op=bible_webimport", 0);
	    });
 	}
};

})(jQuery);
