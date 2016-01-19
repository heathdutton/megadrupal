(function($){

	$(document).ready(function() {

		$('div[data-rot13]').each(function(){
		  var $this = $(this);
			var s = $this.data('rot13');
      s = s.replace(/[a-zA-Z]/g,function(c){return String.fromCharCode((c<="Z"?90:122)>=(c=c.charCodeAt(0)+13)?c:c-26);});
			$this.replaceWith(s);
		});

	});

})(jQuery);
