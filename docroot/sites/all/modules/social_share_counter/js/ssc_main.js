var $ = jQuery.noConflict();
$(function($){
  $(".sharebox").on('click', '.switch', function() {
      $(this).hide();
      $(this).siblings('.secondary').css('display', 'inline-block');
      $(this).siblings('.secondary').children('.switch2').show();
  }); 
  $(".sharebox").on('click', '.switch2', function() {
      $(this).parent('.secondary').siblings('.switch').show();
      $(this).parent('.secondary').hide();         
  }); 
});
(function($){
	$.fn.cSButtons = function(options) {
		// Default params
  		var defaults = 
  		{
  			"url" 	: null,
  			"type" 	: 'facebook',
  			"txt"	: null,
  			"via"	: null,
  			"count"	: false,
  			"count_position" : 'right',
  			"apikey": null,
  			"media" : null,
  			"lang"	: 'en-US',
  			"total"	: null,
  		}

  		var parametres 	= $.extend(defaults, options);
  		var totalShare 	= 0;
  		var nbButtons 	= this.length;
  		var i 			= 0;
  		var jg			= 0;

  		if (Drupal.settings.SSC.isClean) {
  			Drupal.settings.SSC.share = Drupal.settings.SSC.share + '?';
  		}
  		else {
  			Drupal.settings.SSC.share = Drupal.settings.SSC.share + '&';
  		}

  		return this.each(function()	{	
  			i++;
  			// If option on the link attributs
  			var url 		= ($(this).attr('data-url') != undefined && $(this).attr('data-url') != '') ?  $(this).attr('data-url') : parametres.url;
  			var type 		= ($(this).attr('data-type') != undefined && $(this).attr('data-type') != '') ?  $(this).attr('data-type') : parametres.type;
  			var txt 		= ($(this).attr('data-txt') != undefined && $(this).attr('data-txt') != '') ?  $(this).attr('data-txt') : parametres.txt;
  			var via 		= ($(this).attr('data-via') != undefined && $(this).attr('data-via') != '') ?  $(this).attr('data-via') : parametres.via;
  			var count 		= ($(this).attr('data-count') != undefined && $(this).attr('data-count') != '') ?  $(this).attr('data-count') : parametres.count;
  			var cPosition 	= ($(this).attr('data-count-position') != undefined && $(this).attr('data-count-position') != '') ?  $(this).attr('data-count-position') : parametres.count_position;
  			var apikey 		= ($(this).attr('data-apikey') != undefined && $(this).attr('data-apikey') != '') ?  $(this).attr('data-apikey') : parametres.apikey;
  			var media 		= ($(this).attr('data-media') != undefined && $(this).attr('data-media') != '') ?  $(this).attr('data-media') : parametres.media;
  			var lang 		= ($(this).attr('data-lang') != undefined && $(this).attr('data-lang') != '') ?  $(this).attr('data-lang') : parametres.lang;
  			var popupWidth 	= 0;
  			var popupHeight = 0;
  			var shareUrl 	= '';

  			// Url use to share
  			url				= (url != null) ? url : document.URL;
  			$URL 			= url;


  			switch(type) {
  				case 'twitter': // Twitter share
  					var twitter = $(this);
  					popupWidth 	= 550;
  					popupHeight = 420;
  					shareUrl 	= (txt != null) ? 'https://twitter.com/intent/tweet?original_referer=' + encodeURIComponent(document.URL) + '&text=' + encodeURIComponent(txt) + '&url=' + encodeURIComponent(url) : 'https://twitter.com/intent/tweet?original_referer=' + encodeURIComponent(document.URL) + '&url=' + encodeURIComponent(url);
  					shareUrl	= (via != null) ? shareUrl + '&via=' + via : shareUrl;

  					if(count) {
  						$.getJSON(Drupal.settings.SSC.share+ 'url=' + $URL + '&service=twitter', function( resultdata ) {
  							totalShare += (isNaN(parseInt(resultdata.count))) ? 0 : parseInt(resultdata.count);
  							if(parametres.total != null && i == nbButtons && totalShare != 0)
  								$(parametres.total).text(totalShare);
  						});
  					}		
  				break;
  				case 'google': // Google + share
  					var google = $(this);
  					jg++;

  					popupWidth 	= 600;
  					popupHeight = 600;
  					shareUrl 	= 'https://plus.google.com/share?url=' + url;
  					shareUrl 	= (lang != null) ? shareUrl + '&hl=' + lang : shareUrl;
  					
  					if(count) {
  						if(apikey != null) {
  							$.post('https://clients6.google.com/rpc?key=' + apikey,  
  							{ 
  								"method":"pos.plusones.get",
  								"id":"p",
  								"params":{
  									"nolog":true,
  									"id": $URL,
  									"source":"widget",
  									"userId":"@viewer",
  									"groupId":"@self"
  								},
  								"jsonrpc":"2.0",
  								"key":"p",
  								"apiVersion":"v1"
  							}, 
  							function(gpdata) {
  								totalShare += (isNaN(parseInt(gpdata.result.metadata.globalCounts.count))) ? 0 : parseInt(gpdata.result.metadata.globalCounts.count);
  								if(parametres.total != null && i == nbButtons  && totalShare != 0)
  									$(parametres.total).text(totalShare);
  							});
  						}
  						else // Alternative yandex
  						{
  							window['gplus'+jg] = google;
  							if (!window.services) window.services = {};
  							window.services.gplus = {
  								cb: function(number) {
  									window['gplusnb'+jg] = (number == '') ? 0 : number;
  								}
  							};

  							$.getScript( 'http://share.yandex.ru/gpp.xml?url=' + $URL + '&callback=?', function(){
  								totalShare += (isNaN(parseInt(window['gplusnb'+jg]))) ? 0 : parseInt(window['gplusnb'+jg]);
  								if(parametres.total != null && i == nbButtons && totalShare != 0)
  									$(parametres.total).text(totalShare);
  							});
  						}
  					}
  				break;
  				case 'linkedin' : // Linkedin share
  					var linkedin 	= $(this);
  					popupWidth 	= 600;
  					popupHeight = 213;
  					shareUrl 	= 'https://www.linkedin.com/cws/share?url=' + url;

  					if(count) {
  						$.getJSON(Drupal.settings.SSC.share+ 'url=' + $URL + '&service=linkedin',  function( resultdata ) {
  							totalShare += (isNaN(parseInt(resultdata.count))) ? 0 : parseInt(resultdata.count);
  							if(parametres.total != null && i == nbButtons && totalShare != 0)
  								$(parametres.total).text(totalShare);
  						});
  					}
  				break;
  				case 'stumbleupon': // StumbleUpon share
  					var stumbleupon = $(this);
  					popupWidth 	= 1000;
  					popupHeight = 617;
  					shareUrl	= 'http://www.stumbleupon.com/submiturl=' + url;
  					shareUrl 	= (txt != null) ? shareUrl + '&title=' + txt : shareUrl;
  					
					if(count) {
  						$.get(Drupal.settings.SSC.share+ '?url=' + $URL + '&service=stumbleupon',  function( resultdata ) {
  							totalShare += (isNaN(parseInt(resultdata.count))) ? 0 : parseInt(resultdata.count);
  							if(parametres.total != null && i == nbButtons && totalShare != 0)
  								$(parametres.total).text(totalShare);
  						});
  					}

  				break;
  				case 'pinterest': // Pinterest share
  					var pinterest = $(this);
  					popupWidth 	= 1000;
  					popupHeight = 617;
  					shareUrl	= 'http://www.pinterest.com/pin/create/button/?url=' + url;
  					shareUrl 	= (media != null) ? shareUrl + '&media=' + media : shareUrl;
  					shareUrl 	= (txt != null) ? shareUrl + '&description=' + txt : shareUrl;

  					if(count) {
  						$.getJSON(Drupal.settings.SSC.share+ 'url=' + $URL + '&service=pinterest',  function( resultdata ) {
  							totalShare += (isNaN(parseInt(resultdata.count))) ? 0 : parseInt(resultdata.count);
  							if(parametres.total != null && i == nbButtons && totalShare != 0)
  								$(parametres.total).text(totalShare);
  						});
  					}
  				break;
  				default: // Default Facebook share
  					var facebook = $(this);
  					popupWidth 	= 670;
  					popupHeight = 340;
  					shareUrl	= 'https://www.facebook.com/sharer/sharer.php?u=' + url;

  					if(count) {
  						$.getJSON(Drupal.settings.SSC.share+ 'url=' + $URL + '&service=facebook',  function( resultdata ) {
  							totalShare += (isNaN(parseInt(resultdata.count))) ? 0 : parseInt(resultdata.count);
  							if(parametres.total != null && i == nbButtons && totalShare != 0)
  								$(parametres.total).text(totalShare);
  						});
  					}
  				break;
  			}

  			// Click to Action Open Popup Share 
  			$(this).on('click tap', function() {
  				// Center popup to the screen
  				var left = (screen.width/2)-(popupWidth/2);
  				var top = (screen.height/2)-(popupHeight/2);

  				popupWindow = window.open(shareUrl,'popUpWindow','height=' + popupHeight + ',width=' + popupWidth + ',left=' + left + ',top=' + top + ',resizable=yes,scrollbars=yes,toolbar=no,menubar=no,location=no,directories=no,status=yes');
  			});
  		});
  	};
})(jQuery);

jQuery(".csbuttons").cSButtons({total:".counts",url:Drupal.settings.SSC.page});