/**
 * @file
 * A JavaScript used to initiate the HTMLCS library. The following datascript
 * was taken from: http://squizlabs.github.io/HTML_CodeSniffer.
 */

(function ($, Drupal, window, document, undefined) {

    /**
     * Incharge of starting the HTML_Codesniffer library.
     */
    Drupal.behaviors.htmlcs = {
        attach: function(context, settings) {
            var _p = Drupal.settings.htmlcs.modulePath;
            var _i = function(s,cb) {
                var sc = document.createElement('script');
                sc.onload = function() {
                    sc.onload = null;sc.onreadystatechange = null;
                    cb.call(this);
                };
                sc.onreadystatechange = function(){
                    if(/^(complete|loaded)$/.test(this.readyState) === true){
                        sc.onreadystatechange = null;sc.onload();
                    }
                };
                sc.src = s;
                if (document.head) {
                    document.head.appendChild(sc);
                }
                else {
                    document.getElementsByTagName('head')[0].appendChild(sc);
                }
            };
            var options = {path:_p};

            $('.htmlcs-toggle-input').change(function () {
                if (!this.checked) {
                    HTMLCSAuditor.close();
                }
                else {
                    _i(_p + 'HTMLCS.js', function(){
                        HTMLCSAuditor.run(Drupal.settings.htmlcs.defaultReport, null, options);
                    });
                }
            });

        }
    };

})(jQuery, Drupal, this, this.document);
