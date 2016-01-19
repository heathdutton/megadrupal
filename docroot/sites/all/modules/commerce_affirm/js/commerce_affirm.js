/**
 * @file
 * Initializes Affirm API
 */

(function () {

    /**
     * Allows Affirm to include their javascript.
     */
    Drupal.behaviors.commerce_affirm = {

        attach: function (context, settings) {

            var settings = Drupal.settings.commerce_affirm || {};

            var url = (settings.ApiMode == 'live')
                ? 'https://cdn1.affirm.com/js/v2/affirm.js'
                : 'https://cdn1-sandbox.affirm.com/js/v2/affirm.js';

            var _affirm_config = {
                public_api_key: settings.hasOwnProperty('PublicKey') ? settings.PublicKey : '',
                script:         url
            };
            (function(l,g,m,e,a,f,b){var d,c=l[m]||{},h=document.createElement(f),n=document.getElementsByTagName(f)[0],k=function(a,b,c){return function(){a[b]._.push([c,arguments])}};c[e]=k(c,e,"set");d=c[e];c[a]={};c[a]._=[];d._=[];c[a][b]=k(c,a,b);a=0;for(b="set add save post open empty reset on off trigger ready setProduct".split(" ");a<b.length;a++)d[b[a]]=k(c,e,b[a]);a=0;for(b=["get","token","url","items"];a<b.length;a++)d[b[a]]=function(){};h.async=!0;h.src=g[f];n.parentNode.insertBefore(h,n);delete g[f];d(g);l[m]=c})(window,_affirm_config,"affirm","checkout","ui","script","ready");
        }
    }

})();
