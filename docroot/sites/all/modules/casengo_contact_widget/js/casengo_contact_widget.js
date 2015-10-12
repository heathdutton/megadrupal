(function ($) {
  /**
   * This file contains the JavaScript
   * for loading the Casengo Contact Widget from Drupal.
   */
  Drupal.behaviors.casengoContactWidget = {
    attach: function (context, settings) {
      var po = document.createElement('script');
      po.type = 'text/javascript';
      po.async = true;
      po.src = '//' + settings.subDomainName + '.casengo.com/apis/vip-widget.js?r=' + new Date().getTime();
      var s = document.getElementsByTagName('script')[0];
      s.parentNode.insertBefore(po, s);
    }
  };
})();
