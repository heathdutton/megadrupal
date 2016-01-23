var bontactCustomer;
(function ($) {
    Drupal.behaviors.bontact = {
        attach: function (context, settings) {
						bontactCustomer = Drupal.settings.bontact.customerId;
						var locationRoot = (('https:' == document.location.protocol) ? 'https://' : 'http://');
						var newElem = document.createElement('script');  
						newElem.setAttribute('src', locationRoot+'dashboard.bontact.com/widget/bontact.script.js'); 
						newElem.setAttribute('type', 'text/javascript'); 
						document.getElementsByTagName('head')[0].appendChild(newElem);        	
        	}
    }
    Drupal.behaviors.bontact;
})(jQuery);
