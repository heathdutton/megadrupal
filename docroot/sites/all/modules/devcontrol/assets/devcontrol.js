
(function ($) { 
"use strict";

Drupal.behaviors.DevControl = {
    attach: function (context) {

        var jContext  = $(context),
            jDocument = $(document),
            jBar      = jDocument.find("#devcontrol");

        jBar.once('devcontrol', function () {

            var expanded = true;
            jBar.find('#devcontrol-expand').click(function () {
                if (expanded) {
                    jBar.removeClass("expanded");
                } else {
                    jBar.addClass("expanded");
                }
                expanded = !expanded;
            });

            jBar.find('.devcontrol-item ul').once('devcontrol', function () {

                var jThis   = $(this),
                    jParent = jThis.parent();

                jParent.click(function () {
                      jThis.show();

                      jDocument.mouseup(function (event) {
                          if (jThis.has(event.target).length === 0) {
                              jThis.hide();
                          }

                          jDocument.mouseup(null);
                      });
                });
            });
        });
    }
};

// End of strict mode
})(jQuery);
